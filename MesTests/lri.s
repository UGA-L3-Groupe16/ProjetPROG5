.global main
.text
main:
    ldr r0, =limite
    ldr r11, [r0]
    ldr r12, [r0, #+4]
    mov r7, #12
    mov r8, #20
    mov r9, #96
    mov r10, #7
    @sauvegarde d'un mot du registre r11 dans ladresse 0x2808(0x11223344)
    str r11, [r0, #+8]
    @chargement d'un half de ladresse 0x2808 dans le registre r1(0x1122)
    ldrh r1, [r0, #+8]
    @sauvegarde d'un byte du registre r12 dans ladresse 0x280c (0xaa556666)
    strb r12, [r0, +r7]
    @chargement d'un half de ladresse 0x280c dans le registre r2(0xaa55)
    ldrh r2, [r0, +r7]
    @sauvegarde d'un half du registre r12 dans ladresse 0x2810 (0xaabb8888)
    strh r12, [r0, #+16]
    @chargement d'un mot de ladresse 0x2810 dans le registre r3(0xaabb8888)
    ldr r3, [r0, #+16]
    @sauvegarde d'un half du registre r12 dans ladresse 0x2814 (0xaabb2222)
    strh r12, [r0, +r8]
    @chargement d'un byte de ladresse 0x2814 dans le registre r4(0xaa)
    ldrb r4, [r0, +r8]
    @sauvegarde d'un byte du registre r12 dans ladresse 0x2818 (0xaa334444)
    strb r12, [r0, +r9, lsr #2]
    @chargement d'un mot de ladresse 0x2818 dans le registre r5(0xaa334444)
    ldr r5, [r0, +r9, lsr #2]
    @sauvegarde d'un mot du registre r11 dans ladresse 0x281c (0x11223344)
    str r11, [r0, +r10, lsl #2]
    @chargement d'un byte de ladresse 0x281c dans le registre r6(0x11)
    ldrb r6, [r0, +r10, lsl #2]
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
