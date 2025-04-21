//
// Created by Ben on 4/21/2025.
//

#ifndef FILE_IO_H
#define FILE_IO_H

#include "movie.h"

void save_movies(const char *filename, Movie *movies, int count);
void load_movies(const char *filename, Movie **movies, int *count);

#endif