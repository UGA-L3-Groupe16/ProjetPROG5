.global main
.text

main:
	mov r1,#0xFFFFFFFF
	mov r2,#30
	adds r0,r1,r2
	mov r2,#5
	mov r3,#2
	add r3,r1,#4
	sub r2,r1,#6
	and r3,r1,r2
	eor r3,r1,r2
	sub r3,r1,r2
	rsb r3,r1,r2
	add r3,r1,r2
	adc r3,r1,r2
	sbc r3,r1,r2
	rsc r3,r1,r2
	orr r3,r1,r2
	mov r3,r1
	bic r3,r1,r2
	mvn r3,r1

end:
	swi 0x123456
