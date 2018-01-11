.global main
.text
main:
    mov r0, #0x21
    mov r1, #0xF
    mov r2, #0x100
    str r0, [r2, #15]
    ldr r3, [r2, #15]
    swi 0x123456