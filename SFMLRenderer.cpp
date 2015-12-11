#include <cstring>

#include "SFMLRenderer.h"


// SFMLRenderer::Window
SFMLRenderer::Window::Window(int2 size, const std::string& label):
    _realWindow(sf::VideoMode(size.x, size.y), label),
    _label(label) {
    pixels = new byte[size.x * size.y * 4];

    memset(pixels, 0x00, size.x * size.y * 4);
}

void SFMLRenderer::Window::render() {
    _realWindow.clear();

    sf::CircleShape shape(200.0F);
    shape.setFillColor(sf::Color::Green);
    _realWindow.draw(shape);
    _realWindow.display();
}

// SFMLRenderer
SFMLRenderer* SFMLRenderer::gRenderer = nullptr;

SFMLRenderer::SFMLRenderer() {
    pushWindow({400, 400}, "emu2A03"); // main window
}

SFMLRenderer::~SFMLRenderer() {
    printf("%d\n", numberOfWindows());
    for(int i = 0; i < numberOfWindows(); i++)
        delete popWindow();

    printf("%d\n", numberOfWindows());
}

SFMLRenderer::Window* SFMLRenderer::pushWindow(int2 size, const std::string& label) {
    windows.push_back(new SFMLRenderer::Window(size, label));
    return windows.back();
}

SFMLRenderer::Window* SFMLRenderer::popWindow() {
    if(!windows.empty())
    {
        SFMLRenderer::Window* win = windows.back();
        windows.pop_back();
        return win;
    }
}

const int SFMLRenderer::numberOfWindows() {
    return windows.size();
}

SFMLRenderer::Window* SFMLRenderer::getWindow(int index) {
    return windows[index];
}

void SFMLRenderer::tick() {
    for(SFMLRenderer::Window* window : windows)
        window->render();
}
