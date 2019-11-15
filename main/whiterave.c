#include "../parser/parser.h"
#include "../blockcipher/blockcipher.h"
#include "../blockcipher/error_blockcipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) { printf("Welcome to whiterave. Pass -h for help.\n"); exit(EXIT_SUCCESS); }

	start_conf *sc = parse_arguments(argc, argv);

    struct electronic_code_book_enc_ret *test;
    if((test = electronic_code_book_enc(sc->message, strlen(sc->message), sc->keysize)) == NULL) {
        ecb_errno(sc->message, strlen(sc->message), sc->keysize);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < test->number_of_blocks; ++i) {
        for (int j = 0; j < test->keysize / 8; ++j) {
            printf("%c", (test->enc)[i][j]);
        }
    }

    printf("\n");
	return EXIT_SUCCESS;
}