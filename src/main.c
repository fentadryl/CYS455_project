/*
    Authors :  Andrew Krasuski
    CS455 - Secure Software Development Final Project
    Verification version
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "movie.h"
#include "file_io.h"
#include "ui.h"
#include "auth.h"


// Primary author:       Andrew Krasuski
// Description:          Entry point for the Movie Manager application; handles user authentication and main menu loop
// Inputs:               none (uses get_profile to obtain user file)
// Outputs:              interactive menu-driven program; returns EXIT_SUCCESS on exit
// Vulnerability ID 9-5: (Poor Usability) – handles invalid menu choices with clear feedback
// Vulnerability ID 6-8: (Failure to Handle Errors Correctly) – cleans up dynamic memory before exit
int main() {
    char filename[128];
    Movie *movies = NULL;
    int count = 0;

    get_profile(filename, sizeof(filename));
    load_movies(filename, &movies, &count); // <-- Load saved movies

    int choice;
    while (1) {
        print_menu();
        choice = safe_int_input();
        if (choice == -1) continue;

        switch (choice) {
            case 1:
                add_movie(&movies, &count);
            break;
            case 2:
                list_movies(movies, count);
            break;
            case 3:
                save_movies(filename, movies, count);
            break;
            case 4:
                search_movies(movies, count);
            break;
            case 5:
                filter_movies(movies, count);
            break;
            case 6:
                free(movies);
            printf("Exiting...\n");
            return EXIT_SUCCESS;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}
