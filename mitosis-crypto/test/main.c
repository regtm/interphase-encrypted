#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mitosis-crypto.h"

typedef struct _hmac_sha256_vector {
    uint8_t key[131];
    size_t key_len;
    uint8_t data[153];
    size_t data_len;
    uint8_t expected[MITOSIS_HMAC_OUTPUT_SIZE];
} hmac_sha256_test_vector;

typedef struct _hkdf_sha256_vector {
    uint8_t ikm[80];
    uint8_t ikm_len;
    uint8_t salt[80];
    uint8_t salt_len;
    uint8_t info[80];
    uint8_t info_len;
    uint8_t expected_prk[32];
    uint8_t expected_okm[82];
    uint8_t L;
} hkdf_sha256_test_vector;

typedef struct _aes_ctr_vector {
    uint8_t key[AES_BLOCK_SIZE];
    uint8_t nonce[AES_BLOCK_SIZE];
    uint8_t expected_encrypted_counter[AES_BLOCK_SIZE];
    uint8_t plaintext[AES_BLOCK_SIZE];
    uint8_t expected_ciphertext[AES_BLOCK_SIZE];
} aes_ctr_test_vector;

#define RUN_TEST_LOG(test) \
    bool test ##_result = test(); \
    if(! test ##_result) { \
        printf("%s failed!\n", #test); \
        ++failures; \
    } \
    result &= test ##_result; \


void print_hex(const char* label, const uint8_t* bytes, size_t len) {
    for(int idx = 0; idx < len; ++idx) {
        if(idx == 0) {
            printf("%s:\t%02x ", label, bytes[idx]);
        } else if (idx == len - 1) {
            printf("%02x\n", bytes[idx]);
        } else {
            printf("%02x ", bytes[idx]);
        }
    }
}

bool compare_expected(const uint8_t* actual, const uint8_t* expected, size_t len, const char* func, const char* label) {
    if(memcmp(actual, expected, len) != 0) {
        printf("%s: expected %s doesn't match actual\n", func, label);
        print_hex("Actual  ", actual, len);
        print_hex("Expected", expected, len);
        int idx = 0;
        while(actual[idx] == expected[idx]) ++idx;
        printf("Index %d doesn't match (%02x actual vs. %02x expected)\n", idx, actual[idx], expected[idx]);
        return false;
    }
    return true;
}

bool hmac_sha256_kat() {
    hmac_sha256_test_vector test_cases[] = {
        {
            {
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b
            },
            20,
            "Hi There",
            8,
            {
                0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53,
                0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b,
                0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7,
                0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7
            }
        },
        {
            "Jefe",
            4,
            "what do ya want for nothing?",
            28,
            {
                0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e,
                0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7,
                0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83,
                0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43
            }
        },
        {
            {
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa
            },
            131,
            "Test Using Larger Than Block-Size Key - Hash Key First",
            54,
            {
                0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f,
                0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f,
                0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14,
                0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54
            }
        },
        {
            {
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
                0xaa, 0xaa, 0xaa
            },
            131,
            "This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.",
            152,
            {
                0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb,
                0x27, 0x63, 0x5f, 0xbc, 0xd5, 0xb0, 0xe9, 0x44,
                0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93,
                0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2
            }
        },
    };
    mitosis_hmac_context_t state = { 0 };

    for(int test_idx = 0; test_idx < sizeof(test_cases)/sizeof(test_cases[0]); ++test_idx) {
        hmac_sha256_test_vector* test_case = &test_cases[test_idx];

        uint8_t* key = test_case->key;
        if(!mitosis_hmac_init(&state, key, test_case->key_len)) {
            printf("%s: failed HMAC init\n", __func__);
            return false;
        }

        uint8_t* data = test_case->data;
        if(!mitosis_hmac_hash(&state, data, test_case->data_len)) {
            printf("%s: failed HMAC hash\n", __func__);
            return false;
        }
        uint8_t result[MITOSIS_HMAC_OUTPUT_SIZE] = { 0 };
        if(!mitosis_hmac_complete(&state, result)) {
            printf("%s: failed HMAC complete\n", __func__);
            return false;
        }

        uint8_t* expected = test_case->expected;
        if(!compare_expected(result, expected, sizeof(result), __func__, "HMAC")) {
            return false;
        }
    }
    return true;
}

bool hkdf_kat() {
    hkdf_sha256_test_vector test_cases[] = {
        {
            {
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
            },
            22,
            { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c },
            13,
            { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9 },
            10,
            {
                0x07, 0x77, 0x09, 0x36, 0x2c, 0x2e, 0x32, 0xdf,
                0x0d, 0xdc, 0x3f, 0x0d, 0xc4, 0x7b, 0xba, 0x63,
                0x90, 0xb6, 0xc7, 0x3b, 0xb5, 0x0f, 0x9c, 0x31,
                0x22, 0xec, 0x84, 0x4a, 0xd7, 0xc2, 0xb3, 0xe5
            },
            {
                0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a, 0x90, 0x43,
                0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a, 0x2d, 0x2d, 0x0a, 0x90,
                0xcf, 0x1a, 0x5a, 0x4c, 0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4,
                0xc5, 0xbf, 0x34, 0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18,
                0x58, 0x65
            },
            42
        },
        {
            {
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
                0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
                0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
                0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
                0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
                0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f
            },
            80,
            {
                0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
                0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
                0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d,
                0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91,
                0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
                0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5,
                0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf
            },
            80,
            {
                0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9,
                0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3,
                0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
                0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
                0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1,
                0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb,
                0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5,
                0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
            },
            80,
            {
                0x06, 0xa6, 0xb8, 0x8c, 0x58, 0x53, 0x36, 0x1a,
                0x06, 0x10, 0x4c, 0x9c, 0xeb, 0x35, 0xb4, 0x5c,
                0xef, 0x76, 0x00, 0x14, 0x90, 0x46, 0x71, 0x01,
                0x4a, 0x19, 0x3f, 0x40, 0xc1, 0x5f, 0xc2, 0x44
            },
            {
                0xb1, 0x1e, 0x39, 0x8d, 0xc8, 0x03, 0x27, 0xa1, 0xc8, 0xe7,
                0xf7, 0x8c, 0x59, 0x6a, 0x49, 0x34, 0x4f, 0x01, 0x2e, 0xda,
                0x2d, 0x4e, 0xfa, 0xd8, 0xa0, 0x50, 0xcc, 0x4c, 0x19, 0xaf,
                0xa9, 0x7c, 0x59, 0x04, 0x5a, 0x99, 0xca, 0xc7, 0x82, 0x72,
                0x71, 0xcb, 0x41, 0xc6, 0x5e, 0x59, 0x0e, 0x09, 0xda, 0x32,
                0x75, 0x60, 0x0c, 0x2f, 0x09, 0xb8, 0x36, 0x77, 0x93, 0xa9,
                0xac, 0xa3, 0xdb, 0x71, 0xcc, 0x30, 0xc5, 0x81, 0x79, 0xec,
                0x3e, 0x87, 0xc1, 0x4c, 0x01, 0xd5, 0xc1, 0xf3, 0x43, 0x4f,
                0x1d, 0x87
            },
            82
        },
        {
            {
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
            },
            22,
            { 0 },
            0,
            { 0 },
            0,
            {
                0x19, 0xef, 0x24, 0xa3, 0x2c, 0x71, 0x7b, 0x16,
                0x7f, 0x33, 0xa9, 0x1d, 0x6f, 0x64, 0x8b, 0xdf,
                0x96, 0x59, 0x67, 0x76, 0xaf, 0xdb, 0x63, 0x77,
                0xac, 0x43, 0x4c, 0x1c, 0x29, 0x3c, 0xcb, 0x04
            },
            {
                0x8d, 0xa4, 0xe7, 0x75, 0xa5, 0x63, 0xc1, 0x8f,
                0x71, 0x5f, 0x80, 0x2a, 0x06, 0x3c, 0x5a, 0x31,
                0xb8, 0xa1, 0x1f, 0x5c, 0x5e, 0xe1, 0x87, 0x9e,
                0xc3, 0x45, 0x4e, 0x5f, 0x3c, 0x73, 0x8d, 0x2d,
                0x9d, 0x20, 0x13, 0x95, 0xfa, 0xa4, 0xb6, 0x1a,
                0x96, 0xc8
            },
            42
        },
        {
            {
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
                0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
            },
            22,
            { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c },
            13,
            { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9 },
            10,
            {
                0x07, 0x77, 0x09, 0x36, 0x2c, 0x2e, 0x32, 0xdf,
                0x0d, 0xdc, 0x3f, 0x0d, 0xc4, 0x7b, 0xba, 0x63,
                0x90, 0xb6, 0xc7, 0x3b, 0xb5, 0x0f, 0x9c, 0x31,
                0x22, 0xec, 0x84, 0x4a, 0xd7, 0xc2, 0xb3, 0xe5
            },
            {
                0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a, 0x90, 0x43,
                0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a, 0x2d, 0x2d, 0x0a, 0x90
            },
            16
        }
    };

    uint8_t* okm = NULL;
    bool result = true;
    for(int test_idx = 0; test_idx < sizeof(test_cases)/sizeof(test_cases[0]); ++test_idx) {
        hkdf_sha256_test_vector* test_case = &test_cases[test_idx];
        uint8_t prk[MITOSIS_HMAC_OUTPUT_SIZE];
        okm = malloc(test_case->L);

        if(!mitosis_hkdf_extract(test_case->ikm, test_case->ikm_len, test_case->salt, test_case->salt_len, prk)) {
            printf("%s: mitosis_hkdf_extract failed!\n", __func__);
            result = false;
            goto error;
        }
        if(!compare_expected(prk, test_case->expected_prk, sizeof(prk), __func__, "PRK")) {
            result =  false;
            goto error;
        }
        if(!mitosis_hkdf_expand(prk, sizeof(prk), test_case->info, test_case->info_len, okm, test_case->L)) {
            printf("%s: mitosis_hkdf_expand failed!\n", __func__);
            result = false;
            goto error;
        }
        if(!compare_expected(okm, test_case->expected_okm, test_case->L, __func__, "OKM")) {
            result = false;
            goto error;
        }
        free(okm);
        okm = NULL;
    }
error:
    if(okm != NULL) {
        free(okm);
    }
    return result;
}

bool aes_ctr_kat() {
    aes_ctr_test_vector test_cases[] = {
        {
            {
                0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
            }, // key
            {
                0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
                0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
            }, // nonce
            {
                0xec, 0x8c, 0xdf, 0x73, 0x98, 0x60, 0x7c, 0xb0,
                0xf2, 0xd2, 0x16, 0x75, 0xea, 0x9e, 0xa1, 0xe4
            }, // encrypted counter
            {
                0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
            }, // ctr plaintext
            {
                0x87, 0x4d, 0x61, 0x91, 0xb6, 0x20, 0xe3, 0x26,
                0x1b, 0xef, 0x68, 0x64, 0x99, 0x0d, 0xb6, 0xce
            } // expected ctr ciphertext
        }, // test case 1
        {
            "my very eager mo", // key
            "ther just served", // nonce
            {
                0xa0, 0xf3, 0x59, 0x59, 0x0a, 0xad, 0x4e, 0xcd,
                0x1b, 0x70, 0x20, 0x38, 0x14, 0xf9, 0x6d, 0x69
            }, // encrypted counter
            " us nine pizzas!", // ctr plaintext
            {
                0x80, 0x86, 0x2a, 0x79, 0x64, 0xc4, 0x20, 0xa8,
                0x3b, 0x00, 0x49, 0x42, 0x6e, 0x98, 0x1e, 0x48
            } // expected ctr ciphertext
        } // test case 2
    };
    bool result = true;
    mitosis_encrypt_context_t context;

    for(int test_idx = 0; test_idx < sizeof(test_cases)/sizeof(test_cases[0]); ++test_idx) {
        aes_ctr_test_vector* test_case = &test_cases[test_idx];
        uint8_t output[AES_BLOCK_SIZE] = { 0 };

        result = mitosis_aes_ctr_init(test_case->key, test_case->nonce, &context);
        if(!result) {
            printf("%s: mitosis_aes_ctr_init failed!\n", __func__);
            return false;
        }

        // Test encryption
        result = mitosis_aes_ctr_encrypt(&context, sizeof(output), test_case->plaintext, output);
        if(!result) {
            printf("%s: mitosis_aes_ctr_encrypt failed!\n", __func__);
            return false;
        }
        if(!compare_expected(context.ecb.ciphertext, test_case->expected_encrypted_counter, sizeof(test_case->expected_encrypted_counter), __func__, "encrypted counter")) {
            return false;
        }
        if(!compare_expected(output, test_case->expected_ciphertext, sizeof(output), __func__, "encrypted plaintext")) {
            return false;
        }

        // Test decryption
        result = mitosis_aes_ctr_decrypt(&context, sizeof(output), test_case->expected_ciphertext, output);
        if(!result) {
            printf("%s: mitosis_aes_ctr_decrypt failed!\n", __func__);
            return false;
        }
        if(!compare_expected(context.ecb.ciphertext, test_case->expected_encrypted_counter, sizeof(test_case->expected_encrypted_counter), __func__, "encrypted counter")) {
            return false;
        }
        if(!compare_expected(output, test_case->plaintext, sizeof(output), __func__, "decrypted plaintext")) {
            return false;
        }

        // Test in-place encryption/decryption
        memcpy(output, test_case->plaintext, sizeof(output));
        result = mitosis_aes_ctr_encrypt(&context, sizeof(output), output, output);
        if(!result) {
            printf("%s: mitosis_aes_ctr_encrypt in-place failed!\n", __func__);
            return false;
        }
        if(!compare_expected(output, test_case->expected_ciphertext, sizeof(output), __func__, "encrypted plaintext in-place")) {
            return false;
        }
        result = mitosis_aes_ctr_decrypt(&context, sizeof(output), output, output);
        if(!result) {
            printf("%s: mitosis_aes_ctr_decrypt in-place failed!\n", __func__);
            return false;
        }
        if(!compare_expected(output, test_case->plaintext, sizeof(output), __func__, "decrypted plaintext in-place")) {
            return false;
        }
    }
    return result;
}

bool verify_key_generation() {
    mitosis_crypto_context_t context;
    bool result = true;

    result = mitosis_crypto_init(&context, true);
    if(!result) {
        printf("%s: %s mitosis_crypto_init failed!\n", __func__, "left");
        return false;
    }

    print_hex("       left key", context.encrypt.ctr.key, sizeof(context.encrypt.ctr.key));
    print_hex("left inner hmac", context.hmac.inner_key, sizeof(context.hmac.inner_key));
    print_hex("left outer hmac", context.hmac.outer_key, sizeof(context.hmac.outer_key));
    print_hex("     left nonce", context.encrypt.ctr.iv_bytes, sizeof(context.encrypt.ctr.iv_bytes));

    result = mitosis_crypto_init(&context, false);
    if(!result) {
        printf("\n%s: %s mitosis_crypto_init failed!\n", __func__, "right");
        return false;
    }

    print_hex("\n       right key", context.encrypt.ctr.key, sizeof(context.encrypt.ctr.key));
    print_hex("right inner hmac", context.hmac.inner_key, sizeof(context.hmac.inner_key));
    print_hex("right outer hmac", context.hmac.outer_key, sizeof(context.hmac.outer_key));
    print_hex("     right nonce", context.encrypt.ctr.iv_bytes, sizeof(context.encrypt.ctr.iv_bytes));

    return result;
}

bool verify_key_encryption_decryption_test() {
    bool result = true;
    uint8_t data[] = {'a', 'b', 'c'};

    mitosis_crypto_context_t keys;
    for(int i = 0; i < 2; ++i) {
        char* key_half = (i ? "left" : "right");

        result = mitosis_crypto_init(&keys, i);
        if(!result) {
            printf("%s: %s mitosis_crypto_init failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_aes_ctr_encrypt(&(keys.encrypt), sizeof(data), data, data);
        if(!result) {
            printf("%s: %s mitosis_aes_ctr_encrypt in-place failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_aes_ctr_decrypt(&(keys.encrypt), sizeof(data), data, data);
        if(!result) {
            printf("%s: %s mitosis_aes_ctr_decrypt in-place failed!\n", __func__, key_half);
            return false;
        }

        if(!compare_expected(data, (uint8_t*)"abc", sizeof(data), __func__, key_half)) {
            return false;
        }
    }

    return result;
}

bool end_to_end_test() {
    bool result = true;
    const uint8_t verify[] = { 'a', 0xaa, 'c', 0x55 };
    const uint32_t counter = 0xe7b00a25;
    mitosis_crypto_payload_t data;
    uint8_t hmac_scratch[MITOSIS_HMAC_OUTPUT_SIZE];

    memcpy(data.data, verify, sizeof(verify));
    data.counter = counter;

    mitosis_crypto_context_t keys;
    for(int i = 0; i < 2; ++i) {
        char* key_half = (i ? "left" : "right");

        result = mitosis_crypto_init(&keys, i);
        if(!result) {
            printf("%s: %s mitosis_crypto_init failed!\n", __func__, key_half);
            return false;
        }

        keys.encrypt.ctr.iv.counter = data.counter;

        result = mitosis_aes_ctr_encrypt(&keys.encrypt, sizeof(data.data), data.data, data.data);
        if(!result) {
            printf("%s: %s mitosis_aes_ctr_encrypt in-place failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_hmac_hash(&keys.hmac, data.data, sizeof(data.data) + sizeof(data.counter));
        if(!result) {
            printf("%s: %s mitosis_hmac_hash failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_hmac_complete(&keys.hmac, hmac_scratch);
        if(!result) {
            printf("%s: %s mitosis_hmac_complete failed!\n", __func__, key_half);
            return false;
        }

        memcpy(data.mac, hmac_scratch, sizeof(data.mac));
        memset(hmac_scratch, 0, sizeof(hmac_scratch));

        // reinitialize keys to clear any state they had.
        result = mitosis_crypto_init(&keys, i);
        if(!result) {
            printf("%s: second %s mitosis_crypto_init failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_hmac_hash(&keys.hmac, data.data, sizeof(data.data) + sizeof(data.counter));
        if(!result) {
            printf("%s: second %s mitosis_hmac_hash failed!\n", __func__, key_half);
            return false;
        }

        result = mitosis_hmac_complete(&keys.hmac, hmac_scratch);
        if(!result) {
            printf("%s: second %s mitosis_hmac_complete failed!\n", __func__, key_half);
            return false;
        }

        if(!compare_expected(data.mac, hmac_scratch, sizeof(data.mac), __func__, key_half)) {
            printf("%s: %s hash verify failed!\n", __func__, key_half);
            return false;
        }

        keys.encrypt.ctr.iv.counter = data.counter;

        result = mitosis_aes_ctr_decrypt(&(keys.encrypt), sizeof(data.data), data.data, data.data);
        if(!result) {
            printf("%s: %s mitosis_aes_ctr_decrypt in-place failed!\n", __func__, key_half);
            return false;
        }

        if(!compare_expected(data.data, verify, sizeof(verify), __func__, key_half)) {
            printf("%s: %s data verify failed!\n", __func__, key_half);
            return false;
        }
    }

    return result;
}

int main(int argc, char** argv) {
    bool result = true;
    int failures = 0;

    RUN_TEST_LOG(hmac_sha256_kat);
    RUN_TEST_LOG(hkdf_kat);
    RUN_TEST_LOG(aes_ctr_kat);
    RUN_TEST_LOG(verify_key_generation);
    RUN_TEST_LOG(verify_key_encryption_decryption_test);
    RUN_TEST_LOG(end_to_end_test);

    if (result) {
        printf("All tests passed! :)\n");
    } else {
        printf("%d failures! :(\n", failures);
    }
    return 0;
}
