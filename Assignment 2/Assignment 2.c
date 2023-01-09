/**************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

***************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define N 100000
#define M 500
#define DIM 16
#define SIZE 100
   
// This function helps us to calculate random data input value using RAND_MAX function.
#define generateData (rand() / (double)RAND_MAX)

// We are generating random data input points, which call generateData function to fetch value.
#define input_pt(data) { data.x[0] = generateData; data.x[1] = generateData; data.x[2] = generateData; data.x[3] = generateData; data.x[4] = generateData; data.x[5] = generateData; data.x[6] = generateData; data.x[7] = generateData; data.x[8] = generateData; data.x[9] = generateData; data.x[10] = generateData; data.x[11] = generateData; data.x[12] = generateData; data.x[13] = generateData; data.x[14] = generateData; data.x[15] = generateData; }

// This function helps us to calculate Gaussian Distribution using RAND_MAX function.
#define generateDistribution (sqrt(-2 * log(rand() / (double)RAND_MAX)) * cos(2 * M_PI * rand() / (double)RAND_MAX))

// We are generating random data for projection, which call generateDistribution function to fetch value.
#define projection_pt(projection) { projection.x[0] = generateDistribution; projection.x[1] = generateDistribution; projection.x[2] = generateDistribution; projection.x[3] = generateDistribution; projection.x[4] = generateDistribution; projection.x[5] = generateDistribution; projection.x[6] = generateDistribution; projection.x[7] = generateDistribution; projection.x[8] = generateDistribution; projection.x[9] = generateDistribution; projection.x[10] = generateDistribution; projection.x[11] = generateDistribution; projection.x[12] = generateDistribution; projection.x[13] = generateDistribution; projection.x[14] = generateDistribution; projection.x[15] = generateDistribution; }

// We have created a node with value of x[DIM], which will holds the 16 dimesion values.
struct node{
    double x[DIM];      // stores input data points
    double y[DIM];      // stores gaussian value of input vector with W scaling factor value
    double z[DIM];      // stores Round value of the above calculated dot product value (Gaussian values)
    double c[DIM];      // stores hash table key for each input data which helps us to identify bucket
    double r[DIM];      // stores the normalize form of the input data
    double k[DIM];      // stores the generated hash table values for each input data which helps to store in the hash table
};


// We have create node for creating buckets for our data value
struct hashNode
{
    double data;
    struct hashNode *next;
};

// create chin set for storing multiple values in hash table
struct hashNode *chain[SIZE];

// Initaliza hash table (buckets) with empty value
void init()
{
    int i;
    for(i = 0; i < SIZE; i++)
        chain[i] = NULL;
}


// Insert the value in bucket / hash table
void insert(double keys, double value)
{
    //create a newnode with value
    struct hashNode *newNode = malloc(sizeof(struct hashNode));
    newNode->data = value;
    newNode->next = NULL;

    //calculate hash key
    int key = (int) keys % SIZE;

    //check if chain[key] is empty
    if(chain[key] == NULL)
        chain[key] = newNode;
    //collision
    else
    {
        //add the node at the end of chain[key].
        struct hashNode *temp = chain[key];
        while(temp->next)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }
}


// Print the Hash table that we have generated based on the input data
void print()
{
    int i;

    for(i = 0; i < SIZE; i++)
    {
        struct hashNode *temp = chain[i];
        
        if(temp != NULL)
        {
            printf("bucket[%d]-->",i);
            while(temp)
            {
                printf(" %g ",temp->data);
                temp = temp->next;
            }
            // printf("NULL\n");
            printf("\n");
        }
        else
        {
            continue;
        }
    }
}

// Search function is used to find nearest or exact bucket for th given query point
void search(struct node *query_pt)
{
    int i, flag=0;
    
    double key = query_pt[0].c[0];
    
    for(i = 0; i < SIZE; i++)
    {
        struct hashNode *temp = chain[i];
        
        // If we found the exact key, then it will return exact cluster number
        
        if(temp != NULL && i == key)
        {
            printf("We found exact matched Closest bucket[%d] is available for our Query Point Data-->%g (Hash Code) for query point --> [%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n",i, key, query_pt[0].x[0], query_pt[0].x[1], query_pt[0].x[2], query_pt[0].x[3], query_pt[0].x[4], query_pt[0].x[5], query_pt[0].x[6], query_pt[0].x[7], query_pt[0].x[8], query_pt[0].x[9], query_pt[0].x[10], query_pt[0].x[11], query_pt[0].x[12] , query_pt[0].x[13], query_pt[0].x[14], query_pt[0].x[15]);
    
            while(temp)
            {
                printf(" %g ",temp->data);
                temp = temp->next;
            }
            // printf("NULL\n");
            printf("\n");
            break;
        }
        else
        {
            flag = 1;
            continue;
        }
    }
    
    // If we don't find the exact key, then we'll search for nearest neighbour for that query point and we'll return the nearest cluster number from our expected query point.
    
    if(flag == 1)
    {
        
        double expectedKey = key;
        
        int i=key-1;
        int j=key+1;
        
        int f = 0;
        
        while(i>=0 || j<=SIZE)
        {
            struct hashNode *temp1 = chain[i];
            struct hashNode *temp2 = chain[j];
            if(temp1 != NULL)
            {
                printf("Closest bucket[%d] is available for our Query Point Data-->%g (Hash Code) for query point --> [%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n",i, key, query_pt[0].x[0], query_pt[0].x[1], query_pt[0].x[2], query_pt[0].x[3], query_pt[0].x[4], query_pt[0].x[5], query_pt[0].x[6], query_pt[0].x[7], query_pt[0].x[8], query_pt[0].x[9], query_pt[0].x[10], query_pt[0].x[11], query_pt[0].x[12] , query_pt[0].x[13], query_pt[0].x[14], query_pt[0].x[15]);
    
                while(temp1)
                {
                    printf(" %g ",temp1->data);
                    temp1 = temp1->next;
                }
                // printf("NULL\n");
                printf("\n");
                f = 0;
                break;
            }
            else if(temp2 != NULL)
            {
                printf("Closest bucket[%d] (Hash Code) is available for our Query Point Data-->%g (Hash Code) for query point --> [%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n",j, key, query_pt[0].x[0], query_pt[0].x[1], query_pt[0].x[2], query_pt[0].x[3], query_pt[0].x[4], query_pt[0].x[5], query_pt[0].x[6], query_pt[0].x[7], query_pt[0].x[8], query_pt[0].x[9], query_pt[0].x[10], query_pt[0].x[11], query_pt[0].x[12] , query_pt[0].x[13], query_pt[0].x[14], query_pt[0].x[15]);
    
                while(temp1)
                {
                    printf(" %g ",temp2->data);
                    temp1 = temp2->next;
                }
                // printf("NULL\n");
                printf("\n");
                f = 0;
                break;
            }
            else
            {
                f == 1;
                i++;
                j--;
            }
        }
        
        if(f == 1)
        {
            printf("We don't have any nearest neighbour for query point. It's an empty bucket and this is our newest point in bucket.");
        }
        
    }
}

int main()
{
    printf("////////. Locality Sensitive Hashing  ./////////\n\n");

   
   
    
    printf("\n/******Get the input data and calculate its hash code and store in buckets****/\n\n");
    
    int i;
    struct node *ndata, *projection, *query_pt;
   
    /**Input Point**/
   
    ndata = (struct node*) calloc(N, sizeof(struct node));
     
     
    // Read the input data 
    for (i = 0; i < N; i++)
    {
        input_pt(ndata[i]);
    }
   
    printf("Input size %d\n", N);
   
    // Print Randomly generated Input Data for LSH

    printf("\nInput Data\n");
    int j;
    for(j=0; j<N; j++)
    {
        printf("[%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n", ndata[j].x[0], ndata[j].x[1], ndata[j].x[2], ndata[j].x[3], ndata[j].x[4], ndata[j].x[5], ndata[j].x[6], ndata[j].x[7], ndata[j].x[8], ndata[j].x[9], ndata[j].x[10], ndata[j].x[11], ndata[j].x[12] , ndata[j].x[13], ndata[j].x[14], ndata[j].x[15] );
    }
    printf("\n");
   
   
   
    /**Create a random projection point with size of M for scaling factor (W) **/

    projection = (struct node*) calloc(M, sizeof(struct node));
     
    for (i = 0; i < M; i++)
    {
        projection_pt(projection[i]);
    }
   
    printf("Projection vector size %d\n", M);
   
    // Print Randomly generated m values which is calculated using Gaussian for LSH
   
    printf("\nProjection Data\n");
    for(j=0; j<M; j++)
    {
        printf("[%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n", projection[j].x[0] , projection[j].x[1] , projection[j].x[2] , projection[j].x[3] , projection[j].x[4] , projection[j].x[5] , projection[j].x[6] , projection[j].x[7] , projection[j].x[8] , projection[j].x[9] , projection[j].x[10] , projection[j].x[11], projection[j].x[12] , projection[j].x[13] , projection[j].x[14], projection[j].x[15]);
    }
    printf("\n");
   
    /** Compute Hash Values by taking dot product***/
   
    int k=0, o=0;
   
    for(i=0;i<N; i++)
    {
        int s = 0;
        for(j=0; j<M; j++)
        {
            double sum = 0;
            for(o=0; o<DIM; o++)
            {
                sum += ndata[i].x[o] * projection[j].x[o];
            }
            // printf("%g\n", sum);
            ndata[i].y[s] = sum;
            s++;
        }
    }
   
    // Print the Dot product of Data and Projection Value for LSH
   
    printf("\nDot Product Data\n");
    for(j=0; j<N; j++)
    {
        printf("[%g, %g] \n", ndata[j].y[0], ndata[j].y[1]);
    }
    printf("\n");
   
   
    //Convert Dot Product into Binary Valuation

    for(i=0;i<N; i++)
    {
        ndata[i].z[0] = round(ndata[i].y[0]);
        ndata[i].z[1] = round(ndata[i].y[1]);
    }
   
    printf("\nSet of Similar value Data\n");
    for(j=0; j<N; j++)
    {
        printf("[%g, %g] \n", ndata[j].z[0], ndata[j].z[1]);
    }
    printf("\n");
   
   
   
    // Convert values Dot product valution into Key for preparing our Hash table
   
    for(i=0;i<N; i++)
    {
        ndata[i].c[0] = ndata[i].z[0]*10 + ndata[i].z[1];
    }
   
    printf("\nSet of Similar value Data\n");
    for(j=0; j<N; j++)
    {
        printf("[%g] \n", ndata[j].c[0]);
    }
    printf("\n");
   
   
    //Convert Input Vector into Normal Valuation for Normalization in hash table

    for(i=0;i<N; i++)
    {
        ndata[i].r[0] = round(ndata[i].x[0]);
        ndata[i].r[1] = round(ndata[i].x[1]);
        ndata[i].r[2] = round(ndata[i].x[2]);
        ndata[i].r[3] = round(ndata[i].x[3]);
        ndata[i].r[4] = round(ndata[i].x[4]);
        ndata[i].r[5] = round(ndata[i].x[5]);
        ndata[i].r[6] = round(ndata[i].x[6]);
        ndata[i].r[7] = round(ndata[i].x[7]);
        ndata[i].r[8] = round(ndata[i].x[8]);
        ndata[i].r[9] = round(ndata[i].x[9]);
        ndata[i].r[10] = round(ndata[i].x[10]);
        ndata[i].r[11] = round(ndata[i].x[11]);
        ndata[i].r[12] = round(ndata[i].x[12]);
        ndata[i].r[13] = round(ndata[i].x[13]);
        ndata[i].r[14] = round(ndata[i].x[14]);
        ndata[i].r[15] = round(ndata[i].x[15]);
    }
   
    printf("\nNormalize form of input Data\n");
    for(j=0; j<N; j++)
    {
        printf("[%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n", ndata[j].r[0], ndata[j].r[1], ndata[j].r[2], ndata[j].r[3], ndata[j].r[4], ndata[j].r[5], ndata[j].r[6], ndata[j].r[7], ndata[j].r[8], ndata[j].r[9], ndata[j].r[10], ndata[j].r[11], ndata[j].r[12] , ndata[j].r[13], ndata[j].r[14], ndata[j].r[15] );
    }
    printf("\n");
    
   
    // Concat above normalize input data into one single value for hash table
    
    for(i=0;i<N; i++)
    {
        for(j=0; j<DIM; j++)
        {
            ndata[i].k[0] = ndata[i].k[0]*10 + ndata[i].r[j];
        }
    }
    
    printf("\nSet of Similar value Data\n");
    for(j=0; j<N; j++)
    {
        printf("[%g] \n", ndata[j].k[0]);
    }
    printf("\n");
    
   
    // Create a bucket for our dataset and put keys and values in hash table
    
    printf("\n/******Partition the dataset into multiple buckets****/\n\n");
    
    init();

    for(i=0; i<N; i++)
    {
        double key = ndata[i].c[0];
        double value = ndata[i].k[0];
        insert(key, value);
    }
    
    print();
    
    
    printf("\n/******Check the generated query point and it's nearest cluster/bucket****/\n");
    
    // Check the given query and its nearest neightbour
    
    query_pt = (struct node*) calloc(1, sizeof(struct node));
     
    input_pt(query_pt[0]);
   
    // Print Randomly generated Input Data for LSH

    printf("\nQuery Point Data\n");
    
    printf("[%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n", query_pt[0].x[0], query_pt[0].x[1], query_pt[0].x[2], query_pt[0].x[3], query_pt[0].x[4], query_pt[0].x[5], query_pt[0].x[6], query_pt[0].x[7], query_pt[0].x[8], query_pt[0].x[9], query_pt[0].x[10], query_pt[0].x[11], query_pt[0].x[12] , query_pt[0].x[13], query_pt[0].x[14], query_pt[0].x[15]);
    printf("\n");
       
    // calculate hash value for query point with given m random vector
    
    int s=0;
    for(j=0; j<M; j++)
    {
        double sum = 0;
        for(o=0; o<DIM; o++)
        {
            sum += query_pt[0].x[o] * projection[j].x[o];
        }
        // printf("%g\n", sum);
        query_pt[0].y[s] = sum;
        s++;
    }
    
    printf("\nDot Product for query_pt\n");
    printf("[%g, %g] \n", query_pt[0].y[0], query_pt[0].y[1]);
    printf("\n");
   
       
    //Convert Dot Product of query_pt into Binary Valuation

    query_pt[0].z[0] = round(query_pt[0].y[0]);
    query_pt[0].z[1] = round(query_pt[0].y[1]);
   
    printf("\nSet of Similar value Data for query_pt\n");
    printf("[%g, %g] \n", query_pt[0].z[0], query_pt[0].z[1]);
    printf("\n");
   
    
       
    // Convert values Dot product valution into Key for preparing our Hash mutable
   
    query_pt[0].c[0] = query_pt[0].z[0]*10 + query_pt[0].z[1];
   
    printf("\nSet of Similar value Data\n");
    printf("[%g] \n", query_pt[0].c[0]);
    printf("\n");
   
   
    //Convert Input Vector into Normal Valuation for Normalization in hash table

    query_pt[0].r[0] = round(query_pt[0].x[0]);
    query_pt[0].r[1] = round(query_pt[0].x[1]);
    query_pt[0].r[2] = round(query_pt[0].x[2]);
    query_pt[0].r[3] = round(query_pt[0].x[3]);
    query_pt[0].r[4] = round(query_pt[0].x[4]);
    query_pt[0].r[5] = round(query_pt[0].x[5]);
    query_pt[0].r[6] = round(query_pt[0].x[6]);
    query_pt[0].r[7] = round(query_pt[0].x[7]);
    query_pt[0].r[8] = round(query_pt[0].x[8]);
    query_pt[0].r[9] = round(query_pt[0].x[9]);
    query_pt[0].r[10] = round(query_pt[0].x[10]);
    query_pt[0].r[11] = round(query_pt[0].x[11]);
    query_pt[0].r[12] = round(query_pt[0].x[12]);
    query_pt[0].r[13] = round(query_pt[0].x[13]);
    query_pt[0].r[14] = round(query_pt[0].x[14]);
    query_pt[0].r[15] = round(query_pt[0].x[15]);
   
    printf("\nNormalize form of input Data\n");
    printf("[%g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g, %g] \n", query_pt[0].r[0], query_pt[0].r[1], query_pt[0].r[2], query_pt[0].r[3], query_pt[0].r[4], query_pt[0].r[5], query_pt[0].r[6], query_pt[0].r[7], query_pt[0].r[8], query_pt[0].r[9], query_pt[0].r[10], query_pt[0].r[11], query_pt[0].r[12] , query_pt[0].r[13], query_pt[0].r[14], query_pt[0].r[15] );
    printf("\n");
    
    
    // Concat above normalize query_pt data into one single value for hash table
    
    for(j=0; j<DIM; j++)
    {
        query_pt[0].k[0] = query_pt[0].k[0]*10 + query_pt[0].r[j];
    }
    
    printf("\nSet of Similar value query_pt Data\n");
    printf("[%g] \n", ndata[0].k[0]);
    printf("\n");
    
    
    // Search the calculated hash code for query point in hash table and return the result
    
    printf("\n/******Search result for the calculated query points****/\n\n");
    
    search(query_pt);
    
    return 0;
}