#include "topik8.h"

#include <stdio.h>
#include <stdlib.h>

static int load_adjacency_matrix(int matrix[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES], int *vertex_count) {
    FILE *file = fopen(GRAPH_FILE, "r");
    int i;
    int j;

    if (file == NULL || fscanf(file, "%d", vertex_count) != 1 ||
        *vertex_count <= 0 || *vertex_count > MAX_GRAPH_VERTICES) {
        if (file != NULL) fclose(file);
        return 0;
    }
    for (i = 0; i < *vertex_count; i++) {
        for (j = 0; j < *vertex_count; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fclose(file);
                return 0;
            }
        }
    }
    fclose(file);
    return 1;
}

static void display_vertex(const AppState *app, int vertex) {
    if (vertex >= 0 && (size_t)vertex < app->package_count) {
        const TicketPackage *package = &app->packages[vertex];
        printf("%s-P%d(ID %d)", package->category, package->presale, package->id);
    } else {
        printf("Vertex-%d", vertex + 1);
    }
}

static void dfs_visit(const AppState *app,
                      int matrix[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
                      int vertex_count, int current, int visited[]) {
    int neighbor;
    visited[current] = 1;
    display_vertex(app, current);
    printf("  ");

    for (neighbor = 0; neighbor < vertex_count; neighbor++) {
        if (matrix[current][neighbor] != 0 && !visited[neighbor]) {
            dfs_visit(app, matrix, vertex_count, neighbor, visited);
        }
    }
}

static void run_bfs(const AppState *app,
                    int matrix[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES],
                    int vertex_count, int start) {
    int queue[MAX_GRAPH_VERTICES];
    int visited[MAX_GRAPH_VERTICES] = {0};
    int front = 0;
    int rear = 0;
    int current;
    int neighbor;

    queue[rear++] = start;
    visited[start] = 1;
    printf("BFS: ");

    while (front < rear) {
        current = queue[front++];
        display_vertex(app, current);
        printf("  ");

        for (neighbor = 0; neighbor < vertex_count; neighbor++) {
            if (matrix[current][neighbor] != 0 && !visited[neighbor]) {
                visited[neighbor] = 1;
                queue[rear++] = neighbor;
            }
        }
    }
    printf("\n");
}

void graph_menu(const AppState *app) {
    int matrix[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES] = {{0}};
    int visited[MAX_GRAPH_VERTICES] = {0};
    int vertex_count = 0;
    int package_id;
    int start;
    int choice;

    if (!load_adjacency_matrix(matrix, &vertex_count)) {
        printf("Data graph tidak dapat dibaca dari %s.\n", GRAPH_FILE);
        return;
    }
    if ((size_t)vertex_count > app->package_count) {
        printf("Jumlah vertex graph melebihi jumlah paket. Periksa zona.txt.\n");
        return;
    }

    printf("\n=== GRAPH RELASI TAHAP PRE-SALE ===\n");
    printf("Setiap vertex adalah paket tiket. Sisi berarah menunjukkan tahap presale berikutnya.\n");
    printf("Paket yang tersedia sebagai titik awal:\n");
    for (start = 0; start < vertex_count; start++) {
        display_vertex(app, start);
        printf("\n");
    }

    package_id = read_int_range("Masukkan ID paket sebagai vertex awal: ", 1, 9999);
    start = find_package_index_linear(app, package_id);
    if (start < 0 || start >= vertex_count) {
        printf("ID paket tersebut tidak memiliki vertex pada graph.\n");
        return;
    }

    printf("1. DFS (Depth First Search)\n2. BFS (Breadth First Search)\n");
    choice = read_int_range("Pilih traversal: ", 1, 2);
    if (choice == 1) {
        printf("DFS: ");
        dfs_visit(app, matrix, vertex_count, start, visited);
        printf("\n");
    } else {
        run_bfs(app, matrix, vertex_count, start);
    }
}
