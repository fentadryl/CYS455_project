#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define TITLE_LEN 100
#define GENRE_LEN 50
#define REVIEW_LEN 500
#define LINE_LEN 700

typedef struct {
    char title[TITLE_LEN];
    char genre[GENRE_LEN];
    int year;
    int rating; // 1 to 10
    char review[REVIEW_LEN];
} Movie;

void safe_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Input error.\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

int safe_int_input() {
    char buf[20];
    long val;
    safe_input(buf, sizeof(buf));
    char *endptr;
    errno = 0;
    val = strtol(buf, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Invalid numeric input.\n");
        return -1;
    }
    return (int)val;
}

void print_menu() {
    printf("\nMovie Manager Menu:\n");
    printf("1. Add Movie\n");
    printf("2. List Movies\n");
    printf("3. Save to File\n");
    printf("4. Search Movies\n");
    printf("5. Filter by Min Rating\n");
    printf("6. Exit\n");
    printf("Enter choice: ");
}

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

void get_user_filename(char *filename, size_t size) {
    char username[50];
    printf("Enter your username: ");
    safe_input(username, sizeof(username));
    snprintf(filename, size, "%s_movies.txt", username);
    printf("%s", filename);
}

int main() {
    char filename[128];
    Movie *movies = NULL;
    int count = 0;

    get_user_filename(filename, sizeof(filename));
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