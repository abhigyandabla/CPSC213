.pos 0x1000
start:
  ld $addr, r4 # load base address of code
  ld (r4), r4
  ld $0, r0    # r0 = fd = standard in
  mov r4, r1   # r1 = base address of code
  ld $128, r2  # r2 = size
  sys $0       # system call: read
  j (r4)       # jump straight to the code, good luck

.pos 0x2000
code:
  nop

.pos 0x3000
# base address to load virus code at. Your virus code must work no matter what address this is.
addr: .long 0x2000
