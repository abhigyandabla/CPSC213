.pos 0x1000
code:
	# v0 = s.x[i];
	ld $i, r0		# r0 = address of i
	ld (r0), r0		# r0 = i
        ld $s, r1               # r1 = address of s
	ld (r1, r0, 4), r2	# r2 = address of s.x[i]
	ld $v0, r3              # r3 = address of v0
	st r2, (r3)             # v0 = s.x[i]

        # v1 = s.y[i];
	# r1 still has address of s, r0 still has value of i
	ld $2, r3		# r3 = 2
	ld (r1, r3, 4), r3	# r3 = address of s.y
	ld (r3, r0, 4), r3      # r3 = address of s.y[i]
	ld $v1, r4             	# r4 = address of v1
	st r3, (r4)             # v1 = s.y[i]

        # v2 = s.z->x[i];
	# r1 still has address of s, r0 still has value of i
	ld $3, r3		# r3 = 3
	ld (r1, r3, 4), r3	# r3 = address of s.z
	ld (r3, r0, 4), r3	# r3 = address of s.z->x[i]
	ld $v2, r2              # r2 = address of v2
	st r3, (r2)             # v2 = s.z->x[i]

	#v3 = s.z->z->y[i];
	# r1 still has address of s, r0 still has value of i
	ld 0xc(r1), r5          # r5 = address of s.z
	ld 0xc(r5), r5          # r5 = address of s.z->z
	ld 0x8(r5), r5          # r5 = address of s.z->z->y
	ld (r5, r0, 4), r6      # r6 = address of s.z->z->y[i]
	ld $v3, r7              # r7 = address of v3
	st r6, (r7)             # v3 = s.z->z->y[i]
	halt			# halt

.pos 0x2000
static:
i:	.long 1		        # i
v0:	.long 9		        # v0
v1:	.long 4		       	# v1
v2:	.long 2			# v2
v3:	.long 3		        # v3
s:	.long 1		    	# s.x[0]
        .long 2		 	# s.x[1]
        .long s_y           	# s.y
        .long s_z          	# s.z

.pos 0x3000
heap:
s_y:            
	.long 0x1		# s.y[0]
	.long 0x2		# s.y[1]
s_z:            
	.long 0x3		# s.z.x[0]
	.long 0x4		# s.z.x[1]
	.long 0x5		# s.z.y
	.long s_z_z        	# s.z->z
s_z_z:          
     	.long 0x6		# s.z->x[0]
     	.long 0x7		# s.z->x[1]
      	.long s_z_z_y       	# s.z->y
      	.long 0x8		# s.z->z
s_z_z_y:
      	.long 0x9		# s.z->z->y[0]
      	.long 0x10		# s.z->z->y[1]