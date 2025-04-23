/*
    Authors : Andrew Krasuski
    CS455 - Secure Software Development Final Project
    Verification version
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "ui.h"
#include "movie.h"

// Primary author:       Andrew Krasuski
// Description:          Print the main menu options for the Movie Manager.
// Inputs:               none
// Outputs:              writes menu text to stdout
// Vulnerability ID 9-1: (Poor Usability) – clear, numbered prompts guide the user.
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

// Primary author:       Andrew Krasuski
// Description:          Safely read an integer from stdin, validating format and range.
// Inputs: none          (reads via fgets inside)
// Outputs:              returns valid int, or -1 on invalid input
// Vulnerability ID 3-3: (Integer Overflows) – uses errno, strtol, and bounds checking to avoid overflow.
// Vulnerability ID 6-3: (Failure to Handle Errors Correctly) – returns -1 on bad input rather than undefined behavior.
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

/ Primary author:        Andrew Krasuski
// Description:          Safely read a line of text from stdin, removing newline and checking for errors.
// Inputs:               buffer pointer and its size
// Outputs:              buffer filled with user input, exits on read error
// Vulnerability ID 1-1: (Buffer Overruns) – uses fgets with explicit size limit and null-termination.
// Vulnerability ID 6-3: (Failure to Handle Errors Correctly) – checks fgets return and exits gracefully on error.
void safe_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Input error.\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}
