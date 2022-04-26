.pos 0x100
		ld $3, r0		# r0 = 3
		ld $t, r1		# r1 = address of t[0]
		ld (r1, r0, 4), r0	# r0 = t[3]
		ld $g, r2		# r2 = address of g
		st r0, (r2)		# g = t[3]
		ld (r2), r3		# r3 = g
		ld (r1, r3, 4), r4	# r4 = t[g]
		st r4, (r2)		# g = t[g]
		ld $j, r3		# r3 = address of j
		ld $w, r2		# r2 = address of w
		st r2, (r3)		# j = &w
		ld (r3), r4		# r4 = j
		ld $2, r5		# r5 = 2
		st r5, (r4)		# *j = 2
		ld $9, r5		# r5 = 9
		ld (r1, r5, 4), r6	# r6 = t[9]
		shl $2, r6		# r6 = t[9] << 2
		add r1, r6		# r6 = &t[t[9]]
		st r6, (r3)		# j = &t[t[9]]
		ld 0x4(r1), r6		# r6 = t[1]
		ld (r3), r5		# r5 = j
		ld (r5), r7		# r7 = *j
		add r7, r6		# r6 = *j + t[1]
		st r6, (r5)		# *j = *j + t[1]
                halt                    # halt
.pos 0x1000
g:		 .long 0x00000000	# g
.pos 0x1004
w:		 .long 0x00000000	# w
.pos 0x1008
j:               .long 0x00000000       # j
.pos 0x2000
t:               .long 0x12345678       # t[0]
                 .long 0x12345678       # t[1]
                 .long 0x12345678       # t[2]
                 .long 0x12345678       # t[3]
                 .long 0x12345678       # t[4]
                 .long 0x12345678       # t[5]
                 .long 0x12345678       # t[6]
                 .long 0x12345678       # t[7]
                 .long 0x12345678       # t[8]
                 .long 0x12345678       # t[9]