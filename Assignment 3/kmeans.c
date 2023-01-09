#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

// Creates an array of random datapoints for floats. Each number has a value from 0 - 1
float* create_datapoints(const int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  for (int i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Distance**2 Euclidean between d-vectors pointed to by v1, v2.
float distance2(const float *v1, const float *v2, const int d) {
  float dist = 0.0;
  for (int i=0; i<d; i++) {
    float diff = v1[i] - v2[i];
    dist += diff * diff;
  }
  return dist;
}

// Assign a datapoints to the correct cluster by computing its distances to each cluster centroid.
int assign_vector(const float* site, float* centroids,
		const int k, const int d) {
  int best_cluster = 0;
  float best_dist = distance2(site, centroids, d);
  float* centroid = centroids + d;
  for (int c = 1; c < k; c++, centroid += d) {
    float dist = distance2(site, centroid, d);
    if (dist < best_dist) {
      best_cluster = c;
      best_dist = dist;
    }
  }
  return best_cluster;
}


// Add a data point into a sum of all vectors
void calculateTotalVector(const float * site, float * sum, const int d) {
  for (int i=0; i<d; i++) {
    sum[i] += site[i];
  }
}

// Print the centroids.
void printAllCentroid(float * centroids, const int k, const int d) {
  float *p = centroids;
  printf("Centroids:\n");
  for (int i = 0; i<k; i++) {
    for (int j = 0; j<d; j++, p++) {
      printf("%f ", *p);
    }
    printf("\n");
  }
}

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Application: kmeans num sites per proc, num means, num dimensions\n");
    exit(1);
  }

  int vectors_per_process = atoi(argv[1]);
  int k = atoi(argv[2]);  // number of clusters.
  int d = atoi(argv[3]);  // dimension of data.

  srand(42000);

  // Initial MPI and find process rank and number of processes.
  MPI_Init(NULL, NULL);
  int rank, nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  //
  // The vectors assigned to this process.
  float* vectors;  
  assert(vectors = malloc(vectors_per_process * d * sizeof(float)));

  // The sum of sites assigned to each cluster by this process.
  // k vectors of d elements.
  float* sums;
  assert(sums = malloc(k * d * sizeof(float)));

  // The number of sites assigned to each cluster by this process. k integers.
  int* counts;
  assert(counts = malloc(k * sizeof(int)));

  // The current centroids against which sites are being compared.
  float* centroids;
  assert(centroids = malloc(k * d * sizeof(float)));

  // The cluster assignments for each site.
  int* labels;
  assert(labels = malloc(vectors_per_process * sizeof(int)));
  
  //
  // All the sites for all the processes.
  // site_per_proc * nprocs vectors of d floats.
  float* all_vector = NULL;

  // Sum of sites assigned to each cluster by all processes.
  float* total_sums = NULL;

  // Number of sites assigned to each cluster by all processes.
  int* grand_counts = NULL;

  // Result of program: a cluster label for each site.

  int* all_labels;
  if (rank == 0) {
    all_vector = create_datapoints(d * vectors_per_process * nprocs);

    // Take the first k sites as the initial cluster centroids.
    for (int i = 0; i < k * d; i++) {
      centroids[i] = all_vector[i]; 
    }
    printAllCentroid(centroids, k, d);
    assert(total_sums = malloc(k * d * sizeof(float)));
    assert(grand_counts = malloc(k * sizeof(int)));
    assert(all_labels = malloc(nprocs * vectors_per_process * sizeof(int)));
  }

  // Root sends each process its share of sites.
  MPI_Scatter(all_vector,d*vectors_per_process, MPI_FLOAT, vectors,
              d*vectors_per_process, MPI_FLOAT, 0, MPI_COMM_WORLD);

  
  float norm = 1.0;
  
  while (norm > 0.00001) { 

    // Broadcast the current cluster centroids to all processes.
    MPI_Bcast(centroids, k*d, MPI_FLOAT,0, MPI_COMM_WORLD);

    // Each process resets the accumulators in its cluster.
    for (int i = 0; i < k*d; i++) sums[i] = 0.0;
    for (int i = 0; i < k; i++) counts[i] = 0;


    // Find the closest centroid to each site and assign to cluster.
    float* site = vectors;
    for (int i = 0; i < vectors_per_process; i++, site += d) 
    {
      int cluster = assign_vector(site, centroids, k, d);
      counts[cluster]++;
      calculateTotalVector(site, &sums[cluster*d], d);
    }

    // Gather and sum at root all cluster sums for individual processes.
    MPI_Reduce(sums, total_sums, k * d, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(counts, grand_counts, k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
      // The Root Process divides the sums of each cluster by the count of each cluster to calculate new centroids.
      for (int i = 0; i<k; i++) {
        for (int j = 0; j<d; j++) {
          int dij = d*i + j;
          total_sums[dij] /= grand_counts[i];
        }
      }
      // Perform Normalization
      norm = distance2(total_sums, centroids, d*k);
      printf("norm: %f\n",norm);
      // In centroids, insert fresh centroids from grand sums.
      for (int i=0; i<k*d; i++) {
	      centroids[i] = total_sums[i];
      }
      printAllCentroid(centroids,k,d);
    }
    // Broadcast the norm.  This will be used in the loop test for all processes. value
    MPI_Bcast(&norm, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  // Calculate a final label for each vector now that the centroids are determined.
  float* tempVector = vectors;
  for (int i = 0; i < vectors_per_process; i++, tempVector += d) {
    labels[i] = assign_vector(tempVector, centroids, k, d);
  }

  // Gather all labels into root process.
  MPI_Gather(labels, vectors_per_process, MPI_INT, all_labels, vectors_per_process, MPI_INT, 0, MPI_COMM_WORLD);

  // Root can print out all sites and labels.
  if ((rank == 0) && 1) {
    float* site = all_vector; 
    for (int i = 0; i < nprocs * vectors_per_process; i++, site += d) {
      for (int j = 0; j < d; j++) printf("%f ", site[j]);
      printf("%4d\n", all_labels[i]);
    }
  }
      
  MPI_Finalize();

}
