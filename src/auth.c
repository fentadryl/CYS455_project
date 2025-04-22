//
// Created by Ben on 4/21/2025.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "auth.h"
#include "ui.h"
//Open source sha256 implementation
#include "sha256.h"

#define SALT_LENGTH 16
#define HASH_LENGTH (SHA256_BLOCK_SIZE * 2) // Hex representation

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50

#define CREDENTIALS_FILE "user_credentials.dat"
#define MAX_LINE_LENGTH 256

// Generate random salt
void generate_salt(char *salt, size_t length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    srand(time(NULL));

    for (size_t i = 0; i < length - 1; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[length - 1] = '\0';
}

// Convert binary hash to hex string
void bin_to_hex(const BYTE *bin, size_t bin_len, char *hex) {
    const char hex_digits[] = "0123456789abcdef";
    for (size_t i = 0; i < bin_len; i++) {
        hex[i*2]   = hex_digits[(bin[i] >> 4) & 0x0F];
        hex[i*2+1] = hex_digits[bin[i] & 0x0F];
    }
    hex[bin_len*2] = '\0';
}

// Hash a password with salt
void hash_password(const char *password, const char *salt, char *output_hash) {
    SHA256_CTX ctx;
    BYTE hash_bin[SHA256_BLOCK_SIZE];
    char salted_password[256];

    // Combine password and salt
    snprintf(salted_password, sizeof(salted_password), "%s%s", password, salt);
    //printf("%s\n", salted_password);

    // Compute hash
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE*)salted_password, strlen(salted_password));
    sha256_final(&ctx, hash_bin);

    // Convert to hex string
    bin_to_hex(hash_bin, SHA256_BLOCK_SIZE, output_hash);
}

//Save Username,salt,and hash in .dat file
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

//read .dat file for a specific username and return salt and hash
int get_user_credentials(const char *username, char *salt, char *hashed_password) {
    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (!file) {
        perror("Failed to open credentials file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

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

                found = 1;
                break;
            }
        }
    }

    fclose(file);
    return found;
}

//generate salt and hash for user and add them to database
int register_user(const char *username, const char *password) {
    char salt[SALT_LENGTH + 1];
    char hashed_password[HASH_LENGTH + 1];

    // Generate random salt
    generate_salt(salt, sizeof(salt));

    // Hash the password
    hash_password(password, salt, hashed_password);

    // Store username, salt and hashed_password in database
    return save_user_credentials(username, salt, hashed_password);
}

//verify if a username and password combonation is valid for login
int verify_user(const char *username, const char *password) {
    char stored_salt[SALT_LENGTH + 1];
    char stored_hash[HASH_LENGTH + 1];
    char computed_hash[HASH_LENGTH + 1];

    // Retrieve salt and hash from database
    if (!get_user_credentials(username, stored_salt, stored_hash)) {
        return 0; // User not found
    }

    // Compute hash of provided password with stored salt
    hash_password(password, stored_salt, computed_hash);

    // Compare hashes
    return strcmp(computed_hash, stored_hash) == 0;
}



void get_profile (char *filename, size_t size) {

    char username[50];
    printf("Enter your username: ");
    safe_input(username, sizeof(username));

    char password[50];
    printf("Enter your password: ");
    safe_input(password, sizeof(password));

    int choice = 0;
    while (1) {
        if (verify_user(username, password) == 0) {
            //if it does not, register user
            printf("Username and password do not match.\n");

            printf("1.Try again\n"
                    "or\n"
                    "2. Register new user?\n");

            choice = safe_int_input();
            printf("choice is %d\n",choice);

            if (choice == 2) {
                register_user(username, password);
                break;
            }
            printf("Enter your username: ");
            safe_input(username, sizeof(username));

            printf("Enter your password: ");
            safe_input(password, sizeof(password));
            } else {

                break;

        }
    }


    snprintf(filename, size, "%s_movies.txt", username);
    printf("%s", filename);
}

