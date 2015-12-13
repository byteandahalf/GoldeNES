#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "types.h"

class renderer {
public:
    static renderer* backend;
    static const std::string MAIN_WINDOW_LABEL;

    renderer(int, int);
    ~renderer();

    void setInitialWindow();

    sf::RenderWindow* getMainWindow();

    inline sf::Color* getPixels();
    inline void setPixel(int, int, const sf::Color&);
    inline const sf::Color& getPixel(int, int);

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
    sf::Color* pixels;
};
