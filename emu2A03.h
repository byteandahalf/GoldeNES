#pragma once
#include <memory>

#include "opcodes.h"
#include "types.h"
struct MemoryMap;

struct emu2A03 {
	static emu2A03* CPU;

	std::unique_ptr<MemoryMap> memory_map;

	reg reg_A, reg_X, reg_Y, reg_ST;
	address reg_SP, reg_PC;

	bool is_done; // end the program in case of exception

	emu2A03(void*);
	~emu2A03();

	void execute();
	Mode get_mode(Opcode);

	void reset();
	void end();
};
