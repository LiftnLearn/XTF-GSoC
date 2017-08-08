/**
 * @file tests/mk_hcall/main.c
 * @ref test-mk_hcall
 *
 * @page test-mk_hcall mk_hcall
 *
 * @todo Docs for test-mk_hcall
 *
 * @see tests/mk_hcall/main.c
 */
#include <xtf.h>
#include <xen/version.h>
//#include "callback.h"

#define TRACE_BUFFER_SIZE 500
#define POINTER(x) ((x >= 0xFFF00 && x < 0x110000) ? 0 : x)

const char test_title[] = "Test mk_hcall";

//void event_callback(void) {
//    printk("event callback\n");
//}
//
//void failsafe_callback(void) {
//    printk("failsafe callback\n");
//}
//
//void end_program(void) {
//    xtf_failure("event_callback\n");
//    xtf_exit();
//}

void test_main(void)
{
    uint64_t arr[TRACE_BUFFER_SIZE];
    long ans = 0;
    (void) ans;

    HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 0, TRACE_BUFFER_SIZE, arr);
    HYPERCALL3(long, __HYPERVISOR_grant_table_op, 0, POINTER(63974), 1945555047933877647);

    ans = HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 1, TRACE_BUFFER_SIZE, arr);

    printk("%ld\n", ans);
    for(long i = 0; i < ans; ++i) {
        printk("%" PRIx64 "\n", arr[i]);
    }

    printk("stop: %ld \n", ans);

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
