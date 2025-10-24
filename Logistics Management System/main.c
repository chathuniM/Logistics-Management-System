
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define INF 999999
#define FUEL_PRICE 310.0

int main(){
printf("Logistics Management system\n");
return 0;
}

struct Vehicle {
    char name[10];
    int capacity;
    int rate_per_km;
    int avg_speed;
    int fuel_eff;
};


struct Delivery {
    int source;
    int dest;
    int weight;
    int vehicle_type;
    float min_distance;
    float delivery_cost;
    float fuel_used;
    float fuel_cost;
    float total_cost;
    float profit;
    float customer_charge;
    float time;
};

char cities[MAX_CITIES][50];
int num_cities = 0;
int distance[MAX_CITIES][MAX_CITIES];
int min_dist[MAX_CITIES][MAX_CITIES];
struct Vehicle vehicles[3] = {
    {"Van", 1000, 30, 60, 12},
    {"Truck", 5000, 40, 50, 6},
    {"Lorry", 10000, 80, 45, 4}
};

struct Delivery deliveries[MAX_DELIVERIES];
int num_deliveries = 0;

void initialize_distances();
void floyd_warshall();
void load_routes();
void save_routes();
void load_deliveries();
void save_deliveries();
void add_city();
void rename_city();
void remove_city();
void edit_distance();
void display_distances();
void delivery_request();
void reports();
int find_city_index(char* name);
void display_menu();
void city_management_menu();
void distance_management_menu();
void initialize_distances() {
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            distance[i][j] = (i == j) ? 0 : INF;
        }
    }
}

void floyd_warshall() {
    memcpy(min_dist, distance, sizeof(distance));
    for (int k = 0; k < num_cities; k++) {
        for (int i = 0; i < num_cities; i++) {
            for (int j = 0; j < num_cities; j++) {
                if (min_dist[i][k] + min_dist[k][j] < min_dist[i][j]) {
                    min_dist[i][j] = min_dist[i][k] + min_dist[k][j];
                }
            }
        }
    }
}
void load_routes() {
    FILE *fp = fopen("routes.txt", "r");
    if (fp == NULL) {
        initialize_distances();

        num_cities = 3;
        strcpy(cities[0], "Colombo");
        strcpy(cities[1], "Kandy");
        strcpy(cities[2], "Galle");
        distance[0][1] = distance[1][0] = 120;
        distance[0][2] = distance[2][0] = 115;
        distance[1][2] = distance[2][1] = 230;
        printf("Loaded default cities and distances.\n");
        return;
    }
    initialize_distances();
    fscanf(fp, "%d", &num_cities);
    for (int i = 0; i < num_cities; i++) {
        fscanf(fp, "%s", cities[i]);
    }
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            fscanf(fp, "%d", &distance[i][j]);
        }
    }
    fclose(fp);
    printf("Loaded routes from file.\n");
}

void save_routes() {
    FILE *fp = fopen("routes.txt", "w");
    if (fp == NULL) {
        printf("Error saving routes.\n");
        return;
    }
    fprintf(fp, "%d\n", num_cities);
    for (int i = 0; i < num_cities; i++) {
        fprintf(fp, "%s\n", cities[i]);
    }
    for (int i = 0; i < num_cities; i++) {
        for (int j = 0; j < num_cities; j++) {
            fprintf(fp, "%d ", distance[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("Routes saved successfully.\n");
}
void load_deliveries() {
    FILE *fp = fopen("deliveries.txt", "r");
    if (fp == NULL) return;
    fscanf(fp, "%d", &num_deliveries);
    for (int i = 0; i < num_deliveries; i++) {
        fscanf(fp, "%d %d %d %d %f %f %f %f %f %f %f %f",
               &deliveries[i].source, &deliveries[i].dest, &deliveries[i].weight, &deliveries[i].vehicle_type,
               &deliveries[i].min_distance, &deliveries[i].delivery_cost, &deliveries[i].fuel_used, &deliveries[i].fuel_cost,
               &deliveries[i].total_cost, &deliveries[i].profit, &deliveries[i].customer_charge, &deliveries[i].time);
    }
    fclose(fp);
}

void save_deliveries() {
    FILE *fp = fopen("deliveries.txt", "w");
    if (fp == NULL) {
        printf("Error saving deliveries.\n");
        return;
    }
    fprintf(fp, "%d\n", num_deliveries);
    for (int i = 0; i < num_deliveries; i++) {
        fprintf(fp, "%d %d %d %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                deliveries[i].source, deliveries[i].dest, deliveries[i].weight, deliveries[i].vehicle_type,
                deliveries[i].min_distance, deliveries[i].delivery_cost, deliveries[i].fuel_used, deliveries[i].fuel_cost,
                deliveries[i].total_cost, deliveries[i].profit, deliveries[i].customer_charge, deliveries[i].time);
    }
    fclose(fp);
    printf("Deliveries saved successfully.\n");
}

