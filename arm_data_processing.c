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

#define OPCODE_TST 0b1000
#define OPCODE_CMN 0b1011

int carry_from(uint64_t calcul) {
	uint64_t masque = 1;
	masque <<= 32;
	return (calcul & masque) >> 32;
}

typedef uint32_t (*data_proc_callback)(arm_core p, uint32_t,uint32_t,int);

void update_N_Z(arm_core p, uint32_t loperand, uint32_t roperand, uint32_t res) {
	uint32_t tempCPSR = arm_read_register(p, CPSR);
	tempCPSR |= get_bit(res, 31) << 31;			// N
	tempCPSR |= (res == 0) << 30;				// Z
	arm_write_register(p, CPSR, tempCPSR);
}

void clear_flags(arm_core p) {
	uint32_t tempCPSR = arm_read_register(p, CPSR);
	arm_write_register(p, CPSR, tempCPSR & (~(uint32_t)0 >> 4));
}

uint32_t cmd_and(arm_core p, uint32_t a, uint32_t b, int S) {
	return a & b;
}

uint32_t cmd_eor(arm_core p, uint32_t a, uint32_t b, int S) {
	return a ^ b;
}

uint32_t cmd_sub(arm_core p, uint32_t a, uint32_t b, int S) {
	if(S) {
		clear_flags(p);
		uint32_t tempCPSR = arm_read_register(p, CPSR);
		tempCPSR |= (a >= b) << 29;
		tempCPSR |= get_bit(a, 31) != get_bit(b,31) && get_bit(b, 31) == get_bit(a-b,31) << 28;
		arm_write_register(p, CPSR, tempCPSR);
	}
	return a - b;
}

uint32_t cmd_rsb(arm_core p, uint32_t a, uint32_t b, int S) {
	return cmd_sub(p, b, a, S);
}

uint32_t cmd_add(arm_core p, uint32_t a, uint32_t b, int S) {
	if(S) {
		clear_flags(p);
		uint32_t tempCPSR = arm_read_register(p, CPSR);
		tempCPSR |= carry_from((uint64_t)a + (uint64_t)b) << 29;
		tempCPSR |= ((get_bit(a,31) == get_bit(b,31)) && (get_bit(a,31) != get_bit(a+b,31))) << 28;
		arm_write_register(p, CPSR, tempCPSR);
	}
	return a + b;
}

