#pragma once
#include <memory>

#include "opcodes.h"
#include "types.h"
struct MemoryMap;

struct Processor {
	std::unique_ptr<MemoryMap> memory_map;

	reg reg_A, reg_X, reg_Y;
	address reg_SP, reg_PC;
	status_register reg_ST;

	bool close_requested;

	Processor(byte*);
	~Processor();
	void close();

	void execute();
	Mode get_mode(Opcode);

	void reset();
	void end();
};
