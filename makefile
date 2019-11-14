all:
	+$(MAKE) -C ./main/
	+$(MAKE) -C ./parser/
	+$(MAKE) -C ./blockcipher/
	+$(MAKE) -C ./keygen/
