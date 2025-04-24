#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "auth.h"
#include "ui.h"
#include "sha256.h"

#define SALT_LENGTH 16
#define HASH_LENGTH (SHA256_BLOCK_SIZE * 2)
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_LINE_LENGTH 256
#define CREDENTIALS_FILE "user_credentials.dat"

void generate_salt(char *salt, size_t length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    srand(time(NULL));
    for (size_t i = 0; i < length - 1; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[length - 1] = '\0';
}

void bin_to_hex(const BYTE *bin, size_t bin_len, char *hex) {
    const char hex_digits[] = "0123456789abcdef";
    for (size_t i = 0; i < bin_len; i++) {
        hex[i * 2]     = hex_digits[(bin[i] >> 4) & 0x0F];
        hex[i * 2 + 1] = hex_digits[bin[i] & 0x0F];
    }
    hex[bin_len * 2] = '\0';
}

void hash_password(const char *password, const char *salt, char *output_hash) {
    SHA256_CTX ctx;
    BYTE hash_bin[SHA256_BLOCK_SIZE];
    char salted_password[256];

    snprintf(salted_password, sizeof(salted_password), "%s%s", password, salt);
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE*)salted_password, strlen(salted_password));
    sha256_final(&ctx, hash_bin);
    bin_to_hex(hash_bin, SHA256_BLOCK_SIZE, output_hash);
}

int save_user_credentials(const char *username, const char *salt, const char *hashed_password) {
    FILE *file = fopen(CREDENTIALS_FILE, "a");
    if (!file) {
        perror("Failed to open credentials file");
        return 0;
    }

    fprintf(file, "%s:%s:%s\n", username, salt, hashed_password);
    fclose(file);
    return 1;
}

int get_user_credentials(const char *username, char *salt, char *hashed_password) {
    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (!file) {
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char stored_salt[SALT_LENGTH + 1];
        char stored_hash[HASH_LENGTH + 1];

        if (sscanf(line, "%[^:]:%[^:]:%s", stored_username, stored_salt, stored_hash) == 3) {
            if (strcmp(username, stored_username) == 0) {
                strncpy(salt, stored_salt, SALT_LENGTH);
                salt[SALT_LENGTH] = '\0';
                
                strncpy(hashed_password, stored_hash, HASH_LENGTH);
                hashed_password[HASH_LENGTH] = '\0';
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

int register_user(const char *username, const char *password) {

    if (strchr(username, ':')) {
        printf("Error: Username cannot contain the ':' character.\n");
        return 0;
    }

    char dummy_salt[SALT_LENGTH + 1];
    char dummy_hash[HASH_LENGTH + 1];

    if (get_user_credentials(username, dummy_salt, dummy_hash)) {
        printf("Error: Username '%s' already exists.\n", username);
        return 0;
    }

    char salt[SALT_LENGTH + 1];
    char hashed_password[HASH_LENGTH + 1];

    generate_salt(salt, sizeof(salt));
    hash_password(password, salt, hashed_password);

    if (save_user_credentials(username, salt, hashed_password)) {
        printf("Registration successful for user '%s'\n", username);
        return 1;
    }

    return 0;
}

int verify_user(const char *username, const char *password) {
    char stored_salt[SALT_LENGTH + 1];
    char stored_hash[HASH_LENGTH + 1];
    char computed_hash[HASH_LENGTH + 1];
    
    //no user found
    if (!get_user_credentials(username, stored_salt, stored_hash)) {
        return 0;
    }

    //user was found, check if password matches hash
    hash_password(password, stored_salt, computed_hash);
    return strcmp(computed_hash, stored_hash) == 0;
}

void get_profile(char *filename, size_t size) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    while (1) {
        printf("Enter your username: ");
        safe_input(username, sizeof(username));

        printf("Enter your password: ");
        safe_input(password, sizeof(password));

        if (verify_user(username, password)) {
            printf("Login successful. Welcome, %s!\n", username);
            break;
        }

        printf("Authentication failed.\n");
        printf("1. Try again\n2. Register new user\nChoice: ");
        int choice = safe_int_input();

        if (choice == 2) {
            if (register_user(username, password)) {
                break;
            }
        }
    }

    snprintf(filename, size, "%s_movies.txt", username);
    printf("Your movie list file: %s\n", filename);
}
