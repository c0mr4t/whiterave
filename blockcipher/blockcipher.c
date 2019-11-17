#include "blockcipher.h"
#include "error_blockcipher.h"
#include "../keygen/keygen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifndef MAX_M_LEN
#define MAX_M_LEN 65536
#endif

struct electronic_code_book_parameters* electronic_code_book_enc(struct electronic_code_book_parameters* input) {
	if (input->message == NULL) {
		FILE *fd = fopen(input->m_file, "r");
		input->message = (char *) malloc(MAX_M_LEN * sizeof(char));
		input->message = fgets(input->message, MAX_M_LEN, fd);
	}

	if (error_ecb_enc(input->message, strlen(input->message), input->keysize) == ERROR) // override
		return NULL;

	int keysize_bytes = input->keysize / 8;

	if (strlen(input->message) < keysize_bytes) {
		input->keysize = strlen(input->message) * 8;
		keysize_bytes = input->keysize / 8;
	}

	struct electronic_code_book_parameters *ret = input; 
	ret->m_len = strlen(input->message);
	ret->key = generate_ecb_key(input->keysize);
	ret->keysize = input->keysize; // not necessary
	ret->number_of_blocks = 1 + ((strlen(input->message) - 1) / (keysize_bytes)); // ceil(int division)
	ret->blocksize = keysize_bytes;
	ret->enc = (char **) malloc(ret->number_of_blocks * sizeof(char *));
	for (int i = 0; i < ret->number_of_blocks; ++i)
		ret->enc[i] = (char *) malloc(keysize_bytes * sizeof(char));

	pthread_t threads[ret->number_of_blocks];
	thread_arg_ecb_enc *arg;
	for (int i = 0; i < ret->number_of_blocks; ++i) {
		arg = (thread_arg_ecb_enc *) malloc(sizeof(thread_arg_ecb_enc));
		
		arg->id = i;
		arg->key = ret->key;
		arg->keysize_bytes = keysize_bytes;
		arg->blocksize = keysize_bytes;
		arg->message = &(input->message[arg->id * arg->blocksize]);
		arg->enc_block = &(*(ret->enc)[arg->id]);

		if (pthread_create(&threads[i], NULL, thread_start_ecb_enc_block, (void *) arg)) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	free(arg);

	for (int i = 0; i < ret->number_of_blocks; ++i) {
		if (pthread_join(threads[i], NULL)) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	return ret;
}

void *thread_start_ecb_enc_block (void *arg) {
	thread_arg_ecb_enc thread_data = *((thread_arg_ecb_enc *) arg);
	for (int i = 0; i < thread_data.keysize_bytes; ++i)
		thread_data.enc_block[i] = thread_data.message[i] ^ ((char *) thread_data.key)[i];
	
	return NULL;
}

struct electronic_code_book_parameters* electronic_code_book_dec(struct electronic_code_book_parameters* input) {
	struct electronic_code_book_parameters *ret = input;
	ret->message = (char *) malloc (input->m_len * sizeof(char));	

	int keysize_bytes = input->keysize / 8;

	pthread_t threads[ret->number_of_blocks];
	thread_arg_ecb_dec *arg;
	for (int i = 0; i < ret->number_of_blocks; ++i) {
		arg = (thread_arg_ecb_dec *) malloc(sizeof(thread_arg_ecb_dec));
		
		arg->id = i;
		arg->key = ret->key;
		arg->keysize_bytes = keysize_bytes;
		arg->blocksize = keysize_bytes;
		arg->message = &(ret->message[arg->id * arg->blocksize]);
		arg->dec_block = &(*(ret->enc)[arg->id]);

		if (pthread_create(&threads[i], NULL, thread_start_ecb_dec_block, (void *) arg)) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	free(arg);

	for (int i = 0; i < ret->number_of_blocks; ++i) {
		if (pthread_join(threads[i], NULL)) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	return ret;
}

void *thread_start_ecb_dec_block (void *arg) {
	thread_arg_ecb_dec thread_data = *((thread_arg_ecb_dec *) arg);
	for (int i = 0; i < thread_data.keysize_bytes; ++i)
		thread_data.message[i] = thread_data.dec_block[i] ^ ((char *) thread_data.key)[i];
	
	return NULL;
}

