#ifndef XTF_X86_32_HYPERCALL_H
#define XTF_X86_32_HYPERCALL_H

/*
 * Hypercall primatives for 32bit
 *
 * Inputs: %ebx, %ecx, %edx, %esi, %edi, %ebp (arguments 1-6)
 */

#define _hypercall32_2(type, name, a1, a2)                              \
    ({                                                                  \
        long __res, __ign1, __ign2;                                     \
        asm volatile (                                                  \
            "call hypercall_page + %c[offset]"                          \
            : "=a" (__res), "=b" (__ign1), "=c" (__ign2)                \
            : [offset] "i" (__HYPERVISOR_##name * 32),                  \
              "1" ((long)(a1)), "2" ((long)(a2))                        \
            : "memory" );                                               \
        (type)__res;                                                    \
    })

#endif /* XTF_X86_32_HYPERCALL_H */

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
