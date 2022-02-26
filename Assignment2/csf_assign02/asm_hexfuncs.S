// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  akuri1@jhu.edu

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


	hex_read:	
		subq $8, %rsp
		movq $0, %rax

		//sys_read()
		movq $3, %eax

		//file descriptor to read from stdin
		movq $0, %ebx

		//pointer to input buffer
		movq %rdi, %ecx

		//number of bytes to read
		movq $16, %edx

		addq $8, %rsp
		ret
	//found an article online on how to read files
	//https://www.tutorialspoint.com/assembly_programming/assembly_file_management.htm


	hex_write_string:
		subq $8, %rsp
		movq $0, %rax

		//sys_write()
		movq $4, %eax

		//file descriptor for stdout
		movq $1, %ebx
	
		//pointer to output buffer
		movq %rdi, %ecx

		//use string size as number of bytes to write
		call string_size
		movq %rax, %edx
	
		addq $8, %rsp
		ret
	

	string_size:
		subq $8, %rsp
		movq $0, %rax
		movq $0, %r10
	
	.SizeFirstFunc:
		cmpq (%rdi), %r10	
		je .SizeReturn
		incq %rax
		addq $1, %rdi
		jmp .SizeFirstFunc
		
	.SizeReturn:
		addq $8, %rsp
		ret

	
	hex_format_offset:	

		// sets backwards counter for inserting chars in buf[]
		movq $8, %rax
		// puts null pointer at buf[2] marking end of hex code
		movb $0, (%rsi, %rax, 1)
		decq %rax
		jmp .OffsetfirstCond

	.OffsetfirstCond:
	// cheks to make sure final character in hex hasn't already
	// been put in buf if so return
		cmpq $0, %rax
		jge .Offsettop
		ret

	.Offsettop:
	// makes a copy of %rdi but byte sized and shifts bits
		movb %dil, %r11b
		shrq $4, %rdi
		shlq $4, %rdi

	// checks to see what hex character to put in %rax index of buf
		subb %dil, %r11b

		cmpb $10, %r11b

	// if difference greater than 10 an appropriate hex character[a-f] will be
	// put in buf and like wise if less than 10 [0-9]
		jge .OffsetGreater10
		addb $48, %r11b
		movb %r11b, (%rsi, %rax, 1)
		jmp .Lout

	.Offsetout:
	//shifts bits in %rdi to make calculations for what hex character to put in buf
		shrq $4, %rdi
		decq %rax
		jmp .OffsetfirstCond

	.OffsetGreater10:
	// adds difference in %r11b to int corresponding to ascii for 0 so that
	// the appropriate character goes into its proper index in buf
		subb $10, %r11b
		addb $97, %r11b
		movb %r11b, (%rsi, %rax, 1)
		jmp .Offsetout


	
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
		// sets backwards counter for inserting chars in buf[]
		movq $2, %rax
		// puts null pointer at buf[2] marking end of hex code
		movb $0, (%rsi, %rax, 1)
		decq %rax
		jmp .LfirstCond

	.LfirstCond:
	// cheks to make sure final character in hex hasn't already
	// been put in buf if so return
		cmpq $0, %rax
		jge .Ltop
		ret

	.Ltop:
	// makes a copy of %rdi but byte sized and shifts bits
		movb %dil, %r11b
		shrq $4, %rdi
		shlq $4, %rdi

	// checks to see what hex character to put in %rax index of buf
		subb %dil, %r11b

		cmpb $10, %r11b

	// if difference greater than 10 an appropriate hex character[a-f] will be
	// put in buf and like wise if less than 10 [0-9]
		jge .ConvertGreater10
		addb $48, %r11b
		movb %r11b, (%rsi, %rax, 1)
		jmp .Lout

	.Lout:
	//shifts bits in %rdi to make calculations for what hex character to put in buf
		shrq $4, %rdi
		decq %rax
		jmp .LfirstCond

	.ConvertGreater10:
	// adds difference in %r11b to int corresponding to ascii for 0 so that
	// the appropriate character goes into its proper index in buf
		subb $10, %r11b
		addb $97, %r11b
		movb %r11b, (%rsi, %rax, 1)
		jmp .Lout
	
	
	hex_to_printable:
	// variables for comparison are set up
		subq $8, %rsp
		movq $32, %r10
		movq $127, %r11

	// jumps to first condition
		jmp .PrintablefirstCond

	.PrintablefirstCond:
	// compares the values in %r10 and %rdi and if
	// %rdi is less than lowest possible printable value
	// then the int ascii for '.' is put in %rax and returned
		cmpq %r10, %rdi
		jl .LchangeToDot
	
	.PrintablesecondCond:
	// compares the values in %r10 and %rdi and if
	// %rdi is greater than the greatest possible printable value
	// then the int ascii for '.' is put in %rax and returned
		cmpq %r11, %rdi
		jg .LchangeToDot
	// if all else passes then the original %rdi is within the
	// acceptable range and thus put in %rax to be returned next
		movq %rdi, %rax

	
	.PrintableReturn:
	// if all else passes then the value in %rax is returned
		addq $8, %rsp
		ret
	
	.LchangeToDot:
	// %rax will now have the int ascii for '.' and will be returned
	// via .PrintableReturn
		movq $46, %rax
		jmp .PrintableReturn

	
	

/* vim:ft=gas:
 */
