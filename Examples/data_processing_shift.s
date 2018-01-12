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

	@ sans intéractions avec les flags
	and	r4, r1, r2 @ 456 & (-40) => 456 attendu
	eor r4, r1, r2 @ 456 ^ (-40) => -496 (4294966800) attendu
	sub r4, r1, r2 @ 456 - (-40) => 496 attendu
	rsb r4, r1, r2 @ (-40) - 496 => -496 attendu
	add r4, r1, r2 @ 456 + (-40) => 416 attendu
	orr r4, r1, r2 @ 456 | (-40) => (-40) (4294967256) attendu
	orr r4, r1, #49 @ 456 | 49 => 505 attendu
	mvn r4, r1 @ ~456 => -457 (4294966839) attendu

	@ Instructions qui interagissent avec les flags (C = 0)
	adc r5, r1, r2 @ 456 + (-40) + 0 => 416 attendu
	sbc r5, r1, r2 @ 456 - (-40) - 1 => 495 attendu
	rsc r5, r1, r2 @ (-40) - 456 - 1 => -497 (4294966799) attendu

	@tests
	@tst r1,r2	@ flags après 456 & (-40) => 0b0000
	@teq 

end:
	swi 0x123456
