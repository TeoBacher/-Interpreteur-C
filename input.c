#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "input.h"

// Function to read and interpret a file line by line
void interpretFile(const char* fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;  
        interpret(line);  
    }

    fclose(file);
}

// Function for interactive REPL mode
void interactiveMode() {
    char inputLine[256];

    printf("Mode interactif. Tapez 'exit' pour quitter.\n");

    while (1) {
        printf("> ");
        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) {
            break;
        }

        inputLine[strcspn(inputLine, "\n")] = 0;

        if (strcmp(inputLine, "exit") == 0) {
            break;
        }

        interpret(inputLine);  // Appel à la fonction interpret() du lexer
    }
}

// Function to handle input based on user's choice (file or interactive mode)
void handleInput() {
    int mode = 0;
    
    do {
        printf("Choisissez le mode d'exécution:\n");
        printf("1. Mode fichier\n");
        printf("2. Mode interactif\n");
        printf("Entrez votre choix (1 ou 2): ");
        scanf("%d", &mode);
        getchar();  // Consommer le retour à la ligne
    } while (mode != 1 && mode != 2);
    
    if (mode == 1) {
        interpretFile("code.txt");  // Remplacer par le nom du fichier
    } else if (mode == 2) {
        interactiveMode();
    }
}
