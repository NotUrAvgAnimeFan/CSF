/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */
	
	.sHexToPrint: .string "%u"
	
	

	
	.section .text
	
	
	/* TODO: add functions here */
		.globl hex_read

		.globl hex_write_string

		.globl hex_format_offset
		
		.globl string_size
	
		.globl format_as_hex_variable_string_size
	
		.globl hex_format_byte_as_hex

		.globl hex_to_printable

	

	/*
	format_as_hex_variable_string_size:	
		popq %rdi
		popq %rbx

		movl %rdi, %eax
		decq %eax
		
		jmp .LcheckCond
	
	.Ltop:
		movl %rbx, %rsi
		sarl $4, %rbx
		shll $4, %rbx
		movl %rsi, %esi
		subl %rbx, %esi
		
		cmp $10, %esi
		jge .LelsePart
		
		jmp .Lout	
	.LelsePart:
	
	.Lout:
		sarl $4, %rbx
		decq %eax
		
	
		
	.LcheckCond:
		cmpl $0, %eax
		jge .Ltop
		
		ret
		
	
	
	hex_format_byte_as_hex:	
		popq %bl
		movb %al, 0x00
		movq $0, %rcx
		movq $256, %rax

		jmp .LfirstCond

	.LloopTop:
		incb %al
		incq %rcx
	
	.LfirstCond:
		cmpq %rax, %rcx
		jl .LtestReturn
		cmpb %bl, %al
		je .LloopTop
	
	.LtestReturn:
		push %rcx
		push $2
		call format_as_hex_variable_string_size
		ret
	
	*/
	hex_to_printable:
		movq $32, %r10
		movq $127, %r11

	
		jmp .PrintablefirstCond

	.PrintablefirstCond:
		cmpq %r10, %rdi
		jl .LchangeToDot
	
	.PrintablesecondCond:
		cmpq %r11, %rdi
		jg .LchangeToDot
		movq %rdi, %rax

	
	.PrintableReturn:
		ret
	
	.LchangeToDot:
		
		movq $46, %rax
		jmp .PrintableReturn

	
	

/* vim:ft=gas:
 */
