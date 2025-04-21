//
// Created by Ben on 4/21/2025.
//

#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "ui.h"

void get_user_filename(char *filename, size_t size) {

    char username[50];
    printf("Enter your username: ");
    safe_input(username, sizeof(username));
    snprintf(filename, size, "%s_movies.txt", username);
    printf("%s", filename);
}

