#pragma once
#include <memory>
#include <stdint.h>

#include "opcodes.h"

typedef int8_t byte;
typedef int16_t address; // The 6502 has a 16-bit address bus, so addresses are only 2 bytes
typedef byte reg;

struct emu6502 {
	static emu6502* CPU;

	struct RAM {
		static int SIZE;
		static short MAX_ADDRESS;
		static byte ZERO_PAGE_LEN;

		byte MEM[0x2000]; // 0x00 - 0x1FFF
		byte MEMORY_MAPPED_IO[0x2020]; // 0x2000 - 0x401F
		byte MIRROR[0x3FE0]; // 0x4020 - 0x7FFF;			Still don't know what this section is used for.
		// Games with 1 ROM bank will copy it into both of these sections,
		// games with 2 ROM banks will store one in each section here.
		byte PRG_ROM_BANK_1[0x4000]; // 0x8000 - 0xBFFF
		byte PRG_ROM_BANK_2[0x4000]; // 0xC000 - 0xFFFF

		RAM();

		void init(void*);
	};

	std::unique_ptr<emu6502::RAM> ram;

	reg reg_A, reg_X, reg_Y, reg_SP, reg_ST;
	address reg_PC;

	bool is_done; // end the program in case of exception

	emu6502(void*);
	~emu6502();

	void execute();
	byte read(address);
	void write(address, byte);
	Mode get_mode(Opcode);

	void end();
};
