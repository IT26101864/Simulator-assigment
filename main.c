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
} EscortShip;

float calculateRange(float velocity, float angle_degrees) {
    float angle_radians = angle_degrees * (M_PI / 180.0);
    float range = (velocity * velocity * sin(2 * angle_radians)) / G;
    return range;
}

void saveInitialConditions(EscortShip e_ships[], int num_ships, Battleship b) {
    FILE *file = fopen("results.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    fprintf(file, "Battleship Type: %c | Max Velocity: %.2f\n", b.type, b.v_max);
    fprintf(file, "--- Escort Ships ---\n");
    for (int i = 0; i < num_ships; i++) {
        fprintf(file, "ID: %d | Type: %c | V_max: %.2f | Impact Power: %.2f\n", 
                e_ships[i].id, e_ships[i].type, e_ships[i].v_max, e_ships[i].impact_power);
    }
    
    fclose(file);
    printf("Initial conditions saved to results.txt\n");
}

void startSimulation() {
    int n_ships, k_points;
    float canvas_size;
    char b_type;
    int simulation_mode;
    
    printf("\n--- Advanced Simulation Setup ---\n");
    printf("Enter Battleship type notation (U, M, R, or S): ");
    scanf(" %c", &b_type);
    
    printf("Enter canvas grid size (e.g., 100.0): ");
    scanf("%f", &canvas_size);
    
    printf("Enter number of escort ships (N): ");
    scanf("%d", &n_ships);

    printf("Enter number of path iterations (k for Part 1-B): ");
    scanf("%d", &k_points);

    printf("Select Simulation Mode:\n");
    printf("1. Part 1-A / 1-B (Standard Path Simulation)\n");
    printf("2. Part 1-B Simulation 2 (Gun Jammed: Angle restricted to theta_min to 90)\n");
    printf("3. Part 1-C (Cumulative Partial Impact Power Tracking)\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &simulation_mode);
    
    Battleship b = {b_type, 50.0, 0.01, 1.0, 0.0};
    
    EscortShip *e_ships = malloc(n_ships * sizeof(EscortShip));
    if (e_ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    char types[] = {'A', 'B', 'C', 'D', 'E'};
    float default_ips[] = {0.08, 0.06, 0.07, 0.05, 0.04}; // Part 1-C impact powers
    
    for (int i = 0; i < n_ships; i++) {
        int type_idx = rand() % 5;
        e_ships[i].id = i + 1;
        e_ships[i].type = types[type_idx];
        e_ships[i].v_max = 10.0 + (rand() % 20);
        e_ships[i].angle_max = 45.0;
        e_ships[i].angle_min = (simulation_mode == 2) ? 15.0 : 0.0; // Part 1-B gun jam restriction
        e_ships[i].impact_power = default_ips[type_idx];
        e_ships[i].is_destroyed = 0;
    }
    
    saveInitialConditions(e_ships, n_ships, b);
    
    FILE *results_file = fopen("results.txt", "a");
    if (results_file != NULL) {
        fprintf(results_file, "\n--- Simulation Execution Log ---\n");
    }

    // Simulation Loop across k iterations (Part 1-B & 1-C)
    for (int iter = 1; iter <= k_points; iter++) {
        printf("\nExecuting Path Iteration %d / %d...\n", iter, k_points);
        
        for (int i = 0; i < n_ships; i++) {
            if (e_ships[i].is_destroyed) continue;

            float test_angle = e_ships[i].angle_max;
            if (simulation_mode == 2 && test_angle < e_ships[i].angle_min) {
                test_angle = e_ships[i].angle_min; // Apply gun jam constraint
            }

            float sample_range = calculateRange(e_ships[i].v_max, test_angle);
            
            if (sample_range <= canvas_size) {
                e_ships[i].is_destroyed = 1; // Battleship destroys E ship
                printf(" -> Battleship destroyed Escort Ship ID %d (Type %c)\n", e_ships[i].id, e_ships[i].type);
                if (results_file != NULL) {
                    fprintf(results_file, "Iteration %d: Escort Ship %d destroyed.\n", iter, e_ships[i].id);
                }
            }

            // Part 1-C: Accumulate damage on Battleship if mode 3 is active
            if (simulation_mode == 3 && !e_ships[i].is_destroyed) {
                b.total_damage_taken += e_ships[i].impact_power;
            }
        }
    }

    if (simulation_mode == 3) {
        printf("\nCumulative Impact on Battleship: %.2f%%\n", b.total_damage_taken * 100.0);
        if (results_file != NULL) {
            fprintf(results_file, "Cumulative Damage on Battleship: %.2f%%\n", b.total_damage_taken * 100.0);
        }
    }

    if (results_file != NULL) {
        fclose(results_file);
    }
    
    printf("Simulation completed successfully and logged to results.txt!\n");
    free(e_ships);
}

void showSetup() {
    int setup_choice;
    int status;
    printf("\n--- Setup Menu ---\n");
    printf("1. Battleship Properties\n2. Escort Ships Settings\n3. Seed Value\n4. Return to Main Menu\n");
    printf("Enter choice: ");
    status = scanf("%d", &setup_choice);
    if (status != 1) {
        while (getchar() != '\n'); 
        setup_choice = 0; 
    }
}

void showStatistics() {
    FILE *file = fopen("results.txt", "r");
    if (file == NULL) {
        printf("\nNo past simulation results found. Run a simulation first!\n");
        return;
    }
    
    printf("\n--- Past Simulation Statistics ---\n");
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);
}

int main() {
    int choice;
    int status; 

    do {
        printf("\n--- Naval Battle Simulator ---\n");
        printf("1. Start Simulation\n2. View Instructions\n3. Simulation Statistics\n4. Exit\n");
        printf("Enter choice: ");
        
        status = scanf("%d", &choice);

        if (status != 1) {
            while (getchar() != '\n'); 
            choice = 0; 
        }

        switch(choice) {
            case 1:
                showSetup();
                startSimulation();
                break;
            case 2:
                printf("\nInstructions: The battleship must minimize cumulative impact while destroying escort ships across path iterations.\n");
                break;
            case 3:
                showStatistics();
                break;
            case 4:
                printf("\nExiting simulator. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 4.\n");
        }
    } while (choice != 4);

    return 0;
}