uint32_t cmd_adc(arm_core p, uint32_t a, uint32_t b, int S) {
	if(S) {
		clear_flags(p);
		uint32_t tempCPSR = arm_read_register(p, CPSR);
		tempCPSR |= carry_from((uint64_t)a + (uint64_t)b + (uint64_t)get_bit(arm_read_cpsr(p), C)) << 29;
		tempCPSR |= ((get_bit(a,31) == get_bit(b,31)) && (get_bit(a,31) == get_bit(a+b,31))) << 28;
		arm_write_register(p, CPSR, tempCPSR);
	}
	return a + b + get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_sbc(arm_core p, uint32_t a, uint32_t b, int S) {
	if(S) {
		clear_flags(p);
		uint32_t tempCPSR = arm_read_register(p, CPSR);
		tempCPSR |= (a >= b) << 29;
		tempCPSR |= get_bit(a, 31) != get_bit(b,31) && get_bit(b, 31) == get_bit(a-b,31) << 28;
		arm_write_register(p, CPSR, tempCPSR);
	}
	return a - b - !get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_rsc(arm_core p, uint32_t a, uint32_t b, int S) {
	return b - a - !get_bit(arm_read_cpsr(p), C);
}

uint32_t cmd_tst(arm_core p, uint32_t a, uint32_t b, int S) {
	clear_flags(p);
	update_N_Z(p, a, b, a & b);
}

uint32_t cmd_teq(arm_core p, uint32_t a, uint32_t b, int S) {
	clear_flags(p);
	update_N_Z(p, a, b, a ^ b);
}

uint32_t cmd_cmp(arm_core p, uint32_t a, uint32_t b, int S) {
	cmd_sub(p, a, b, 1); // clear_flags a l'interieur
	update_N_Z(p, a, b, a - b);
}

uint32_t cmd_cmn(arm_core p, uint32_t a, uint32_t b, int S) {
	cmd_add(p, a, b, 1); // clear_flags a l'interieur
	update_N_Z(p, a, b, a + b);
}

uint32_t cmd_orr(arm_core p, uint32_t a, uint32_t b, int S) {
	return a | b;
}

uint32_t cmd_mov(arm_core p, uint32_t a, uint32_t b, int S) {
	return b;
}

uint32_t cmd_bic(arm_core p, uint32_t a, uint32_t b, int S) {
	return a & ~b;
}

uint32_t cmd_mvn(arm_core p, uint32_t a, uint32_t b, int S) {
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
	&cmd_tst,
	&cmd_teq,
	&cmd_cmp,
	&cmd_cmn,
	&cmd_orr,
	&cmd_mov,
	&cmd_bic,
	&cmd_mvn
};

#define SHIFT_LSL	0b00
#define	SHIFT_LSR	0b01
#define SHIFT_ASR	0b10
#define SHIFT_ROR	0b11

// /!\ /!\ Les instructions n'updatent pour l'instant pas les flags /!\ /!\.
uint32_t shifts(uint8_t shift_bits, uint32_t loperand, uint32_t roperand) {
	uint32_t bits_restants = loperand & (~0 >> (32 - (roperand % 32)));
	switch(shift_bits) {
		case SHIFT_LSL:
			return loperand << roperand;
		case SHIFT_LSR:
			return loperand >> roperand;
		case SHIFT_ASR:
			return asr(loperand, roperand);
		case SHIFT_ROR:
			return ror(loperand, roperand);
		default:
			return 88;
	}
}

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
	uint32_t opcode = get_bits(ins, 24, 21);
	uint8_t rn = (uint8_t)get_bits(ins, 19, 16);
	uint8_t	rd = (uint8_t)get_bits(ins, 15, 12);
	uint32_t bit_4 = get_bit(ins, 4);
	uint32_t shifter_operand;
	uint32_t shift = get_bits(ins, 6, 5);
	uint32_t rm = get_bits(ins, 3, 0);
	uint32_t S = get_bit(ins, 20);
	if(bit_4) {
		uint8_t rs = (uint8_t)get_bits(ins, 11, 8);
		rs = arm_read_register(p, rs);
		shifter_operand = shifts(shift, arm_read_register(p,rm),rs);
	} else {
		uint32_t shift_imm = get_bits(ins, 11, 7);	
		shifter_operand = shifts(shift, arm_read_register(p,rm),shift_imm);
	}
	uint32_t op1 = arm_read_register(p,rn);
	uint32_t res = data_proc_operations[opcode](p, op1, shifter_operand, S);
	if(opcode <= OPCODE_TST || opcode >= OPCODE_CMN) {
		if(S) {
			update_N_Z(p, op1, shifter_operand, res);
		}
		arm_write_register(p, rd, res);
	}
	return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
	uint32_t opcode = get_bits(ins, 24, 21);
	uint8_t rn = (uint8_t)get_bits(ins, 19, 16);
	uint8_t	rd = (uint8_t)get_bits(ins, 15, 12);
	uint32_t S = get_bit(ins, 20);
	uint32_t rotate_imm = get_bits(ins, 11, 8);
	uint32_t immed_8 = get_bits(ins, 7, 0);
	uint32_t shifter_operand = ror(immed_8,(rotate_imm * 2));
	uint32_t op1 = arm_read_register(p,rn);
	uint32_t res = data_proc_operations[opcode](p, op1, shifter_operand, S);
	if(opcode <= OPCODE_TST || opcode >= OPCODE_CMN) {
		if(S) {
			update_N_Z(p, op1, shifter_operand, res);
		}
		arm_write_register(p, rd, res);
	}
	return 0;
}

