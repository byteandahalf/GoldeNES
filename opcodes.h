#pragma once

typedef uint8_t Opcode;
typedef uint8_t Mode;
#define MODE_ZEROPAGE 0x00
#define MODE_ZEROPAGE_X 0x01
#define MODE_ZEROPAGE_Y 0x02
#define MODE_ABSOLUTE 0x03
#define MODE_ABSOLUTE_X 0x04
#define MODE_ABSOLUTE_Y 0x05
#define MODE_INDIRECT 0x06
#define MODE_IMPLIED 0x07
#define MODE_ACC 0x08
#define MODE_IMMEDIATE 0x09
#define MODE_RELATIVE 0x0A
#define MODE_PREINDEXED 0x0B
#define MODE_POSTINDEXED 0x0C