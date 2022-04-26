.pos 0x100
		 ld $0x3, r0		# r0 = 3
		 ld $array, r1          # r1 = address of array[0]
                 ld (r1, r0, 4), r2     # r2 = array[3]
		 ld $0x2, r3		# r3 = 2
		 ld (r1, r3, 4), r4     # r4 = array[2]
		 st r4, (r1, r0, 4)     # array[3] = array[2]
		 st r2, (r1, r3, 4)     # array[2] = array[3]
		 halt                   # halt
.pos 0x1000
orig:            .long 0xffffffff         # orig
.pos 0x2000
array:           .long 0xffffffff         # b[0]
                 .long 0xffffffff         # b[1]
                 .long 0x00000002         # b[2]
                 .long 0x00000003         # b[3]
                 .long 0xffffffff         # b[4]
                 .long 0xffffffff         # b[5]
