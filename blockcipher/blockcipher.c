#include "blockcipher.h"
#include "error_blockcipher.h"
#include "../keygen/keygen.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_start_ecb_enc_block (void *arg);

// m_len = length(message) in bytes
struct electronic_code_book_enc_ret* electronic_code_book_enc(char* message, int m_len, int keysize) {
	if (!error_ecb_enc(message, m_len, keysize)) // override
		return NULL;

	struct electronic_code_book_enc_ret *ret = malloc(sizeof(*ret)); 

	int keysize_bytes = keysize / 8;

	if (m_len < keysize_bytes) {
		keysize = m_len * 8;
		keysize_bytes = keysize / 8;
	}

	ret->key = generate_ecb_key(keysize);
	ret->keysize = keysize;
	ret->number_of_blocks = 1 + ((m_len - 1) / (keysize_bytes)); // ceil(int division)
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
		arg->message = &(message[i * arg->blocksize]);
		arg->ret = &(*(ret->enc)[arg->id]);

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
		thread_data.ret[i] = (thread_data.message)[i] ^ ((char *) thread_data.key)[i];
	
	return NULL;
}

// char* electronic_code_book_dec(char* message, int m_len, int *key, int keysize) {

// }

// void *thread_start_ecb_dec_block (void *arg) {
// 	thread_arg_ecb_enc thread_data = *((thread_arg_ecb_enc *) arg);
// 	for (int i = 0; i < thread_data.keysize_bytes; ++i)
// 		thread_data.ret[i] = (thread_data.message)[i] ^ ((char *) thread_data.key)[i];
	
// 	return NULL;
// }

