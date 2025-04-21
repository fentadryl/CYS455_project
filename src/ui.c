//
// Created by Ben on 4/21/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ui.h"
#include "movie.h"

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

void safe_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Input error.\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}



/* Other UI functions */
