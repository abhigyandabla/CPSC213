.pos 0x100
                 ld $n, r0		# r0 = address of n
		 ld (r0), r1		# r1 = n
		 ld $d, r2		# r2 = address of d[0]
		 ld (r2, r1, 4), r3     # r3 = d[n]
		 inca r1		# r1 = n + 4
		 ld (r2, r1, 4), r4     # r4 = d[n + 4]
		 ld $y, r5		# r5 = address of y
		 add r3, r4		# r4 = r3 + r4
		 st r4, (r5)		# s = r4
		 ld $m, r6		# r6 = address of m
		 ld $0xf0f0f0f, r7      # r7 = 0xf0f0f0f
		 and r7, r4		# r4 = r4 & 0xf0f0f0f
		 st r4, (r6)		# m = r4
		 halt			# halt
.pos 0x1000
n:		 .long 0x00000002	# n
.pos 0x1004
m:		 .long 0x00000000	# m
.pos 0x1008
y: 		 .long 0x00000000	# y
.pos 0x2000
d:               .long 0x00000000	# d[0]
		 .long 0x00000000	# d[1]
		 .long 0x00000012	# d[2]
		 .long 0x00000000	# d[3]
		 .long 0x00000000	# d[4]
		 .long 0x00000000	# d[5]
		 .long 0x000000ae	# d[6]
		 .long 0x00000000	# d[7]