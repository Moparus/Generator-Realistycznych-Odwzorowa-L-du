#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "SFML/Graphics.hpp"
#include "vector"
#include "perlin.h"
#include "DayTime.h"
#include "GameMenu.h"
#include "Options.h"
#include "GlobalVariables.h"

using namespace std;
using namespace sf;

const int mapSize = 13;
const int rectSize = 75;
int windowSize;
const int viewSize = 750;
const double playerSize = rectSize / 2;

int worldSettings[4];

Texture treeTexture;
Texture fungusTexture;

Font font;

Keyboard::Key controls[4];

int playerColor[3];

class Play
{
public:
    Play() {}

    bool flag = true;
    bool isLoaded = false;

    vector<vector<int>> fungusCollection{};

    Perlin* per;

    Perlin* perTrees;
    Perlin* perFungus;

    GameMenu gameMenu;

    void run(RenderWindow& window) {
        if (isLoaded) {
            vector<vector<string>> data = readWorldDataFromFile("Saves.txt");
            if (data.size() > 0)
            {
                worldSettings[0] = stoi(data[0][0]);
                worldSettings[1] = stoi(data[0][1]);
                worldSettings[2] = stoi(data[0][2]);
                worldSettings[3] = stoi(data[0][3]);

                posX = stoi(data[0][4]);
                posY = stoi(data[0][5]);

                openFile("Fungus.txt");
            }
        }

        Perlin tmpPer(worldSettings[0], worldSettings[1], worldSettings[2], worldSettings[3]);
        per = &tmpPer;
        Perlin tmpPerTrees(6, 8, 10, worldSettings[3] * 14);
        perTrees = &tmpPerTrees;
        Perlin tmpPerFungus(8, 8, 10, worldSettings[3] * 41);
        perFungus = &tmpPerFungus;

        font.loadFromFile("arial.ttf");
        treeTexture.loadFromFile("./resources/drzewo.png");
        fungusTexture.loadFromFile("./resources/grzyb.png");

        Image image;
        image.create(playerSize, playerSize, Color(playerColor[0], playerColor[1], playerColor[2], 255));
        Texture texture;
        texture.loadFromImage(image);
        Sprite player(texture);
        player.setPosition(Vector2f((windowSize - playerSize) / 2, (windowSize - playerSize) / 2));

        RectangleShape nightMode(Vector2f(windowSize, windowSize));
        nightMode.setFillColor(Color(0, 0, 0, 0));

        Text clockText;
        clockText.setFont(font);
        clockText.setString("15:00");
        clockText.setCharacterSize(50);
        clockText.setFillColor(Color::Red);
        clockText.setPosition((windowSize + viewSize) / 2 - 140, (windowSize - viewSize) / 2);

        Text eq;
        eq.setFont(font);
        string fungus = (fungusCollection.empty()) ? "0" : to_string(fungusCollection.size());
        eq.setString("Fungusow: " + fungus);
        eq.setCharacterSize(25);
        eq.setFillColor(Color::Red);
        eq.setPosition(125, (windowSize - viewSize) / 2);

        Event e;

        int speed = 100;

        vector<RectangleShape> map = renderMap(posX, posY);

        Clock clock;
        Clock mainClock;
        DayTime clok;;

        while (window.isOpen() && flag)
        {
            view.reset(FloatRect((windowSize - viewSize) / 2, (windowSize - viewSize) / 2, viewSize, viewSize));

            window.clear();
            
            Time timer = mainClock.getElapsedTime();

            if (timer.asMilliseconds() >= 500)
            {
                clok.setDayTime(1);
                mainClock.restart();
            }

            clockText.setString(clok.getDayTime());
            nightMode.setFillColor(Color(0, 0, 0, clok.getDarkness()));
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();
                if (e.type == Event::KeyPressed) {
                    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                        GameMenu gameMenu;
                        Texture backgroundTexture;
                        backgroundTexture.loadFromFile("./resources/tlo1.png");
                        Sprite background(backgroundTexture);
                        view.reset(sf::FloatRect(0, 0, windowSize, windowSize));
                        window.setView(view);
                        while (window.isOpen() && gameMenu.flag) {
                            window.clear();

                            window.draw(background);
                            gameMenu.draw(window);
                            gameMenu.eventHandler(window);

                            window.display();
                        }
                        if (gameMenu.saveFlag) {
                            saveWorldDataToFile(worldSettings, posX, posY);
                            saveFile("Fungus.txt");
                            return;
                        }
                        flag = !gameMenu.saveFlag;
                    }
                    Time elapsed = clock.getElapsedTime();
                    if (elapsed.asMilliseconds() >= speed) {
                        if (Keyboard::isKeyPressed(controls[0])) {
                            if (round_to(per->Get((posY + 6) / 1000, (posX + 5) / 1000), 1) > 75)
                                break;
                            posX--;
                        }
                        if (Keyboard::isKeyPressed(controls[1])) {
                            if (round_to(per->Get((posY + 6) / 1000, (posX + 7) / 1000), 1) > 75)
                                break;
                            posX++;
                        }
                        if (Keyboard::isKeyPressed(controls[2])) {
                            if (round_to(per->Get((posY + 5) / 1000, (posX + 6) / 1000), 1) > 75)
                                break;
                            posY--;
                        }
                        if (Keyboard::isKeyPressed(controls[3])) {
                            if (round_to(per->Get((posY + 7) / 1000, (posX + 6) / 1000), 1) > 75)
                                break;
                            posY++;
                        }
                        system("cls");
                        cout << per->Get((posY + 6) / 1000, (posX + 6) / 1000) << endl;
                        cout << posX+6 << " " << posY+6 << endl;

                        //Zbieranie grzyb�w
                         //0.9 musi by� zalezne od 'Rozrzedzanie grzyb�w'
                        double perlinPositionValue = per->Get((posY + 6) / 1000, (posX + 6) / 1000);
                        if (((perFungus->Get((posY + 6) / 1000, (posX + 6) / 1000) - (int)perFungus->Get((posY + 6) / 1000, (posX + 6) / 1000)) > 0.96) && !isValueInArray(fungusCollection, posX+6, posY+6) && (perlinPositionValue > 0 && perlinPositionValue < 75))
                        {
                            fungusCollection.push_back({static_cast<int>(posX + 6), static_cast<int>(posY + 6)});
                            eq.setString("Fungusow: " + to_string(fungusCollection.size()));
                        }

                        map = renderMap(posX, posY);

                        clock.restart();

                        int valueOfPosition = map[6 * mapSize + 6].getOutlineThickness();
                        if (valueOfPosition <= -300) {
                            player.setColor(Color(playerColor[0], playerColor[1], playerColor[2], 128));
                            continue;
                        }
                        else {
                            player.setColor(Color(playerColor[0], playerColor[1], playerColor[2], 255));
                        }

                        if (valueOfPosition <= -10) {
                            speed = 300;
                        }
                        else {
                            speed = 100;
                        }
                    }
                }
            }

