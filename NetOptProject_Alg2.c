#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SZ 305

typedef struct
{
	int degree;
	int cost;
	int adj[SZ];
}node;

typedef struct
{
	int vertex;
	int degree;
}degreeInfo;

node graph[SZ];
int visited[SZ];
int queue[SZ], head, tail;

void bfs(node graph[], int n);
int comp(const void *a, const void *b);
void initialize(int visited[], node graph[], int n, int u);
void enqueue(int a);
int dequeue(void);

int main()
{
    int n, i, j;
    FILE *fpt = fopen("graphs.txt", "r");
    if(!fpt)
    	printf("Input file 'graphs.txt' is not found\n");
    else
    {
 		while(fscanf(fpt, "%d", &n) == 1)
		{
 			printf("Nodes = %d\n", n);
			for(i = 0; i < n; i++)
			{
				fscanf(fpt, "%d", &graph[i].degree);
				for(j = 0; j < graph[i].degree; j++)
					fscanf(fpt, "%d", &graph[i].adj[j]);
			}

			bfs(graph, n);

			for(i = 0; i < n; i++)
				printf("%d -> %d\n", i, graph[i].cost);
			printf("\n");
		}
    }

    if(fpt)
    	fclose(fpt);
    return 0;
}

void bfs(node graph[], int n)
{
	int i, u, v, childNo;
	degreeInfo childList[SZ];
	int childCount;
	initialize(visited, graph, n, 0);
	head = tail = 0;
	enqueue(0);
	visited[0] = 1;
	for(i = 0; i < n; i++)
		printf("%d ", visited[i]);
	printf("\n");
	while(!isEmpty())
	{
		u = dequeue();

		childCount = 0;
		for(i = 0; i < graph[u].degree; i++)
		{
			v = graph[u].adj[i];
			if(!visited[v])
			{
				childList[childCount].vertex = v;
				childList[childCount].degree = graph[v].degree;
				childCount++;
			}
		}

		qsort(childList, childCount, sizeof(degreeInfo), comp);

		childNo = 1;
		for(i = 0; i < childCount; i++)
		{
			v = childList[i].vertex;
			graph[v].cost = graph[u].cost + childNo;
			enqueue(v);
			visited[v] = 1;
			childNo++;
		}
	}
}

int comp(const void *a, const void *b)
{
	degreeInfo *x = (degreeInfo*)a;
	degreeInfo *y = (degreeInfo*)b;
	if(x->degree == y->degree)
		return x->vertex - y->vertex;
	return y->degree - x->degree;
}

int isEmpty()
{
	if(head >= tail)
		return 1;
	return 0;
}

void enqueue(int a)
{
	queue[tail++] = a;
}

int dequeue(void)
{
	int val = queue[head];
	head++;
	return val;
}

void initialize(int visited[], node graph[], int n, int u)
{
	memset(visited, 0, n * sizeof(int));
	graph[u].cost = 0;
}
