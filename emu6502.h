#pragma once
#include <memory>

#include "opcodes.h"
#include "types.h"
struct MemoryMap;

struct emu6502 {
	static emu6502* CPU;

	std::unique_ptr<MemoryMap> memory_map;

	reg reg_A, reg_X, reg_Y, reg_SP, reg_ST;
	address reg_PC;

	bool is_done; // end the program in case of exception

	emu6502(void*);
	~emu6502();

	void execute();
	Mode get_mode(Opcode);

	void reset();
	void end();
};
