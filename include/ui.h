#ifndef UI_H
#define UI_H

#include "movie.h"

void print_menu();
int safe_int_input();
void safe_input(char *buffer, size_t size);
void search_movies(Movie *movies, int count);
void filter_movies(Movie *movies, int count);

#endif
