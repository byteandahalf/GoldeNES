#include <stdio.h>

#include "emu6502.h"
#include "MemoryMap.h"


// CPU

emu6502* emu6502::CPU = nullptr;

emu6502::emu6502(void* program) {
	memory_map = std::unique_ptr<MemoryMap>(new MemoryMap(this));
	memory_map->load_program(program);

	reset();
}

emu6502::~emu6502() {
	memory_map.release();
}

void emu6502::reset() {
	reg_PC = memory_map->read(0xFFFC); // 0xFFFC holds the reset address
	reg_A = reg_X = reg_Y = 0x00;
	reg_SP = 0xFF;
	reg_ST = 0x00;

	is_done = false;
}

void emu6502::end() {
	is_done = true;
}

void emu6502::execute() {
	// execute next instruction

	++reg_PC; // this is probably wrong, temporary for now
	Opcode opcode = memory_map->read(reg_PC);
	byte memory;
	Mode mode = get_mode(opcode);
	byte temp; // extra memory used for calculations only

	// switch out the addressing mode so that we know what to use as operands to instructions
	switch(mode) {
	default:
		memory = memory_map->read(reg_PC + 1); // temporary
	}

	// decode instruction and execute accordingly
	switch(opcode) {
	default:
		printf("ERROR: Unknown operand at address %d\n", reg_PC);
		end();
	}
}

Mode emu6502::get_mode(Opcode instr) {
	// Get the addressing mode for the given instruction
	switch(instr) {
	default:
		return MODE_TEMP;
	}
}


// main loop

int main() {
	emu6502::CPU = new emu6502(nullptr);
	printf("%s \n", "Welcome to Byteandahalf's horrible 6502 emulator!");

	while(true) {
		if(emu6502::CPU->is_done)
			break;

		emu6502::CPU->execute();
	}

	printf("%s \n", "The program seems to have crashed!");
}
