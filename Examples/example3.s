.global main
.text
main:
    ldr r0, =limite
    ldrh r1, [r0], #+4
    ldr r0, =limite
    mov r4, #4
    ldrh r2, [r0], +r4
    swi 0x123456
.data
limite:
    .word 0x12345678
    .word 0x55667788
