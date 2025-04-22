//
// Created by Ben on 4/22/2025.
//

// sha256.h
#ifndef SHA256_H
#define SHA256_H

#include <stddef.h>

typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word

typedef struct {
    BYTE data[64];
    WORD datalen;
    unsigned long long bitlen;
    WORD state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ctx, BYTE hash[]);

#endif
