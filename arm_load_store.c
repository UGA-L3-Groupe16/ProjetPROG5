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

uint32_t Op_bit_I(arm_core p, uint32_t address, uint16_t offset, uint32_t ins){
//Offset immediat ou via registre, pour STR/STRB

	uint8_t reg_offset;		//Rm
	uint32_t addr_off;
	uint8_t shift_imm;

	if(get_bit(ins, 25)){
	//Registre

		reg_offset = get_bits(ins, 3, 0);
		if (!get_bits(ins, 11, 4)){
			offset = arm_read_register(p, reg_offset);
		} else {
			shift_imm = get_bits(ins, 11, 7);
			if (get_bits(ins, 6, 5)){
				if (shift_imm){
					offset = arm_read_register(p, reg_offset)>>shift_imm;
				} else{
					offset = 0;
					if ((get_bits(ins, 6, 5) == 0b10)&&(get_bit(arm_read_register(p, reg_offset), 31))){
						offset = ~offset;
					}else if(get_bits(ins, 6, 5) == 0b11){
						offset=(get_bit(arm_read_cpsr(p), C)<<31)|(arm_read_register(p, reg_offset)>>1);
					}
				}
			}else{
				offset=arm_read_register(p, reg_offset)<<shift_imm;
			}
		}
	} else {
	//Immediat

		offset = get_bits(ins, 11, 0);
	}
	addr_off = Op_bit_U(address, offset, ins);
	return addr_off;
}

uint32_t Op_bit_22(arm_core p, uint32_t address, uint16_t offset, uint32_t ins){
//Offset immediat ou via registre, pour STRH

	uint8_t immedH;			//haut de l'offset
	uint8_t immedL;			//bas de l'offset
	uint8_t reg_offset;		//Rm
	uint32_t addr_off;

	if (get_bit(ins, 22)){
	//Immediat

		immedH = get_bits(ins, 11, 8);
		immedL = get_bits(ins, 3, 0);
		offset = (immedH << 4) | immedL;
	} else {
	//Registre

		reg_offset = get_bits(ins, 3, 0);
		offset = arm_read_register(p, reg_offset);
	}
	addr_off = Op_bit_U(address, offset, ins);
	return addr_off;
}

uint32_t Op_bit_W(arm_core p, uint32_t address, uint16_t offset, uint32_t ins, uint8_t dest){
//Ecriture de la nouvelle adresse dans le registre ou pas
	uint32_t addr_off;

	if (get_bit(ins, 26)){
		addr_off = Op_bit_I(p, address, offset, ins);
	} else {
		addr_off = Op_bit_22(p, address, offset, ins);
	}
	if (get_bit(ins, 21)){
	//Pre-indexed addressing -> enregistrement dans le registre

		arm_write_register(p, dest, addr_off);
	}
	return addr_off;
}

