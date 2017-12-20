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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

uint32_t Op_bit_U(uint32_t address, uint16_t offset, uint32_t ins){	
//Signe de l'offset

	if(get_bit(ins, 23)){
		return address + offset;
	} else {
		return address - offset;
	}
}

void Op_bit_I(arm_core p, uint32_t address, uint16_t offset, uint32_t ins){	
//Offset immediat ou via registre, pour STR/STRB

	uint8_t reg_offset;		//Rm
	
	if(get_bit(ins, 25)){
	//Registre

		reg_offset = get_bits(ins, 3, 0);
		offset = arm_read_register(p, reg_offset);
		address = Op_bit_U(address, offset, ins);
	} else {
	//Immediat

		offset = get_bits(ins, 11, 0);
		address = Op_bit_U(address, offset, ins);
	}
}

void Op_bit_22(arm_core p, uint32_t address, uint16_t offset, uint32_t ins){	
//Offset immediat ou via registre, pour STRH
	
	uint8_t immedH;			//haut de l'offset
	uint8_t immedL;			//bas de l'offset
	uint8_t reg_offset;		//Rm
	
	if (get_bit(ins, 22)){
	//Immediat

		immedH = get_bits(ins, 11, 8);
		immedL = get_bits(ins, 3, 0);
		offset = (immedH << 4) | immedL;
		Op_bit_U(address, offset, ins);
	} else {
	//Registre

		reg_offset = get_bits(ins, 3, 0);
		offset = arm_read_register(p, reg_offset);
		address = Op_bit_U(address, offset, ins);
	}
}

void Op_bit_W(arm_core p, uint32_t address, uint16_t offset, uint32_t ins, uint8_t dest){
//Ecriture de la nouvelle adresse dans le registre ou pas

	if (get_bit(ins, 26)){
		Op_bit_I(p, address, offset, ins);
	} else {
		Op_bit_22(p, address, offset, ins);
	}
	if (get_bit(ins, 21)){
	//Pre-indexed addressing -> enregistrement dans le registre

		arm_write_register(p, dest, address);
	}
}

int store(arm_core p , uint32_t ins, uint8_t source, uint8_t dest, uint32_t address){
	uint32_t value;			//Valeur stockée, word
	uint16_t value_half;	//Valeur stockée, half-word
	uint8_t value_byte;		//Valeur stockée, byte
	uint16_t offset;
	int success;

	if (get_bit(ins, 26)){
		//STR / STRB

		if (get_bit(ins, 22)){		
		//STRB

			value_byte = (arm_read_register(p, source)) & 0xFF;
		} else {	
		//STR

			value = arm_read_register(p, source);
		}
		if (get_bit(ins, 24)){	
		//Pre-indexed addressing / offset addressing

			Op_bit_W(p, address, offset, ins, dest);
			if (get_bit(ins, 22)){
			//STRB

				success = arm_write_byte(p, address, value_byte);
			} else {	
			//STR

				success = arm_write_word(p, address, value);
			}
		} else {	
		//Post-indexed addressing

			if (get_bit(ins, 22)){		
			//STRB

				success = arm_write_byte(p, address, value_byte);
			} else {	
			//STR

				success = arm_write_word(p, address, value);
			}
			Op_bit_I(p, address, offset, ins);
			arm_write_register(p, dest, address);					
		}
	} else {	
	//STRH

		value_half = (arm_read_register(p, source)) & 0xFFFF;
		if (get_bit(ins, 24)){	
		//Pre-indexed addressing / offset addressing

			Op_bit_W(p, address, offset, ins, dest);
			success = arm_write_half(p, address, value_half);
		} else {	
		//Post-indexed addressing		

			success = arm_write_half(p, address, value_half);
			Op_bit_22(p, address, offset, ins);
			arm_write_register(p, dest, address);
		}
	}
	return success;
}

int arm_load_store(arm_core p, uint32_t ins) {

	uint32_t value;			//Valeur stockée, word
	uint16_t value_half;	//Valeur stockée, half-word
	uint8_t value_byte;		//Valeur stockée, byte
	uint8_t source;			//Rn
	uint8_t dest;			//Rd
	uint32_t address;
	int success;

	source = get_bits(ins, 19, 16);
	dest = get_bits(ins, 15, 12);
	address = arm_read_register(p, dest);
	if (get_bit(ins, 20)){	
	//LOAD

		//success = load(p, ins, source, dest, address);
	} else {
	//STORE

		success = store(p, ins, source, dest, address);
	}
    return success;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
