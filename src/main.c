//
// Created by Andrew on 4/21/2025
//

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "movie.h"
#include "file_io.h"
#include "ui.h"
#include "auth.h"

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
