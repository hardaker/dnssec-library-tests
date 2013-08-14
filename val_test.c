#include <sys/time.h>
#include <validator/validator-config.h>
#include <validator/validator.h>

#define LOOKUP_NAME "www.dnssec-tools.org"
#define LOOKUP_TYPE "A"

int
main(int argc, char **argv) {
    int number = 100000;
    val_status_t    val_status;
    int retval;
    struct addrinfo *val_ainfo = NULL;
    char           *service = NULL;
    struct addrinfo hints;
    time_t starttime, endtime;
    int i;
    val_context_t *context;

    if (argc > 1) {
        number = atoi(argv[1]);
    }

    // run once to check various results
    memset(&hints, 0, sizeof(struct addrinfo));
    retval = val_getaddrinfo(NULL, LOOKUP_NAME, service, &hints, &val_ainfo, &val_status);

    if (!val_isvalidated(val_status)) {
        fprintf(stderr, "something very wrong; not validated response returned\n");
    }
    fprintf(stderr, "validated response returned\n");

    // without a context
    fprintf(stderr, "starting %d queries without context....\n", number);
    // actually run the tests, timing them
    starttime = time(&starttime);
    for (i = 0; i < number; i++) {
        retval = val_getaddrinfo(NULL, LOOKUP_NAME, service, &hints, &val_ainfo, &val_status);
    }
    endtime = time(&endtime);
    fprintf(stderr, "time elapsed for %d queries: %f\n", number, difftime(endtime, starttime));

    // with context
    val_create_context(NULL, &context);
    fprintf(stderr, "starting %d queries with a context....\n", number);
    retval = val_getaddrinfo(context, LOOKUP_NAME, service, &hints, &val_ainfo, &val_status); // fully init it
    // actually run the tests, timing them
    starttime = time(&starttime);
    for (i = 0; i < number; i++) {
        retval = val_getaddrinfo(context, LOOKUP_NAME, service, &hints, &val_ainfo, &val_status);
    }
    endtime = time(&endtime);
    

    fprintf(stderr, "time elapsed for %d queries: %f\n", number, difftime(endtime, starttime));
}

