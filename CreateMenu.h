#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Menu.h"
#include "Play.h"
#include "GlobalVariables.h"

using namespace std;
using namespace sf;

class CreateMenu : public Menu
{
public:
    CreateMenu()
    {
        Text biomNormal("Normalny", font, 50);
        textRect = biomNormal.getLocalBounds();
        biomNormal.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        biomNormal.setPosition(windowSize / 2.0f, windowSize / 2.0f - 200);

        Text biomReal("Realistyczny", font, 50);
        textRect = biomReal.getLocalBounds();
        biomReal.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        biomReal.setPosition(windowSize / 2.0f, windowSize / 2.0f - 100);

        Text biomMountain("Gorzysty", font, 50);
        textRect = biomMountain.getLocalBounds();
        biomMountain.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        biomMountain.setPosition(windowSize / 2.0f, windowSize / 2.0f);

        Text biomFlat("Plaski", font, 50);
        textRect = biomFlat.getLocalBounds();
        biomFlat.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        biomFlat.setPosition(windowSize / 2.0f, windowSize / 2.0f + 100);

        Text backText("Back", font, 50);
        textRect = backText.getLocalBounds();
        backText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        backText.setPosition(windowSize / 2.0f, windowSize / 2.0f + 200);

        elements.push_back(biomNormal);
        elements.push_back(biomReal);
        elements.push_back(biomMountain);
        elements.push_back(biomFlat);
        elements.push_back(backText);

        selectedElement = Normal;
        selectElement();
    }

    bool flag = true;

protected:
    enum ElementMenu { Normal, Real, Mountain, Flat, Back };
    ElementMenu selectedElement;

    void selectElement() {
        elements[0].setFillColor(normalColor);
        elements[1].setFillColor(normalColor);
        elements[2].setFillColor(normalColor);
        elements[3].setFillColor(normalColor);
        elements[4].setFillColor(normalColor);

        switch (selectedElement) {
        case Normal:
            elements[0].setFillColor(hoverColor);
            break;
        case Real:
            elements[1].setFillColor(hoverColor);
            break;
        case Mountain:
            elements[2].setFillColor(hoverColor);
            break;
        case Flat:
            elements[3].setFillColor(hoverColor);
            break;
        case Back:
            elements[4].setFillColor(hoverColor);
            break;
        }
    }

    void keyEvent(RenderWindow& window) {
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (selectedElement > Normal) {
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
            switch (selectedElement) {
            case Normal:
                worldSettings[0] = 6;
                worldSettings[1] = 8;
                play.run(window);
                break;
            case Real:
                worldSettings[0] = 8;
                worldSettings[1] = 2;
                play.run(window);
                break;
            case Mountain:
                worldSettings[0] = 8;
                worldSettings[1] = 8;
                worldSettings[2] = 120;
                play.run(window);
                break;
            case Flat:
                worldSettings[0] = 4;
                worldSettings[1] = 4;
                play.run(window);
                break;
            }
        
            flag = false;
        }
    }
};