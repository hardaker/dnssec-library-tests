UTILS=test_utils.c

all: val_test unbound_test keys ldns_test

val_test: val_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lval-threads -lsres -lpthread -lcrypto -lssl

unbound_test: unbound_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lunbound

ldns_test: ldns_test.c
	gcc -o $@ $(UTILS) $(CFLAGS) $< -lldns

keys:
	dig . dnskey > $@
