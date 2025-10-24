
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define INF 999999
#define FUEL_PRICE 310.0

int main(){
printf("Logistics Management System\n");
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

