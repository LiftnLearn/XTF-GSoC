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

#define TRACE_BUFFER_SIZE 500

const char test_title[] = "Test edge_count";

void test_main(void)
{
//    uint64_t arr[TRACE_BUFFER_SIZE];
        
    int ret;
    char str[100];

    //for(int j = 0; j < 10; ++j) {
    //    ret = read(str, 100);
    //    printk("%u %u\n", ((uint32_t*) str)[0], ((uint32_t*) str)[1]);
    //    printk("%d Read: %s\n", ret, str+8);
    //}

    for( int i = 0; i < 10; ++i ) {
        ret = pv_console_read(str, 100);
        printk("Hello from the XTF-server: %d\n", ret);
    }

//    printk("%d Start tracing: %ld\n", DOMID_SELF,
//        hypercall_edge_trace(DOMID_SELF, 0, TRACE_BUFFER_SIZE, arr));

//    hypercall_xen_version(0, NULL);

//   printk("Stop tracing: %ld\n",
 //       hypercall_edge_trace(DOMID_SELF, 1, TRACE_BUFFER_SIZE, arr));


//    long ans = hypercall_edge_trace(DOMID_SELF, 1, TRACE_BUFFER_SIZE, arr);
//
//    for(long i = 0; i < ans; ++i) {
//        printk("%" PRIx64 "\n", arr[i]);
//    }
//
//    printk("stop: %ld \n", ans);

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
