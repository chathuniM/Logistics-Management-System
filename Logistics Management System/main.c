
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

