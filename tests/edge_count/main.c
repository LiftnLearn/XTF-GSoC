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
        long hypercall_num = *(long*) test_case_str; //TODO: or is this an int?
        arg1 = *(((long*) test_case_str) + 1);
        arg2 = *(((long*) test_case_str) + 2);
        arg3 = *(((long*) test_case_str) + 3);
        arg4 = *(((long*) test_case_str) + 4);

        //TODO: actually use received string, do fancy parsing
        /* execute test case */
        //hypercall_xen_version(0, NULL);
        //I suppose we don't care about the return value?

        switch(hypercall_num)
        {
            case __HYPERVISOR_set_trap_table:
                (void) HYPERCALL4(long, __HYPERVISOR_set_trap_table, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_mmu_update:
                (void) HYPERCALL4(long, __HYPERVISOR_mmu_update, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_set_gdt:
                (void) HYPERCALL4(long, __HYPERVISOR_set_gdt, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_stack_switch:
                (void) HYPERCALL4(long, __HYPERVISOR_stack_switch, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_set_callbacks:
                (void) HYPERCALL4(long, __HYPERVISOR_set_callbacks, arg1, arg2, arg3, arg4);
                break;
            case __HYPERVISOR_fpu_taskswitch:
            case __HYPERVISOR_sched_op_compat:
            case __HYPERVISOR_platform_op:
            case __HYPERVISOR_set_debugreg:
            case __HYPERVISOR_get_debugreg:
            case __HYPERVISOR_update_descriptor:
            case __HYPERVISOR_memory_op:
            case __HYPERVISOR_multicall:
            case __HYPERVISOR_update_va_mapping:
            case __HYPERVISOR_set_timer_op:
            case __HYPERVISOR_event_channel_op_compat:
            case __HYPERVISOR_xen_version:
                (void) HYPERCALL4(long, __HYPERVISOR_xen_version, arg1, arg2, arg3, arg4);
            case __HYPERVISOR_console_io:
            case __HYPERVISOR_physdev_op_compat:
            case __HYPERVISOR_grant_table_op:
            case __HYPERVISOR_vm_assist:
            case __HYPERVISOR_update_va_mapping_otherdomain:
            case __HYPERVISOR_iret:
            case __HYPERVISOR_vcpu_op:
            case __HYPERVISOR_set_segment_base:
            case __HYPERVISOR_mmuext_op:
            case __HYPERVISOR_xsm_op:
            case __HYPERVISOR_nmi_op:
            case __HYPERVISOR_sched_op:
            case __HYPERVISOR_callback_op:
            case __HYPERVISOR_xenoprof_op:
            case __HYPERVISOR_event_channel_op:
            case __HYPERVISOR_physdev_op:
            case __HYPERVISOR_hvm_op:
            case __HYPERVISOR_sysctl:
            case __HYPERVISOR_domctl:
            case __HYPERVISOR_kexec_op:
            case __HYPERVISOR_tmem_op:
            case __HYPERVISOR_xc_reserved_op:
            case __HYPERVISOR_xenpmu_op:
            default:
                (void) HYPERCALL4(long, __HYPERVISOR_xenpmu_op, arg1, arg2, arg3, arg4);
        }
    
        printk("%ld %ld %ld %ld %ld", hypercall_num, arg1, arg2, arg3, arg4);
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
