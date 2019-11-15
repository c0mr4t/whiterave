#include "error_blockcipher.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef NO_ERROR
#define NO_ERROR 0
#endif

#ifndef ERROR
#define ERROR -1
#endif

int global_errno;

int error_ecb_enc(char* message, int m_len, int keysize) {
	if (m_len < 0)
		global_errno = 1;

	if (m_len == 0)
		global_errno = 2;

	if (keysize <= 0)
		global_errno = 3;

	if ((keysize % 8) != 0)
		global_errno = 4;

	/***************************************************/

	if (global_errno)
		return NO_ERROR;
	else
		return ERROR;
}

void ecb_errno(char *message, int m_len, int keysize) {
	switch(global_errno) {
		case 0: fprintf(stderr, "electronic_code_book_enc: no errors, everything appears to be fine\n"); 																		break;
		case 1: fprintf(stderr, "electronic_code_book_enc: message length is below zero (m_len = %d)\n", m_len);																break;
		case 2: fprintf(stderr, "electronic_code_book_enc: message length equals zero\n");		 																				break;
		case 3: fprintf(stderr, "electronic_code_book_enc: the chosen keysize is equal or less than 0 (keysize = %d)\n", keysize);									 			break;
		case 4: fprintf(stderr, "electronic_code_book_enc: the chosen keysize is not a multiple of 8 bit (keysize = %d, keysize mod 8 = %d)\n", keysize, keysize % 8); 			break;
		/***********************************************/

		default: fprintf(stderr, "electronic_code_book_enc: global_errno equals an unpexcted value (global_errno = %d)\n", global_errno); 										break;
	}

	global_errno = 0;
}
