#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GlobalVariables.h";

using namespace std;
using namespace sf;

class Menu {
public:
    Menu() {
        font.loadFromFile("arial.ttf");
    }

    void draw(RenderWindow& window) {
        for (auto& element : elements)
        {
            window.draw(element);
        }
    }

    void eventHandler(RenderWindow& window) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::KeyPressed) {
                keyEvent(window);
            }
        }
    }

protected:
    Font font;
    FloatRect textRect;
    vector<Text> elements;
    Color normalColor = Color::White;
    Color hoverColor = Color::Red;
    View view;

    virtual void selectElement() = 0;
    virtual void keyEvent(RenderWindow& window) = 0;
};
