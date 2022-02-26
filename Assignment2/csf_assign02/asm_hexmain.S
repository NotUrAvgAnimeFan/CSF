
/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	subq $8, %rsp

	movl $0, %ebx   //chars_in_data_buf
	movl $0, %ebp   //how_many_overflow

	.jmp .LqueuedCharsCondition
	

	//put queued chars into data_buf
	.LqueuedCharsLoop:
	movl (bufoverflow, %ebx, 1), (databuf, %ebx, 1)
	incl %ebx

	.LqueuedCharsCondition:
	cmpl %ebp, %ebx
	jl .LqueuedCharsLoop
	
	movl $0, %ebp

	//continually gets chars from standard input

	
	.LReturn:
	addq $8, %rsp
	ret

	
/* vim:ft=gas:
 */
