#pragma once

#include <string>
#include <vector>

#include "types.h"

#include <SFML/Graphics.hpp>

class SFMLRenderer {
public:
    static SFMLRenderer* gRenderer;

    struct Window {
        const std::string& _label;
        sf::RenderWindow _realWindow;
        byte* pixels;

        Window(int2, const std::string&);

        void render();
    };

private:
    std::vector<Window*> windows;

public:
    Window* pushWindow(int2, const std::string&);
    void popWindow(); // pops last window from windows
    Window* getWindow(int);

    SFMLRenderer();

    void tick();
};
