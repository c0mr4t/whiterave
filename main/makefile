OBJ = ../main/whiterave.o ../parser/parser.o ../keygen/keygen.o ../blockcipher/blockcipher.o ../blockcipher/error_blockcipher.o

../whiterave: $(OBJ)
	gcc -o $@ $(OBJ) -pthread -lbsd