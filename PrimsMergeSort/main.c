#include <stdio.h>
#include <stdlib.h>

#define MAX 9223372036854775805


typedef struct Node {
    int verhol;
    long weight;
    struct Node* next;
} Node;


typedef struct mygraph{
    int nymNodes;
    Node** adjaencyList;
} Graph;

typedef struct {
    int vertex;
    long key;
} PQNode;

typedef struct {
    PQNode* array;
    int size;
    int capacity;
    int* pos;
} PriorityQueue;
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1][2], R[n2][2];


    for (i = 0; i < n1; i++) {
        L[i][0] = arr[2 * (l + i)];
        L[i][1] = arr[2 * (l + i) + 1];
    }
    for (j = 0; j < n2; j++) {
        R[j][0] = arr[2 * (m + 1) + 2 * j];
        R[j][1] = arr[2 * (m + 1) + 2 * j + 1];
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i][0] < R[j][0] || (L[i][0] == R[j][0] && L[i][1] < R[j][1])) {
            arr[2 * k] = L[i][0];
            arr[2 * k + 1] = L[i][1];
            i++;
        } else {
            arr[2 * k] = R[j][0];
            arr[2 * k + 1] = R[j][1];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[2 * k] = L[i][0];
        arr[2 * k + 1] = L[i][1];
        i++;
        k++;
    }

    while (j < n2) {
        arr[2 * k] = R[j][0];
        arr[2 * k + 1] = R[j][1];
        j++;
        k++;
    }
}
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void destroyPriorityQueue(PriorityQueue* pq) {
    free(pq->pos);
    free(pq->array);
    free(pq);
}
PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->pos = (int*)malloc(capacity * sizeof(int));
    pq->array = (PQNode*)malloc(capacity * sizeof(PQNode));
    return pq;
}
int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

