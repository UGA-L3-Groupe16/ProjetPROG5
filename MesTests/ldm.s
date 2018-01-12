.global main
.text
main:
    mov r4, #1
    ldr r1, =donnee
    ldr r5, [r1, +r4, lsl #2]
    ldmia r1!, {r6, r7, r9}
    ldr r1, =donnee
    ldmia r1, {r6, r7, r9}
    ldr r1, =donnee
    ldmib r1!, {r6, r7, r9}
    ldr r1, =donnee
    ldmib r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    ldmda r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    ldmda r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    ldmdb r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    ldmdb r1, {r6, r7, r9}
    swi 0x123456
.data
donnee:
    .word 0xff
    .word 0xffff
fin:
    .word 0xffffff
    .word 0xffffffff