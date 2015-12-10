#include <stdio.h>
#include <string.h>

#include "emu2A03.h"
#include "MemoryMap.h"

MemoryMap::MemoryMap(emu2A03* cpu) {
	this->cpu = cpu;

	// RAM
	ZERO_PAGE = new byte[0x100];
	STACK = new byte[0x100];
	RAM = new byte[0x600];
	RAM_MIRROR = new byte[0x800 * 0x3]; // RAM is mirrored 3 times.
	// IO registers
	IO_PPU = new byte[0x8];
	IO_PPU_MIRROR = new byte[0x8 * 0x3FF]; // mirrors every 8 bytes 1023 times (total of 1024 copies)
	IO_CONTROLLER = new byte[0x20];
	// Expansion ROM
	EXPANSION_ROM = new byte[0x1FE0];
	// Save RAM
	SRAM = new byte[0x2000];
	// Program ROM
	PRG_ROM_BANK1 = new byte[0x4000];
	PRG_ROM_BANK2 = new byte[0x4000];

	memset(ZERO_PAGE, 0x00, 0x100);
	memset(STACK, 0x00, 0x100);
	memset(RAM, 0x00, 0x600);
	memset(RAM_MIRROR, 0x00, 0x800 * 3);
	memset(IO_PPU, 0x00, 0x8);
	memset(IO_PPU_MIRROR, 0x00, 0x8 * 1023);
	memset(IO_CONTROLLER, 0x00, 0x20);
	memset(EXPANSION_ROM, 0x00, 0x1FE0);
	memset(SRAM, 0x00, 0x2000);
	memset(PRG_ROM_BANK1, 0x00, 0x4000);
	memset(PRG_ROM_BANK2, 0x00, 0x4000);
}

void MemoryMap::load_program(byte* program) {
	// Copy the contents of the loaded program's PRG ROM into memory
}

uint16_t MemoryMap::swap_endian(address addr) {
	return ((addr & 0xFF) << 8) | ((addr & 0xFF00) >> 8);
}

byte MemoryMap::read8(address addr) {
	if(addr < 0x100) {
		return ZERO_PAGE[addr];
	}
	else if(addr < 0x200) {
		return STACK[addr - 0x100];
	}
	else if(addr < 0x800) {
		return RAM[addr - 0x200];
	}
	else if(addr < 0x2000) {
		return RAM_MIRROR[addr - 0x800];
	}
	else if(addr < 0x2008) {
		return read_from_io_ppu(addr);
	}
	else if(addr < 0x4000) {
		return IO_PPU_MIRROR[addr - 0x2008];
	}
	else if(addr < 0x4020) {
		return read_from_io_controller(addr);
	}
	else if(addr < 0x6000) {
		return EXPANSION_ROM[addr - 0x4020];
	}
	else if(addr < 0x8000) {
		return SRAM[addr - 0x6000];
	}
	else if(addr < 0xC000) {
		return PRG_ROM_BANK1[addr - 0x8000];
	}
	else if(addr < 0x10000) {
		return PRG_ROM_BANK2[addr - 0xC000];
	}
	else {
		printf("ERROR: Attempting to read from out-of-range address %d.", addr);
		cpu->end();
		return 0x00;
	}
}

uint16_t MemoryMap::read16(address addr) {
	return (read8(addr) << 8) | (read8(addr + 1));
}

address MemoryMap::readAddrLE(address addr) { // reads an address in little endian order
	return swap_endian(read16(addr));
}

void MemoryMap::write(address addr, byte data) {
	if(addr < 0x100) {
		ZERO_PAGE[addr] = data;
		for(int i = 0x0; i < 0x2000; i += 0x800)
			RAM_MIRROR[addr + i] = data;
	}
	else if(addr < 0x200) {
		STACK[addr - 0x100] = data;
		for(int i = 0x0; i < 0x2000; i += 0x800)
			RAM_MIRROR[addr + i] = data;
	}
	else if(addr < 0x800) {
		RAM[addr - 0x200] = data;
		for(int i = 0x0; i < 0x2000; i += 0x800)
			RAM_MIRROR[addr + i] = data;
	}
	else if(addr >= 0x2000 && addr < 0x2008) {
		write_to_io_ppu(addr, data);
	}
	else if(addr < 0x4020) {
		write_to_io_controller(addr, data);
	}
	else if(addr < 0x6000) {
		EXPANSION_ROM[addr - 0x4020] = data;
	}
	else if(addr < 0x8000) {
		SRAM[addr - 0x6000] = data;
	}
	else if(addr < 0xC000) {
		PRG_ROM_BANK1[addr - 0x8000] = data;
	}
	else if(addr < 0x10000) {
		PRG_ROM_BANK2[addr - 0xC000] = data;
	}
	else {
		printf("ERROR: Attempting to write to out-of-range address %d.", addr);
		cpu->end();
	}
}

byte MemoryMap::read_from_io_ppu(address addr) {
	// TODO: implement IO callbacks
	return IO_PPU[addr - 0x2000];
}

byte MemoryMap::read_from_io_controller(address addr) {
	// TODO: implement IO callbacks
	return IO_CONTROLLER[addr - 0x4000];
}

void MemoryMap::write_to_io_ppu(address addr, byte data) {
	// TODO: implement IO callbacks
	addr -= 0x2000;
	IO_PPU[addr] = data;

	for(int i = 0x0; i < 0x400; i += 0x8)
		IO_PPU_MIRROR[addr + i] = data;
}

void MemoryMap::write_to_io_controller(address addr, byte data) {
	// TODO: implement IO callbacks
	IO_CONTROLLER[addr - 0x4000] = data;
}
