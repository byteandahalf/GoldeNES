# twoaohthree

An emulator for the 2A03 processor(the NES variant of the 6502) from 1980, written in C++.
I am making this project solely for the purpose of learning how the 6502 processor works, as
well as emulators in general. This will probably never be perfect since I am not very smart.
This project has taken some inspiration from [tkieft's nes emulator](https://github.com/tkieft/nes-emulator),
but I have not copy/pasted any of the code, I simply refer to his code as an already working
example to compare to my code. Do whatever you want with this code, I don't really care.

## Building

`g++ -std=c++11 -o emu2A03.exe emu2A03.cpp MemoryMap.cpp`