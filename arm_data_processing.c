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

#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint32_t opcode = plage(ins, 21, 24);
	uint8_t rn = (uint8_t)plage(ins, 16, 19);
	uint8_t	rd = (uint8_t)plage(ins, 12, 15);
    return UNDEFINED_INSTRUCTION;
}

int 

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_shift(arm_core p, uint32_t inst, uint8_t rn, uint8_t rd) {
/*
	uint32_t shift = plage(ins, 5, 5); // 1 = Right, 0 = Left

	if(!plage(ins, 4, 4)) { // Valeur immediate
		uint32_t shift_amount = plage(ins, 7, 11);
	} else {
		uint8_t rs = (uint8_t)plage(ins, 8, 11);
	}
	write_regis*/
    return UNDEFINED_INSTRUCTION;
}
