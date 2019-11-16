struct electronic_code_book_parameters* electronic_code_book_enc(struct electronic_code_book_parameters* input);
struct electronic_code_book_parameters* electronic_code_book_dec(struct electronic_code_book_parameters* input);

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

struct electronic_code_book_parameters {
	char *message;
	char *m_file;

	char **enc;
	char *enc_file;
	
	int m_len;
	int enc_len;

	int *key;
	int keysize;
	
	int number_of_blocks;
	int blocksize;
};