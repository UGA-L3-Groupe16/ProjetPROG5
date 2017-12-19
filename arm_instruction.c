/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

#define EQ 		0x0000
#define NE 		0x0001
#define CS		0x0010
#define CC		0x0011
#define MI 		0x0100
#define PL		0x0101
#define VS		0x0110
#define VC 		0x0111
#define HI		0x1000
#define LS		0x1001
#define GE		0x1010
#define LT		0x1011
#define GT		0x1100
#define LE		0x1101
#define AL		0x1110
#define EUUUH	0x1111

static int arm_execute_instruction(arm_core p) {
	int result;
	uint32_t value;
	result = arm_fetch(p, &value);
	uint32_t cond = plage(value, 28, 31);
	uint32_t cpsr = read_cpsr(p->reg);
	
	switch(cond) {
		case EQ:	if(!Z)	return 0;
			break;
		case NE:	if(Z)	return 0;
			break;
		case CS:	if(!C)	return 0;
			break;
		case CC:	if(C)	return 0;
			break;
		case MI:	if(!N)	return 0;
			break;
		case PL:	if(N)	return 0;
			break;
		case VS:	if(!V)	return 0;
			break;
		case VC:	if(V)	return 0;
			break;
		case HI:	if(!(C && !Z))	return 0;
			break;
		case LS:	if(C && !Z)	return 0;
			break;
		case GE:	if(!(N == V))	return 0;
			break;
		case LT:	if(N == V)	return 0;
			break;
		case GT:	if(!(Z == 0 && N == V))	return 0;
			break;
		case LE:	if(Z== 1 || N!= V)	return 0;
			break;
		case AL:	
			break;
		case EUUUH:	return 0;
			break;
		default:	// On rentrera jamais la dedans
			break;
	}
	switch(plage(value, 25, 27)) {
		case 0x000:
			arm_data_processing(p, value);
			break;
		case 0x001:
			break;
		case 0x010:
			break;
		case 0x011:
			break;
		case 0x100:
			break;
		case 0x101:
			break;
		case 0x110:
			break;
		case 0x111:
			break;
		default:
			break;
	}
    return result;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
