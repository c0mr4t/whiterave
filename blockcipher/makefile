BC = blockcipher.c
BCH = blockcipher.h
ERR_BC = error_blockcipher.c
ERR_BCH = error_blockcipher.h

blockcipher.o: $(BC) $(BCH) $(ERR_BC) $(ERR_BCH)
	gcc -c $(BC) $(ERR_BC)

error_blockcipher.o: $(ERR_BC)
	gcc -c $(ERR_BC)
	
