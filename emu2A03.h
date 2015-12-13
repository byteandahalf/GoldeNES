#pragma once
#include <memory>

#include "opcodes.h"
#include "types.h"
struct MemoryMap;

struct emu2A03 {
	static emu2A03* CPU;

	std::unique_ptr<MemoryMap> memory_map;

	reg reg_A, reg_X, reg_Y;
	address reg_SP, reg_PC;
	status_register reg_ST;

	bool close_requested;

	emu2A03(byte*);
	~emu2A03();
	void close();

	void execute();
	Mode get_mode(Opcode);

	void reset();
	void end();
};
