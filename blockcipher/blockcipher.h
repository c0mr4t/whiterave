struct electronic_code_book_enc_ret* electronic_code_book_enc(char* message, int m_len, int keysize);
char* electronic_code_book_dec(char** messages, int m_len, int *key, int keysize);

char* CipherBlockChainingEnc(char* message, int key);
char* CipherBlockChainingDec(char* message, int key);

char* PropagatingCipherBlockChainingEnc(char* message, int key);
char* PropagatingCipherBlockChainingDec(char* message, int key);

char* CipherFeedbackEnc(char* message, int key);
char* CipherFeedbackDec(char* message, int key);

char* OuputFeedbackEnc(char* message, int key);
char* OuputFeedbackDec(char* message, int key);

char* CounterEnc(char* message, int key);
char* CounterDec(char* message, int key);

typedef struct {
	int id;
	int *key;
	int keysize_bytes;
	char *message;
	int blocksize;
	char *ret;
} thread_arg_ecb_enc;

struct electronic_code_book_enc_ret {
	char **enc;
	int *key;
	int keysize;
	int number_of_blocks;
	int blocksize;
};