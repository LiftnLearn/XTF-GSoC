/**
 * @file tests/xtf-server/main.c
 * @ref test-xtf-server
 *
 * @page test-xtf-server xtf-server
 *
 * This is the XTF-server for fuzzing
 * the hypervisor. It waits for input
 * via the console (thus in form of a
 * string) and parses it into a test case.
 *
 * There are some information encoded about
 * the hypercalls, i.e. some are excluded as
 * they might not return for certain arguments
 * or as they might stop the domain. Also, the
 * pointer macro is used to prevent the overwriting
 * of the code segments when passing buffers into Xen.
 *
 * Further improvements here would be to provide valid
 * buffers, etc. and encode more information about hypercalls.
 *
 * @see tests/xtf-server/main.c
 */
#include <xtf.h>

#define TEST_CASE_STR_SIZE 1000

/* avoid overwriting the code section in XTF when passing
   buffers to Xen */
#define POINTER(x) ((x >= 0xFFF00 && x < 0x110000) ? 0 : x)

const char test_title[] = "Test xtf-server";

char test_case_str[TEST_CASE_STR_SIZE];

void test_main(void)
{
    int ret;

    while( 1 )
    {
        /* receive test case */
        memset(test_case_str, 0, TEST_CASE_STR_SIZE);
        ret = pv_console_read_some(test_case_str, TEST_CASE_STR_SIZE);

        if(ret <= 0)
            xtf_failure("Couldn't read from AFL");

        long arg1, arg2, arg3, arg4;
        unsigned long hypercall_num = (*(long*) test_case_str) % 41;
        arg1 = *(((long*) test_case_str) + 1);
        arg2 = *(((long*) test_case_str) + 2);
        arg3 = *(((long*) test_case_str) + 3);
        arg4 = *(((long*) test_case_str) + 4);

        /* hypercalls annotated mostly according to xen/xen/include/xen/hypercall.h */

        /* execute test case */
        switch(hypercall_num)
        {
            case __HYPERVISOR_set_trap_table:
                (void) HYPERCALL1(long, __HYPERVISOR_set_trap_table, POINTER(arg1));
                break;
            case __HYPERVISOR_mmu_update:
                break;
            case __HYPERVISOR_set_gdt:
                (void) HYPERCALL2(long, __HYPERVISOR_set_gdt, POINTER(arg1), arg2);
                break;
            case __HYPERVISOR_stack_switch:
                (void) HYPERCALL2(long, __HYPERVISOR_stack_switch, arg1, arg2);
                break;
            case __HYPERVISOR_set_callbacks:
                (void) HYPERCALL4(long, __HYPERVISOR_set_callbacks, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_fpu_taskswitch:
                (void) HYPERCALL1(long, __HYPERVISOR_fpu_taskswitch, arg1);
                break;
            case __HYPERVISOR_sched_op_compat:
                break;
            case __HYPERVISOR_platform_op:
                (void) HYPERCALL4(long, __HYPERVISOR_platform_op, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_set_debugreg:
                (void) HYPERCALL2(long, __HYPERVISOR_set_debugreg, arg1, arg2);
                break;
            case __HYPERVISOR_get_debugreg:
                (void) HYPERCALL1(long, __HYPERVISOR_get_debugreg, arg1);
                break;
            case __HYPERVISOR_update_descriptor:
                (void) HYPERCALL1(long, __HYPERVISOR_update_descriptor, arg1);
                break;
            case __HYPERVISOR_memory_op:
                break;
            case __HYPERVISOR_multicall:
                break;
            case __HYPERVISOR_update_va_mapping:
//                (void) HYPERCALL4(long, __HYPERVISOR_update_va_mapping, arg1, arg2, arg3, arg4);
//                HYPERCALL4(long, 14, 1054551, 73014444032, 1114129, 9007207844680008);
                break;
            case __HYPERVISOR_set_timer_op:
                break;
            case __HYPERVISOR_event_channel_op_compat:
                break;
            case __HYPERVISOR_xen_version:
                (void) HYPERCALL2(long, __HYPERVISOR_xen_version, arg1, POINTER(arg2));
//              HYPERCALL2(long, __HYPERVISOR_xen_version, XENVER_changeset, NULL); (overwriting code .text segment)
                break;
            case __HYPERVISOR_console_io:
                break;
            case __HYPERVISOR_physdev_op_compat:
                break;
            case __HYPERVISOR_grant_table_op:
//                (void) HYPERCALL3(long, __HYPERVISOR_grant_table_op, arg1, POINTER(arg2), arg3);
//    HYPERCALL3(long, __HYPERVISOR_grant_table_op, 0, POINTER(63974), 1945555047933877647);
                break;
            case __HYPERVISOR_vm_assist:
                (void) HYPERCALL2(long, __HYPERVISOR_vm_assist, arg1, arg2);
                break;
            case __HYPERVISOR_update_va_mapping_otherdomain:
                (void) HYPERCALL4(long, __HYPERVISOR_update_va_mapping_otherdomain, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_iret:
                break;
            case __HYPERVISOR_vcpu_op:
//                (void) HYPERCALL4(long, __HYPERVISOR_vcpu_op, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_set_segment_base:
//                (void) HYPERCALL4(long, __HYPERVISOR_set_segment_base, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_mmuext_op:
                 (void) HYPERCALL4(long, __HYPERVISOR_mmuext_op, arg1, arg2, arg3, arg4);
                break;
           case __HYPERVISOR_xsm_op:
                (void) HYPERCALL1(long, __HYPERVISOR_xsm_op, POINTER(arg1));
                break;
            case __HYPERVISOR_nmi_op:
                (void) HYPERCALL2(long, __HYPERVISOR_nmi_op, arg1, POINTER(arg2));
                break;
            case __HYPERVISOR_sched_op:
                break;
            case __HYPERVISOR_callback_op:
                (void) HYPERCALL4(long, __HYPERVISOR_callback_op, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_xenoprof_op:
                (void) HYPERCALL2(long, __HYPERVISOR_xenoprof_op, arg1, POINTER(arg2));
                break;
            case __HYPERVISOR_event_channel_op:
                break;
            case __HYPERVISOR_physdev_op:
                break;
            case __HYPERVISOR_hvm_op:
                (void) HYPERCALL2(long, __HYPERVISOR_hvm_op, arg1, POINTER(arg2));
                break;
            case __HYPERVISOR_sysctl:
                break;
            case __HYPERVISOR_domctl:
                break;
            case __HYPERVISOR_kexec_op:
                (void) HYPERCALL2(long, __HYPERVISOR_kexec_op, arg1, POINTER(arg2));
                break;
            case __HYPERVISOR_tmem_op:
                (void) HYPERCALL1(long, __HYPERVISOR_tmem_op, arg1);
                break;
            case __HYPERVISOR_xc_reserved_op:
                (void) HYPERCALL4(long, __HYPERVISOR_xc_reserved_op, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_xenpmu_op:
                break;
            default:
                break;
        }

        /* A string to inform AFL that the hypercall has finished. Has
           to be shorter than 80 characters inorder to guarantee reading
           with a single read() call in AFL. */
        printk("Executed\n");
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
