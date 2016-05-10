#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

void generateRandomGraph(int n, double rho);
int getARandomNumber(int range);

int main()
{
    //Seed for random number
    srand ( time(NULL) );
    int n;
    double rho;
    printf("Give number of nodes and density\n");
    while(scanf("%d %lf", &n, &rho) == 2)
    {
        if(n < 1)
            printf("Number of nodes is not valid, please give valid input.\n\n");
        //Test if given rho is valid or not
        else if(rho < 0.0 || rho > 1.0)
            printf("Density is out of bound. It has to be >= 0 && <= 1.0.\n\n");
        else
        {
            if(rho < 0.3 || rho > 0.7)
                printf("Density is beyond assignment scope, but valid. Processing anyway.\n\n");
            generateRandomGraph(n, rho);
        }

        printf("Give number of nodes and density\n");
    }

    return 0;
}

void generateAndWriteGraph(int n, double rho, int** listToSelect, int** adjMatrix)
{
    //To write the output to file
    FILE *fptG = fopen("graphs.txt", "a");
    if(!fptG)
        printf("File could not be created due to some file create permission r other problem. Program aborted.\n");
    else
    {
        int i, j, k;

        //Initialize the matrix listToSelect
        //This matrix will assist in generating simple graph by avoiding self loop, and multi edges
        //This matrix will also aid in time efficiency, by avoiding unnecessary random edge generation
        for(i = 1; i <= n; i++)
        {
            listToSelect[i][0] = i * n + n - 1;
            for(j = 1, k = 1; j <= n; j++)
            {
                if(i != j)
                    listToSelect[i][k++] = j;
                adjMatrix[i][j] = 0;
            }
        }

        //Calculate necessary number of edges based on n and rho
        int maxEdge = (n * (n - 1)) / 2;
        int m = (int) round(rho * maxEdge);
        if(m > maxEdge)
            m = maxEdge;
        int u, v, ind1, ind2, count, nodeCount = n;
        int *temp;

        //This loop will run at most n*n times
        for(i = 1; i <= m;)
        {
            //Generate random edge, by avoiding repeat edges
            ind1 = getARandomNumber(nodeCount) + 1;
            u = listToSelect[ind1][0] / n;
            count = listToSelect[ind1][0] % n;
            ind2 = getARandomNumber(count) + 1;
            v = listToSelect[ind1][ind2];
            listToSelect[ind1][ind2] = listToSelect[ind1][count];
            listToSelect[ind1][0]--;

            //Update listToSelect matrix to aid random number generation
            if(listToSelect[ind1][0] % n == 0)
            {
                temp = listToSelect[ind1];
                listToSelect[ind1] = listToSelect[nodeCount];
                listToSelect[nodeCount] = temp;
                nodeCount--;
            }

            //Test if edge (b, a) is generated after (a, b) edge
            if(adjMatrix[u][v])
                continue;
            //printf("%d %d\n", u, v);

            adjMatrix[u][v] = adjMatrix[v][u] = 1;
            i++;
        }

        //Print the adjacency matrix
        printf("%d %d\n", n, m);
        fprintf(fptG, "%d\n", n);

        for(i = 1; i <= n; i++)
		{
			int degree = 0;
			for(j = 1; j <= n; j++)
				if(adjMatrix[i][j])
					degree++;
			printf("%d\t", degree);
			fprintf(fptG, "%d\t", degree);
			for(j = 1; j <= n; j++)
			{
				if(adjMatrix[i][j])
				{
					printf("%d ", j - 1);
					fprintf(fptG, "%d ", j - 1);
				}
			}
			printf("\n");
			fprintf(fptG, "\n");
		}

        fprintf(fptG, "\n");
        printf("Graph with n = %d and m = %d generated and saved to file graphs.txt\n\n", n, m);
    }

    if(fptG)
        fclose(fptG);
}

void generateRandomGraph(int n, double rho)
{
    //Allocate, initialize, free memory, and call the graph generation function
    int **listToSelect, **adjMatrix;
    int i, size = n + 1;
    listToSelect = (int**)malloc(size * sizeof(int*));
    adjMatrix = (int**)malloc(size * sizeof(int*));

    if(!listToSelect || !adjMatrix)
        printf("Not enough memory available. Please consider small number of node.\n");
    else
    {
        int memoryFlag = 1;
        for(i = 0; i <= n; i++)
        {
            listToSelect[i] = (int*) malloc(size*sizeof(int));
            adjMatrix[i] = (int*) malloc(size*sizeof(int));
            if(!listToSelect[i] || !adjMatrix[i])
                memoryFlag = 0;
        }
        if(!memoryFlag)
            printf("Not enough memory available. Please consider small number of node.\n");
        else
            generateAndWriteGraph(n, rho, listToSelect, adjMatrix);
        for(i = 0; i <= n; i++)
        {
            if(listToSelect[i])
                free(listToSelect[i]);
            if(adjMatrix[i])
                free(adjMatrix[i]);
        }
    }
    if(adjMatrix)
        free(adjMatrix);
    if(listToSelect)
        free(listToSelect);
}

//Generate a random number up to (2^30)-1
int getARandomNumber(int range)
{
    int random = rand() << 15 | rand();
    return ((random % range) + range) % range;
}
