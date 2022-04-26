.pos 0x100
		ld $a, r0		# r0 = address of a
		ld (r0), r1		# r1 = a
		inc r1			# r1 = r1 + 1
		inca r1			# r1 = r1 + 4
		shl $0x7, r1		# r1 = r1 << 7
		ld (r0), r2		# r2 = a
		and r2, r1		# r1 = r1 & a
		shr $0x3, r1		# r1 = r1 >> 3
		ld $j, r2		# r2 = address of j
		st r1, (r2)		# j = r1
                halt                    # halt
.pos 0x1000
j:               .long 0x00000000       # j
.pos 0x2000
a:               .long 0x12345678       # a