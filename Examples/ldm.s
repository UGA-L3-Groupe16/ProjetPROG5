.global main
.text
main:
    mov r4, #1
    ldr r1, =donnee
    ldr r5, [r1, +r4, lsl #2]
    @mets la valeurs des adresses 0x2800-0x2808 dans r6,r7 et r9, avec mise a jour de r1
    ldmia r1!, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeurs des adresses 0x2800-0x2808 dans r6,r7 et r9, sans mise a jour de r1
    ldmia r1, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeurs des adresses 0x2804-0x280c dans r6,r7 et r9, avec mise a jour de r1
    ldmib r1!, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeurs des adresses 0x2804-0x280c dans r6,r7 et r9, sans mise a jour de r1
    ldmib r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    @mets la valeurs des adresses 0x2804-0x280c dans r6,r7 et r9, avec mise a jour de r1
    ldmda r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    @mets la valeurs des adresses 0x2804-0x280c dans r6,r7 et r9, sans mise a jour de r1
    ldmda r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    @mets la valeurs des adresses 0x2800-0x2808 dans r6,r7 et r9, avec mise a jour de r1
    ldmdb r1!, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    @mets la valeurs des adresses 0x2800-0x2808 dans r6,r7 et r9, sans mise a jour de r1
    ldmdb r1, {r6, r7, r9}
    swi 0x123456
.data
donnee:
    .word 0xff
    .word 0xffff
fin:
    .word 0xffffff
    .word 0xffffffff