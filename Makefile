val_test: val_test.c
	gcc -o $@ $< -lval-threads -lsres -lpthread -lcrypto -lssl
