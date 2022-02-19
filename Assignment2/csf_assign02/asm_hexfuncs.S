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
		
	
	*/
	hex_format_byte_as_hex:
		movq %rdi, %r11
		movq $1, %rbx
		jmp .LfirstCond

	.LfirstCond:
		shr $4, %rdi
		shl $4, %rdi

		subq %rdi, %r11

		cmp $10, %r11

		jl .ConvertLess10
	
		jge .ConvertGreater10

	.ConvertLess10:
		movq $48, %r12
		addq %r11, %r12

		movq %r12, (%rsi, %rbx, $1)
		cmpq %rbx, $0
		je .LtestReturn
		shr $4, %rdi
		moveq %rdi, %r11
		decq %rbx
		jmp .LfirstCond

	.ConvertGreater10:
	
		movq $97, %r12
		subq $10, %r11
		addq %r11, %r12
		movq %r12, (%rsi, %rbx, $1)
		cmpq %rbx, $0
		je .LtestReturn
		shr $4, %rdi
		moveq %rdi, %r11
		decq %rbx
		jmp .LfirstCond
		
	.LtestReturn:
		movq %rsi, %rax
		ret
	
	
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
