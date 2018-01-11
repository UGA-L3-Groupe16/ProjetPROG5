/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

#define EQ 		0b0000
#define NE 		0b0001
#define CS		0b0010
#define CC		0b0011
#define MI 		0b0100
#define PL		0b0101
#define VS		0b0110
#define VC 		0b0111
#define HI		0b1000
#define LS		0b1001
#define GE		0b1010
#define LT		0b1011
#define GT		0b1100
#define LE		0b1101
#define AL		0b1110
#define EUUUH	0b1111

static int arm_execute_instruction(arm_core p) {
	int result;
	uint32_t value;
	result = arm_fetch(p, &value);
	uint32_t cond = get_bits(value, 31, 28);
	uint32_t cpsr = arm_read_cpsr(p);
	//printf("%x", value);

	switch(cond) {
		case EQ:	if(!get_bit(cpsr, Z))	return 0;
			break;
		case NE:	if(get_bit(cpsr,Z))	return 0;
			break;
		case CS:	if(!get_bit(cpsr,C))	return 0;
			break;
		case CC:	if(get_bit(cpsr,C))	return 0;
			break;
		case MI:	if(!get_bit(cpsr,N))	return 0;
			break;
		case PL:	if(get_bit(cpsr,N))	return 0;
			break;
		case VS:	if(!get_bit(cpsr,V))	return 0;
			break;
		case VC:	if(get_bit(cpsr,V))	return 0;
			break;
		case HI:	if(!(get_bit(cpsr,C) && !get_bit(cpsr,Z)))	return 0;
			break;
		case LS:	if(get_bit(cpsr,C) && !get_bit(cpsr,Z))	return 0;
			break;
		case GE:	if(!(get_bit(cpsr,N) == get_bit(cpsr,V)))	return 0;
			break;
		case LT:	if(get_bit(cpsr,N) == get_bit(cpsr,V))	return 0;
			break;
		case GT:	if(!(get_bit(cpsr,Z) == 0 && get_bit(cpsr,N) == get_bit(cpsr,V)))	return 0;
			break;
		case LE:	if(get_bit(cpsr,Z) == 1 || get_bit(cpsr,N) != get_bit(cpsr,V))	return 0;
			break;
		case AL:
			break;
		case EUUUH:	return 0;
			break;
		default:	// On rentrera jamais la dedans
			break;
	}
	switch(get_bits(value, 27, 25)) {
		case 0b000:	
			if(get_bits(value, 7, 4) != 0b1011)
				arm_data_processing_shift(p, value);
			else
				arm_load_store(p, value);
			break;
		case 0b001:	arm_data_processing_immediate_msr(p, value);	
			break;
		case 0b010:	arm_load_store(p, value);
			break;
		case 0b011: arm_load_store(p, value);
			break;
		case 0b100:	arm_load_store_multiple(p, value);
			break;
		case 0b101: arm_branch(p, value);
			break;
		case 0b110:	arm_coprocessor_load_store(p, value);
			break;
		case 0b111:
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
