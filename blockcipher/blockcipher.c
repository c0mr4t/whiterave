#include "blockcipher.h"
#include "error_blockcipher.h"
#include "../keygen/keygen.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_start_ecb_enc_block (void *arg);

// m_len = length(message) in bytes
char** electronic_code_book_enc(char* message, int m_len, int keysize) {

	if (!error_ecb_enc(message, m_len, keysize))
		return NULL;

	int keysize_bytes = keysize / 8;
	int *key = generate_ecb_key(keysize);

	int number_of_blocks = 1 + ((m_len - 1) / (keysize_bytes)); // ceil(int div)
	char **ret = (char **) malloc(number_of_blocks * sizeof(char *));
	for (int i = 0; i < number_of_blocks; ++i)
		ret[i] = (char *) malloc(keysize_bytes * sizeof(char));

	pthread_t threads[number_of_blocks];
	thread_arg_ecb_enc *arg;
	for (int i = 0; i < number_of_blocks; ++i) {
		arg = (thread_arg_ecb_enc *) malloc(sizeof(thread_arg_ecb_enc));
		
		arg->id = i;
		arg->key = key;
		arg->keysize_bytes = keysize_bytes;
		arg->blocksize = keysize_bytes;
		arg->message = &(message[i * arg->blocksize]);
		arg->ret = &(*ret[arg->id]);

		if (pthread_create(&threads[i], NULL, thread_start_ecb_enc_block, (void *) arg)) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	free(arg);

	for (int i = 0; i < number_of_blocks; ++i) {
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