void swapPQNode(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}
void minHeapify(PriorityQueue* pq, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < pq->size && pq->array[left].key <= pq->array[smallest].key)
        smallest = left;

    if (right < pq->size && pq->array[right].key <= pq->array[smallest].key)
        smallest = right;

    if (smallest != idx) {
        pq->pos[pq->array[smallest].vertex] = idx;
        pq->pos[pq->array[idx].vertex] = smallest;

        swapPQNode(&pq->array[smallest], &pq->array[idx]);
        minHeapify(pq, smallest);
    }
}
int dequeue(PriorityQueue* pq) {
    if (isEmpty(pq))
        return -1;

    PQNode root = pq->array[0];
    PQNode* lastNode = &pq->array[pq->size - 1];
    pq->array[0] = *lastNode;

    pq->pos[root.vertex] = pq->size - 1;
    pq->pos[lastNode->vertex] = 0;

    --pq->size;
    minHeapify(pq, 0);

    return root.vertex;
}
void enqueue(PriorityQueue* pq, int vertex, long key) {
    if (pq->size == pq->capacity) {
        return;
    }


    int index = pq->size;
    pq->array[index].vertex = vertex;
    pq->array[index].key = key;
    pq->pos[vertex] = index;
    pq->size++;

    int i = pq->size - 1;
    while (i > 0 && pq->array[(i - 1) / 2].key >= pq->array[i].key) {
        swapPQNode(&pq->array[i], &pq->array[(i - 1) / 2]);
        pq->pos[pq->array[i].vertex] = i;
        pq->pos[pq->array[(i - 1) / 2].vertex] = (i - 1) / 2;
        i = (i - 1) / 2;
    }
}
void decreaseKey(PriorityQueue* pq, int vertex, long key) {
    int i = pq->pos[vertex];
    pq->array[i].key = key;

    while (i && pq->array[i].key <= pq->array[(i - 1) / 2].key) {
        pq->pos[pq->array[i].vertex] = (i - 1) / 2;
        pq->pos[pq->array[(i - 1) / 2].vertex] = i;

        swapPQNode(&pq->array[i], &pq->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void primMST(Graph* g, int root,int *first) {
    if (root < 0 || root >= g->nymNodes||g->adjaencyList[root]==NULL) {
        if (*first == 0) {
            printf("search %d failed", root);
            *first = 1;
        } else {
            printf("\nsearch %d failed", root);
        }
        return;
    }

    int parent[g->nymNodes];
    long key[g->nymNodes];

    for (int i = 0; i < g->nymNodes; ++i) {
        parent[i] = -1;
        key[i] = MAX;
    }
    int *inQueue = calloc(g->nymNodes,sizeof(int));
    PriorityQueue* pq = createPriorityQueue(g->nymNodes);
    key[root]=0;
    enqueue(pq, root, key[root]);


    int *inMST = calloc(g->nymNodes, sizeof(int));
    int numIslandVertices=0;
    while (!isEmpty(pq)) {
        int u = dequeue(pq);
        inMST[u]=1;

        Node* current = g->adjaencyList[u];
        while (current != NULL) {
            int v = current->verhol;
            long weight = current->weight;

            if (!inMST[v] && weight < key[v]) {
                if (parent[v] == -1){
                    numIslandVertices++;
                }
                if (parent[u] != v){
                    parent[v] = u;
                    key[v] = weight;
                }


                if (!inQueue[v]) {
                    enqueue(pq, v, key[v]);
                    inQueue[v] = 1;
                } else {
                    decreaseKey(pq, v, key[v]);
                }

            }
            current = current->next;
        }

    }
    free(inMST);

    destroyPriorityQueue(pq);
    free(inQueue);



    long totalWeight = 0;
    int arrForOtcov[(numIslandVertices-1)*2];
    int l=0;
    for (int i = 0; i < g->nymNodes; ++i) {
        if (parent[i] != -1) {
            totalWeight += key[i];
            if (i>parent[i]){
                arrForOtcov[2*l]=parent[i];
                arrForOtcov[2*l+1]=i;
            } else{
                arrForOtcov[2*l]=i;
                arrForOtcov[2*l+1]=parent[i];
            }
            l++;
        }
    }


    mergeSort(arrForOtcov, 0, l - 1);


    if (*first==0){
        *first=1;
    } else{
        printf("\n");
    }
    printf("%ld: [", totalWeight);
    int firstEdgePrinted = 0;
    for (int i = 0; i < l; ++i) {
        if (firstEdgePrinted == 0) {
            printf("(%d, %d)", arrForOtcov[2*i],arrForOtcov[2*i+1]);
            firstEdgePrinted = 1;
        } else {
            printf(", (%d, %d)", arrForOtcov[2*i],arrForOtcov[2*i+1]);
        }
    }
    printf("]");

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

void update_edge(Graph* g, int from_node, int to_node, long weight, int *first) {

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
Node* createNode(int v, long w) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) exit(EXIT_FAILURE);

    newNode->verhol = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}
void add_edge(Graph* graph, int from, int to, long weight, int *first) {

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
void printGraph(Graph* graph) {
    printf("Graph:\n");
    for (int i = 0; i < graph->nymNodes; ++i) {
        Node* current = graph->adjaencyList[i];
        printf("%d -> ", i);
        while (current != NULL) {
            printf("(%d, %d) ", current->verhol, current->weight);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    int first=0;
    Graph *g =NULL;


    int n;
    scanf("%d",&n);
    getchar();

    g = createGraph(n);

    char bukva;
    int from, to;
    long value;
    while(scanf("%c", &bukva)!= EOF){
        switch (bukva) {
            case '(':
                scanf("%d, %d, %ld)", &from, &to, &value);
                add_edge(g, from, to, value, &first);
                break;
            case 'i':
                scanf("%d %d %ld",&from, &to, &value);

                add_edge(g, from, to, value,&first);
                break;
            case 'd':
                scanf("%d %d",&from, &to);

                delete_edge(g, from, to,&first);
                break;
            case 's':
                scanf("%d",&from);

                primMST(g, from,&first);
                break;
            case 'u':
                scanf("%d %d %ld",&from, &to, &value);
                update_edge(g, from, to, value,&first);
                break;
            case 'o':
                printGraph(g);
                break;
            default:
                break;
        }
    }
    destroyGraph(g);
    return 0;
}

