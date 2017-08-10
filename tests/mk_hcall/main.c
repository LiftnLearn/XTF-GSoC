/**
 * @file tests/mk_hcall/main.c
 * @ref test-mk_hcall
 *
 * @page test-mk_hcall mk_hcall
 *
 * This test case is a helper to debug tracing and
 * fuzzing. It executes a particular hypercall and
 * prints the information obtained from tracing to
 * the console..
 *
 * @see tests/mk_hcall/main.c
 */
#include <xtf.h>
#include <xen/version.h>

#define TRACE_BUFFER_SIZE 500
#define POINTER(x) ((x >= 0xFFF00 && x < 0x110000) ? 0 : x)

const char test_title[] = "Test mk_hcall";

void test_main(void)
{
    uint64_t arr[TRACE_BUFFER_SIZE];
    long ans;

    HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 0, TRACE_BUFFER_SIZE, arr);

    /* the actual hypercall that should be traced */
    HYPERCALL3(long, __HYPERVISOR_grant_table_op, 0, POINTER(63974), 7);

    ans = HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 1, TRACE_BUFFER_SIZE, arr);

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
