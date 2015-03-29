#include <xtf/types.h>
#include <xtf/console.h>
#include <xtf/hypercall.h>

#include <arch/x86/config.h>

#ifdef CONFIG_ENV_pv
/* Filled in by head_pv.S */
start_info_t *start_info = NULL;
#endif

static void xen_console_write(const char *buf, size_t len)
{
    hypercall_console_write(buf, len);
}

void arch_setup(void)
{
    register_console_callback(xen_console_write);
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