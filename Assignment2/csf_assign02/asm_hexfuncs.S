// Ricardo Manuel Morales Gonzalez  rmorale5@jhu.edu
// Ana Kuri  akuri1@jhu.edu

/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */
	
	.sHexToPrint: .string "%u"
	
	

	
	.section .text
	
	
		.globl hex_read

		.globl hex_write_string

		.globl hex_format_offset
		
		.globl string_size
	
		.globl hex_format_byte_as_hex

		.globl hex_to_printable


	hex_read:
		
		subq $8, %rsp //align stack
		leaq 0(%rdi), %rsi //move start of buffer into %rsi
		movq $0, %rdi // %rdi is 0 so read will read from stdin
		movq $16, %rdx // read will read up to 16 bites

		call read // read function is called
		
		addq $8, %rsp //align stack
		ret

	hex_write_string:
		
		subq $8, %rsp // align stack
		pushq %rdi // saves buffer in stack
		call string_size // gets size of string in %rdi
		popq %rdi // gets intact buffer to write from stack
		movq %rdi, %rsi // moves buffer to %rsi
		movq $1, %rdi // %rdi gets 1 so write will write to stdout
		movq %rax, %rdx //string size is now in %rax so whole string will print out

		call write // write function is called
		
		addq $8, %rsp // stack alignment
		ret
	

	string_size:
		subq $8, %rsp // stack alignment
		movq $0, %rax // counter of how many chars in string
		movb $0, %cl // used to store value of buffer[i]
		movb $0, %dl // used for comparison and indicates presence of null character
	
	.SizeFirstFunc:
		movb (%rdi, %rax, 1), %cl // buffer[i] now in %cl
		cmpb %cl, %dl	//compares buffer[i] to '\0'
		je .SizeReturn // if buffer[i] == '\0' jump to SizeReturn
		incq %rax // if no jump move to the next index in buffer
		jmp .SizeFirstFunc // loop back to SizeFirstFunc
		
	.SizeReturn:
		addq $8, %rsp // stack alignment
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
		movq $126, %r11

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
