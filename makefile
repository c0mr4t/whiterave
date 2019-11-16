OBJ = ./main/whiterave.o ./parser/parser.o ./mediator/mediator.o ./keygen/keygen.o ./blockcipher/blockcipher.o ./blockcipher/error_blockcipher.o

whiterave: $(OBJ)
	gcc -o $@ $(OBJ) -pthread -lbsd -Wall

all:
	+$(MAKE) -C ./main/
	+$(MAKE) -C ./parser/
	+$(MAKE) -C ./mediator/
	+$(MAKE) -C ./blockcipher/
	+$(MAKE) -C ./keygen/
