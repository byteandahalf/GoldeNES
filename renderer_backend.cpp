#include <cstring>

#include "renderer_backend.h"

// Colors
Color Color::RED{0xFF, 0x00, 0x00, 0xFF};
Color Color::ORANGE{0xFF, 0xA5, 0x00, 0xFF};
Color Color::YELLOW{0xFF, 0xFF, 0x00, 0xFF};
Color Color::GREEN{0x00, 0xFF, 0x00, 0xFF};
Color Color::BLUE{0x00, 0x00, 0xFF, 0xFF};
Color Color::PURPLE{0x80, 0x00, 0x80, 0xFF};
Color Color::BLACK{0xFF, 0xFF, 0xFF, 0xFF};
Color Color::WHITE{0x00, 0x00, 0x00, 0xFF};


const int Renderer::WIDTH = 160;
const int Renderer::HEIGHT = 120;
const int Renderer::SCALE = 4;
const std::string Renderer::MAIN_WINDOW_LABEL = "GoldeNES";

Renderer::Renderer():
    ticks(0),
    main_window(new sf::RenderWindow(sf::VideoMode(WIDTH* SCALE, HEIGHT * SCALE), Renderer::MAIN_WINDOW_LABEL)) {

    pixels = new Color[(WIDTH * SCALE) * (HEIGHT * SCALE)];

    for(int x = 0; x < 640; x++) {
        for(int y = 0; y < 480; y++)
            setPixel(x, y, Color::BLUE);
    }

    canvas.create(WIDTH * SCALE, HEIGHT * SCALE);
    screen.setTexture(canvas);
}

Renderer::~Renderer() {
    delete pixels;
    delete main_window.release();
}

sf::RenderWindow* Renderer::getMainWindow() {
    return main_window.get();
}

Color* Renderer::getPixels() {
    return pixels;
}

void Renderer::setPixel(int x, int y, const Color& color) {
    pixels[x + (y * main_window->getSize().x)] = color;
}

const Color& Renderer::getPixel(int x, int y) {
    return pixels[x + (y * main_window->getSize().x)];
}

const int Renderer::screenSize() {
    return main_window->getSize().x * main_window->getSize().y;
}

void Renderer::draw() {
    canvas.update((sf::Uint8*) pixels);
    main_window->draw(screen);
}

void Renderer::tickMoveWindow() {
    // have we attempted to drag or close this window?
    sf::Event windowEvent;
    while(main_window->pollEvent(windowEvent))
    {
        if(windowEvent.type == sf::Event::Closed)
            main_window->close();
    }
}

void Renderer::tick() {
    tickMoveWindow();

    main_window->clear();
    draw();
    main_window->display();

    ++ticks;
}

void Renderer::close() {
    main_window->close();
    delete this;
}
