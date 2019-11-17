#include "./mediator.h"
#include "../blockcipher/blockcipher.h"
#include "../blockcipher/error_blockcipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calling_with_start_conf(start_conf* input) {
    if (input->blockcipher != NULL) {
        if (strcmp(input->blockcipher, "ecb\n")) {
            call_ecb(input);
        }
    }
}

void call_ecb(start_conf* input) {
    struct electronic_code_book_parameters *ecb_input = (struct electronic_code_book_parameters *) malloc(sizeof(*ecb_input));
    ecb_input->message = input->message;
    ecb_input->m_file = input->m_file;
    ecb_input->keysize = input->keysize;

    struct electronic_code_book_parameters *ecb_output;
    if((ecb_output = electronic_code_book_enc(ecb_input)) == NULL) {
        ecb_errno(ecb_input->message, strlen(ecb_input->message), ecb_input->keysize);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ecb_output->number_of_blocks; ++i) {
        for (int j = 0; j < ecb_output->keysize / 8; ++j) {
            printf("%c", (ecb_output->enc)[i][j]);
        }
    }

    printf("\n");
}