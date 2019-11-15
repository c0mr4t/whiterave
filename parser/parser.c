#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_parsing_error(arg) \
    do { fprintf(stderr, "Usage: %s [-e <rsa|aes>] [-b <blockcipher>] [-s <keysize> (default: 256)]? [-m <message> | -f <m_file>] \n", arg); exit(EXIT_FAILURE); } while (0)

#ifndef MAX_KEYSIZE_BLOCKCIPHER
#define MAX_KEYSIZE_BLOCKCIPHER 256
#endif

start_conf* parse_arguments (int argc, char *argv[]) {
	int opt = 0;

    start_conf *ret = (start_conf *) malloc(sizeof(start_conf));
    ret->keysize = MAX_KEYSIZE_BLOCKCIPHER;

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
            case 's':
                if (ret->keysize != 512)
                    handle_parsing_error(argv[0]);
                ret->keysize = atoi(optarg);
                break;
            case 'm':
                if (ret->message != NULL || ret->m_file != NULL)
                    handle_parsing_error(argv[0]);
                ret->message = optarg;
                break;
            case 'f': // not implemented yet
                if (ret->m_file != NULL || ret->message != NULL)
                    handle_parsing_error(argv[0]);
                ret->m_file = optarg;
                break;
            case 'h':
                print_help_context(argv);
            default: /* '?' */
        	    handle_parsing_error(argv[0]);
        }
    }

    return ret;
}

void print_help_context(char *argv[]) { handle_parsing_error(argv[0]); }