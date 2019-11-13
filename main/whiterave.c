#include "whiterave.h"
#include "../blockcipher/blockcipher.h"
#include "../blockcipher/error_blockcipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define handle_parsing_error(arg) \
    do { fprintf(stderr, "Usage: %s [-e <rsa|aes>] [-b <blockcipher>] [-k <keyfile>] [-s <keysize>] [-m <message>] \n", arg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char *argv[]) {
	start_conf *sc = parse_arguments(argc, argv);
    // if(sc == NULL);
        // print_help_context(argv);

    char* x = sc->message;

    char **test;
    if((test = electronic_code_book_enc(x, strlen(x), sc->keysize)) == NULL) {
        ecb_errno(x, strlen(x), sc->keysize);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < strlen(x) / (sc->keysize / 8); ++i) {
        for (int j = 0; j < sc->keysize / 8; ++j) {
            printf("%c", test[i][j]);
        }
    }

    printf("\n");
	return EXIT_SUCCESS;
}

// -e encrypt?, -b block-cipher|null, -k key_file?, -s keysize, 

start_conf* parse_arguments (int argc, char *argv[]) {
	int opt = 0;

    start_conf *ret = (start_conf *) malloc(sizeof(start_conf));

    // if keysize and keyfile is provided, one could check a constraint if size matches with key in file
	while ((opt = getopt(argc, argv, "he:b:k:s:m:f:")) != -1) {
    	switch (opt) {
        	case 'e':
        		if (ret->encryption != NULL)
        			handle_parsing_error(argv[0]);
        		ret->encryption = optarg;
           		break;
            case 'b':
            	if (ret->blockcipher != NULL)
            		handle_parsing_error(argv[0]);
            	ret->blockcipher = optarg;
               	break;
            case 'k':
            	if (ret->keyfile != NULL && ret->keysize == 0)
            		handle_parsing_error(argv[0]);
                ret->keyfile = optarg;
               	break;
            case 's':
                // if (ret->keyfile == NULL && ret->keysize != 0)
                //     handle_parsing_error(argv[0]);
                ret->keysize = atoi(optarg);
                break;
            case 'm':
                // if (ret->message == NULL && ret->m_file == NULL)
                //     handle_parsing_error(argv[0]);
                ret->message = optarg;
                break;
            case 'f': // not implemented yet
                if (ret->m_file == NULL && ret->message == NULL)
                    handle_parsing_error(argv[0]);
                ret->m_file = optarg;
                break;
            case 'h':
                // return NULL;
            default: /* '?' */
        	    handle_parsing_error(argv[0]);
        }
    }

    return ret;
}

void print_help_context(char *argv[]) {
    handle_parsing_error(argv[0]);
}