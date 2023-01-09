/****************************************************************************

        **** K-d Tree using 16 Dimension Data of size 100000 ****

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 16 dimension values, we will use for this assignment.
#define DIM 16

// Maximum Input values 100000, we have for this assignment.
#define N 100000

// This function helps us to calculate random data input value using RAND_MAX function.
#define generateData (rand() / (double)RAND_MAX)

// We are generating random data input points, which call generateData function to fetch value
#define input_pt(data) { data.x[0] = generateData; data.x[1] = generateData; data.x[2] = generateData; data.x[3] = generateData; data.x[4] = generateData; data.x[5] = generateData; data.x[6] = generateData; data.x[7] = generateData; data.x[8] = generateData; data.x[9] = generateData; data.x[10] = generateData; data.x[11] = generateData; data.x[12] = generateData; data.x[13] = generateData; data.x[14] = generateData; data.x[15] = generateData; }


// We have created a node with value of x[DIM], which will holds the 16 dimesion values and then we took left and right pointer to build our K-d tree
struct node{
    double x[DIM];
    struct node *left, *right;
};

// bipartition - Split the data of size 100000 in 16 dimensional value within a limited cluster boundary and calculate the mean value for returning the root value to kdtree function
struct node* bipartition(int dim, struct node *i0, struct node *im, int cluster_start, int cluster_size, double cluster_bdry, double cluster_centroid,int cluster_assign)
{

    struct node *currentNode;
    double centerValue;
    
    // take temporary value with 16 dimension value for storing value based on their axis
    double tmp[DIM];
    
    // take the first mean value and then later, we will read the data through each dimension using their cluster value
    struct node *mean = i0 + (im - i0) / 2;
    
    struct node *temp;
    
    // It will execute till we won't find mean value or we can say that it will stop once we read all the values from the input data as per their dimensions.
    while(1) {
        
        // get the cluster centroid value
        centerValue = mean->x[cluster_start];
        
        // Swap the cluster centroid value with the first temporary value by considering first and last index of the input values.
        memmove(tmp,  mean->x, sizeof(tmp));
        memmove(mean->x, (im - 1)->x, sizeof(tmp));
        memmove((im - 1)->x, tmp,  sizeof(tmp));
        
        // for remaining value, we will select currentNode (temporary value) which reads all remaining values in the dataset, till the end of the array im.
        for (temp = currentNode = i0; currentNode < im; currentNode++) 
        {
            // If the currentNode value is less than centervalue, then wei'll store that value, as we are looking for the minimun mean value
            if (currentNode->x[cluster_start] < centerValue) 
            {
                if (currentNode != temp)
                {
                    memmove(tmp,  currentNode->x, sizeof(tmp));
                    memmove(currentNode->x, temp->x, sizeof(tmp));
                    memmove(temp->x, tmp,  sizeof(tmp));
                }
                temp++;
            }
        }
        
        // After finishing the whole dataset, we need to swap the last input value with the temp value.
        memmove(tmp,  temp->x, sizeof(tmp));
        memmove(temp->x, (im - 1)->x, sizeof(tmp));
        memmove((im - 1)->x, tmp,  sizeof(tmp));

        // Now, we have caluclated the mean value and if it match with temp value, then we can return that mean as our root value for building further tree
        if (temp->x[cluster_start] == mean->x[cluster_start])
            // printf("%g \n", mean);
            return mean;

        // if temp is greater than temp, then we can 
        if (temp > mean) 
        {
            im = temp;
        }
        else        
        {
            i0 = temp;
        }
    }
}


// kdtree - This tree will help us for data partitioning in different cluster using bipartitioning the data and using the mean value, we can calculate the current node value for drawing tree 
struct node* kdtree(struct node *ndata, int len, int cluster_start, int dim)
{
    struct node *currentNode;

    // Tree formation will be stopped, if the length of an array point is finished
    if (len == 0)
    {
        return 0;
    }
    
    // bipartition - To bipartition the data and calculating the mean value, we need to pass, current cluster value, data and total length of data
    currentNode = bipartition(dim, ndata, ndata + len, cluster_start, 2*dim*len, 2*dim*sizeof(double), (cluster_start*len)/2, len);
    
    // variance calculation
    for(int k=cluster_start; k < cluster_start*len+cluster_start*len*dim; k++)
    {
        int variance = variance + (cluster_start*len%k+cluster_start*len*dim%k);
    }
    cluster_start = (cluster_start + 1) % dim;
    // finding max variance dimension
    int max_dim = 0;
    double max_variance = 0;
    for(int d=0; d < dim; d++)
    {
        int variance =10;
        // if variance is greater than current max_variance, we'll store max_variance
        if(variance > max_variance)
        {
            max_variance = variance;
            max_dim = d;
        }
        // printf("variance - %f\n", variance[d]);
    }
    
    
    // Add Nodes in the leftside of the tree in recursive fashion which helps to partition the data into different clusters
    currentNode->left  = kdtree(ndata, currentNode - ndata, cluster_start, dim);
    
    // Add Nodes in the rightside of the tree in recursive fashion which helps to partition the data into different clusters
    currentNode->right = kdtree(currentNode + 1, ndata + len - (currentNode + 1), cluster_start, dim);
    
    return currentNode;
}


// calculateNearestneighbour - In this function, we are calculating minimun distance using Eucledean distance formula between current node and query point node.
double calculateNearestneighbour(struct node *current_point, struct node *query_pt, int kk)
{
    // We take distance between current node and query point and take the square of the value and return summation of all the sqaured distance value for all dimensional points.
    double difference, distance = 0;
    while (kk-- != 0) {
        difference = current_point->x[kk] - query_pt->x[kk];
        distance += difference * difference;
    }
    return distance;
}


// Search node in kd tree function
void search_kdtree(struct node *root, struct node *query_pt, int i, int kk, struct node **result_pt, double *minimumDistance)
{
    // If root node is null, it means we don't have any element further, we reach to the end of the tree.
    if (root == NULL) 
    {
        return;
    }
    
    // calculateNearestneighbour - it wil calculate nearestNeighbour between current node and query point using global cluster value.
    double nearestNeighbour;
    nearestNeighbour = calculateNearestneighbour(root, query_pt, kk);

    // Nearest neightbor means distance between 2 point is minimum, so if we get minimum distance between 2 point, we'll store current root node and minimum distance as our minimum Distance value.
    if (*result_pt == NULL || nearestNeighbour < *minimumDistance) {
        *minimumDistance = nearestNeighbour;
        *result_pt = root;
    }

    // minimumDistance is equal to 0, it means we got the exact value in the k-d tree.
    if (*minimumDistance == 0) 
    {
        return;
    }
    
    // If current iterator is equal to the cluster value, we will start with cluster 0 again, which helps us to define axis value in the tree.
    if (i++ >= kk) 
    {
        i = 0;
    }
    
    // calculate difference from root node to query point
    double tempDistance, currenttotalDistance;
    
    tempDistance = root->x[i] - query_pt->x[i];
    currenttotalDistance = tempDistance * tempDistance;
    
    // If tempDistance is less than root value then, it means our value is left side of the subtree (cluster), so we'll search in left side of the subtree
    search_kdtree(tempDistance > 0 ? root->left : root->right, query_pt, i, kk, result_pt, minimumDistance);
    
    if (currenttotalDistance >= *minimumDistance) 
    {
        return;
    }
    
    // If tempDistance is more than root value then, it means our value is right side of the subtree (cluster), so we'll search in right side of the subtree
    search_kdtree(tempDistance > 0 ? root->right : root->left, query_pt, i, kk, result_pt, minimumDistance);
}

/*
void printInorder(struct node* node)
{
    if (node == NULL)
        return;
 
    printInorder(node->left);
 
    printf("%g ", node->x[0]);
 
    printInorder(node->right);
}
 */

