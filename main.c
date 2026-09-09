#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 9.81 

typedef struct {
    char type;
    float v_max;
    float gamma;
    float current_ip;
    float total_damage_taken;
    int firings;
    float time_delay;
} Battleship;

typedef struct {
    int id;
    char type;
    float v_max;
    float v_min;
    float angle_max;
    float angle_min;
    float impact_power;
    float gamma;
    int is_destroyed;
    int firings;
    float time_delay;
} EscortShip;

float calculateRange(float velocity, float angle_degrees) {
    float angle_radians = angle_degrees * (M_PI / 180.0);
    return (velocity * velocity * sin(2 * angle_radians)) / G;
}

float calculateDegradedImpact(float IP0, float gamma, int n) {
    return IP0 * exp(-gamma * n);
}

void saveInitialConditions(EscortShip e_ships[], int num_ships, Battleship b) {
    FILE *file = fopen("results.txt", "w");
    if (file != NULL) {
        fprintf(file, "Battleship Type: %c | Max Velocity: %.2f\n", b.type, b.v_max);
        fprintf(file, "--- Escort Ships ---\n");
        for (int i = 0; i < num_ships; i++) {
            fprintf(file, "ID: %d | Type: %c | IP: %.2f | Time Delay: %.2fs\n", 
                    e_ships[i].id, e_ships[i].type, e_ships[i].impact_power, e_ships[i].time_delay);
        }
        fclose(file);
    }
}

void startSimulation() {
    int n_ships, k_points, mode;
    float canvas_size;
    char b_type;
    
    printf("\n--- Advanced Simulation Setup ---\n");
    printf("Enter Battleship type (U, M, R, S): ");
    scanf(" %c", &b_type);
    printf("Enter canvas size: ");
    scanf("%f", &canvas_size);
    printf("Enter number of escort ships: ");
    scanf("%d", &n_ships);
    printf("Enter path iterations (k): ");
    scanf("%d", &k_points);
    printf("Select Mode (1: Basic, 2: Gun Jam, 3: Impact Tracking, 4: Part 2 Time & Degradation): ");
    scanf("%d", &mode);
    
    Battleship b = {b_type, 50.0, 0.05, 1.0, 0.0, 0, 2.5}; 
    
    EscortShip *e_ships = malloc(n_ships * sizeof(EscortShip));
    char types[] = {'A', 'B', 'C', 'D', 'E'};
    float default_ips[] = {0.08, 0.06, 0.07, 0.05, 0.04};
    
    for (int i = 0; i < n_ships; i++) {
        int t_idx = rand() % 5;
        e_ships[i].id = i + 1;
        e_ships[i].type = types[t_idx];
        e_ships[i].v_max = 10.0 + (rand() % 20);
        e_ships[i].angle_max = 45.0;
        e_ships[i].angle_min = (mode == 2) ? 15.0 : 0.0;
        e_ships[i].impact_power = default_ips[t_idx];
        e_ships[i].gamma = 0.1; 
        e_ships[i].is_destroyed = 0;
        e_ships[i].firings = 0;
        e_ships[i].time_delay = 1.5 + (rand() % 3); 
    }
    
    saveInitialConditions(e_ships, n_ships, b);
    FILE *rf = fopen("results.txt", "a");

    for (int iter = 1; iter <= k_points; iter++) {
        printf("\nIteration %d...\n", iter);
        for (int i = 0; i < n_ships; i++) {
            if (e_ships[i].is_destroyed) continue;

            float test_angle = (mode == 2 && e_ships[i].angle_max < e_ships[i].angle_min) ? e_ships[i].angle_min : e_ships[i].angle_max;
            if (calculateRange(e_ships[i].v_max, test_angle) <= canvas_size) {
                
                b.firings++;
                float current_b_ip = (mode == 4) ? calculateDegradedImpact(1.0, b.gamma, b.firings) : 1.0;
                
                if (current_b_ip > 0.5) { 
                    e_ships[i].is_destroyed = 1;
                    printf(" -> Destroyed Ship %d. (B Fired %d times. Reload: %.1fs)\n", e_ships[i].id, b.firings, b.time_delay);
                } else {
                    printf(" -> Battleship impact degraded too much to destroy Ship %d instantly.\n", e_ships[i].id);
                }
            }

            if ((mode == 3 || mode == 4) && !e_ships[i].is_destroyed) {
                e_ships[i].firings++;
                float active_ip = (mode == 4) ? calculateDegradedImpact(e_ships[i].impact_power, e_ships[i].gamma, e_ships[i].firings) : e_ships[i].impact_power;
                b.total_damage_taken += active_ip;
            }
        }
    }
    
    if (mode == 3 || mode == 4) printf("\nTotal Damage on Battleship: %.2f%%\n", b.total_damage_taken * 100.0);
    if (rf) fclose(rf);
    free(e_ships);
}

void showSetup() {
    int setup_choice;
    printf("\n--- Setup Menu ---\n1. Battleship Properties\n2. Escort Ships Settings\n3. Seed Value\n4. Return\nEnter choice: ");
    if (scanf("%d", &setup_choice) != 1) { while (getchar() != '\n'); }
}

void showStatistics() {
    FILE *file = fopen("results.txt", "r");
    if (!file) { printf("\nNo results found.\n"); return; }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) printf("%s", buffer);
    fclose(file);
}

int main() {
    int choice;
    do {
        printf("\n--- Naval Battle Simulator ---\n1. Start Simulation\n2. Instructions\n3. Statistics\n4. Exit\nEnter choice: ");
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); choice = 0; }
        switch(choice) {
            case 1: showSetup(); startSimulation(); break;
            case 2: printf("\nInstructions: Manage firing delays and track power degradation.\n"); break;
            case 3: showStatistics(); break;
            case 4: printf("\nExiting.\n"); break;
            default: printf("\nInvalid choice.\n");
        }
    } while (choice != 4);
    return 0;
}
