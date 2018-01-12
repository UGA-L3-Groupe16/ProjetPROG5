.global main
.text

main:
	mov r1, #456
	mov r2, #-40
	mov r3, r1, LSL #2
	mov r3, r1, LSR #2
	mov r3, r1, ASR #3
	mov r3, r2, ASR #3
	mov r3, r1, ROR #4

end:
	swi 0x123456
