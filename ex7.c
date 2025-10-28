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

Vertex stackArray[MAX_VERTICES];
int stackTop = -1;

void STACKpush(Vertex v) {
    if (stackTop < MAX_VERTICES - 1) {
        stackArray[++stackTop] = v;
    }
}

Vertex STACKpop() {
    if (stackTop >= 0) {
        return stackArray[stackTop--];
    }
    return -1; // Error o pila vacía
}

int STACKempty() {
    return stackTop == -1;
}

// --- Variables Globales para el DFS ---
int pre[MAX_VERTICES];
int discoveryCount;

Link createNode(Vertex neighbor, Link next) {
    Link t = (Link)malloc(sizeof(struct AdjNode));
    t->neighbor = neighbor;
    t->next = next;
    return t;
}

Graph* readAdjacencyFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Graph *G = (Graph*)malloc(sizeof(Graph));
    if (fscanf(file, "%d", &G->numVertices) != 1) { free(G); fclose(file); return NULL; }
    if (G->numVertices <= 0 || G->numVertices > MAX_VERTICES) { free(G); fclose(file); return NULL; }

    for (int i = 0; i < G->numVertices; i++) G->adjList[i] = NULL;

    for (int i = 0; i < G->numVertices; i++) {
        Vertex u;
        if (fscanf(file, "%d", &u) != 1) break; 

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

// --- 7. Implementación de dfsIterativo ---
void dfsIterativo(Graph *G, Vertex start) {
    STACKpush(start);

    while (!STACKempty()) {
        Vertex v = STACKpop(); 

        // Se recorre la lista de adyacencia
        for (Link t = G->adjList[v]; t != NULL; t = t->next) {
            Vertex w = t->neighbor;

            if (pre[w] == -1) {
                pre[w] = discoveryCount++; 
                STACKpush(w);
            }
        }
    }
}

// --- Función Envoltorio (Wrapper) ---
void GRAPHdfsIterativo(Graph *G) {
    Vertex u;
    discoveryCount = 0;
    stackTop = -1; // Resetear la pila

    for (u = 0; u < G->numVertices; u++)
        pre[u] = -1;

    for (u = 0; u < G->numVertices; u++)
        if (pre[u] == -1) {
            pre[u] = discoveryCount++;
            dfsIterativo(G, u);
        }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    Graph *TheGraph = readAdjacencyFile(argv[1]);

    if (TheGraph == NULL) return 1;

    printf(">>> DFS Iterativo para un Grafo con %d vertices <<<\n", TheGraph->numVertices);
    
    GRAPHdfsIterativo(TheGraph);
    
    // Imprimir resultado final
    printf("\n>>> Orden de Descubrimiento Final (pre[]) <<<\n");
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