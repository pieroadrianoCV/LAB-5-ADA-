#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100

typedef int Vertex;

typedef struct AdjNode {
    Vertex neighbor;
    struct AdjNode *next;
} *Link;

typedef struct {
    int numVertices;
    Link adjList[MAX_VERTICES];
} Graph;

int pre[MAX_VERTICES];
int discoveryCount;
int recursionDepth;

Link createNode(Vertex neighbor, Link next) {
    Link t = (Link)malloc(sizeof(struct AdjNode));
    t->neighbor = neighbor;
    t->next = next;
    return t;
}

Graph* readAdjacencyFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    Graph *G = (Graph*)malloc(sizeof(Graph));
    if (fscanf(file, "%d", &G->numVertices) != 1) {
        free(G);
        fclose(file);
        return NULL;
    }

    if (G->numVertices <= 0 || G->numVertices > MAX_VERTICES) {
        free(G);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < G->numVertices; i++) {
        G->adjList[i] = NULL;
    }

    for (int i = 0; i < G->numVertices; i++) {
        Vertex u;
        if (fscanf(file, "%d", &u) != 1) {
            break; 
        }

        char c;
        while ((c = fgetc(file)) != '\n' && c != EOF) {
            ungetc(c, file);
            Vertex v;
            if (fscanf(file, "%d", &v) == 1) {
                G->adjList[u] = createNode(v, G->adjList[u]); 
            } else {
                break;
            }
        }
    }

    fclose(file);
    return G;
}

void dfsR(Graph *G, Vertex u) {
    for (int i = 0; i < recursionDepth; i++)
        printf("  "); 
    printf("dfsR(G, %d)\n", u);

    recursionDepth++;

    pre[u] = discoveryCount++;
    
    for (Link t = G->adjList[u]; t != NULL; t = t->next) {
        Vertex v = t->neighbor;
        
        for (int i = 0; i < recursionDepth; i++)
            printf("  "); 
        printf("%d-%d", u, v);

        if (pre[v] == -1) {
            printf(" -> \n");
            dfsR(G, v);
        } else {
            printf(" (visited, pre[%d]=%d)\n", v, pre[v]);
        }
    }

    recursionDepth--;

    for (int i = 0; i < recursionDepth; i++)
        printf("  "); 
    printf("End dfsR(G, %d)\n", u);
}

void GRAPHdfs(Graph *G) {
    Vertex u;
    discoveryCount = 0;
    recursionDepth = 0;
    
    for (u = 0; u < G->numVertices; u++)
        pre[u] = -1;

    for (u = 0; u < G->numVertices; u++)
        if (pre[u] == -1) {
            printf("\n--- Starting Connected Component from Vertex %d ---\n", u);
            dfsR(G, u);
        }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    Graph *TheGraph = readAdjacencyFile(argv[1]);

    if (TheGraph == NULL) {
        return 1;
    }

    printf(">>> DFS Traversal for a Graph with %d vertices <<<\n", TheGraph->numVertices);
    
    GRAPHdfs(TheGraph);
    
    printf("\n>>> Final Discovery Order (pre[]) <<<\n");
    printf("Vertex: ");
    for (Vertex v = 0; v < TheGraph->numVertices; v++) {
        printf("%4d", v);
    }
    printf("\npre[v]: ");
    for (Vertex v = 0; v < TheGraph->numVertices; v++) {
        printf("%4d", pre[v]);
    }
    printf("\n");

    free(TheGraph); 
    
    return 0;
}