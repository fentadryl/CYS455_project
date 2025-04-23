//
// Created by Abel on 4/21/2025
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "movie.h"
#include "file_io.h"

#define LINE_LEN 700

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
