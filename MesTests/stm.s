.global main
.text
main:
    mov r4, #1
    ldr r1, =donnee
    ldr r5, [r1, +r4, lsl #2]
    mov r6, #0xFF
    mov r7, #0xFFFF
    mov r9, #0xFFFFFFFF
    stmia r1!, {r6, r7, r9}
    ldr r1, =donnee
    stmia r1, {r6, r7, r9}
    ldr r1, =donnee
    stmib r1!, {r6, r7, r9}
    ldr r1, =donnee
    stmib r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    stmda r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    stmda r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    stmdb r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    stmdb r1, {r6, r7, r9}
    swi 0x123456
.data
donnee:
    .word 0x11223344
    .word 0x55667788
fin:
    .word 0xffff6789
    .word 0x11112222
