/*
 * a is a small program that prints out the a records
 * for a particular domain
 * (c) NLnet Labs, 2005 - 2008
 * See the file LICENSE for the license
 */

#include <ldns/ldns.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define LOOKUP_NAME "www.dnssec-tools.org"

int
main(int argc, char *argv[])
{
        ldns_resolver *res;
        ldns_rdf *domain;
        ldns_pkt *p;
        ldns_rr_list *a;
        ldns_status s;
        
        struct timeval starttime, endtime;
        int i;

        int number = 100000;

        if (argc > 1) {
            number = atoi(argv[1]);
        }

        p = NULL;
        a = NULL;
        domain = NULL;
        res = NULL;
        
        /* create a rdf from the command line arg */
        domain = ldns_dname_new_frm_str(LOOKUP_NAME);

        /* create a new resolver from /etc/resolv.conf */
        s = ldns_resolver_new_frm_file(&res, "resolv.conf");

        if (s != LDNS_STATUS_OK) {
                exit(EXIT_FAILURE);
        }

        /* use the resolver to send a query for the a 
         * records of the domain given on the command line
         */
        p = ldns_resolver_query(res,
                                domain,
                                LDNS_RR_TYPE_A,
                                LDNS_RR_CLASS_IN,
                                LDNS_RD);

        ldns_rdf_deep_free(domain);
        
        if (!p)  {
                exit(EXIT_FAILURE);
        } else {
                /* retrieve the A records from the answer section of that
                 * packet
                 */
                a = ldns_pkt_rr_list_by_type(p,
                                              LDNS_RR_TYPE_A,
                                              LDNS_SECTION_ANSWER);
                if (!a) {
                        fprintf(stderr, 
                                        " *** invalid answer name %s after A query for %s\n",
                                        argv[1], argv[1]);
                        ldns_pkt_free(p);
                        ldns_resolver_deep_free(res);
                        exit(EXIT_FAILURE);
                } else {
                        ldns_rr_list_sort(a); 
                        ldns_rr_list_print(stdout, a);
                        ldns_rr_list_deep_free(a);
                }
        }
        ldns_pkt_free(p);

        fprintf(stderr, "starting %d queries....\n", number);

        // actually run the tests, timing them
        gettimeofday(&starttime, NULL);
        for (i = 0; i < number; i++) {
            p = ldns_resolver_query(res,
                                    domain,
                                    LDNS_RR_TYPE_A,
                                    LDNS_RR_CLASS_IN,
                                    LDNS_RD);
        }
        gettimeofday(&endtime, NULL);
        fprintf(stderr, "time elapsed (ms) for %d queries: %d\n", number, timeofday_diff(&starttime, &endtime));


        
        ldns_resolver_deep_free(res);
        return 0;
}
