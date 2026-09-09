#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void startSimulation() {
    printf("Simulation starting...\n");
    // Part 1 logic will go here
}

void showSetup() {
    int setup_choice;
    printf("\n--- Setup Menu ---\n");
    printf("1. Battleship Properties\n2. Escort Ships Settings\n3. Seed Value\n4. Return to Main Menu\n");
    printf("Enter choice: ");
    scanf("%d", &setup_choice);
}

int main() {
    int main() {
    int choice;
    int status; // Tracks if scanf successfully read a number

    do {
        printf("\n--- Naval Battle Simulator ---\n");
        printf("1. Start Simulation\n2. View Instructions\n3. Simulation Statistics\n4. Exit\n");
        printf("Enter choice: ");
        
        status = scanf("%d", &choice);

        // If the user inputs a letter, clear the buffer to prevent the infinite loop
        if (status != 1) {
            while (getchar() != '\n'); 
            choice = 0; // Forces the switch statement to the 'default' error case
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
