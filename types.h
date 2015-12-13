#pragma once

#include <cstdint>

typedef int8_t byte;
typedef int16_t address; // The 6502 has a 16-bit address bus, so addresses are only 2 bytes
typedef byte reg;

struct pixel {
    byte r, g, b, a;
};

// status flags
#define STATUS_CARRY 0
#define STATUS_ZERO 1
#define STATUS_INTERRUPT 2
#define STATUS_BCD 3 // not used in the NES
#define STATUS_BRK 4 // Software interrupt (BRK instruction)
#define STATUS_UNUSED 5 // Unused flag that is always 1
#define STATUS_OVERFLOW 6
#define STATUS_SIGN 7

struct status_register {
	byte status;

	status_register() {
		status = 0x00 + (1 << STATUS_UNUSED);
	}

	void setbit(byte bit, bool value) {
		value? status |= (1 << bit) : status &= ~(1 << bit);
	}

	void set_carry(bool carry) {
		setbit(STATUS_CARRY, carry);
	}
	bool carry() {
		return status & STATUS_CARRY;
	}

	void set_zero(bool zero) {
		setbit(STATUS_ZERO, zero);
	}
	bool zero() {
		return status & STATUS_ZERO;
	}

	void set_interrupt(bool interrupt) {
		setbit(STATUS_INTERRUPT, interrupt);
	}
	bool interrupt() {
		return status & STATUS_INTERRUPT;
	}

	void set_brk(bool brk) {
		setbit(STATUS_BRK, brk);
	}
	bool brk() {
		return status & STATUS_BRK;
	}

	void set_overflow(bool overflow) {
		setbit(STATUS_OVERFLOW, overflow);
	}
	bool overflow() {
		return status & STATUS_OVERFLOW;
	}

	void set_sign(bool sign) {
		setbit(STATUS_SIGN, sign);
	}
	bool sign() {
		return status & STATUS_SIGN;
	}

	operator byte() {
		return status;
	}
};
