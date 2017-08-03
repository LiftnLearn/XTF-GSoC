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
//    uint64_t arr[TRACE_BUFFER_SIZE];
//    xen_callback_register_t callback_struct;
//    callback_struct.flags = 0;
    long ans;
    (void) ans;


//    callback_struct.type = CALLBACKTYPE_event;
//    callback_struct.address = (unsigned long) event_callback;
//    callback_struct.address = (unsigned long) hypervisor_callback;
//    hypercall_register_callback(&callback_struct);
//
//    callback_struct.type = CALLBACKTYPE_failsafe;
//    callback_struct.address = (unsigned long) failsafe_callback;
//    hypercall_register_callback(&callback_struct);

//    HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 0, TRACE_BUFFER_SIZE, arr);
//    HYPERCALL4(long, 1, 19774012653568, 5046755972349952, 1049600, 0);
//    HYPERCALL2(long, __HYPERVISOR_sched_op_compat, 1, 0); //13954867);
//    HYPERCALL4(long, 14, 1054551, 73014444032, 1114129, 9007207844680008);
 //   printk("hello");
//    ans = HYPERCALL2(long, __HYPERVISOR_xen_version, XENVER_changeset, 1048570);
    ans = HYPERCALL2(long, __HYPERVISOR_xen_version, XENVER_changeset, 0x100000);
//    ans = HYPERCALL2(long, __HYPERVISOR_xen_version, XENVER_changeset, NULL);
    printk("Prink after hypercall\n");
//    printk("%ld\n", ans);

//    ans = HYPERCALL4(long, __HYPERVISOR_trace_pc, DOMID_SELF, 1, TRACE_BUFFER_SIZE, arr);

 //   for(long i = 0; i < ans; ++i) {
 //       printk("%" PRIx64 "\n", arr[i]);
 //   }

 //   printk("stop: %ld \n", ans);

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
