UTILS=test_utils.c

val_test: val_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lval-threads -lsres -lpthread -lcrypto -lssl
