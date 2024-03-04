#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Play.h"
#include "PlaySubMenu.h"
#include "Options.h"
#include "Menu.h"

using namespace std;
using namespace sf;

class GameMenu : public Menu {
public:
    GameMenu() {
        Text continueText("Continue", font, 50);;
        textRect = continueText.getLocalBounds();
        continueText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        continueText.setPosition(windowSize / 2.0f, windowSize / 2.0f - 100);

        Text saveText("Save", font, 50);;
        textRect = saveText.getLocalBounds();
        saveText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        saveText.setPosition(windowSize / 2.0f, windowSize / 2.0f);

        Text exitText("Exit", font, 50);;
        textRect = exitText.getLocalBounds();
        exitText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        exitText.setPosition(windowSize / 2.0f, windowSize / 2.0f + 100);

        elements.push_back(continueText);
        elements.push_back(saveText);
        elements.push_back(exitText);

        normalColor = Color::White;
        hoverColor = Color::Green;

        selectedElement = Continue;
        selectElement();
    }

    bool flag = true;
    bool saveFlag = false;

protected:
    enum ElementMenu { Continue, Save, Exit};
    ElementMenu selectedElement;

    void selectElement() {
        elements[0].setFillColor(normalColor);
        elements[1].setFillColor(normalColor);
        elements[2].setFillColor(normalColor);

        switch (selectedElement) {
        case Continue:
            elements[0].setFillColor(hoverColor);
            break;
        case Save:
            elements[1].setFillColor(hoverColor);
            break;
        case Exit:
            elements[2].setFillColor(hoverColor);
            break;
        }
    }

    void keyEvent(RenderWindow& window) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedElement > Continue) {
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

            switch (selectedElement) {
            case Continue:
                flag = false;
                return;
                break;
            case Save:
                flag = false;
                saveFlag = true;
                return;
                break;
            case Exit:
                window.close();
                break;
            }
        }
    }
};
