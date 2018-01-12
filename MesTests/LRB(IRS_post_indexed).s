.global main
.text
main:
    ldr r0, =limite
    ldr r11, [r0]
    ldr r12, [r0, #+4]
    mov r7, #4
    mov r9, #96
    mov r10, #7
    add r0, r0, #8
    str r11, [r0], +r7
    strb r12, [r0], #+4
    strh r12, [r0], #+4
    strh r12, [r0], +r7
    strb r12, [r0], +r9, lsr #2
    ldr r0, =limite
    str r11, [r0], +r10, lsl #2
    ldr r0, =limite
    add r0, r0, #8
    ldrh r1, [r0], #+4
    ldrh r2, [r0], +r7
    ldr r3, [r0], #+4
    ldrb r4, [r0], +r7
    ldr r5, [r0], +r9, lsr #2
    ldr r0, =limite
    ldrb r6, [r0], +r10, lsl #2
    swi 0x123456
.data
limite:
    .word 0x11223344
    .word 0xaabbccdd
    .word 0xeeeeffff
    .word 0x55556666
    .word 0x77778888
    .word 0x11112222
    .word 0x33334444
    .word 0x99990000
