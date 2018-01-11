.global main
.text

main:
	mov r0,#45
	mov r1,#45
	cmp r1,r0

end:
	swi 0x123456
