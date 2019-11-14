#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_parsing_error(arg) \
    do { fprintf(stderr, "Usage: %s [-e <rsa|aes>] [-b <blockcipher>] [-k <keyfile>] [-s <keysize>] [-m <message>] \n", arg); exit(EXIT_FAILURE); } while (0)

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
                if (ret->keyfile != NULL || ret->keysize != 0)
                    handle_parsing_error(argv[0]);
                ret->keysize = atoi(optarg);
                break;
            case 'm':
                if (ret->message != NULL || ret->m_file != NULL)
                    handle_parsing_error(argv[0]);
                ret->message = optarg;
                break;
            case 'f': // not implemented yet
                if (ret->m_file == NULL && ret->message == NULL)
                    handle_parsing_error(argv[0]);
                ret->m_file = optarg;
                break;
            case 'h':
                print_help_context(argv);
                exit(EXIT_SUCCESS);
            default: /* '?' */
        	    handle_parsing_error(argv[0]);
        }
    }

    return ret;
}

void print_help_context(char *argv[]) { handle_parsing_error(argv[0]); }