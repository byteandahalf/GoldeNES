#include <stdio.h>
#include <string.h>

#include "emu6502.h"


// RAM

int emu6502::RAM::SIZE = 0x10000;
short emu6502::RAM::MAX_ADDRESS = 0xFFFF;
byte emu6502::RAM::ZERO_PAGE_LEN = 0x100; // Zero Page is 0x00 - 0xFF

emu6502::RAM::RAM() {
	memset(MEM, 0x00, 0x2000);
	memset(MEMORY_MAPPED_IO, 0x00, 0x2020);
	memset(MIRROR, 0x00, 0x3FE0);
	memset(PRG_ROM_BANK_1, 0x00, 0x4000);
	memset(PRG_ROM_BANK_2, 0x00, 0x4000);
}

void emu6502::RAM::init(void* program) {
	// Copy the contents of the loaded program's PRG ROM into RAM
}


// CPU

emu6502* emu6502::CPU = nullptr;

emu6502::emu6502(void* program) {
	ram = std::unique_ptr<emu6502::RAM>(new emu6502::RAM());
	ram->init(program);

	reset();
}

emu6502::~emu6502() {
	ram.release();
}

void emu6502::reset() {
	reg_PC = read(0xFFFC); // 0xFFFC holds the reset address
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
	Opcode opcode = read(reg_PC);
	byte memory;
	Mode mode = get_mode(opcode);
	byte temp; // extra memory used for calculations only

	switch(mode) {
		// switch out the addressing mode so that we know what to use as operands to instructions
	default:
		memory = read(reg_PC + 1); // temporary
	}

	switch(opcode) {
		// decode instruction and execute accordingly
	default:
		printf("ERROR: Unknown operand at address %d\n", reg_PC);
		end();
	}
}

byte emu6502::read(address addr) {
	if(addr < 0x2000)
		return ram->MEM[addr];
	if(addr < 0x4020)
		return ram->MEMORY_MAPPED_IO[addr];
	if(addr < 0x8000)
		return ram->MIRROR[addr];
	if(addr < 0xC000)
		return ram->PRG_ROM_BANK_1[addr];
	if(addr < 0x10000)
		return ram->PRG_ROM_BANK_2[addr];

	printf("ERROR: Read attempt at address %d out of range\n", addr);
	end();

	return 0x00;
}

void emu6502::write(address addr, byte data) {
	if(addr < 0x2000)
		ram->MEM[addr] = data;
	else if(addr < 0x4020)
		ram->MEMORY_MAPPED_IO[addr] = data;
	else if(addr < 0x8000)
		ram->MIRROR[addr] = data;
	else if(addr < 0xC000)
		ram->PRG_ROM_BANK_1[addr] = data;
	else if(addr < 0x10000)
		ram->PRG_ROM_BANK_2[addr] = data;
	else {
		printf("ERROR: Write attempt at address %d out of range\n", addr);
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
