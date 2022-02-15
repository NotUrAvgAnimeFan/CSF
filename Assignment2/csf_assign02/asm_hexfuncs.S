/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */
	
	.sHexToPrint: .string "%u"
	
	

	
	.section .text
	
	
	/* TODO: add functions here */

		.globl format_as_hex_variable_string_size

	
		.globl hex_format_byte_as_hex

		.globl hex_to_printable

	


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
		popq %rbx
		mov al,0x00
		movl $0, %eax
		movl $255, %rax

		jmp .LfirstCond

	.LloopTop:
		inc al
		incl %eax
	
	.LfirstCond:
		cmpl %rax, %eax
		jl .LtestReturn
		cmpl %rbx, al
		je .LloopTop
	
	.LtestReturn:
		push %eax
		push $2
		call format_as_hex_variable_string_size
		ret
	
	
	hex_to_printable:
	
		popq %rbx 
		mov al,0x20
		movl $32, %eax
		movl $127, %rax

	
		jmp .LfirstCond
	
	.LloopTop:

		inc al
		inc %eax

	
	.LfirstCond:
		cmpl %rax, %eax
		jl .LchangeToDot
		cmpl %rbx,al
		je .LloopTop

	.LtestReturn:
		push %eax
		ret
	
	.LchangeToDot:
		
		movl $46, %eax
		call .LtestReturn

	
	

/* vim:ft=gas:
 */
