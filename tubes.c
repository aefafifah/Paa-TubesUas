#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct Edge {
    int source, destination;
    int distance; // jarak
    int cost;     // biaya
};

void printPath(int* predecessor, int current) {
    if (predecessor[current] == -1) {
        printf("%d", current + 1);
        return;
    }
    printPath(predecessor, predecessor[current]);
    printf(" -> %d", current + 1);
}

void BellmanFord(struct Edge* edges, int V, int E, int source, int destination, const char* priority) {
    int* distance = (int*)malloc(V * sizeof(int));
    int* cost = (int*)malloc(V * sizeof(int));
    int* predecessor = (int*)malloc(V * sizeof(int));

    // Inisialisasi jarak, biaya, dan pendahulu dari source ke semua kota
    for (int i = 0; i < V; i++) {
        distance[i] = INT_MAX;
        cost[i] = INT_MAX;
        predecessor[i] = -1;
    }
    distance[source] = 0;
    cost[source] = 0;

    // Lakukan relaksasi pada setiap tepi sebanyak (jumlah_kota - 1) kali
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].source;
            int v = edges[j].destination;
            int dist = edges[j].distance;
            int cst = edges[j].cost;

            if (priority[0] == 'd') {
                // Perbarui nilai jarak berdasarkan prioritas jarak
                if (distance[u] != INT_MAX && distance[u] + dist < distance[v]) {
                    distance[v] = distance[u] + dist;
                    cost[v] = cost[u] + cst; // tetap perbarui biaya
                    predecessor[v] = u;
                    printf("Iteration %d: Updated Distance[%d] = %d, Cost[%d] = %d\n", i, v, distance[v], v, cost[v]);
                }
            } else if (priority[0] == 'c') {
                // Perbarui nilai biaya berdasarkan prioritas biaya
                if (cost[u] != INT_MAX && cost[u] + cst < cost[v]) {
                    cost[v] = cost[u] + cst;
                    distance[v] = distance[u] + dist; // tetap perbarui jarak
                    predecessor[v] = u;
                    printf("Iteration %d: Updated Distance[%d] = %d, Cost[%d] = %d\n", i, v, distance[v], v, cost[v]);
                }
            }
        }
    }

    // Tampilkan hasil berdasarkan prioritas
    if ((priority[0] == 'd' && distance[destination] == INT_MAX) || 
        (priority[0] == 'c' && cost[destination] == INT_MAX)) {
        printf("There is no path from city %d to city %d.\n", source + 1, destination + 1);
    } else {
        printf("Path: ");
        printPath(predecessor, destination);
        printf("\n");
        
        if (priority[0] == 'd') {
            printf("Total distance: %d km\n", distance[destination]);
            printf("Total cost: %d\n", cost[destination]);
        } else if (priority[0] == 'c') {
            printf("Total cost: %d\n", cost[destination]);
            printf("Total distance: %d km\n", distance[destination]);
        }
    }

    // Hitung jarak maksimal dan biaya maksimal
    int max_distance = 0;
    int max_cost = 0;
    for (int i = 0; i < V; i++) {
        if (distance[i] != INT_MAX && distance[i] > max_distance) {
            max_distance = distance[i];
        }
        if (cost[i] != INT_MAX && cost[i] > max_cost) {
            max_cost = cost[i];
        }
    }
    printf("Max distance to any city: %d km\n", max_distance);
    printf("Max cost to any city: %d\n", max_cost);

    free(distance);
    free(cost);
    free(predecessor);
}

int main() {
    int V = 10; // jumlah kota
    int E = 15; // jumlah tepi
    struct Edge edges[] = {
        {0, 1, 10, 5}, {0, 2, 20, 6}, {1, 3, 30, 7}, {1, 4, 40, 10}, {2, 5, 50, 5},
        {2, 5, 10, 7}, {3, 6, 20, 8}, {4, 6, 30, 10}, {4, 7, 40, 6}, {5, 7, 50, 7},
        {6, 8, 10, 5}, {6, 9, 20, 0}, {7, 8, 30, 6}, {7, 9, 40, 5}, {8, 9, 50, 3}
    };

    int source, destination;
    char priority[20];

    printf("Enter start city (1-10): ");
    scanf("%d", &source);
    printf("Enter end city (1-10): ");
    scanf("%d", &destination);
    printf("Enter priority (distance/cost): ");
    scanf("%s", priority);

    if (source < 1 || source > 10 || destination < 1 || destination > 10) {
        printf("Invalid city index\n");
        return 1;
    }

    if (strcmp(priority, "cost") == 0 || strcmp(priority, "distance") == 0) {
        BellmanFord(edges, V, E, source - 1, destination - 1, priority);
    } else {
        printf("Invalid priority\n");
        return 1;
    }

    return 0;
}
