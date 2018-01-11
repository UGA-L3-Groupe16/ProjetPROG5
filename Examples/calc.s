.global main
.text

main:
	mov r1,#150
	mov r2,#5
	mov r3,#-8
	mov r4,#0xFFFFFFFA

	adds r0,r1,r2
	adds r0,r2,r3
	adds r0,r3,r4
	adds r0,r4,r1
	subs r0,r1,r2
	subs r0,r2,r3
	subs r0,r3,r4
	subs r0,r4,r1

end:
	swi 0x123456
