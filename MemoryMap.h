#pragma once

#include "types.h"
struct emu6502;

#define MEMORY_SIZE			0x10000
#define NUMBER_OF_SECTIONS	0xB

struct MemoryMap {
	emu6502* cpu;

	// RAM
	byte* ZERO_PAGE;		// 0x0000 - 0x00FF
	byte* STACK;			// 0x0100 - 0x01FF
	byte* RAM;				// 0x0200 - 0x07FF
	byte* RAM_MIRROR;		// 0x0800 - 0x1FFF
	// IO registers
	byte* IO_PPU;			// 0x2000 - 0x2007
	byte* IO_PPU_MIRROR;	// 0x2008 - 0x3FFF
	byte* IO_CONTROLLER;	// 0x4000 - 0x401F
	// Expansion ROM
	byte* EXPANSION_ROM;	// 0x4020 - 0x5FFF
	// Save RAM
	byte* SRAM;				// 0x6000 - 0x7FFF
	// Program ROM
	byte* PRG_ROM_BANK1;	// 0x8000 - 0xBFFF
	byte* PRG_ROM_BANK2;	// 0xC000 - 0xFFFF


	MemoryMap(emu6502*);
	void load_program(void*);

	byte read(address);
	byte read_from_io_ppu(address);
	byte read_from_io_controller(address);

	void write(address, byte);
	void write_to_io_ppu(address, byte);
	void write_to_io_controller(address, byte);
};
