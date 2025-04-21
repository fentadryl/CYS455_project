//
// Created by Ben on 4/21/2025.
//

#ifndef MOVIE_H
#define MOVIE_H

#define TITLE_LEN 100
#define GENRE_LEN 50
#define REVIEW_LEN 500

typedef struct {
    char title[TITLE_LEN];
    char genre[GENRE_LEN];
    int year;
    int rating;
    char review[REVIEW_LEN];
} Movie;

int is_duplicate(Movie *movies, int count, const char *title, int year);
void add_movie(Movie **movies, int *count);
void list_movies(Movie *movies, int count);

#endif
