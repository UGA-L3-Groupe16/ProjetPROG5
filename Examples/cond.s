.global main
.text

andd:	
	and r1, r2
	mov pc, lr   		@ retour a linstruction apres le branchement
main:
    mov r1, #1
    mov r2, #3
    mov r3, #5
    subs r4, r2, r3     @ met -2 dans r4 et maj des flags N=1, Z=0, C=0, V=0
    addeq r5,  r1, #1   @ comme Z=0 rien ne se passe 
    addne r5,  r1, #1   @ comme Z=0 2 est mis dans r5
    addcs r5,  r5, #1   @ comme C=0 rien ne se passe 
    addcc r5,  r5, #1   @ comme C=0 3 est mis dans r5
    addmi r5,  r5, #1   @ comme N=1 4 est mis dans r5
    addpl r5,  r5, #1   @ comme N=1 rien ne se passe 
    addvs r5,  r5, #1   @ comme V=0 rien ne se passe 
    addvc r5,  r5, #1   @ comme V=0 5 est mis dans r5
    addhi r5,  r5, #1   @ comme C=0 et Z=0 rien ne se passe (C doit etre egal a 1)
    addls r5,  r5, #1   @ comme C=0 et Z=0 6 est mis dans r5 (C doit etre egal a 0 ou Z a 1)
    addge r5,  r5, #1   @ comme N=1 et V=0 rien ne se passe (N doit etre egal a V)
    addlt r5,  r5, #1   @ comme N=1 et V=0 7 est mis dans r5 (N doit etre different a V)
    addgt r5,  r5, #1   @ comme Z=0, N=1 et V=0 rien ne se passe (Z doit etre egal a 0 et N doit etre egal a V)
    addle r5,  r5, #1   @ comme Z=0, N=1 et V=0 8 est mis dans r5 (Z doit etre egal a 1 ou N doit etre different a V)
    addal r5,  r5, #1   @ 9 est mis dans r5, inconditionnel
	bl andd 			@ branchement a andd avec memorisation de ladresse de retour 
	b end				@ branchement a end
	sub r3, r1, r2		@ ignoré
	add r4, r5, r1		@ ignoré

end: 
	swi 0x123456
