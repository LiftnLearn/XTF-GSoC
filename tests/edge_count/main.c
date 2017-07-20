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

//        for(long i = 0; i < ans; ++i) {
//            printk("%" PRIx64 "\n", arr[i]);
//        }

void test_main(void)
{
    int ret;

    while( 1 )
    {
        /* receive test case */
        ret = pv_console_read_some(test_case_str, TEST_CASE_STR_SIZE);

        if(ret <= 0)
            xtf_failure("Couldn't read from AFL");

        long arg1, arg2, arg3, arg4;
//        long hypercall_num = *(long*) test_case_str; //TODO: or is this an int?
        arg1 = *(((long*) test_case_str) + 1);
        arg2 = *(((long*) test_case_str) + 2);
        arg3 = *(((long*) test_case_str) + 3);
        arg4 = *(((long*) test_case_str) + 4);

        //TODO: actually use received string, do fancy parsing
        /* execute test case */
        //hypercall_xen_version(0, NULL);
        (void)HYPERCALL4(long, __HYPERVISOR_mmu_update, arg1, arg2, arg3, arg4);
        //I suppose we don't care about the return value?
    
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
