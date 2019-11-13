ERR_BC = error_blockcipher.c error_blockcipher.h
blockcipher.o: blockcipher.c blockcipher.h $(ERR_BC)
	gcc -c blockcipher.c error_blockcipher.c

error_blockcipher.o: $(ERR_BC)
	gcc -c error_blockcipher.c
	
