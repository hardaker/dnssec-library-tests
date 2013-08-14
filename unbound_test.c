#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unbound.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define LOOKUP_NAME "www.dnssec-tools.org"

int main(int argc, char **argv)
{
	struct ub_ctx* ctx;
	struct ub_result* result;
	int retval;
        int i;
        struct timeval starttime, endtime;

        int number = 100000;
        if (argc > 1) {
            number = atoi(argv[1]);
        }

	/* create context */
	ctx = ub_ctx_create();
	if(!ctx) {
		printf("error: could not create unbound context\n");
		return 1;
	}

	/* read /etc/resolv.conf for DNS proxy settings (from DHCP) */
	if( (retval=ub_ctx_resolvconf(ctx, "resolv.conf")) != 0) {
		printf("error reading resolv.conf: %s. errno says: %s\n", 
			ub_strerror(retval), strerror(errno));
		return 1;
	}

	/* read /etc/hosts for locally supplied host addresses */
	if( (retval=ub_ctx_hosts(ctx, "hosts")) != 0) {
		printf("error reading hosts: %s. errno says: %s\n", 
			ub_strerror(retval), strerror(errno));
		return 1;
	}

	/* read public keys for DNSSEC verification */
	if( (retval=ub_ctx_add_ta_file(ctx, "keys")) != 0) {
		printf("error adding keys: %s\n", ub_strerror(retval));
		return 1;
	}

	/* query for webserver */
	retval = ub_resolve(ctx, LOOKUP_NAME, 1, 1, &result);
	if(retval != 0) {
		printf("resolve error: %s\n", ub_strerror(retval));
		return 1;
	}

	/* show first result */
	if(result->havedata)
		printf("The address is %s\n", 
			inet_ntoa(*(struct in_addr*)result->data[0]));
	/* show security status */
	if(!result->secure) {
            fprintf(stderr, "something very wrong; not validated response returned\n");
            exit(1);
        }
        fprintf(stderr, "validated response returned\n");

        // Note: this is without proper memory freeing
        fprintf(stderr, "starting %d queries without context....\n", number);
        gettimeofday(&starttime, NULL);
        for (i = 0; i < number; i++) {
            retval = ub_resolve(ctx, LOOKUP_NAME, 1, 1, &result);
        }
        gettimeofday(&endtime, NULL);
        fprintf(stderr, "time elapsed (ms) for %d queries: %d\n", number, timeofday_diff(&starttime, &endtime));

	ub_resolve_free(result);
	ub_ctx_delete(ctx);

	return 0;
}
