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

typedef uint32_t (*data_proc_callback)(arm_core p, uint32_t,uint32_t);

uint32_t cmd_and(arm_core p, uint32_t a, uint32_t b) {
	return a & b;
}

uint32_t cmd_eor(arm_core p, uint32_t a, uint32_t b) {
	return a ^ b;
}

uint32_t cmd_sub(arm_core p, uint32_t a, uint32_t b) {
	return a - b;
}

uint32_t cmd_rsb(arm_core p, uint32_t a, uint32_t b) {
	return b - a;
}

uint32_t cmd_add(arm_core p, uint32_t a, uint32_t b) {
	return a + b;
}

uint32_t cmd_adc(arm_core p, uint32_t a, uint32_t b) {
	return a + b + get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_sbc(arm_core p, uint32_t a, uint32_t b) {
	return a - b - !get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_rsc(arm_core p, uint32_t a, uint32_t b) {
	return b - a - !get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_orr(arm_core p, uint32_t a, uint32_t b) {
	return a | b;
}

uint32_t cmd_mov(arm_core p, uint32_t a, uint32_t b) {
	return b;
}

uint32_t cmd_bic(arm_core p, uint32_t a, uint32_t b) {
	return a & ~b;
}

uint32_t cmd_mvn(arm_core p, uint32_t a, uint32_t b) {
	return ~b;
}

data_proc_callback data_proc_operations[] = {
	&cmd_and,
	&cmd_eor,
	&cmd_sub,
	&cmd_rsb,
	&cmd_add,
	&cmd_adc,
	&cmd_sbc,
	&cmd_rsc,
	NULL,
	NULL,
	NULL,
	NULL,
	&cmd_orr,
	&cmd_mov,
	&cmd_bic,
	&cmd_mvn
};

#define SHIFT_LSL	0x00
#define	SHIFT_LSR	0x01
#define SHIFT_ASR	0x10
#define SHIFT_ROR	0x11

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint32_t opcode = get_bits(ins, 24, 21);
	uint8_t rn = (uint8_t)get_bits(ins, 19, 16);
	uint8_t	rd = (uint8_t)get_bits(ins, 15, 12);
	uint32_t I = get_bit(ins, 25);
	uint32_t bit_4 = get_bit(ins, 4);
	uint32_t shifter_operand;
	uint32_t shift = get_bits(ins, 6, 5);
	uint32_t rm = get_bits(ins, 3, 0);
	if(bit_4) {
		uint8_t rs = (uint8_t)get_bits(ins, 11, 8);
		shifter_operand = ((rm << (shift == SHIFT_LSL)*rs) >> (shift == SHIFT_LSR)*rs);
	} else {
		uint32_t shift_imm = get_bits(ins, 11, 7);	
		shifter_operand = ((rm << (shift == SHIFT_LSL)*shift_imm) >> (shift == SHIFT_LSR)*shift_imm);
	}
	if(opcode <= 7 || opcode >= 12) // Ne marche pas pour tst, teq, cmp, cmn
		arm_write_register(p, rd, data_proc_operations[opcode](p, rn, shifter_operand));
	else
		;
	return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

