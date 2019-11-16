#ifndef NO_ERROR
#define NO_ERROR 0
#endif

#ifndef ERROR
#define ERROR -1
#endif

int error_ecb_enc(char* message, int m_len, int keysize);
void ecb_errno(char* message, int m_len, int keysize);

