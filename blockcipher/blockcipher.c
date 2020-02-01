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

#define ECB_FLAG 0
#define CBC_FLAG 1

struct electronic_code_book_parameters* electronic_code_book_enc(struct electronic_code_book_parameters* input) {
	message_or_file(input->message, input->m_file);

	if (error_ecb_enc(input->message, strlen(input->message), input->keysize) == ERROR) // override
		return NULL;

	input->keysize = adjust_keysize(input->message, input->keysize);
	int keysize_bytes = input->keysize / 8;

	struct electronic_code_book_parameters *ret = input; // ret object contains also cleartext message..
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
	// free of error checking etc (yet) because ecb_enc must have been called before

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

struct cipher_block_chaining_parameters* cipher_block_chaining_enc(struct cipher_block_chaining_parameters* input) {
	message_or_file(input->message, input->m_file);

	// if (error_cbc_enc(input->message, strlen(input->message), input->keysize) == ERROR) // override
	// 	return NULL;

	input->keysize = adjust_keysize(input->message, input->keysize);
	int keysize_bytes = input->keysize / 8;

	struct cipher_block_chaining_parameters *ret = input; // ret object contains also cleartext message..
	ret->m_len = strlen(input->message);
	ret->key = generate_ecb_key(input->keysize);
	ret->keysize = input->keysize; // not necessary
	ret->number_of_blocks = 1 + ((strlen(input->message) - 1) / (keysize_bytes)); // ceil(int division)
	ret->blocksize = keysize_bytes;
	ret->enc = (char **) malloc(ret->number_of_blocks * sizeof(char *));
	for (int i = 0; i < ret->number_of_blocks; ++i)
		ret->enc[i] = (char *) malloc(keysize_bytes * sizeof(char));
	ret->init_vector = generate_ecb_key(input->keysize);


	for (int j = 0; j < keysize_bytes; ++j) {
		ret->enc[0][j] = ret->message[0 * keysize_bytes + j] ^ ((char *) ret->init_vector)[j];
		ret->enc[0][j] = ret->enc[0][j] ^ ((char *) ret->key)[j];
	}

	// encryption could get parallelized bytewise..
	for (int i = 1; i < ret->number_of_blocks; ++i) {
		for (int j = 0; j < keysize_bytes; ++j) {
			ret->enc[i][j] = ret->message[i * keysize_bytes + j] ^ ((char *) ret->enc[i - 1])[j];
			ret->enc[i][j] = ret->enc[i][j] ^ ((char *) ret->key)[j];
		}
	}

	return ret;	
}

struct cipher_block_chaining_parameters* cipher_block_chaining_dec(struct cipher_block_chaining_parameters* input) {
	// free of error checking etc because ecb_enc must have been called before

	struct cipher_block_chaining_parameters *ret = input;
	ret->message = (char *) malloc (input->m_len * sizeof(char));	

	int keysize_bytes = input->keysize / 8;

	for (int j = 0; j < keysize_bytes; ++j) {
		ret->enc[0][j] = ret->enc[0][j] ^ ((char *) ret->key)[j];
		ret->message[0] = ret->enc[0][j] ^ ((char *) ret->init_vector)[j];
	}

	// decryption could get parallelized bytewise as well..
	for (int i = 1; i < ret->number_of_blocks; ++i) {
		for (int j = 0; j < keysize_bytes; ++j) {
			ret->enc[i][j] = ret->enc[i][j] ^ ((char *) ret->key)[j];
			ret->message[i] = ret->enc[i][j] ^ ((char *) ret->enc[i - 1])[j];		
		}
	}

	return ret;
}

void message_or_file(char *message, char *file) {
	if (message == NULL) {
		FILE *fd = fopen(file, "r");
		message = (char *) malloc(MAX_M_LEN * sizeof(char));
		message = fgets(message, MAX_M_LEN, fd);
	}
}

int adjust_keysize(char *message, int keysize) {
	if (strlen(message) < keysize / 8) {
		keysize = strlen(message) * 8;
	}

	return keysize;
}

// void prepare_for_encryption(void* ret, char *message, int keysize, int keysize_bytes, int flag) {
// 	switch (flag) {
// 		case ECB_FLAG: 
// 			ret = ((struct electronic_code_book_parameters *) ret);
// 			break;
// 		case CBC_FLAG: 
// 			ret = ((struct cipher_block_chaining_parameters *) ret);
// 			break;
// 	}

// 	ret->m_len = strlen(message);
// 	ret->key = generate_ecb_key(keysize);
// 	ret->keysize = keysize; // not necessary
// 	ret->number_of_blocks = 1 + ((strlen(message) - 1) / (keysize_bytes)); // ceil(int division)
// 	ret->blocksize = keysize_bytes;
// 	ret->enc = (char **) malloc(ret->number_of_blocks * sizeof(char *));
// 	for (int i = 0; i < ret->number_of_blocks; ++i)
// 		ret->enc[i] = (char *) malloc(keysize_bytes * sizeof(char));
// }