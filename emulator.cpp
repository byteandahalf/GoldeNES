#include "cpu.h"
#include "renderer_backend.h"

#include "emulator.h"

Processor* Emulator::CPU = nullptr;
Renderer* Emulator::renderer;

// main loop
int main() {
	Emulator::CPU = new Processor(nullptr);
	Emulator::renderer = new Renderer();

	printf("%s \n", "Booting up...");

	while(Emulator::renderer->getMainWindow()->isOpen() && !Emulator::CPU->close_requested)
	{
		Emulator::CPU->execute();
		Emulator::renderer->tick();
	}

	Emulator::CPU->close();
	Emulator::renderer->close();

	printf("%s \n", "Ending the process...");
	return 0;
}
