#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_LANGUAGES 40
#define MAX_WORD_LENGTH 30
#define MAX_GUESSES 50

// List of programming languages
const char *programming_languages[MAX_LANGUAGES] = {
    "python", "java", "javascript", "c", "cpp", "csharp", "ruby", "swift", "kotlin",
    "typescript", "go", "rust", "php", "dart", "scala", "r", "perl", "lua", "haskell",
    "matlab", "fortran", "pascal", "cobol", "bash", "powershell", "sql", "objectivec",
    "elixir", "julia", "scheme", "lisp", "prolog", "scratch", "visualbasic", "assembly",
    "fsharp", "groovy", "erlang", "clojure", "vhdl"
};

// Function to play a single round of Hangman
void hangman() {
    srand(time(NULL)); // Seed random number generator

    const char *word = programming_languages[rand() % MAX_LANGUAGES];
    int word_length = strlen(word);
    int attempts_remaining = word_length + 2;

    char display_word[MAX_WORD_LENGTH];
    char guessed_letters[MAX_GUESSES];
    int guessed_count = 0;

    // Initialize display word with underscores
    for (int i = 0; i < word_length; i++) {
        display_word[i] = '_';
    }
    display_word[word_length] = '\0';

    printf("\nGuess the programming language!\n");

    while (attempts_remaining > 0) {
        printf("\nCurrent word: ");
        for (int i = 0; i < word_length; i++) {
            printf("%c ", display_word[i]);
        }

        printf("\nAttempts remaining: %d\n", attempts_remaining);
        printf("Guessed letters: ");
        for (int i = 0; i < guessed_count; i++) {
            printf("%c ", guessed_letters[i]);
        }
        printf("\n");

        // Get user guess
        char guess;
        printf("Pick a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (!isalpha(guess)) {
            printf("Please enter a valid letter.\n");
            continue;
        }

        int already_guessed = 0;
        for (int i = 0; i < guessed_count; i++) {
            if (guessed_letters[i] == guess) {
                already_guessed = 1;
                break;
            }
        }
        if (already_guessed) {
            printf("You already guessed that letter. Try another one.\n");
            continue;
        }

        guessed_letters[guessed_count++] = guess;

        int correct_guess = 0;
        for (int i = 0; i < word_length; i++) {
            if (word[i] == guess) {
                display_word[i] = guess;
                correct_guess = 1;
            }
        }

        if (correct_guess) {
            printf("Correct guess!\n");
        } else {
            attempts_remaining--;
            printf("Wrong guess!\n");
        }

        // Check win condition
        if (strchr(display_word, '_') == NULL) {
            printf("\nYou win!\nThe word was: %s\n", word);
            return;
        }
    }

    printf("\nGame Over!\nThe correct word was: %s\n", word);
}

int main() {
    char name[50];
    printf("Enter your name: ");
    scanf("%49s", name);

    printf("Welcome to the Hangman Game, %s!\n", name);

    while (1) {
        hangman();
        char play_again;
        printf("\nWould you like to play again (y/n)? ");
        scanf(" %c", &play_again);

        if (tolower(play_again) != 'y') {
            printf("Thanks for playing Hangman!\n");
            break;
        }
    }

    return 0;
}
