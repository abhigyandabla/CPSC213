.pos 0x100
                 ld $0, r0              # r0 = i' = 0
                 ld $0, r1              # r1 = c' = 0
                 ld $a, r2              # r2 = &a
                 ld $b, r3              # r3 = &b
                 ld $n, r4              # r4 = &n
                 ld (r4), r4            # r4 = n
                 not r4                 # r4 = ~n
                 inc r4                 # r4 = -n
loop:            mov r4, r5             # r5 = -n
                 add r0, r5             # r5 = i' - n
                 beq r5, end_loop       # if (i - n == 0), go to end_loop
                 ld (r2, r0, 4), r6     # r6 = a[i]
                 ld (r3, r0, 4), r7     # r7 = b[i]
                 not r7                 # r7 = ~b[i]
                 inc r7                 # r7 = -b[i]
                 add r6, r7             # r7 = a[i] - b[i]
                 bgt r7, then           # if (a[i] - b[i] > 0) go to then
else:            br end_if              # go to end_if
then:            inc r1                 # r1 = c'++
end_if:          inc r0                 # r0 = i'++
                 br loop                # go to loop
end_loop:        ld $i, r2              # r2 = &i
		     ld $c, r3		    # r3 = &c
                 st r0, (r2)            # i = i'
                 st r1, (r3)            # c = c'
                 halt                   # halt      
.pos 0x1000
i:               .long 0xffffffff       # i
n:               .long 5	          # n
a:               .long 10	          # a[0]
                 .long 20		    # a[1]
                 .long 30	          # a[2]
                 .long 40	          # a[3]
                 .long 50		    # a[4]
b:               .long 11	          # b[0]
                 .long 20		    # b[1]
                 .long 28	          # b[2]
                 .long 44		    # b[3]
                 .long 48	          # b[4]
c:               .long 0	          # c