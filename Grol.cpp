#include<iostream>
#include "SFML/Graphics.hpp"
#include "MainMenu.h"
#include "GlobalVariables.h"

using namespace std;
using namespace sf;

int MainMapSize = 13;
int MainRectSize = 75;

int main()
{
    windowSize = MainMapSize * MainRectSize;

    RenderWindow window(VideoMode(windowSize, windowSize), "GROL");

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("./resources/tlo.png");
    Sprite background(backgroundTexture);

    controls[0] = Keyboard::Left;
    controls[1] = Keyboard::Right;
    controls[2] = Keyboard::Up;
    controls[3] = Keyboard::Down;

    playerColor[0] = 255;
    playerColor[1] = 0;
    playerColor[2] = 0;

    worldSettings[2] = 100;

    MainMenu mainMenu;

    while (window.isOpen())
    {
        window.clear();

        window.draw(background);
        mainMenu.draw(window);
        mainMenu.eventHandler(window);

        window.display();
    }

    return 0;
}