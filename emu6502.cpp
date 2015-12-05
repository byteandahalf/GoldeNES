#include <stdio.h>

#include "emu6502.h"


// RAM

emu6502::RAM::RAM() {
	memset(MEM, 0x00, 0x2000);
	memset(MEMORY_MAPPED_IO, 0x00, 0x2020);
	memset(MIRROR, 0x00, 0x3FE0);
	memset(PRG_ROM_BANK_1, 0x00, 0x4000);
	memset(PRG_ROM_BANK_2, 0x00, 0x4000);
}


emu6502::RAM::~RAM() {
	delete[] MEM;
	delete[] MEMORY_MAPPED_IO;
	delete[] MIRROR;
	delete[] PRG_ROM_BANK_1;
	delete[] PRG_ROM_BANK_2;
}

void emu6502::RAM::init(void* program) {
	// Copy the contents of the loaded program's PRG ROM into RAM
}


// CPU

emu6502* emu6502::CPU = nullptr;

emu6502::emu6502(void* program) {
	ram = std::unique_ptr<emu6502::RAM>(new emu6502::RAM());
	ram->init(program);

	reg_PC = 0x0000; // TODO: Load the reset address from 0xFFFC
	reg_A = reg_X = reg_Y = 0x00;
	reg_SP = 0xFF;
	reg_ST = 0x00;

	is_done = false;
}

emu6502::~emu6502() {
	delete RAM;
}

void emu6502::end() {
	is_done = true;
}

void emu6502::execute() {
	// execute next instruction

	++reg_PC; // this is probably wrong, temporary for now
	Instruction instr = read(reg_PC);
	byte memory;
	Mode mode = get_mode(instr);
	byte temp; // extra memory used for calculations only

	switch(mode) {
		// switch out the addressing mode so that we know what to use as operands to instructions
	default:
		memory = read(reg_PC + 1); // temporary
	}

	switch(instr) {
		// decode instruction and execute accordingly
	default:
		printf("ERROR: Unknown operand at address %d", reg_PC);
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

	printf("ERROR: Read attempt at address %d out of range", addr);
	end();

	return 0x00;
}

void emu6502::write(address addr, byte data) {
	if(addr < 0x2000)
		ram->MEM[addr] = data;
	if(addr < 0x4020)
		ram->MEMORY_MAPPED_IO[addr] = data;
	if(addr < 0x8000)
		ram->MIRROR[addr] = data;
	if(addr < 0xC000)
		ram->PRG_ROM_BANK_1[addr] = data;
	if(addr < 0x10000)
		ram->PRG_ROM_BANK_2[addr] = data

	printf("ERROR: Write attempt at address %d out of range", addr);
	end();
}

Mode emu6502::get_mode(Instruction instr) {
	// Get the addressing mode for the given instruction
	switch(instr) {
	default:
		return Mode::TEMP;
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