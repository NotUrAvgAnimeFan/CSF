
/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $8, %rsp

	movl $0, %ebx   //chars_in_data_buf
	movl $0, %ebp   //how_many_overflow

	subq $16, %rsp
	movq %rsp, %r9 // r9 is hex_read_buf
	
	subq $16, %rsp
	movq %rsp, %r10 // r10 is bufoverflow

	subq $24, %rsp
	movq %rsp, %r11 // r11 is data_buf array

	subq $16, %rsp
	movq %rsp, %r12 // offset_buf
	
	movq $0, %r8 //offset counter
	
	
	
	jmp .LqueuedCharsCondition
	

	//put queued chars into data_buf
	.LqueuedCharsLoop:

	//movb (%r10, %rbx, 1), (%r11, %rbx, 1)
	pushq (%r10, %rbx, 1)
	popq (%r11, %rbx, 1)
	
	incl %ebx

	.LqueuedCharsCondition:
	cmpl %ebp, %ebx
	jl .LqueuedCharsLoop
	
	movl $0, %ebp // how_many_overflow set to zero

	.LreadInput:
	//continually gets chars from standard input
	movq %r9, %rdi
	call hex_read

	movl $0, %ecx
	movl $0, %edx

	pushq %rax
	
	.LmoveToDataBuf:
	cmpl %eax, %ecx 
	jge .After
	movl %ebx, %edx
	addl %ecx, %edx
	cmpl $16, %edx
	jge .After
	//movb (%r9, %ecx, 1), (%r11, %edx, 1)
	pushq (%r9, %rcx, 1)
	popq (%r11, %rdx, 1)
	incl %ecx
	jmp .LmoveToDataBuf

	.After:
	addl %ecx, %ebx

	.LputInOverflow:
	cmpl %ecx, %eax
	jle .AfterOverflow
	//movb (%r9, %ecx, 1), (%10, %ebp, 1)
	pushq (%r9, %rcx, 1)
	popq (%r10, %rbp, 1)
	incl %ebp
	incl %ecx
	jmp .LputInOverflow
	
	.AfterOverflow:
	cmpl $16, %ebx
	jge .printing
	cmpl $0, %eax
	jle .printing
	jmp .LreadInput

	
	.printing:
	cmpl $0, %ebx
	jle .checkingBiggestLoop //place holder
	movq %r8, %rdi
	movq %r12, %rsi
	call hex_format_offset
	pushq %r8
	movq $8, %r8
	movq $58, (%r12, %r8, 1)
	movq $9, %r8
	movq $32, (%r12, %r8, 1)
	movq $10, %r8
	movq $0, (%r12, %r8, 1)
	popq %r8
	movq %r12, %rdi
	call hex_write_string

	movq $0, %r13 //complete counter
	movq $0, %r14 // int i
	movq $0, %r15 // int j
		
	pushq %r8 // temporarily put offset counter into stack
	subq $8, %rsp // create space for 3 bit conversion array
	movq %rsp, %r8 // pointer to 3 bit conversion stack now in r8 

	pushq %r12 // temporarily put offset_buff pointer into stack
	subq $52, %rsp // create space for 49 bit complete output to print
	movq %rsp, %r12 // pointer to complete output to print
	
	.full_hex_representation_start:
	cmpl %ebx, %r14d
	jge .end_full_hex
	//movb (%r11, %r14d, 1), (%r8, $0, 1)
	pushq %r15
	pushq (%r11, %r14, 1)
	movq $0, %r15
	popq (%r8, %r15, 1)
	movq $0, %rdi
	movb (%r8, %r15, 1), %dil
	popq %r15
	movq %r8, %rsi
	call hex_format_byte_as_hex
	pushq %r15
	movq $2, %r15
	movb $32, (%r8, %r15, 1)
	popq %r15
	
	.full_hex_inner:
	cmpq $3, %r15
	jge .after_inner
	//movb (%r8, %r15, 1), (%r12, %r13, 1)
	pushq (%r8, %r15, 1)
	popq (%r12, %r13, 1)
	incq %r13
	incq %r15
	jmp .full_hex_inner //loop back

	.after_inner:
	movq $0, %r15
	jmp .full_hex_representation_start // loop back to beginning of loop

	.end_full_hex:
	cmpq $49, %r13
	jge .print_entire_hex_output
	movb $32, (%r12, %r13, 1)
	incq %r13
	jmp .end_full_hex

	.print_entire_hex_output:
	movb $0, (%r12, %r13, 1)
	movq %r12, %rdi
	call hex_write_string

	addq $52, %rsp
	popq %r12

	addq $8, %rsp
	popq %r8

	movq $0, %r14 // int i

	.read_string_printed:
	cmpl %ebx, %r14d
	jge .end_read_string_printed
	
	movq $0, %rdi
	movb (%r11, %r14, 1), %dil
	call hex_to_printable
	movb %al, (%r11, %r14, 1)
	incq %r14
	jmp .read_string_printed

	.end_read_string_printed:
	movb $10, (%r11, %r14, 1)
	incq %r14
	movb $0, (%r11, %r14, 1)
	
	movq %r11, %rdi
	
	call hex_write_string

	addq $16, %r8
	movl $0, %ebx

	.checkingBiggestLoop:
	popq %rax
	
	cmpq $0, %rax
	jg .LqueuedCharsCondition
	
	.LReturn:
	addq $16, %rsp
	addq $24, %rsp
	addq $16, %rsp
	addq $16, %rsp
	addq $8, %rsp
	ret

	
	
	
/* vim:ft=gas:
 */
