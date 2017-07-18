/**
 * @file tests/edge_count/main.c
 * @ref test-edge_count
 *
 * @page test-edge_count edge_count
 *
 * @todo Docs for test-edge_count
 *
 * @see tests/edge_count/main.c
 */
#include <xtf.h>

#define MAP_SIZE (1 << 16)
#define TEST_CASE_STR_SIZE 1000

/* Need to have some common understanding of architecture (32/64)
   between fuzzer and this server */

const char test_title[] = "Test edge_count";

/* distinguish between PC buffer and trace bits */
uint64_t trace_bits[MAP_SIZE];

char test_case_str[TEST_CASE_STR_SIZE];

/*
 from afl/docs/technical_details.txt:
   cur_location = <COMPILE_TIME_RANDOM>;
   shared_mem[cur_location ^ prev_location]++;
   prev_location = cur_location >> 1;

    How to do this in practice? Have a hashmap of random values
    assigned to program counters?
*/
//void parse_pcs_for_afl()
//{
//        for(long i = 0; i < ans; ++i) {
//            printk("%" PRIx64 "\n", arr[i]);
//        }
//}

void test_main(void)
{
    int ret;

    while( 1 )
    {
        /* receive test case */
        ret = pv_console_read(test_case_str, TEST_CASE_STR_SIZE);

        if(ret <= 0)
            xtf_failure("Couldn't read from AFL");

        /* execute test case */
        //TODO: actually use received string, do fancy parsing
        hypercall_xen_version(0, NULL);
    
        printk("Just finished executing the test case!\n");
    }

    xtf_success(NULL);
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
