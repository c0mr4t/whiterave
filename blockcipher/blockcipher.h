// bad coding style and only temporary
#ifndef cipher_standard_parameters
#define cipher_standard_parameters	\
	char *message;					\
	char *m_file;					\
									\
	char **enc;						\
	char *enc_file;					\
									\
	int m_len;						\
	int enc_len;					\
									\
	int *key;						\
	int keysize;					\
									\
	int number_of_blocks;			\
	int blocksize;					
#endif





struct electronic_code_book_parameters* electronic_code_book_enc(struct electronic_code_book_parameters* input);
struct electronic_code_book_parameters* electronic_code_book_dec(struct electronic_code_book_parameters* input);
struct electronic_code_book_parameters { cipher_standard_parameters };

void *thread_start_ecb_enc_block (void *arg);
typedef struct {
	int id;
	int *key;
	int keysize_bytes;
	char *message;
	int blocksize;
	char *enc_block;
} thread_arg_ecb_enc;

void *thread_start_ecb_dec_block (void *arg);
typedef struct {
	int id;
	int *key;
	int keysize_bytes;
	char *message;
	int blocksize;
	char *dec_block;
} thread_arg_ecb_dec;



struct cipher_block_chaining_parameters* cipher_block_chaining_enc(struct cipher_block_chaining_parameters* input);
struct cipher_block_chaining_parameters* cipher_block_chaining_dec(struct cipher_block_chaining_parameters* input);
struct cipher_block_chaining_parameters {
	cipher_standard_parameters
	int *init_vector;
};

char* PropagatingCipherBlockChainingEnc(char* message, int key);
char* PropagatingCipherBlockChainingDec(char* message, int key);

char* CipherFeedbackEnc(char* message, int key);
char* CipherFeedbackDec(char* message, int key);

char* OuputFeedbackEnc(char* message, int key);
char* OuputFeedbackDec(char* message, int key);

char* CounterEnc(char* message, int key);
char* CounterDec(char* message, int key);


/* Helper functions */
void message_or_file(char *message, char *file);
int adjust_keysize(char *message, int keysize);
void prepare_for_encryption(void* ret, char *message, int keysize, int keysize_bytes, int flag);