int store(arm_core p , uint32_t ins, uint8_t source, uint8_t dest, uint32_t address){
	uint32_t value;			//Valeur stockée, word
	uint16_t value_half;	//Valeur stockée, half-word
	uint8_t value_byte;		//Valeur stockée, byte
	uint16_t offset;
	uint32_t addr_off;
	int success;

	if (get_bit(ins, 26)){
		//STR / STRB

		if (get_bit(ins, 22)){
		//STRB

			//value_byte = (arm_read_register(p, source)) & 0xFF;
			value_byte = (uint8_t) get_bits(arm_read_register(p, source),31,24);
		} else {
		//STR

			value = arm_read_register(p, source);
		}
		if (get_bit(ins, 24)){
		//Pre-indexed addressing / offset addressing

			addr_off = Op_bit_W(p, address, offset, ins, dest);
			if (get_bit(ins, 22)){
			//STRB

				success = arm_write_byte(p, addr_off, value_byte);
			} else {
			//STR
				success = arm_write_word(p, addr_off, value);
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
			addr_off = Op_bit_I(p, address, offset, ins);
			arm_write_register(p, dest, addr_off);
		}
	} else {
	//STRH

		//value_half = (arm_read_register(p, source)) & 0xFFFF;
		value_half = (uint16_t) get_bits(arm_read_register(p, source),31,16);
		if (get_bit(ins, 24)){
		//Pre-indexed addressing / offset addressing

			addr_off = Op_bit_W(p, address, offset, ins, dest);
			success = arm_write_half(p, addr_off, value_half);
		} else {
		//Post-indexed addressing

			success = arm_write_half(p, address, value_half);
			addr_off = Op_bit_22(p, address, offset, ins);
			arm_write_register(p, dest, addr_off);
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
	uint8_t base;
	uint32_t offset;
	uint32_t address;
	int success;

	source = get_bits(ins, 19, 16);
	dest = get_bits(ins, 15, 12);

	if (get_bit(ins, 20)){

	address = arm_read_register(p, dest);
	if((ins>>26)&1){   //LDR
    if(!((ins>>25)&1))   //valeur immediate
    offset=ins&0XFFF;
    else{
			if(!get_bits(ins,11,4))
		offset=arm_read_register(p,(uint8_t)(ins&0XF));
		else{
			uint32_t rm=arm_read_register(p,(uint8_t)get_bits(ins,3,0));
			uint8_t shift_imm=(uint8_t)get_bits(ins,11,7);

			if(get_bits(ins,6,5)){
				if(shift_imm)
				offset=rm>>shift_imm;

				else{
					offset=0;
					if((get_bits(ins,6,5)==0b10)&&(get_bit(rm,31)))
					offset=~offset;

					else if(get_bits(ins,6,5)==0b11)
					offset=(get_bit(arm_read_cpsr(p), C)<<31)|(rm>>1);
				}
			}
			else
			offset=rm<<shift_imm;

		}
	}
}
else{//LDRH
  if((ins>>22)&1){   //valeur immediate
  offset=(ins&0XF00)>>4;
  offset|=(ins&0XF);
}
  else
  offset=arm_read_register(p,(uint8_t)(ins&0XF));
}

      base=(uint8_t)((ins>>16)&0XF);

      if(!((ins>>24)&1)){
        if((ins>>21)&1)
        return UNDEFINED_INSTRUCTION;

        address=arm_read_register(p,base);
        if((ins>>23)&1)
        arm_write_register(p,base,arm_read_register(p,base)+offset);
        else
        arm_write_register(p,base,arm_read_register(p,base)-offset);
      }else{
        if((ins>>23)&1)
        address=arm_read_register(p,base)+offset;

        else
        address=arm_read_register(p,base)-offset;

        if((ins>>21)&1)
        arm_write_register(p,base,address);
      }

    dest=(uint8_t)((ins>>12)&0XF);

    if((ins>>26)&1){

    if((ins>>22)&1){//chargement d'un byte

    arm_read_byte(p,address,&value_byte);
    arm_write_register(p,dest,value_byte);
    }
    else{//chargement d'un word
		arm_read_word(p,address,&value);

			if(dest==15){
				arm_write_register(p,dest,value & 0xFFFFFFFE);
				if(value%2)
				arm_write_cpsr(p,set_bit(arm_read_cpsr(p),5));
				else
				arm_write_cpsr(p,clr_bit(arm_read_cpsr(p),5));
			}
			else
      arm_write_register(p,dest,value);
    }
        }
        else{
          arm_read_half(p,address,&value_half);
          arm_write_register(p,dest,value_half);

        }


	} else {
	//STORE
		address = arm_read_register(p, source);
		success = store(p, ins, dest, source, address);
	}
    return success;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
  uint32_t address;
  uint32_t start_address;
  uint32_t end_address;
  uint32_t value;
  uint8_t base;
  uint8_t count=0;
  uint8_t i;

        for(i=0;i<16;i++){
          if((ins>>i)&1)
          count++;
				}

        base=(uint8_t)((ins>>16)&0XF);

        if((ins>>24)&1){ //P=1
          if((ins>>23)&1){
            start_address=arm_read_register(p,base)+4;
            end_address=arm_read_register(p,base)+count*4;

            if((ins>>21)&1)  //W=1
            arm_write_register(p,base,end_address);

          }else{
            end_address=arm_read_register(p,base)-4;
            start_address=arm_read_register(p,base)-count*4;

            if((ins>>21)&1)  //W=1
            arm_write_register(p,base,start_address);

          }
        }else{ //P=0
          if((ins>>23)&1){
            start_address=arm_read_register(p,base);
            end_address=arm_read_register(p,base)+(count-1)*4;

            if((ins>>21)&1)  //W=1
            arm_write_register(p,base,end_address+4);

          }else{
            end_address=arm_read_register(p,base);
            start_address=arm_read_register(p,base)-(count-1)*4;

            if((ins>>21)&1)  //W=1
            arm_write_register(p,base,start_address-4);

          }
        }
        address=start_address;

				if((ins>>20)&1){   //LDM(1)

				for(i=0;i<15;i++){
          if((ins>>i)&1){
            arm_read_word(p,address,&value);
            arm_write_register(p,i,value);
            address+=4;
          }
        }

        if((ins>>i)&1){ //chargement de PC
            arm_read_word(p,address,&value);
            arm_write_register(p,15,value & 0xFFFFFFFE);
						if(value%2)
						arm_write_cpsr(p,set_bit(arm_read_cpsr(p),5));
						else
						arm_write_cpsr(p,clr_bit(arm_read_cpsr(p),5));

        }
			}
			else{
				for(i=0;i<15;i++){   //STM(1)
          if((ins>>i)&1){

            value=arm_read_register(p,i);
						arm_write_word(p,address,value);
            address+=4;
          }
        }

			}

    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
