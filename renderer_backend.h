#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "types.h"
#include "colors.h"

class Renderer {
public:
    static const int WIDTH;
    static const int HEIGHT;
    static const int SCALE;
    static const std::string MAIN_WINDOW_LABEL;

    Renderer();
    ~Renderer();

    sf::RenderWindow* getMainWindow();

    inline Color* getPixels();
    inline void setPixel(int, int, const Color&);
    inline const Color& getPixel(int, int);

    const int screenSize();

    void tickMoveWindow();
    void tick();
    void draw();
    void close();

private:
    long ticks;
    std::unique_ptr<sf::RenderWindow> main_window;
    sf::Texture canvas;
    sf::Sprite screen;
    Color* pixels;
};
