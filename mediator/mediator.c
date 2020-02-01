#include "./mediator.h"
#include "../blockcipher/blockcipher.h"
#include "../blockcipher/error_blockcipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calling_with_start_conf(start_conf* input) {
    int error = 1;

    if (input->blockcipher != NULL) {
        if (!strcmp(input->blockcipher, "ecb\0")) {
            call_ecb(input);
            error = 0;
        }

        // if (!strcmp(input->blockcipher, "cbc\0")) {
        //     call_cbc(input);
        //     error = 0;
        // }
    }

    if (error) {
        printf("Error! The chosen block cipher mode of operation is not available yet.\n");
        exit(EXIT_FAILURE);    
    }
}

void call_ecb(start_conf* input) {
    struct electronic_code_book_parameters *ecb_input = (struct electronic_code_book_parameters *) malloc(sizeof(*ecb_input));
    ecb_input->message = input->message;
    ecb_input->m_file = input->m_file;
    ecb_input->keysize = input->keysize;

    // encode
    struct electronic_code_book_parameters *ecb_output_enc;
    if((ecb_output_enc = electronic_code_book_enc(ecb_input)) == NULL) {
        ecb_errno(ecb_input->message, strlen(ecb_input->message), ecb_input->keysize);
        exit(EXIT_FAILURE);
    }


    printf("Encrypted: ");
    for (int i = 0; i < ecb_output_enc->number_of_blocks; ++i) {
        for (int j = 0; j < ecb_output_enc->keysize / 8; ++j) {
            printf("%c", (ecb_output_enc->enc)[i][j]);
        }
    }

    printf("\n");

    // decode
    struct electronic_code_book_parameters *ecb_output_dec;
    if((ecb_output_dec = electronic_code_book_dec(ecb_output_enc)) == NULL) {
        ecb_errno(ecb_output_enc->message, strlen(ecb_output_enc->message), ecb_output_enc->keysize);
        exit(EXIT_FAILURE);
    }

    printf("Decrypted: ");
    for (int i = 0; i < strlen(ecb_output_dec->message); ++i)
        printf("%c", ecb_output_dec->message[i]);

    free(ecb_input);
    free(ecb_output_enc); // unnecessary
    free(ecb_output_dec); // unnecessary

    printf("\n");
}

void call_cbc(start_conf* input) {
    struct cipher_block_chaining_parameters *cbc_input = (struct cipher_block_chaining_parameters *) malloc(sizeof(*cbc_input));
    cbc_input->message = input->message;
    cbc_input->m_file = input->m_file;
    cbc_input->keysize = input->keysize;

    // encode
    struct cipher_block_chaining_parameters *cbc_output_enc;
    if((cbc_output_enc = cipher_block_chaining_dec(cbc_input)) == NULL) {
        // cbc_errno(cbc_input->message, strlen(cbc_input->message), cbc_input->keysize);
        exit(EXIT_FAILURE);
    }


    printf("Encrypted: ");
    for (int i = 0; i < cbc_output_enc->number_of_blocks; ++i) {
        for (int j = 0; j < cbc_output_enc->keysize / 8; ++j) {
            printf("%c", (cbc_output_enc->enc)[i][j]);
        }
    }

    printf("\n");

    // decode
    struct cipher_block_chaining_parameters *cbc_output_dec;
    if((cbc_output_dec = cipher_block_chaining_dec(cbc_output_enc)) == NULL) {
        // cbc_errno(cbc_output_enc->message, strlen(cbc_output_enc->message), cbc_output_enc->keysize);
        exit(EXIT_FAILURE);
    }


    printf("Decrypted: ");
    for (int i = 0; i < strlen(cbc_output_dec->message); ++i)
        printf("%c", cbc_output_dec->message[i]);

    free(cbc_input);
    free(cbc_output_enc); // unnecessary
    free(cbc_output_dec); // unnecessary

    printf("\n");
}