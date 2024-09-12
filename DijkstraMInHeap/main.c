#include <stdio.h>
#include <stdlib.h>

#define MAX 2147483646

typedef struct Node {
    int verhol;
    int weight;
    struct Node* next;
} Node;


typedef struct mygraph{
    int nymNodes;
    Node** adjaencyList;
} Graph;

typedef struct {
    int capacity;
    int size;
    int *array;
} PriorityQueue;

void initialize(PriorityQueue *pq, int capacity) {
    pq->capacity = capacity;
    pq->size = 0;
    pq->array = (int *)malloc(capacity * sizeof(int));
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(PriorityQueue *pq, int idx, int *distances) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && distances[pq->array[left]] < distances[pq->array[smallest]])
        smallest = left;

    if (right < pq->size && distances[pq->array[right]] < distances[pq->array[smallest]])
        smallest = right;

    if (smallest != idx) {
        swap(&pq->array[idx], &pq->array[smallest]);
        minHeapify(pq, smallest, distances);
    }
}

int isEmpty(PriorityQueue *pq) {
    return pq->size == 0;
}

void enqueue(PriorityQueue *pq, int vertex, int *distances) {
    if (pq->size == pq->capacity) return;

    pq->array[pq->size] = vertex;
    pq->size++;

    int i = pq->size - 1;
    while (i > 0 && distances[pq->array[(i - 1) / 2]] > distances[pq->array[i]]) {
        swap(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

}

int dequeue(PriorityQueue *pq, int *distances) {
    if (isEmpty(pq)) return -1;

    int root = pq->array[0];
    pq->array[0] = pq->array[pq->size - 1];
    pq->size--;

    minHeapify(pq, 0, distances);

    return root;
}
void decreaseKey(PriorityQueue *pq, int vertex, int *distances) {
    int i;
    for (i = 0; i < pq->size; i++) {
        if (pq->array[i] == vertex) break;
    }


    while (i  && distances[pq->array[(i - 1) / 2]] > distances[pq->array[i]]) {
        swap(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

}


void search_Vrchol(Graph *g, int from, int to, int *first) {
    if (g==NULL || to == from || from<0 || to<0 || from>=g->nymNodes || to>=g->nymNodes) {
        if (*first == 0) {
            printf("search failed");
            *first = 1;
        } else {
            printf("\nsearch failed");
        }
        return;
    }

    int distances[g->nymNodes];
    int visited[g->nymNodes];
    int parents[g->nymNodes];


    for (int i = 0; i < g->nymNodes; ++i) {
        distances[i] = MAX;
        visited[i] = 0;
        parents[i] = -1;
    }

    distances[from] = 0;

    int *inQueue = calloc(g->nymNodes,sizeof(int));


    PriorityQueue pq;
    initialize(&pq, g->nymNodes);
    enqueue(&pq, from, distances);

    while (!isEmpty(&pq)) {
        int currentNode = dequeue(&pq, distances);
        if (currentNode==to) break;
        visited[currentNode] = 1;


        for (Node *neighbor = g->adjaencyList[currentNode]; neighbor != NULL; neighbor = neighbor->next) {
            int nextNode = neighbor->verhol;
            int weight = neighbor->weight;

            if (!visited[nextNode] && distances[currentNode] != MAX
                && distances[currentNode] + weight < distances[nextNode]) {

                distances[nextNode] = distances[currentNode] + weight;
                parents[nextNode] = currentNode;

                if (!inQueue[nextNode]) {
                    enqueue(&pq, nextNode, distances);
                    inQueue[nextNode] = 1;
                } else {
                    decreaseKey(&pq, nextNode, distances);
                }
            }
        }

    }
    free(pq.array);
    free(inQueue);

    if (distances[to] == MAX) {
        if (*first == 0) {
            printf("search failed");
            *first = 1;
        } else {
            printf("\nsearch failed");
        }
        return;
    }

    int *path = malloc(g->nymNodes * sizeof(int));
    int path_length = 0;
    int current = to;
    while (current != -1) {
        path[path_length++] = current;
        current = parents[current];
    }


    int tufirst = 1;
    if (*first == 0) {
        printf("%d: [", distances[to]);
        for (int i = path_length - 1; i >= 0; --i) {
            if (tufirst) {
                printf("%d", path[i]);
                tufirst = 0;
            } else {
                printf(", %d", path[i]);
            }
        }
        printf("]");
        *first = 1;
    } else {
        printf("\n");
        printf("%d: [", distances[to]);
        for (int i = path_length - 1; i >= 0; --i) {
            if (tufirst) {
                printf("%d", path[i]);
                tufirst = 0;
            } else {
                printf(", %d", path[i]);
            }
        }
        printf("]");
    }

    free(path);
}
void delete_edge(Graph* g, int from_node, int to_node, int *first) {

    if (from_node < 0 || from_node >= g->nymNodes || to_node < 0 || to_node >= g->nymNodes || from_node==to_node) {
        if (*first == 0) {
            printf("delete %d %d failed", from_node, to_node);
            *first = 1;
        } else {
            printf("\ndelete %d %d failed", from_node, to_node);
        }
        return;
    }


    Node* current = g->adjaencyList[from_node];
    Node* prev = NULL;
    while (current != NULL && current->verhol != to_node) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        if (*first == 0) {
            printf("delete %d %d failed", from_node, to_node);
            *first = 1;
        } else {
            printf("\ndelete %d %d failed", from_node, to_node);
        }
        return;
    }


    if (prev == NULL) {
        g->adjaencyList[from_node] = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);

    current = g->adjaencyList[to_node];
    prev = NULL;
    while (current != NULL && current->verhol != from_node) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        exit(-1);
    }
    if (prev == NULL) {
        g->adjaencyList[to_node] = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
}

void update_edge(Graph* g, int from_node, int to_node, int weight, int *first) {

    if (from_node < 0 || from_node >= g->nymNodes || to_node < 0 || to_node >= g->nymNodes|| to_node==from_node) {
        if (*first == 0) {
            printf("update %d %d failed", from_node, to_node);
            *first = 1;
        } else {
            printf("\nupdate %d %d failed", from_node, to_node);
        }
        return;
    }


    Node* current = g->adjaencyList[from_node];
    while (current != NULL && current->verhol != to_node) {
        current = current->next;
    }

    if (current == NULL ) {
        if (*first == 0) {
            printf("update %d %d failed", from_node, to_node);
            *first = 1;
        } else {
            printf("\nupdate %d %d failed", from_node, to_node);
        }
        return;
    }
    if (current->weight+weight<0) {
        if (*first == 0) {
            printf("update %d %d failed", from_node, to_node);
            *first = 1;
        } else {
            printf("\nupdate %d %d failed", from_node, to_node);
        }
        return;
    }
    current->weight = current->weight+weight;


    current = g->adjaencyList[to_node];
    while (current != NULL && current->verhol != from_node) {
        current = current->next;
    }
    if (current == NULL){
        exit(-1);
    }
    current->weight += weight;
}

void destroyGraph(Graph* graph) {
    for (int i = 0; i < graph->nymNodes; ++i) {
        Node* current = graph->adjaencyList[i];
        while (current != NULL) {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->adjaencyList);
    free(graph);
}


Graph* createGraph(int numNodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->nymNodes = numNodes;


    graph->adjaencyList = (Node**)malloc(numNodes * sizeof(Node*));
    for (int i = 0; i < numNodes; ++i) {
        graph->adjaencyList[i] = NULL;
    }

    return graph;
}
Node* createNode(int v, int w) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) exit(EXIT_FAILURE);

    newNode->verhol = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}
void add_edge(Graph* graph, int from, int to, int weight, int *first) {

    if (from==to||from < 0 || from >= graph->nymNodes || to < 0 || to >= graph->nymNodes || weight<0) {
        if (*first == 0) {
            printf("insert %d %d failed", from, to);
            *first = 1;
        } else {
            printf("\ninsert %d %d failed", from, to);
        }
        return;
    }
    Node* current = graph->adjaencyList[from];
    while (current != NULL) {
        if (current->verhol == to){
            if (*first == 0) {
                printf("insert %d %d failed", from, to);
                *first = 1;
            } else {
                printf("\ninsert %d %d failed", from, to);
            }
            return;
        }
        current = current->next;
    }

    Node* newNode = createNode(to, weight);
    newNode->next = graph->adjaencyList[from];
    graph->adjaencyList[from] = newNode;

    newNode = createNode(from, weight);
    newNode->next = graph->adjaencyList[to];
    graph->adjaencyList[to] = newNode;

}
void add_edge_new(Graph* graph, int from, int to, int weight, int *first) {

    if (from==to||from < 0 || from >= graph->nymNodes || to < 0 || to >= graph->nymNodes || weight<0) {
        if (*first == 0) {
            printf("insert %d %d failed", from, to);
            *first = 1;
        } else {
            printf("\ninsert %d %d failed", from, to);
        }
        return;
    }


    Node* newNode = createNode(to, weight);
    newNode->next = graph->adjaencyList[from];
    graph->adjaencyList[from] = newNode;

    newNode = createNode(from, weight);
    newNode->next = graph->adjaencyList[to];
    graph->adjaencyList[to] = newNode;

}
int main() {
    int first=0;
    Graph *g =NULL;

    int n,m;
    scanf("%d %d",&n,&m);

    g = createGraph(n);

    for(int i = 0; i < m; i++){
        int from, to, value;
        char garbage = ' ';
        scanf(" %c %d %c %d %c %d %c", &garbage, &from, &garbage ,&to,&garbage ,&value,&garbage);
        if(from > n || from < 0 || to > n || to < 0 || from == to){
            if (first == 0) {
                printf("insert %d %d failed", from, to);
                first = 1;
            } else {
                printf("\ninsert %d %d failed", from, to);
            }
        }else {
            add_edge_new(g, from, to, value,&first);
        }
    }

    char bukva;
    int from, exit, value;
    while(scanf(" %c", &bukva)!= EOF){
        switch (bukva) {
            case 'i':
                scanf("%d %d %d",&from, &exit, &value);

                add_edge(g, from, exit, value,&first);
                break;
            case 'd':
                scanf("%d %d",&from, &exit);

                delete_edge(g, from, exit,&first);
                break;
            case 's':
                scanf("%d %d",&from, &exit);

                search_Vrchol(g, from, exit,&first);
                break;
            case 'u':
                scanf("%d %d %d",&from, &exit, &value);
                update_edge(g, from, exit, value,&first);
                break;

            default:
                break;
        }
    }
    destroyGraph(g);
    return 0;
}

