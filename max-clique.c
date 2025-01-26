/* Kfir Eitan 328494836 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

void printSolution(int vertices[], int size) {
    // print the vertices
    for (int *ptr = vertices; ptr < vertices + size; ptr++) {
        printf("%d ", *ptr);
    }
    printf("\n");
}

bool isClique(int graph[MAX_VERTICES][MAX_VERTICES], int clique[], int size) {
    
    // pointer to the vertex
    int *ptr1 = clique;
    for (; ptr1 < clique + size; ptr1++) {
        // get the vertex
        const int v = *ptr1;

        // loop unrolling on the rest of the vertices
        int *ptr2 = ptr1 + 1;
        for (; ptr2 < clique + size - 1; ptr2 += 2) {
            // get two other vertices
            const int u1 = *ptr2;
            const int u2 = *(ptr2 + 1);

            // check if the vertices are connected
            if (!(graph[v][u1] | graph[u1][v]) | !(graph[v][u2] | graph[u2][v])) {
                return false;
            }
        }
        for (; ptr2 < clique + size - 1; ptr2++)
        {
            // check the last vertex
            const int u = *ptr2;
            if (!(graph[v][u] | graph[u][v])) {
                return false;
            }
        }     
    }
    
    return true;
}


void generateCombinations(int graph[MAX_VERTICES][MAX_VERTICES], int n, int *clique, int k, int start, int currentSize, int *maxSize, int *maxClique) {

    // if k <= max size so we cant find bigger clique here
    if (k <= *maxSize) {
        return;
    }

    // if remaining vertices + current size < k, we cant find clique of size k
    if (n - start + currentSize < k) {
        return;
    }

    // if current size is equal to k
    if (currentSize == k)
    {
        // if this is clique
        if (isClique(graph, clique, k))
        {
            // change the max size 
            *maxSize = k;
            
            // get pointers to the clique and maxClique
            int *cliquePtr = clique;
            int *maxCliquePtr = maxClique;
            
            // copy the clique to maxClique, loop unrolling
            for (; cliquePtr < clique + k - 1; cliquePtr += 2, maxCliquePtr += 2) {
                *maxCliquePtr = *cliquePtr;
                *(maxCliquePtr + 1) = *(cliquePtr + 1);
            }
            for (; cliquePtr < clique + k; cliquePtr++, maxCliquePtr++) {
                *maxCliquePtr = *cliquePtr;
            }
        }
        return;
    }
    
    // check all the vertices from start to n
    for (int i = start; i < n; i++) {
        
        int *ptr = clique;
        // check if the vertex can be added to the clique, it must be connected to all
        bool canAdd = true;
        for (; ptr < clique + currentSize; ptr++) {
            // if it not connected to all so enter false and break
            if (!(graph[i][*ptr] | graph[*ptr][i])) {
                canAdd = false;
                break;
            }
        }
        if (!canAdd) {
            // if the vertex cant be added to the graph so skip it and move to the next vertex
            continue;
        }
        
        /// add the vertex to the clique
        clique[currentSize] = i;
        generateCombinations(graph, n, clique, k, i + 1, currentSize + 1, maxSize, maxClique);
    }
}

void findMaxClique(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    // allocate the clique, maxClique and create maxSize
    int *clique = (int *)malloc(n * sizeof(int));
    int *maxClique = (int *)malloc(n * sizeof(int));
    int maxSize = 0;

    /**
     * loop unrolling to generate combination of the graph
     * start from the largest possible clique size (n) and go downwards
     */
    int k = n;
    for (; k > 1; k -= 2) {
        generateCombinations(graph, n, clique, k, 0, 0, &maxSize, maxClique);

        // if a clique of size k is found, it is the largest
        if (maxSize == k) {
            break;
        }

        generateCombinations(graph, n, clique, k - 1, 0, 0, &maxSize, maxClique);

        // if a clique of size k - 1 is found, it is the largest
        if (maxSize == k - 1) {
            break;
        }
    }
    for (; k >= 1; --k) {
        generateCombinations(graph, n, clique, k, 0, 0, &maxSize, maxClique);
    }


    // print the max clique
    printf("Clique Members: ");
    printSolution(maxClique, maxSize);
    printf("Size: %d\n", maxSize);

    // free the memory
    free(clique);
    free(maxClique);
}

