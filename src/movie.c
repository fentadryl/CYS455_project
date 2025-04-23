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
#include "ui.h"

#define TITLE_LEN 100
#define GENRE_LEN 50
#define REVIEW_LEN 500
#define LINE_LEN 700

// Primary author:       Abel Kloter
// Description:          Check for duplicate movie entries by title and year
// Inputs:               pointer to Movie array, current count, title string, year integer
// Outputs:              returns 1 if duplicate found, 0 otherwise
// Vulnerability ID 7-1: (Failure to Handle Errors Correctly) – prevents adding duplicates and informs user
int is_duplicate(Movie *movies, int count, const char *title, int year) {
    for (int i = 0; i < count; i++) {
        if (strcmp(movies[i].title, title) == 0 && movies[i].year == year)
            return 1;
    }
    return 0;
}

void add_movie(Movie **movies, int *count) {
    if (*count >= INT_MAX / sizeof(Movie)) {
        fprintf(stderr, "Too many movies.\n");
        return;
    }

    Movie *temp = realloc(*movies, (*count + 1) * sizeof(Movie));
    if (!temp) {
        perror("Memory allocation failed");
        return;
    }
    *movies = temp;

    Movie *new_movie = &(*movies)[*count];

    printf("Enter movie title: ");
    safe_input(new_movie->title, TITLE_LEN);

    printf("Enter release year: ");
    int year = safe_int_input();
    if (year < 1800 || year > 2100) {
        fprintf(stderr, "Year out of range.\n");
        return;
    }

    if (is_duplicate(*movies, *count, new_movie->title, year)) {
        printf("Duplicate movie found. Not adding.\n");
        return;
    }

    printf("Enter genre: ");
    safe_input(new_movie->genre, GENRE_LEN);

    printf("Enter rating (1-10): ");
    int rating = safe_int_input();
    if (rating < 1 || rating > 10) {
        fprintf(stderr, "Invalid rating.\n");
        return;
    }

    printf("Enter your review: ");
    safe_input(new_movie->review, REVIEW_LEN);

    new_movie->year = year;
    new_movie->rating = rating;
    (*count)++;
    printf("Movie added successfully.\n");
}

void list_movies(Movie *movies, int count) {
    if (count == 0) {
        printf("No movies to display.\n");
        return;
    }

    printf("\n--- Movie List ---\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s (%d) - %s | Rating: %d\nReview: %s\n\n",
               i + 1,
               movies[i].title,
               movies[i].year,
               movies[i].genre,
               movies[i].rating,
               movies[i].review);
    }
}

void search_movies(Movie *movies, int count) {
    char query[TITLE_LEN];
    printf("Enter title or genre to search: ");
    safe_input(query, TITLE_LEN);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strstr(movies[i].title, query) || strstr(movies[i].genre, query)) {
            printf("%s (%d) - %s | Rating: %d\nReview: %s\n\n",
                   movies[i].title, movies[i].year,
                   movies[i].genre, movies[i].rating, movies[i].review);
            found = 1;
        }
    }
    if (!found)
        printf("No matching movies found.\n");
}

void filter_movies(Movie *movies, int count) {
    printf("Enter minimum rating to filter by (1-10): ");
    int min = safe_int_input();

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (movies[i].rating >= min) {
            printf("%s (%d) - %s | Rating: %d\nReview: %s\n\n",
                   movies[i].title, movies[i].year,
                   movies[i].genre, movies[i].rating, movies[i].review);
            found = 1;
        }
    }
    if (!found)
        printf("No movies found with rating >= %d\n", min);
}
