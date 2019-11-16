#include "./mediator.h"
#include "../blockcipher/blockcipher.h"
#include "../blockcipher/error_blockcipher.h"

#include <string.h>

void calling_with_start_conf(start_conf* input) {
    if (strcmp(input->blockcipher, "ecb\n")) {
        call_ecb(input);
    }

    // struct electronic_code_book_parameters *ecb_input = (struct electronic_code_book_parameters *) malloc(sizeof(*ecb_input));
    // ecb_input->message = sc->message;
    // ecb_input->m_file = sc->m_file;
    // ecb_input->keysize = sc->keysize;
    // struct electronic_code_book_parameters *test;

    // if((test = electronic_code_book_enc(ecb_input)) == NULL) {
    //     ecb_errno(sc->message, strlen(sc->message), sc->keysize);
    //     exit(EXIT_FAILURE);
    // }

    // for (int i = 0; i < test->number_of_blocks; ++i) {
    //     for (int j = 0; j < test->keysize / 8; ++j) {
    //         printf("%c", (test->enc)[i][j]);
    //     }
    // }

    // printf("\n");
}

void call_ecb(start_conf* input) {

}