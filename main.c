#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 9.81 

typedef struct {
    char type;
    float v_max;
    float gamma;
    float current_ip;
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
        fprintf(file, "ID: %d | Type: %c | V_max: %.2f | Angle_max: %.2f\n", 
                e_ships[i].id, e_ships[i].type, e_ships[i].v_max, e_ships[i].angle_max);
    }
    
    fclose(file);
    printf("Initial conditions saved to results.txt\n");
}

void startSimulation() {
    int n_ships;
    float canvas_size;
    char b_type;
    
    printf("\n--- Start Simulation (Part 1-A) ---\n");
    printf("Enter Battleship type notation (U, M, R, or S): ");
    scanf(" %c", &b_type);
    
    printf("Enter canvas grid size (e.g., 100.0): ");
    scanf("%f", &canvas_size);
    
    printf("Enter number of escort ships (N): ");
    scanf("%d", &n_ships);
    
    Battleship b = {b_type, 50.0, 0.01, 1.0};
    
    EscortShip *e_ships = malloc(n_ships * sizeof(EscortShip));
    if (e_ships == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    char types[] = {'A', 'B', 'C', 'D', 'E'};
    for (int i = 0; i < n_ships; i++) {
        e_ships[i].id = i + 1;
        e_ships[i].type = types[rand() % 5];
        e_ships[i].v_max = 10.0 + (rand() % 20);
        e_ships[i].angle_max = 45.0;
        e_ships[i].is_destroyed = 0;
    }
    
    saveInitialConditions(e_ships, n_ships, b);
    
    // Simple simulation logic check for Part 1-A
    int hit_count = 0;
    for (int i = 0; i < n_ships; i++) {
        // Test a sample attack range calculation
        float sample_range = calculateRange(e_ships[i].v_max, e_ships[i].angle_max);
        if (sample_range <= canvas_size) {
            hit_count++;
            e_ships[i].is_destroyed = 1;
        }
    }
    
    printf("Simulation completed! Escort ships hit by Battleship: %d\n", hit_count);
    
    // Save final stats
    FILE *final_file = fopen("results.txt", "a");
    if (final_file != NULL) {
        fprintf(final_file, "Simulation Results: %d escort ships were hit.\n", hit_count);
        fclose(final_file);
    }
    
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
                printf("\nInstructions: The battleship must minimize impact while destroying escort ships.\n");
                break;
            case 3:
                printf("\nSimulation Statistics: Loading past results from text file...\n");
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
