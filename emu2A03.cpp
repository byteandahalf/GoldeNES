#include <stdio.h>

#include "emu2A03.h"
#include "MemoryMap.h"
#include "SFMLRenderer.h"


// CPU

emu2A03* emu2A03::CPU = nullptr;

emu2A03::emu2A03(byte* program) {
	memory_map = std::unique_ptr<MemoryMap>(new MemoryMap(this));
	memory_map->load_program(program);

	reset();
}

emu2A03::~emu2A03() {
	delete memory_map.release();
}

void emu2A03::reset() {
	reg_PC = memory_map->readAddrLE(0xFFFC); // 0xFFFC holds the reset address
	reg_A = reg_X = reg_Y = 0x00;
	reg_SP = 0x1FF; // set to the top of the stack
	reg_ST = status_register();

	is_done = false;
}

void emu2A03::end() {
	is_done = true;
}

void emu2A03::execute() {
	// execute next instruction

	Opcode opcode = memory_map->read8(reg_PC);
	Mode mode = get_mode(opcode);
	byte operand;
	address operand_addr;
	uint16_t temp; // extra memory used for calculations only

	// switch out the addressing mode so that we know what to use as operands to instructions
	switch(mode) {
	case MODE_ZEROPAGE:
		operand_addr = memory_map->read8(reg_PC + 1);
		reg_PC += 2;
		break;

	case MODE_ZEROPAGE_X:
		operand_addr = memory_map->read8(reg_PC + 1) + reg_X;
		reg_PC += 2;
		break;

	case MODE_ZEROPAGE_Y:
		operand_addr = memory_map->read8(reg_PC + 1) + reg_Y;
		reg_PC += 2;
		break;

	case MODE_ABSOLUTE:
		operand_addr = memory_map->readAddrLE(reg_PC + 1);
		reg_PC += 3;
		break;

	case MODE_ABSOLUTE_X:
		operand_addr = memory_map->readAddrLE(reg_PC + 1) + reg_X;
		reg_PC += 3;
		break;

	case MODE_ABSOLUTE_Y:
		operand_addr = memory_map->readAddrLE(reg_PC + 1) + reg_Y;
		reg_PC += 3;
		break;

	case MODE_INDIRECT:
		operand_addr = memory_map->readAddrLE(memory_map->readAddrLE(reg_PC + 1));
		reg_PC += 3;
		break;

	case MODE_IMPLIED:
		reg_PC += (opcode == 0x00)? 2 : 1;
		break;

	case MODE_ACC:
		operand = reg_A;
		reg_PC++;
		break;

	case MODE_IMMEDIATE:
		operand_addr = reg_PC + 1;
		reg_PC += 2;
		break;

	case MODE_RELATIVE:
		operand_addr = reg_PC + 1;
		reg_PC += 2;
		break;

	case MODE_PREINDEXED:
		operand_addr = memory_map->readAddrLE(memory_map->read8(reg_PC + 1) + reg_X);
		reg_PC += 2;
		break;

	case MODE_POSTINDEXED:
		operand_addr = memory_map->readAddrLE(memory_map->read8(reg_PC + 1)) + reg_Y;
		reg_PC += 2;
		break;

	default:
		printf("ERROR: Unknown addressing mode %d for opcode %d\n", mode, opcode);
		break;
	}

	// decode instruction and execute accordingly
	switch(opcode) {
	default:
		printf("ERROR: Unknown opcode at address %d\n", reg_PC);
		end();
	}
}

Mode emu2A03::get_mode(Opcode instr) {
	// Get the addressing mode for the given instruction
	switch(instr) {
	case 0x69: case 0x29: case 0xC9: case 0xE0: case 0xC0: case 0x49: case 0xA9: case 0xA2: case 0xA0: case 0x09: case 0xE9:
		return MODE_IMMEDIATE;
	case 0x65: case 0x25: case 0x06: case 0x24: case 0xC5: case 0xE4: case 0xC4: case 0xC6: case 0x45: case 0xE6: case 0xA5: case 0xA6: case 0xA4: case 0x46: case 0x05: case 0x26: case 0x66: case 0xE5: case 0x85: case 0x86: case 0x84:
		return MODE_ZEROPAGE;
	case 0x75: case 0x35: case 0x16: case 0xD5: case 0xD6: case 0x55: case 0xF6: case 0xB5: case 0xB4: case 0x56: case 0x15: case 0x36: case 0x76: case 0xF5: case 0x95: case 0x94:
		return MODE_ZEROPAGE_X;
	case 0xB6: case 0x96:
		return MODE_ZEROPAGE_Y;
	case 0x6D: case 0x2D: case 0x0E: case 0x2C: case 0xCD: case 0xEC: case 0xCC: case 0xCE: case 0x4D: case 0xEE: case 0x4C: case 0x20: case 0xAD: case 0xAE: case 0xAC: case 0x4E: case 0x0D: case 0x2E: case 0x6E: case 0xED: case 0x8D: case 0x8E: case 0x8C:
		return MODE_ABSOLUTE;
	case 0x7D: case 0x3D: case 0x1E: case 0xDD: case 0xDE: case 0x5D: case 0xFE: case 0xBD: case 0xBC: case 0x5E: case 0x1D: case 0x3E: case 0x7E: case 0xFD: case 0x9D:
		return MODE_ABSOLUTE_X;
	case 0x79: case 0x39: case 0xD9: case 0x59: case 0xB9: case 0xBE: case 0x19: case 0xF9: case 0x99:
		return MODE_ABSOLUTE_Y;
	case 0x6C:
		return MODE_INDIRECT;
	case 0x61: case 0x21: case 0xC1: case 0x41: case 0xA1: case 0x01: case 0xE1: case 0x81:
		return MODE_PREINDEXED;
	case 0x71: case 0x31: case 0xD1: case 0x51: case 0xB1: case 0x11: case 0xF1: case 0x91:
		return MODE_POSTINDEXED;
	case 0x0A: case 0x4A: case 0x2A: case 0x6A:
		return MODE_ACC;
	case 0x00: case 0x18: case 0x38: case 0x58: case 0x78: case 0xB8: case 0xD8: case 0xF8: case 0xEA: case 0xAA: case 0x8A: case 0xCA: case 0xE8: case 0xA8: case 0x98: case 0x88: case 0xC8: case 0x40: case 0x60: case 0x9A: case 0xBA: case 0x48: case 0x68: case 0x08: case 0x28:
		return MODE_IMPLIED;
	case 0x10: case 0x30: case 0x50: case 0x70: case 0x90: case 0xB0: case 0xD0: case 0xF0:
		return MODE_RELATIVE;
	default:
		return 0x0D;
	}
}


// main loop

int main() {
	emu2A03::CPU = new emu2A03(nullptr);
	SFMLRenderer::gRenderer = new SFMLRenderer();

	printf("%s \n", "Booting up...");

	while(true)
	{
		if(emu2A03::CPU->is_done)
			break;

		emu2A03::CPU->execute();
        SFMLRenderer::gRenderer->tick();
	}

    delete emu2A03::CPU;
    delete SFMLRenderer::gRenderer;
	printf("%s \n", "Ending the process...");
	return 0;
}
