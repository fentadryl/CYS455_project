/*
    Authors : Abel Kloter
    CS455 - Secure Software Development Final Project
    Verification version
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "movie.h"
#include "file_io.h"

#define LINE_LEN 700


// Primary author:       Abel Kloter
// Description:          Save an array of Movie structs to a file, one per line, using '|' as field separator.
// Inputs:               filename string, pointer to Movie array, count of movies
// Outputs:              prints success or error message; writes to file; does not crash on write error.
// Vulnerability ID 6-2: (Failure to Handle Errors Correctly) – checks fopen result and perror on fopen failure.
// Vulnerability ID 6-2: (Failure to Handle Errors Correctly) – checks fprintf return value, prints to stderr and aborts early on error.
void save_movies(const char *filename, Movie *movies, int count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s|%s|%d|%d|%s\n",
                    movies[i].title,
                    movies[i].genre,
                    movies[i].year,
                    movies[i].rating,
                    movies[i].review) < 0) {
            fprintf(stderr, "Error writing to file.\n");
            fclose(fp);
            return;
                    }
    }

    fclose(fp);
    printf("Movies saved to '%s'.\n", filename);
}

// Primary author:       Abel Kloter
// Description:          Load movies from a file into a dynamically resized array.
// Inputs:               filename, pointer to Movie* array, pointer to count
// Outputs:              populates *movies and *count; reallocs array; handles missing file as non-fatal.
// Vulnerability ID 3-2: (Integer Overflows) – checks *count against INT_MAX/sizeof(Movie) to avoid overflow on realloc count.
// Vulnerability ID 6-3: (Failure to Handle Errors Correctly) – checks realloc return and perror on failure.
void load_movies(const char *filename, Movie **movies, int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        // File might not exist yet—don’t treat as fatal
        return;
    }

    char line[LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        if (*count >= INT_MAX / sizeof(Movie)) break;

        Movie *temp = realloc(*movies, (*count + 1) * sizeof(Movie));
        if (!temp) {
            perror("Memory allocation failed");
            fclose(fp);
            return;
        }
        *movies = temp;

        Movie *m = &(*movies)[*count];
        char *token = strtok(line, "|");
        if (!token) continue;
        strncpy(m->title, token, TITLE_LEN - 1);
        m->title[TITLE_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(m->genre, token, GENRE_LEN - 1);
        m->genre[GENRE_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token) continue;
        m->year = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        m->rating = atoi(token);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(m->review, token, REVIEW_LEN - 1);
        m->review[REVIEW_LEN - 1] = '\0';

        (*count)++;
    }

    fclose(fp);
}
