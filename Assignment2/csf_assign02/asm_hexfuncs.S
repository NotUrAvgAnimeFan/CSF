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
		subq $8, %rsp
		movq $2, %rbx
		movq $0, (%rsi, %rbx, 1)
		decq %rbx
		jmp .LfirstCond

	.LfirstCond:

		cmpq $0, %rbx
		jge .Ltop
		addq $8, %rsp
		ret

	.Ltop:
		movq %rdi, %r11
	
		shrq $4, %rdi
		shlq $4, %rdi

		subq %rdi, %r11

		cmp $10, %r11

		jge .ConvertGreater10
		addq $48, %r11
		movq %r11, (%rsi, %rbx, 1) 

		jmp .Lout

	.Lout:
		shrq $4, %rdi
		decq %rbx
		jmp .LfirstCond

	.ConvertGreater10:
	
		subq $10, %r11
		addq $97, %r11
		movq %r11, (%rsi, %rbx, 1)
		jmp .Lout
	
	
	hex_to_printable:
		subq $8, %rsp
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
		addq $8, %rsp
		ret
	
	.LchangeToDot:
		
		movq $46, %rax
		jmp .PrintableReturn

	
	

/* vim:ft=gas:
 */
