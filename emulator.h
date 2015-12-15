#pragma once
class Processor;
class Renderer;

struct Emulator {
    static Processor* CPU;
    static Renderer* renderer;
};
