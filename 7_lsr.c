#include <stdio.h>

#define INF 999
#define MAX 10

int main() {
    int n, i, j, source;
    int cost[MAX][MAX], distance[MAX], visited[MAX], prev[MAX];

    /* 1. Input number of nodes */
    printf("Enter number of nodes: ");
    scanf("%d", &n);

    /* 2. Input cost matrix */
    printf("Enter cost matrix (enter 999 for no link):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }
    /* Print cost matrix */
    printf("\nCost Matrix:\n\n");
    printf("    ");
    for (i = 0; i < n; i++)
        printf("%4d", i);
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%4d", i);
        for (j = 0; j < n; j++) {
            if (cost[i][j] == INF)
                printf("%5s", " ∞");
            else
                printf("%4d", cost[i][j]);
        }
        printf("\n");
    }

    /* 3. Input source router */
    printf("Enter source node: ");
    scanf("%d", &source);

    /* 4. Initialization */
    for (i = 0; i < n; i++) {
        distance[i] = cost[source][i];
        visited[i] = 0;
        prev[i] = source;
    }

    distance[source] = 0;
    visited[source] = 1;

    /* 5. Dijkstra's Algorithm */
    for (i = 1; i < n; i++) {
        int min = INF, u = -1;

        /* Find minimum distance node */
        for (j = 0; j < n; j++) {
            if (!visited[j] && distance[j] < min) {
                min = distance[j];
                u = j;
            }
        }

        visited[u] = 1;

        /* Update distances */
        for (j = 0; j < n; j++) {

            if (!visited[j] && distance[u] + cost[u][j] < distance[j]) {
                distance[j] = distance[u] + cost[u][j];
                prev[j] = u;
            }
        }
    }

    /* 6. Display shortest paths */
    printf("\nShortest paths from node %d:\n", source);
    for (i = 0; i < n; i++) {
        if (i != source) {
            printf("To %d cost = %d Path: %d", i, distance[i], i);
            j = i;
            while (j != source) {
                j = prev[j];
                printf(" <- %d", j);
            }
            printf("\n");
        }
    }

    return 0;
}

