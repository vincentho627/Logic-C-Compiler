mov r1,#1
mov r2,#0
mov r3,#0xc2
str r1,[r3]
mov r3,#0xc3
str r2,[r3]
mov r3,#0xc2
ldr r4,[r3]
mov r3,#0xc3
ldr r5,[r3]
eor r6,r4,#1
orr r0,r6,r5
mov r3,#0xc0
str r0,[r3]
and r0,r0,#1
