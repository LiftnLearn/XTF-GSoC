#include <xtf/lib.h>
#include <xtf/traps.h>
#include <xtf/exlog.h>

#include <arch/decode.h>
#include <arch/lib.h>
#include <arch/processor.h>

bool (*xtf_unhandled_exception_hook)(struct cpu_regs *regs);

/*
 * C entry-point for exceptions, after the per-environment stubs have suitably
 * adjusted the stack.
 */
void do_exception(struct cpu_regs *regs)
{
    const struct extable_entry *ex;
    bool safe = false;

    xtf_exlog_log_exception(regs);

    /* Look in the exception table to see if a redirection has been set up. */
    if ( !safe && (ex = search_extable(regs->ip)) )
    {
        if ( ex->handler )
            safe = ex->handler(regs, ex);
        else
        {
            regs->ip = ex->fixup;
            safe = true;
        }
    }

    /*
     * If the test has installed an unhandled exception hook, call it in the
     * hope that it can resolve the exception.
     */
    if ( !safe && xtf_unhandled_exception_hook )
        safe = xtf_unhandled_exception_hook(regs);

    /* Still unresolved? Give up and panic() with some relevent information. */
    if ( !safe )
    {
        char buf[16];

        x86_exc_decode_ec(buf, ARRAY_SIZE(buf),
                          regs->entry_vector, regs->error_code);

        if ( regs->entry_vector == X86_EXC_PF )
        {
            unsigned long cr2 = read_cr2();

            panic("Unhandled exception at %04x:%p\n"
                  "Vec %u %s[%s] %%cr2 %p\n",
                  regs->cs, _p(regs->ip), regs->entry_vector,
                  x86_exc_short_name(regs->entry_vector), buf, _p(cr2));
        }
        else
            panic("Unhandled exception at %04x:%p\n"
                  "Vec %u %s[%s]\n",
                  regs->cs, _p(regs->ip), regs->entry_vector,
                  x86_exc_short_name(regs->entry_vector), buf);
    }
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