            window.clear();
            window.setView(view);

            for (int i = 0; i < map.size(); i++) {
                window.draw(map[i]);
            }

            for (int i = 0; i < renderTree(map).size(); i++) {
                window.draw(renderTree(map)[i]);
            }

            for (int i = 0; i < renderFungus(map).size(); i++) {
                window.draw(renderFungus(map)[i]);
            }

            window.draw(player);
            window.draw(clockText);
            window.draw(eq);
            window.draw(nightMode);

            window.display();
        }
    }

private:

    double posX = 0;
    double posY = 0;
    View view;

    double round_to(double value, double precision = 1.0)
    {
        return round(value / precision) * precision;
    }

    vector<vector<double>> generateMap(double x1 = 0, double y1 = 0)
    {
        vector<vector<double>> map;
        double x = (x1 / 1000);
        double y = (y1 / 1000);
        for (double i = y; i < (y + 0.013); i += 0.001)
        {
            vector<double> tmp;
            for (double j = x; j < (x + 0.013); j += 0.001)
            {
                double roundTo = (round_to(per->Get(i, j), 1) > 100) ? 100 : round_to(per->Get(i, j), 1);
                double trees = perTrees->Get(i, j);
                int treesLimit = 0; //Szansa na las (0 - 100%, 10 - 0%)
                bool fungusValue = isValueInArray(fungusCollection, (round_to(j, 0.001) * 1000), (round_to(i, 0.001) * 1000));
                if (roundTo > 0 && roundTo < 75) { //Blokada przed tworzeniem drzew i grzyb�w na wodzie i kamieniach
                    if (perFungus->Get(i, j) - (int)perFungus->Get(i, j) > 0.96) //Rozrzedzanie grzyb�w (0.2 - 20%)
                        if (!fungusValue)
                            roundTo = -400 - round_to(per->Get(i, j), 1);
                    else if (trees > treesLimit && !fungusValue) {
                        if ((perTrees->Get(i, j) - (int)perTrees->Get(i, j)) > 0.8) //Rozrzedzanie drzew (0.2 - 20%)
                            roundTo = -300 - round_to(per->Get(i, j), 1);
                    }
                }
                //cout << roundTo << " ";
                tmp.push_back(roundTo);
            }
            //cout << endl;
            map.push_back(tmp);
        }
        return map;
    }

    Color setColor(int value)
    {
        int redValue = 0, greenValue = 0, blueValue = 0;
        if (value <= -400) {
            value = -value - 400;
        }
        if (value <= -300) {
            value = -value - 300;
        }
        if (value > -300 && value <= -10) {
            value = (value < -75) ? -75 : value;
            redValue = 65 + (int)value * 0.3;
            greenValue = 130 + (int)value;
            blueValue = 170 + (int)value * 1.5;
        }
        else if (value <= 75) {
            redValue = 110;
            greenValue = 170 - (int)abs(value) * 0.7;
            blueValue = 80;
        }
        else {
            redValue = 170 - value;
            greenValue = 170 - value;
            blueValue = 155 - value;
        }
        return Color(redValue, greenValue, blueValue, 255);
    }

    vector<RectangleShape> renderMap(int x, int y)
    {
        vector<vector<double>> mapVector = generateMap(x, y);
        vector<RectangleShape> rectArray;

        for (int i = 0; i < mapVector.size(); i++)
        {
            for (int j = 0; j < mapVector[0].size(); j++)
            {
                RectangleShape rect;
                Vector2f rectPos(rectSize * j, rectSize * i);
                rect.setPosition(rectPos);
                rect.setSize(Vector2f(rectSize, rectSize));
                rect.setFillColor(setColor(mapVector[i][j]));
                rect.setOutlineThickness(mapVector[i][j]);
                rect.setOutlineColor(Color(0, 0, 0, 0));
                //rect.setRotation(rand() % 360);
                rectArray.push_back(rect);
            }
        }
        return rectArray;
    }

    vector<RectangleShape> renderTree(vector<RectangleShape> map)
    {
        vector<RectangleShape> treeArray;
        for (int i = 0; i < map.size(); i++) {
            if (map[i].getOutlineThickness() <= -300 && map[i].getOutlineThickness() > -400)
            {
                RectangleShape tree;
                Vector2f treePos = map[i].getPosition();
                tree.setPosition(treePos.x - rectSize / 4, treePos.y - rectSize / 4);
                tree.setSize(Vector2f(rectSize * 1.5, rectSize * 1.5));
                tree.setTexture(&treeTexture);
                //tree.setRotation(rand()% 360);
                treeArray.push_back(tree);
            }
        }
        return treeArray;
    }

    vector<RectangleShape> renderFungus(vector<RectangleShape> map)
    {
        vector<RectangleShape> fungusArray;
        for (int i = 0; i < map.size(); i++) {
            if (map[i].getOutlineThickness() <= -400 && map[i].getOutlineThickness() > -500)
            {
                RectangleShape fungus;
                Vector2f fungusPos = map[i].getPosition();
                fungus.setPosition(fungusPos.x + rectSize / 4, fungusPos.y + rectSize / 4);
                fungus.setSize(Vector2f(rectSize * 0.5, rectSize * 0.5));
                fungus.setTexture(&fungusTexture);
                //fungus.setRotation(rand()% 360);
                fungusArray.push_back(fungus);
            }
        }
        return fungusArray;
    }

    void openFile(string filename)
    {
        fstream file;
        file.open(filename);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            vector<int> row;
            while (getline(ss, token, ';')) {
                stringstream ss2(token);
                string token2;
                vector<int> values;
                while (getline(ss2, token2, ',')) {
                    values.push_back(stoi(token2));
                }
                row.push_back(values[0]);
                row.push_back(values[1]);
            }
            fungusCollection.push_back(row);
        }
        file.close();
    }

    bool isValueInArray(const vector<vector<int>>& array2D, int x, int y) {
        for (const auto& row : array2D)
            if (row.size() >= 2 && row[0] == x && row[1] == y)
                return true;
        return false;  
    }


    void saveFile(string filename)
    {
        ofstream file(filename);
        if (file.is_open()) {
            file.clear();
            for (const auto& row : fungusCollection) {
                for (size_t i = 0; i < row.size(); ++i) {
                    file << row[i];
                    if (i < row.size() - 1) {
                        file << ",";
                    }
                }
                file << ";\n";
            }
            file.close();
            cout << "Data written to the file." << endl;
        }
        else {
            cerr << "Unable to open file for writing." << endl;
        }
    }

    void saveWorldDataToFile(int worldSettings[], int x, int y) {
        ofstream file("Saves.txt");
        if (file.is_open()) {
            for (int i = 0; i < 4; i++)
            {
                file << worldSettings[i] << ";";
            }
            file << x << ";" << y << ";" << endl;
            file.close();
            cout << "World data has been saved to the file." << endl;
        }
        else {
            cerr << "Failed to open the file for writing." << endl;
        }
    }
    vector<vector<string>> readWorldDataFromFile(const string& filename) {
        ifstream file(filename);
        vector<vector<string>> worldDataArray;

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                vector<string> worldData;
                istringstream ss(line);
                string token;
                while (getline(ss, token, ';')) {
                    worldData.push_back(token);
                }
                worldDataArray.push_back(worldData);
            }
            file.close();
        }
        else {
            cerr << "Nie uda�o si� otworzy� pliku do odczytu." << endl;
        }

        return worldDataArray;
    }
};