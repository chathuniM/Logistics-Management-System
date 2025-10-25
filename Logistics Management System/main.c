
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
void add_city() {
    if (num_cities >= MAX_CITIES) {
        printf("Maximum cities reached.\n");
        return;
    }
    printf("Enter city name: ");
    char name[50];
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    for (int i = 0; i < num_cities; i++) {
        if (strcmp(cities[i], name) == 0) {
            printf("City already exists.\n");
            return;
        }
    }
    strcpy(cities[num_cities], name);
    for (int i = 0; i <= num_cities; i++) {
        distance[num_cities][i] = (num_cities == i) ? 0 : INF;
        distance[i][num_cities] = distance[num_cities][i];
    }
    num_cities++;
    floyd_warshall();
    printf("City added.\n");
}

void rename_city() {
    printf("Enter current city name: ");
    char old_name[50];
    fgets(old_name, 50, stdin);
    old_name[strcspn(old_name, "\n")] = 0;
    int index = find_city_index(old_name);
    if (index == -1) {
        printf("City not found.\n");
        return;
    }
    printf("Enter new city name: ");
    char new_name[50];
    fgets(new_name, 50, stdin);
    new_name[strcspn(new_name, "\n")] = 0;
    for (int i = 0; i < num_cities; i++) {
        if (strcmp(cities[i], new_name) == 0) {
            printf("New name already exists.\n");
            return;
        }
    }
    strcpy(cities[index], new_name);
    printf("City renamed.\n");
}
void remove_city() {
    printf("Enter city name to remove: ");
    char name[50];
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = 0;
    int index = find_city_index(name);
    if (index == -1) {
        printf("City not found.\n");
        return;
    }
    for (int i = index; i < num_cities - 1; i++) {
        strcpy(cities[i], cities[i + 1]);
        for (int j = 0; j < num_cities; j++) {
            distance[i][j] = distance[i + 1][j];
            distance[j][i] = distance[j][i + 1];
        }
    }
    num_cities--;
    floyd_warshall();
    printf("City removed.\n");
}

int find_city_index(char* name) {
    for (int i = 0; i < num_cities; i++) {
        if (strcmp(cities[i], name) == 0) return i;
    }
    return -1;
}
void edit_distance() {
    printf("Enter source city name: ");
    char src[50];
    fgets(src, 50, stdin);
    src[strcspn(src, "\n")] = 0;
    int i = find_city_index(src);
    if (i == -1) {
        printf("Source city not found.\n");
        return;
    }
    printf("Enter destination city name: ");
    char dest[50];
    fgets(dest, 50, stdin);
    dest[strcspn(dest, "\n")] = 0;
    int j = find_city_index(dest);
    if (j == -1) {
        printf("Destination city not found.\n");
        return;
    }
    if (i == j) {
        printf("Cannot set distance to itself.\n");
        return;
    }
    printf("Enter distance (km): ");
    int d;
    scanf("%d", &d);
    getchar();
    distance[i][j] = d;
    distance[j][i] = d;
    floyd_warshall();
    printf("Distance updated.\n");
}

void display_distances() {
    printf("Distance Table:\n");
    printf("%-15s", " ");
    for (int i = 0; i < num_cities; i++) {
        printf("%-15s", cities[i]);
    }
    printf("\n");
    for (int i = 0; i < num_cities; i++) {
        printf("%-15s", cities[i]);
        for (int j = 0; j < num_cities; j++) {
            if (distance[i][j] == INF) {
                printf("%-15s", "-");
            } else {
                printf("%-15d", distance[i][j]);
            }
        }
        printf("\n");
    }
}

void delivery_request() {
    if (num_deliveries >= MAX_DELIVERIES) {
        printf("Maximum deliveries reached.\n");
        return;
    }
    printf("Enter source city name: ");
    char src[50];
    fgets(src, 50, stdin);
    src[strcspn(src, "\n")] = 0;
    int source = find_city_index(src);
    if (source == -1) {
        printf("Source city not found.\n");
        return;
    }
    printf("Enter destination city name: ");
    char dest[50];
    fgets(dest, 50, stdin);
    dest[strcspn(dest, "\n")] = 0;
    int destination = find_city_index(dest);
    if (destination == -1) {
        printf("Destination city not found.\n");
        return;
    }
    if (source == destination) {
        printf("Source and destination cannot be the same.\n");
        return;
    }
    printf("Enter weight (kg): ");
    int weight;
    scanf("%d", &weight);
    getchar();
    printf("Select vehicle type (1=Van, 2=Truck, 3=Lorry): ");
    int v_type;
    scanf("%d", &v_type);
    getchar();
    if (v_type < 1 || v_type > 3) {
        printf("Invalid vehicle type.\n");
        return;
    }
    v_type--;
    if (weight > vehicles[v_type].capacity) {
        printf("Weight exceeds vehicle capacity.\n");
        return;
    }
    float D = (float)min_dist[source][destination];
    if (D >= INF) {
        printf("No path available.\n");
        return;
    }
    float R = (float)vehicles[v_type].rate_per_km;
    float S = (float)vehicles[v_type].avg_speed;
    float E = (float)vehicles[v_type].fuel_eff;
    float W = (float)weight;

    float delivery_cost = D * R * (1 + W / 10000);
    float time = D / S;
    float fuel_used = D / E;
    float fuel_cost = fuel_used * FUEL_PRICE;
    float total_cost = delivery_cost + fuel_cost;
    float profit = delivery_cost * 0.25;
    float customer_charge = total_cost + profit;

    printf("======================================================\n");
    printf("DELIVERY COST ESTIMATION\n");
    printf("------------------------------------------------------\n");
    printf("From: %s\n", cities[source]);
    printf("To: %s\n", cities[destination]);
    printf("Minimum Distance: %.0f km\n", D);
    printf("Vehicle: %s\n", vehicles[v_type].name);
    printf("Weight: %d kg\n", weight);
    printf("------------------------------------------------------\n");
    printf("Base Cost: %.0f × %d × (1 + %d/10000) = %.2f LKR\n", D, vehicles[v_type].rate_per_km, weight, delivery_cost);
    printf("Fuel Used: %.2f L\n", fuel_used);
    printf("Fuel Cost: %.2f LKR\n", fuel_cost);
    printf("Operational Cost: %.2f LKR\n", total_cost);
    printf("Profit: %.2f LKR\n", profit);
    printf("Customer Charge: %.2f LKR\n", customer_charge);
    printf("Estimated Time: %.2f hours\n", time);
    printf("======================================================\n");


    deliveries[num_deliveries].source = source;
    deliveries[num_deliveries].dest = destination;
    deliveries[num_deliveries].weight = weight;
    deliveries[num_deliveries].vehicle_type = v_type;
    deliveries[num_deliveries].min_distance = D;
    deliveries[num_deliveries].delivery_cost = delivery_cost;
    deliveries[num_deliveries].fuel_used = fuel_used;
    deliveries[num_deliveries].fuel_cost = fuel_cost;
    deliveries[num_deliveries].total_cost = total_cost;
    deliveries[num_deliveries].profit = profit;
    deliveries[num_deliveries].customer_charge = customer_charge;
    deliveries[num_deliveries].time = time;
    num_deliveries++;
}



