typedef struct {
	char *encryption;
	char *blockcipher;
	char *keyfile;
	int keysize;
	char *message;
	char *m_file;
} start_conf;

start_conf *parse_arguments (int argc, char *argv[]);

void print_help_context(char *argv[]);
