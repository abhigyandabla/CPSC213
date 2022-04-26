.pos 0x100
		ld $3, r0		# r0 = 3
		ld $a, r1		# r1 = address of a
		st r0, (r1)		# a = 3
		ld (r1), r2		# r2 = a
		ld $p, r3		# r3 = address of p
		st r1, (r3)		# p = &a
		ld (r3), r4		# r4 = p
		ld (r4), r5		# r5 = *p
		dec r5			# r5 = *p - 1
		st r5, (r4)		# *p = *p - 1
		ld $b, r6		# r6 = address of b[0]
		st r6, (r3)		# p = &b[0]
		ld (r3), r4		# r4 = p
		inca r4			# r4 = p++
		st r4, (r3)		# p = p++
		ld (r1), r5		# r5 = a
		ld (r6, r5, 4), r7	# r7 = b[a]
		ld (r3), r3		# r3 = p
		st  r7, (r3, r5, 4)	# p[a] = b[a]
		ld (r6), r1		# r1 = b[0]
		ld $12, r0		# r0 = 12
		add r0, r3		# r3 = p + 3
		st r1, (r3)		# *(p + 3) = b[0]
                halt                    # halt
.pos 0x1000
a:		 .long 0x00000001	# a
.pos 0x1004
p:		 .long 0x00000002	# p
.pos 0x2000
b:               .long 0x00000003       # b[0]
                 .long 0x00000004       # b[1]
                 .long 0x00000005       # b[2]
                 .long 0x00000006       # b[3]
                 .long 0x00000007       # b[4]