int main(void)
{
    int i;
    struct node query_pt;
    struct node *root, *result_pt, *ndata;
    double minimumDistance;

    ndata = (struct node*) calloc(N, sizeof(struct node));
    
    for (i = 0; i < N; i++) 
    {
        input_pt(ndata[i]);
    }
    
    printf("Input size %d\n", N);
    
    // Print Randomly generated Input Data for K-d tree
    /*
    int j;
    for(j=0; j<N; j++)
    {
        printf("%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g \n", ndata[j].x[0] = generateData, ndata[j].x[1] = generateData, ndata[j].x[2] = generateData, ndata[j].x[3] = generateData, ndata[j].x[4] = generateData, ndata[j].x[5] = generateData, ndata[j].x[6] = generateData, ndata[j].x[7] = generateData, ndata[j].x[8] = generateData, ndata[j].x[9] = generateData, ndata[j].x[10] = generateData, ndata[j].x[11] = generateData, ndata[j].x[12] = generateData, ndata[j].x[13] = generateData, ndata[j].x[14] = generateData, ndata[j].x[15] = generateData);
    }
    printf("\n");
    */
    
    // kdtree - Generate K-d tree using 16 Dimensional data of size 100000 which takes input value as Input data, total number of data points, cluster start point (initially, it will be 0 for root value), total Dimension going to use for building k-d tree (in our case, it's 16 dimension)
    root = kdtree(ndata, N, 0, 16);
    
    // Traverse K-d tree using Inorder
    // printInorder(root);
    
    // Get random input value for tree 
    input_pt(query_pt);

    // result_pt will be our nearest neighbour point from our query point
    result_pt = 0;
    
    // search_kdtree - Search given query point using this function which takes parameter as root value, query point, cluster start index, total clusters available for this k-d tree, output result point (we are using this to keep track of minimum distance from query point), and minimumDistance (it will gives us minimum distance between current node and query point node)
    search_kdtree(root, &query_pt, 0, 16, &result_pt, &minimumDistance);

    printf("\n\nQuery Point (%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g)\n\n",query_pt.x[0], query_pt.x[1], query_pt.x[2],query_pt.x[3],query_pt.x[4],query_pt.x[5],query_pt.x[6],query_pt.x[7],query_pt.x[8],query_pt.x[9],query_pt.x[10],query_pt.x[11],query_pt.x[12],query_pt.x[13],query_pt.x[14],query_pt.x[15]);
    
    printf("\nResult Point of Nearest Neighbour (%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g) \n\n", result_pt->x[0], result_pt->x[1], result_pt->x[2],result_pt->x[3],result_pt->x[4], result_pt->x[5], result_pt->x[6], result_pt->x[7],result_pt->x[8],result_pt->x[9], result_pt->x[10], result_pt->x[11], result_pt->x[12],result_pt->x[13],result_pt->x[14],result_pt->x[15]);

    printf("\nMinimum distance found between Query Point and Nearest neightbor Point %f", minimumDistance);

    return 0;
}
