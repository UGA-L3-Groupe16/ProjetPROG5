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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
	uint8_t *memory;
	size_t	size;
	int		endianess;
};

typedef struct memory_data memory_data;

memory memory_create(size_t size, int is_big_endian) {
    memory mem = (memory)malloc(sizeof(memory_data));
	mem->memory = (uint8_t*)malloc(sizeof(uint8_t)*size);
	mem->size = size;
	mem->endianess = is_big_endian;
    return mem;
}

size_t memory_get_size(memory mem) {
	return mem->size;
}

void memory_destroy(memory mem) {
	free(mem->memory);
	free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
	if(address >= mem->size)
		return -1;
	else {
		*value = mem->memory[address];
		return 0;
	}
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
	if(address+1 >= mem->size)
		return -1;
	else {
		*value = 0;
		if(mem->endianess) { // Big endian
			*value += mem->memory[address] << 8;
			*value += mem->memory[address+1];
		} else { // Little endian
			*value += mem->memory[address+1] << 8;
			*value += mem->memory[address];
		}
		return 0;
	}
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
	if(address+3 >= mem->size)
		return -1;
	else {
		*value = 0;
		if(mem->endianess) { // Big endian
			*value += mem->memory[address] << 24;
			*value += mem->memory[address+1] << 16;
			*value += mem->memory[address+2] << 8;
			*value += mem->memory[address+3];
		} else { // Little endian
			*value += mem->memory[address+3] << 24;
			*value += mem->memory[address+2] << 16;
			*value += mem->memory[address+1] << 8;
			*value += mem->memory[address];
		}
		return 0;
	}
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
	if(address >= mem->size)
		return -1;
	else {
		mem->memory[address] = value;
		return 0;
	}
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
	if(address+1 >= mem->size)
		return -1;
	else {
		if(mem->endianess) { // Big endian
			mem->memory[address] = (value & (0xFF << 8)) >> 8;
			mem->memory[address+1] = value & 0xFF;
		} else { // Little endian
			mem->memory[address+1] = (value & (0xFF << 8)) >> 8;
			mem->memory[address] = value & 0xFF;
		}
		return 0;
	}
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
	if(address+3 >= mem->size)
		return -1;
	else {
		if(mem->endianess) { // Big endian
			mem->memory[address] = (value & (0xFF << 24)) >> 24;
			mem->memory[address+1] = (value & (0xFF << 16)) >> 16;
			mem->memory[address+2] = (value & (0xFF << 8)) >> 8;
			mem->memory[address+3] = value & 0xFF;
		} else { // Little endian
			mem->memory[address+3] = (value & (0xFF << 24)) >> 24;
			mem->memory[address+2] = (value & (0xFF << 16)) >> 16;
			mem->memory[address+1] = (value & (0xFF << 8)) >> 8;
			mem->memory[address] = value & 0xFF;
		}
		return 0;
	}
}
