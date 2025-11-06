#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL)); // Seed random number generator

    // Define array of words
    char *choices[] = {"Rock", "Paper", "Scissors"};
    int num_choices = sizeof(choices) / sizeof(choices[0]);

    while (1) {  // Infinite loop until user wins
        // Computer choice each round
        int comp_index = rand() % num_choices;
        char *computer = choices[comp_index];

        // User choice
        char user_choice;
        printf("Enter your choice (r for Rock, p for Paper, s for Scissors): ");
        scanf(" %c", &user_choice);

        // Display computer choice
        printf("Computer chose %s\n", computer);

        // Determine result
        if ((user_choice == 'r' && comp_index == 0) ||
            (user_choice == 'p' && comp_index == 1) ||
            (user_choice == 's' && comp_index == 2)) {
            printf("It's a tie!\n\n");
        } 
        else if ((user_choice == 'r' && comp_index == 2) ||
                 (user_choice == 'p' && comp_index == 0) ||
                 (user_choice == 's' && comp_index == 1)) {
            printf("You win!\n");
            break; // exit loop
        } 
        else if ((user_choice == 'r' && comp_index == 1) ||
                 (user_choice == 'p' && comp_index == 2) ||
                 (user_choice == 's' && comp_index == 0)) {
            printf("You lose! Try again.\n\n");
        } 
        else {
            printf("Invalid input! Please enter r, p, or s.\n\n");
        }
    }

    return 0;
}
