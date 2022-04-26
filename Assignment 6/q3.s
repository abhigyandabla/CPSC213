.pos 0x1000
			ld $s, r0			# r0 = &s
			ld (r0), r0			# r0 = s[0]
			ld $0, r1			# r1 = i = 0
			ld $n, r2			# r2 = &n
			ld (r2), r2			# r2 = n
			bgt r2, calc_avg		# if (n > 0), go to calc_avg
			halt

	calc_avg:	mov r1, r3              # r3 = i
                	shl $4, r3              # r3 = 16i
                	mov r1, r4              # r4 = i
                	shl $3, r4              # r4 = 8i
                	add r4, r3              # r3 = 24i
                	add r0, r3              # r3 = s[i]
                	ld 4(r3), r4            # r4 = s[0].grade[0]
                	ld 8(r3), r5            # r5 = s[0].grade[1]
                	add r5, r4              # r4 = s[0].grade[0] + s[0].grade[1]
                	ld 12(r3), r5           # r5 = s[0].grade[2]
                	add r5, r4              # r4 = s[0].grade[0] + s[0].grade[1] + s[0].grade[2]
                	ld 16(r3), r5          	# r5 = s[0].grade[3]
                	add r5, r4              # r4 = s[0].grade[0] + s[0].grade[1] + s[0].grade[2] + s[0].grade[3]
                	shr $2, r4              # r4 = (s[0].grade[0] + s[0].grade[1] + s[0].grade[2] + s[0].grade[3])/4
                	st  r4, 20(r3)          # s[0].average = (s[0].grade[0] + s[0].grade[1] + s[0].grade[2] + s[0].grade[3])/4
                	inc r1                  # i++
			mov r1, r3			# r3 = i
			not r3			# r3 = ~i
			inc r3			# r3 = -i
			add r2, r3			# r3 = n - i
			bgt r3, calc_avg		# if (i <= n), go to calc_avg

	sort:		ld $0, r1			# r1 = i = 0
			ld $n, r5			# r5 = &n
			ld (r5), r5			# r5 = n
			ld $-1, r3			# r3 = -1
			add r3, r5			# r5 = j = n - 1
			ld $j, r4			# r4 = &j
			st r5, (r4)			# r2 = j

	sort_loop:  inc r1			# r1 = i++
			ld $j, r2			# r2 = &j
			ld (r2), r2			# r2 = j
			mov r1, r3			# r3 = i
			not r3			# r3 = ~i
			inc r3			# r3 = -i
			add r2, r3			# r3 = j - i
			bgt r3, comp		# if (j - i > 0), go to comp

			dec r2			# r2 = j--
			ld $j, r4			# r4 = &j
			st r2, 0(r4)		# r2 = j
			ld $0, r1			# r1 = i = 0
			bgt r2, comp		# if (j > 0), go to comp
			j median			# jump to median
			halt
			
	median:	ld $n, r1			# r1 = &n
			ld (r1), r1			# r1 = n
			shr $1, r1			# r1 = n/2 ----> median index
			mov r1, r3			# r3 = n/2
			mov r1, r2			# r2 = n/2
			shl $2, r3			# r3 = 4(n/2)
			shl $1, r2			# r2 = 2(n/2)
			add r3, r2			# r2 = 6(n/2)
			ld (r0, r2, 4), r4	# r4 = avg of median student
			ld $m, r5			# r5 = &m
			st r4, (r5)			# m = median
			halt

	comp:		mov r1, r2			# r2 = i
			mov r1, r3			# r3 = i
			shl $1, r2			# r2 = 2i
			shl $2, r3			# r3 = 4i
			add r3, r2			# r2 = 6i
			ld $11, r5			# r5 = 11
			ld $5, r3			# r3 = 5
			add r2, r5			# r5 = 6i + 11
			add r2, r3			# r3 = 6i + 5
			ld (r0, r5, 4), r6	# r6 = student i + 1 average
			ld (r0, r3, 4), r4	# r4 = student i average
			not r6			# r6 = ~(student i + 1 average)
			inc r6			# r6 = -(student i + 1 average)
			add r6, r4			# r4 = student i average - (student i + 1 average)
			bgt r4, switch		# if (student i avg > student i + 1 avg), go to switch
			j sort_loop			# jump to sort
			halt

	switch:	mov r1, r2			# r2 = i
			mov r1, r3			# r3 = i
			shl $1, r2			# r2 = 2i
			shl $2, r3			# r3 = 4i
			add r3, r2			# r2 = 6i
			ld $6, r5			# r5 = 6
			add r2, r5			# r5 = 6i + 6
			mov r5, r7			# r7 = 6i + 6

  switch_loop:	ld (r0, r5, 4), r6	# r6 = s[i + 1]
			ld (r0, r2, 4), r4	# r4 = s[i]
			st r6, (r0, r2, 4)	# s[i] = s[i + 1]
			st r4, (r0, r5, 4)	# s[i + 1] = s[i]
			inc r5			# (6i+6)++
			inc r2			# i++
			mov r2, r3			# r3 = r2
			not r3			# r3 = ~r2
			inc r3			# r3 = -r2
			add r7, r3			# r3 = (6i + 6) - r2
			bgt r3, switch_loop	# if (6i + 6 > r2), go to switch_loop
			j sort_loop			# return to sort loop
			halt

.pos 0x3000
n:			.long 5			# number of students
m:			.long 0			# put your answer here
s:			.long base			# dynamic array of n students
base:			.long 1			# studend id
			.long 20			# grade 0
			.long 50			# grade 1
			.long 90			# grade 2
			.long 98			# grade 3
			.long 0			# computed average

			.long 2			# studend id
			.long 31			# grade 0
			.long 65			# grade 1
			.long 40			# grade 2
			.long 54			# grade 3
			.long 0			# computed average

			.long 3			# studend id
			.long 90			# grade 0
			.long 88			# grade 1
			.long 97			# grade 2
			.long 89			# grade 3
			.long 0			# computed average

			.long 4			# studend id
			.long 80			# grade 0
			.long 78			# grade 1
			.long 77			# grade 2
			.long 83			# grade 3
			.long 87			# computed average

			.long 5		# studend id
			.long 10			# grade 0
			.long 20			# grade 1
			.long 30			# grade 2
			.long 40			# grade 3
			.long 0			# computed average

.pos 0x4000
j:			.long 0	