#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "Menu.h"
#include "Play.h"
#include "CreateMenu.h"
#include "GlobalVariables.h"

using namespace std;
using namespace sf;

class PlaySubMenu : public Menu {
public:
    PlaySubMenu() {
        Text loadWorldText("Load", font, 50);
        textRect = loadWorldText.getLocalBounds();
        loadWorldText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        loadWorldText.setPosition(windowSize / 2.0f, windowSize / 2.0f - 100);

        Text newWroldText("New World", font, 50);
        textRect = newWroldText.getLocalBounds();
        newWroldText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        newWroldText.setPosition(windowSize / 2.0f, windowSize / 2.0f);

        Text backText("Back", font, 50);
        textRect = backText.getLocalBounds();
        backText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        backText.setPosition(windowSize / 2.0f, windowSize / 2.0f + 100);

        elements.push_back(loadWorldText);
        elements.push_back(newWroldText);
        elements.push_back(backText);

        selectedElement = Load;
        selectElement();
    }

    bool flag = true;

protected:
    enum ElementMenu { Load, NewWorld, Back };
    ElementMenu selectedElement;

    void selectElement() {
        elements[0].setFillColor(normalColor);
        elements[1].setFillColor(normalColor);
        elements[2].setFillColor(normalColor);

        switch (selectedElement) {
        case Load:
            elements[0].setFillColor(hoverColor);
            break;
        case NewWorld:
            elements[1].setFillColor(hoverColor);
            break;
        case Back:
            elements[2].setFillColor(hoverColor);
            break;
        }
    }

    void keyEvent(RenderWindow& window) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedElement > Load) {
                selectedElement = static_cast<ElementMenu>(selectedElement - 1);
                selectElement();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (selectedElement < Back) {
                selectedElement = static_cast<ElementMenu>(selectedElement + 1);
                selectElement();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            Play play;
            CreateMenu createMenu;
            Texture backgroundTexture;
            backgroundTexture.loadFromFile("./resources/tlo1.png");
            Sprite background(backgroundTexture);
            switch (selectedElement) {
            case Load:
                play.isLoaded = true;
                play.run(window);
                break;
            case NewWorld:
                play.isLoaded = false;
                worldSettings[3] = rng(0, 1000);
                while (window.isOpen() && createMenu.flag)
                {
                    window.clear();

                    window.draw(background);

                    createMenu.draw(window);
                    createMenu.eventHandler(window);

                    window.display();
                }
                break;
            }
            flag = false;
        }
    }

    int rng(int odkad, int dokad)
    {
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<mt19937::result_type> dist6(odkad, dokad);
        return dist6(rng);
    }
};