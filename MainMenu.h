#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "PlaySubMenu.h"
#include "Options.h"
#include "GlobalVariables.h"

using namespace std;
using namespace sf;

class MainMenu : public Menu {
public:
    MainMenu() {
        Text playText("Play", font, 50);
        textRect = playText.getLocalBounds();
        playText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        playText.setPosition(windowSize / 2.0f, windowSize / 2.0f - 50);

        Text exitText("Exit", font, 50);
        textRect = exitText.getLocalBounds();
        exitText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        exitText.setPosition(windowSize / 2.0f, windowSize / 2.0f + 50);

        elements.push_back(playText);
        elements.push_back(exitText);

        normalColor = Color::White;
        hoverColor = Color::Yellow;

        selectedElement = playMenu;
        selectElement();
    }

    bool isSubMenu = true;
    bool flag = true;

protected:
    enum ElementMenu { playMenu, Exit };
    ElementMenu selectedElement;

    void selectElement() {
        elements[0].setFillColor(normalColor);
        elements[1].setFillColor(normalColor);

        switch (selectedElement) {
        case playMenu:
            elements[0].setFillColor(hoverColor);
            break;
        case Exit:
            elements[1].setFillColor(hoverColor);
            break;
        }
    }

    void keyEvent(RenderWindow& window) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedElement > playMenu) {
                selectedElement = static_cast<ElementMenu>(selectedElement - 1);
                selectElement();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (selectedElement < Exit) {
                selectedElement = static_cast<ElementMenu>(selectedElement + 1);
                selectElement();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            PlaySubMenu playSM;
            Texture backgroundTexture;
            backgroundTexture.loadFromFile("./resources/tlo.png");
            Sprite background(backgroundTexture);
            switch (selectedElement) {
            case playMenu:
                while (window.isOpen() && playSM.flag)
                {
                    window.clear();

                    window.draw(background);

                    playSM.draw(window);
                    playSM.eventHandler(window);

                    window.display();
                }
                break;
            case Exit:
                window.close();
                break;
            }
        }
    }
};