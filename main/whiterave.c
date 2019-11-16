#include "../mediator/mediator.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) { printf("Welcome to whiterave. Pass -h for help.\n"); exit(EXIT_SUCCESS); }

	start_conf *sc = parse_arguments(argc, argv);

    calling_with_start_conf(sc);
	return EXIT_SUCCESS;
}

