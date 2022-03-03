
/* Hexdump main function, assembly language version */

	.section .text

	.globl main
main:
	/* TODO: add your code here */
	/*
	start_function:
	pushq %rbp
	subq $16, %rsp
	movq %rsp, %rbp // string_read_from_input

	movq %rbp, %rdi
	call hex_read
	movq %rbp, %rdi
	addq $1, %rax
	movb $0, (%rbp, %rax, 1)
	call hex_write_string
	addq $16, %rsp
	popq %rbp
	ret
	*/
	// above this line is example code
	
	subq $8, %rsp // align stack

	movl $0, %ebx   //chars_in_data_buf
	movl $0, %ebp   //how_many_overflow

	subq $16, %rsp
	movq %rsp, %r9 // r9 is hex_read_buf
	
	subq $16, %rsp
	movq %rsp, %r10 // r10 is bufoverflow

	subq $32, %rsp
	movq %rsp, %r11 // r11 is data_buf array

	subq $16, %rsp
	movq %rsp, %r12 // offset_buf
	
	movq $0, %r8 //offset counter
	
	
	jmp .LqueuedCharsCondition
	

	//put queued chars into data_buf
	.LqueuedCharsLoop:

	//movb (%r10, %rbx, 1), (%r11, %rbx, 1)
	movb (%r10, %rbx, 1), %r13b
	movb %r13b, (%r11, %rbx, 1)
	
	incl %ebx

	.LqueuedCharsCondition:
	cmpl %ebp, %ebx // compares chars in data buf & how many overflow
	jl .LqueuedCharsLoop // if chars in data buf less than how many overflow then jump
	
	movl $0, %ebp // how_many_overflow set to zero

	.LreadInput:
	//continually gets chars from standard input
	
	movq %r9, %rdi //hex_read_buf as first argument for hex_read call

	//saves all caller-saved variables so that function call doesn't affect them
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	call hex_read

	//caller-saved variables back to their respective places
	popq %r11 // is back
	popq %r10
	popq %r9
	popq %r8

	
	movl $0, %ecx // z variable in c_hexmain acts as counter
	movl $0, %edx // used to temporarily store and compute comparisons

	pushq %rax // pushes num_chars_read to the stack for safekeeping
	
	.LmoveToDataBuf:
	cmpl %eax, %ecx // z < chars read
	jge .After //if z is greater or equal to chars read then jump
	movq %rbx, %rdx // chars_in_data_buf now in %rdx
	addq %rcx, %rdx // add z to chars_in_data_buf and store in %rdx
	cmpq $16, %rdx // chars_in_data_buf + z < 16
	jge .After // if chars_in_data_buf + z greater than or equal to 16 jump
	//movb (%r9, %ecx, 1), (%r11, %edx, 1)

	movb (%r9, %rcx, 1), %r13b //hex_read_buf[z] now in %r13b
	movb %r13b, (%r11, %rdx, 1)  // hex_read_buf[z] now in data_buf[chars_in_data_buf + z]

	//pushq (%r9, %rcx, 1)
	//popq (%r11, %rdx, 1)
	incl %ecx // increase z counter
	jmp .LmoveToDataBuf

	.After:
	addl %ecx, %ebx // chars_in_data_buf += z

	.LputInOverflow:
	cmpl %ecx, %eax // char_read <= z
	jge .AfterOverflow // if chars read greater than or equal to z then jump
	movb (%r9, %rcx, 1), %r13b // hex_read_buf[z] now in %r13b
	movb %r13b, (%r10, %rbp, 1) // hex_read_buf[z] now in buf_overflow[how_many_overflow]
	
	incl %ebp //how_many_overflow += 1
	incl %ecx // z += 1
	jmp .LputInOverflow
	
	.AfterOverflow:
	cmpl $16, %ebx // compare chars_in_data_buf & 16
	jge .printing // if chars_in_data_buf >= 16 jump
	cmpl $0, %eax // compares char_read & 0
	jle .printing // if char_read <= 0 jump
	jmp .LreadInput // if all else passes loop back to LreadInput

	
	.printing:
	cmpl $0, %ebx // compare chars_in_data_buf & 0
	jle .checkingBiggestLoop //if chars_in_data_buf <= 0 then jump
	movq %r8, %rdi // offset counter now in %rdi
	movq %r12, %rsi // offset_buf now in %rsi

	pushq %rax
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11

	call hex_format_offset 

	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rax
	
	pushq %r8 // push offset_counter into stack for saving and we are now free to temporarily
		  // for arithmetic
	movq $8, %r8
	movb $58, (%r12, %r8, 1) // offset_buf[8] = ':'
	incq %r8
	movb $32, (%r12, %r8, 1) // offset_buf[9] = ' '
	incq %r8
	movb $0, (%r12, %r8, 1) // offset_buf[10] = '\0'
	popq %r8 // offset counter now back in %r8
	movq %r12, %rdi //move offset_buf into %rdi for function call

	pushq %rax
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	call hex_write_string // code breaks here
	
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rax
	
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
	cmpl %ebx, %r14d // comparison between chars in data buf & int i
	jge .end_full_hex // if int i >= chars in data buf jump
	
	pushq %r15
	movb (%r11, %r14, 1), %cl // data_buf[i] now in %cl
	
	movq $0, %r15
	movb %cl, (%r8, %r15, 1) // data_buf[i] now in single_conversion[0]
	
	movq $0, %rdi // clears anything that could have been in %rdi
	movb (%r8, %r15, 1), %dil // hex_format_byte_as_hex first argument
	movq %r8, %rsi // conversion array hex_format_byte_as_hex second argument

	pushq %rax
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11

	call hex_format_byte_as_hex

	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rax
	
	movq $2, %r15
	movb $32, (%r8, %r15, 1) // puts space after 2 hex digits
	popq %r15
	
	.full_hex_inner:
	cmpq $3, %r15 // compare j with 3
	jge .after_inner // if j >= 3 jump
	movb (%r8, %r15, 1), %cl // single conversion[j] now in %cl
	movb %cl, (%r12, %r13, 1) // single conversion[j] now in complete_hex_output[complete_counter + 1]
	incq %r13
	incq %r15
	jmp .full_hex_inner //loop back

	.after_inner:
	movq $0, %r15 // sets j back to 0
	incq %r14 // increments i
	jmp .full_hex_representation_start // loop back to beginning of loop

	.end_full_hex:
	cmpq $49, %r13 // compares complete_counter & 49
	jge .print_entire_hex_output // if complete_counter >= 49 jump
	movb $32, (%r12, %r13, 1) // complete_hex_output[complete_counter] = ' '
	incq %r13 // complete counter += 1
	jmp .end_full_hex

	.print_entire_hex_output:
	movb $0, (%r12, %r13, 1) // complete_hex_output[complete_counter] = '\0' a.k.a. null terminating
	movq %r12, %rdi // moves complete_hex_output array into function first argument

	pushq %rax
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	call hex_write_string

	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rax
	
	addq $52, %rsp // get rid of stack memory for complete_output array
	popq %r12 // %r12  is now offset buf aggain

	addq $8, %rsp // gets rid of single conversion memory in stack
	popq %r8 // %r8 is offset counter again

	movq $0, %r14 // int i

	.read_string_printed: //  prints the original inputted, slight formatting
	cmpl %ebx, %r14d // compares chars_in_data_buf & i
	jge .end_read_string_printed // if i >= chars_in_data_buf jump
	
	movq $0, %rdi // clear everything that might have been in %rdi
	movb (%r11, %r14, 1), %dil // put data_buf[i] into %rdi

	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11

	call hex_to_printable //hex_to_printable(data_buf[i])
	
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx

	//data_buf[i] = hex_to_printable(data_buf[i])
	movb %al, (%r11, %r14, 1) // puts converted char into its correct spot in original string
	incq %r14 // i += 1
	jmp .read_string_printed

	.end_read_string_printed:
	movb $10, (%r11, %r14, 1) // puts new line character at data_buf[chars_in_data_buf]
	incq %r14
	movb $0, (%r11, %r14, 1) // puts null terminator at data_buf[chars_in_data_buf + 1]
	
	movq %r11, %rdi // moves string to print to %rdi

	pushq %rax
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	
	call hex_write_string

	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rax
	
	addq $16, %r8 // adds 16 to offset counter
	movl $0, %ebx //sets chars_in_data_buf to 0
	
	popq %rax // we now have original value of chars read
	popq %rax
	
	///////////////////////////////////////////// code breaks here
	.checkingBiggestLoop:
	cmpq $0, %rax // compare char_read & 0
	jg .LqueuedCharsCondition // if char_read greater than 0 then jump
	
	.LReturn:
	
	addq $16, %rsp
	addq $32, %rsp
	addq $16, %rsp
	addq $16, %rsp
	addq $8, %rsp
	
	ret
	
	
	
	
/* vim:ft=gas:
 */
