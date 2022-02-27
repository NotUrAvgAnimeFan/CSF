
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
	
	
	
	.jmp .LqueuedCharsCondition
	

	//put queued chars into data_buf
	.LqueuedCharsLoop:
	movb (%r10, %ebx, 1), (%r11, %ebx, 1)
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
	movb (%r9, %ecx, 1), (%r11, %edx, 1)
	incl %ecx
	jmp .LmoveToDataBuf

	.After:
	addl %ecx, %ebx

	.LputInOverflow:
	cmpl %ecx, %eax
	jle .AfterOverflow
	movb (%r9, %ecx, 1), (%10, %ebp, 1)
	incl %ebp
	incl %ecx
	jmp .LputInOverflow
	
	.AfterOVerflow:
	cmpl $16, %ebx
	jge .printing
	cmpl $0, %eax
	jle .printing
	jmp .LreadInput

	
	.printing:
	cmpl $0, %ebx
	jle .something //place holder
	movq %r8, %rdi
	movq %r12, %rsi
	call hex_format_offset
	movq $58, (%r12, $8, 1)
	movq $32, (%r12, $9, 1)
	movq $0, (%r12, $10, 1)
	movq %r12, %rdi
	call hex_write_string

	movl $0, %r13d //complete counter
	movl $0, %r14d // int i
	movl $0, %r15d // int j
		
	pushq %r8 // temporarily put offset counter into stack
	subq $8, %rsp // create space for 3 bit conversion array
	movq %rsp, %r8 // pointer to 3 bit conversion stack now in r8 

	pushq %r12 // temporarily put offset_buff pointer into stack
	subq $52, %rsp // create space for 49 bit complete output to print
	movq %rsp, %r12 // pointer to complete output to print
	
	.full_hex_representation_start:
	cmpl %ebx, %r14d
	jge .end_full_hex
	movb (%r11, %r14d, 1), (%r8, $0, 1)
	movq $0, %rdi
	movb (%r8, $0, 1), %dil
	movq %r8, %rsi
	call hex_format_byte_as_hex
	movb $32, (%r8, $2, 1)

	.full_hex_inner:
	cmpl $3, %r15d
	jge .after_inner
	movb (%r8, %r15d, 1), (%r12, %r13d, 1)
	incl %r13d
	incl %r15d
	jmp .full_hex_inner //loop back

	.after_inner:
	movl $0, %r15d
	jmp .full_hex_representation_start // loop back to beginning of loop

	.end_full_hex:
	cmpl $49, %r13d
	jge .print_entire_hex_output
	movb $32, (%r12, %r13d, 1)
	incl %r13d
	jmp .end_full_hex

	.print_entire_hex_output:
	movb $0, (%r12, %r13d, 1)
	movq %r12, %rdi
	call hex_write_string

	addq $52, %rsp
	popq %r12

	addq $8, %rsp
	popq %r8

	movl $0, %r14d // int i

	.read_string_printed:
	cmpl %ebx, %r14d
	jge .end_read_string_printed
	
	movq $0, %rdi
	movb (%r11, %r14d, 1), %dil
	call hex_to_printable
	movb %al, (%r11, %r14d, 1)
	incl %14d
	jmp .read_string_printed:

	.ended_read_string_printed:
	movb $10, (%r11, %r14d, 1)
	incl $r14d
	movb $0, (%r11, %r14d, 1)
	
	movq %r11, %rdi
	
	call hex_write_string

	addq $16, %r8
	movl $0, %ebx

	popq %rax
	cmpl $0, %eax
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
