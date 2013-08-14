UTILS=test_utils.c

all: val_test unbound_test keys

val_test: val_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lval-threads -lsres -lpthread -lcrypto -lssl

unbound_test: unbound_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lunbound

keys:
	dig . dnskey > $@
