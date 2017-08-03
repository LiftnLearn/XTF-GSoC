.globl end_program
.globl event_callback

//#define XEN_GET_VCPU_INFO(reg)  movq HYPERVISOR_shared_info,reg
#define XEN_LOCKED_BLOCK_EVENTS(reg)    movb $1,evtchn_upcall_mask(reg)
#define XEN_LOCKED_UNBLOCK_EVENTS(reg)  movb $0,evtchn_upcall_mask(reg)
#define XEN_TEST_PENDING(reg)   testb $0xFF,evtchn_upcall_pending(reg)

/* Offsets into shared_info_t. */                
#define evtchn_upcall_pending		/* 0 */
#define evtchn_upcall_mask		1

NMI_MASK = 0x80000000
KERNEL_CS_MASK = 0xfc

#define RAX       80
#define RDI      112
#define ORIG_RAX 120       /* + error_code */
#define RIP      128
#define CS       136
#define RFLAGS   144
#define RSP      152

#define __PAGE_SHIFT      12
#define __PAGE_SIZE       (1UL << __PAGE_SHIFT)

.globl shared_info, hypercall_page
        /* Unpleasant -- the PTE that maps this page is actually overwritten */
        /* to map the real shared-info page! :-)                             */
        .align __PAGE_SIZE
//shared_info:
//        .fill __PAGE_SIZE,1,0
//
//hypercall_page:
//        .fill __PAGE_SIZE,1,0

.macro RESTORE_ALL
    movq (%rsp),%r15
    movq 1*8(%rsp),%r14
    movq 2*8(%rsp),%r13
    movq 3*8(%rsp),%r12
    movq 4*8(%rsp),%rbp
    movq 5*8(%rsp),%rbx
    movq 6*8(%rsp),%r11
    movq 7*8(%rsp),%r10
    movq 8*8(%rsp),%r9
    movq 9*8(%rsp),%r8
    movq 10*8(%rsp),%rax
    movq 11*8(%rsp),%rcx
    movq 12*8(%rsp),%rdx
    movq 13*8(%rsp),%rsi
    movq 14*8(%rsp),%rdi
    addq $15*8+8,%rsp
.endm

.macro SAVE_ALL
    /* rdi slot contains rax, oldrax contains error code */
    cld
    subq $14*8,%rsp
    movq %rsi,13*8(%rsp)
    movq 14*8(%rsp),%rsi    /* load rax from rdi slot */
    movq %rdx,12*8(%rsp)
    movq %rcx,11*8(%rsp)
    movq %rsi,10*8(%rsp)    /* store rax */
    movq %r8, 9*8(%rsp)
    movq %r9, 8*8(%rsp)
    movq %r10,7*8(%rsp)
    movq %r11,6*8(%rsp)
    movq %rbx,5*8(%rsp)
    movq %rbp,4*8(%rsp)
    movq %r12,3*8(%rsp)
    movq %r13,2*8(%rsp)
    movq %r14,1*8(%rsp)
    movq %r15,(%rsp)
    movq %rdi, RDI(%rsp)    /* put rdi into the slot */
.endm

.macro HYPERVISOR_IRET
#ifdef CONFIG_PARAVIRT
	testl $NMI_MASK,2*8(%rsp)
	jnz   2f

	/* Direct iret to kernel space. Correct CS and SS. */
	orb   $3,1*8(%rsp)
	orb   $3,4*8(%rsp)
#endif
	iretq

#ifdef CONFIG_PARAVIRT
2:	/* Slow iret via hypervisor. */
	andl  $~NMI_MASK, 16(%rsp)
	pushq $0
	jmp  hypercall_page + (__HYPERVISOR_iret * 32)
#endif
.endm

.globl hypervisor_callback
hypervisor_callback:
        //call end_program

hypervisor_callback2:
//	movq %rdi, %rsp

	/* check against event re-entrant */
	movq RIP(%rsp),%rax
	cmpq $scrit,%rax
	jb 11f
	cmpq $ecrit,%rax
	jb  critical_region_fixup

11: //movq %gs:8,%rax
	//incl %gs:0
    call event_callback
	cmovzq %rax,%rsp
	pushq %rdi
//	call event_callback
    call end_program
	popq %rsp
	decl %gs:0

error_exit:
	movl RFLAGS(%rsp), %eax
	shr $9, %eax			# EAX[0] == IRET_RFLAGS.IF
	//XEN_GET_VCPU_INFO(%rsi)
	andb evtchn_upcall_mask(%rsi),%al
	andb $1,%al			# EAX[0] == IRET_RFLAGS.IF & event_mask
	jnz restore_all_enable_events	#        != 0 => enable event delivery

	RESTORE_ALL
	HYPERVISOR_IRET

restore_all_enable_events:
	RESTORE_ALL
	pushq %rax                      # save rax for it will be clobbered later
	RSP_OFFSET=8                    # record the stack frame layout changes
	//XEN_GET_VCPU_INFO(%rax)         # safe to use rax since it is saved
	XEN_LOCKED_UNBLOCK_EVENTS(%rax)

scrit:	/**** START OF CRITICAL REGION ****/
	XEN_TEST_PENDING(%rax)
	jz 12f
	XEN_LOCKED_BLOCK_EVENTS(%rax)   # if pending, mask events and handle
	                                # by jumping to hypervisor_prologue
12:	popq %rax                       # all registers restored from this point

restore_end:
	jnz hypervisor_prologue         # safe to jump out of critical region
	                                # because events are masked if ZF = 0
	HYPERVISOR_IRET
ecrit:  /**** END OF CRITICAL REGION ****/

# Set up the stack as Xen does before calling event callback
hypervisor_prologue:
	pushq %r11
	pushq %rcx
	jmp hypervisor_callback

# [How we do the fixup]. We want to merge the current stack frame with the
# just-interrupted frame. How we do this depends on where in the critical
# region the interrupted handler was executing, and so if rax has been
# restored. We determine by comparing interrupted rip with "restore_end".
# We always copy all registers below RIP from the current stack frame
# to the end of the previous activation frame so that we can continue
# as if we've never even reached 11 running in the old activation frame.

critical_region_fixup:
	# Set up source and destination region pointers
	leaq RIP(%rsp),%rsi   # esi points at end of src region
	# Acquire interrupted rsp which was saved-on-stack. This points to
	# the end of dst region. Note that it is not necessarily current rsp
	# plus 0xb0, because the second interrupt might align the stack frame.
	movq RSP(%rsp),%rdi   # edi points at end of dst region

	cmpq $restore_end,%rax
	jae  13f

	# If interrupted rip is before restore_end
	# then rax hasn't been restored yet
	movq (%rdi),%rax
	movq %rax, RAX(%rsp)  # save rax
	addq $RSP_OFFSET,%rdi

	# Set up the copy
13:	movq $RIP,%rcx
	shr  $3,%rcx          # convert bytes into count of 64-bit entities
15:	subq $8,%rsi          # pre-decrementing copy loop
	subq $8,%rdi
	movq (%rsi),%rax
	movq %rax,(%rdi)
	loop 15b
16:	movq %rdi,%rsp        # final rdi is top of merged stack
	andb $KERNEL_CS_MASK,CS(%rsp)      # CS might have changed
	jmp  11b


