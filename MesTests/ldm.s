.global main
.text
main:
    mov r4, #1
    ldr r1, =donnee

    @mets la valeur de ladresse 0X2804 dans r5
    ldr r5, [r1, +r4, lsl #2]

    @mets la valeur des adresses 0X2800-0X280C dans r6, r7 et r9, avec une mise a jour de r1
    ldmia r1!, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeur des adresses 0X2800-0X208C dans r6, r7 et r9, sans une mise a jour de r1
    ldmia r1, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeur des adresses 0X2804-0X2810 dans r6, r7 et r9, avec une mise a jour de r1
    ldmib r1!, {r6, r7, r9}
    ldr r1, =donnee
    @mets la valeur des adresses 0X2804-0X2810 dans r6, r7 et r9, sans une mise a jour de r1
    ldmib r1, {r6, r7, r9}
    ldr r1, =fin
    add r1, r1, #4
    @mets la valeur des adresses 0X2804-0X2810 dans r6, r7 et r9, sans une mise a jour de r1
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