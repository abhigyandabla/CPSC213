.pos 0x100
		 ld $tmp, r0		# r0 = address of tmp
		 ld $tos, r1		# r1 = address of tos
		 ld $0, r2		# r2 = 0
		 st r2, (r0)		# tmp = 0
		 st r2, (r1)		# tos = 0
		 ld (r2), r3		# r3 = tos
		 ld $s, r4		# r4 = address of s
		 ld $a, r5		# r5 = address of a
		 ld (r5, r2, 4), r6	# r6 = a[0]
		 st r6, (r4, r3, 4)	# s[tos] = a[0]
		 inc r3			# r3 = tos++
		 st r3, (r1)		# tos = tos++
		 ld $1, r2		# r2 = 1
		 ld (r5, r2, 4), r6	# r6 = a[1]
		 st r6, (r4, r3, 4)	# s[tos] = a[1]
		 inc r3			# r3 = tos++
		 st r3, (r1)		# tos = tos++
		 ld $2, r2		# r2 = 2
		 ld (r5, r2, 4), r6	# r6 = a[2]
		 st r6, (r4, r3, 4)	# s[tos] = a[2]
		 inc r3			# r3 = tos++
		 st r3, (r1)		# tos = tos++
		 dec r3			# r3 = tos--
		 st r3, (r1)		# tos = tos--
		 ld (r1), r3		# r3 = tos
		 ld (r4, r3, 4), r6	# r6 = s[tos]
		 st r6, (r0)		# tmp = s[tos]
		 dec r3			# r3 = tos--
		 st r3, (r1)		# tos = tos--
		 ld (r1), r3		# r3 = tos
		 ld (r4, r3, 4), r6	# r6 = s[tos]
		 ld (r0), r7		# r7 = tmp
		 add r7, r6		# r6 = tmp + s[tos]
		 st r6, (r0)		# tmp = tmp + s[tos]
		 dec r3			# r3 = tos--
		 st r3, (r1)		# tos = tos--
		 ld (r1), r3		# r3 = tos
		 ld (r4, r3, 4), r6	# r6 = s[tos]
		 ld (r0), r7		# r7 = tmp
		 add r7, r6		# r6 = tmp + s[tos]
		 st r6, (r0)		# tmp = tmp + s[tos]
		 halt			# halt
.pos 0x1000
tmp:		 .long 0x00000001	# tmp
.pos 0x1004
tos:		 .long 0x00000002	# tos
.pos 0x1008
a: 		 .long 0x00000003	# a[0]
		 .long 0x00000004	# a[1]
		 .long 0x00000005	# a[2]
.pos 0x2000
s:               .long 0x00000006	# s[0]
		 .long 0x00000007	# s[1]
		 .long 0x00000008	# s[2]
		 .long 0x00000009	# s[3]
		 .long 0x00000000	# s[4]