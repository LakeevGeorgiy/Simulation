#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "Square.h"
#include <iostream>

int countCollisions = 0;

void Collision(Square& smallSquare, Square& largeSquare) {
    if (smallSquare.xPosition + smallSquare.sideLength < largeSquare.xPosition)
        return;

    float xPos = largeSquare.xPosition - smallSquare.sideLength;
    smallSquare.SetPosition(xPos);

    float oldLargeWeight = largeSquare.weight;
    float oldLargeVelocity = largeSquare.xVelocity;
    float oldSmallWeight = smallSquare.weight;
    float oldSmallVelocity = smallSquare.xVelocity;

    largeSquare.ChangeSpeedAfterCollision(oldSmallWeight, oldSmallVelocity);
    smallSquare.ChangeSpeedAfterCollision(oldLargeWeight, oldLargeVelocity);

    ++countCollisions;
}

void UpdateFunction(float smallWeight, float largeWeight, float smallVelocity,
                    float largeVelocity, Square& small, Square& large){

    small.InputData(smallWeight, smallVelocity);
    large.InputData(largeWeight, largeVelocity);
}

void ResetFunction(Square& smallSquare, Square& largeSquare, const int smallXPostion, const int smallYPosition,
                   const int largeXPosition, const int largeYPosition){

    smallSquare.Reset(smallXPostion, smallYPosition);
    largeSquare.Reset(largeXPosition, largeYPosition);
    countCollisions = 0;
}

int main() {

    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int frameLimit = 60;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Simulation!");
    window.setFramerateLimit(frameLimit);
    ImGui::SFML::Init(window);

    const int smallSide = 50;
    int smallWeight = 1;
    const int beginXPositionSmall = windowWidth / 5;
    const int beginYPositionSmall = windowHeight - smallSide;
    Square smallSquare(smallSide, beginXPositionSmall, beginYPositionSmall, 0, 0, smallWeight);

    const int largeSide = 150;
    int largeWeight = 1;
    const int beginXPositionLarge = windowWidth - 2 * largeSide;
    const int beginYPositionLarge = windowHeight - largeSide;
    Square largeSquare(largeSide, beginXPositionLarge, beginYPositionLarge, 0, 0, largeWeight);
    largeSquare.SetColor(160, 160, 160);

    int collisionsXPosition = windowWidth - 250;
    int collisionsYPosition = 50;
    sf::Font arialFont;
    arialFont.loadFromFile("C:\\Users\\lakee\\source\\repos\\C#\\PhisicsSimulation\\Project1\\arial.ttf");
    sf::Text text("Total collisions", arialFont, 20);
    text.setPosition(collisionsXPosition, collisionsYPosition);
    text.setFillColor(sf::Color::White);

    sf::Text Velocity1("Velocity small: ", arialFont, 20);
    Velocity1.setPosition(0, 0);
    Velocity1.setFillColor(sf::Color::White);

    sf::Text Velocity2("Velocity large: ", arialFont, 20);
    Velocity2.setPosition(300, 0);
    Velocity2.setFillColor(sf::Color::White);

    const float floorHeight = 15;
    sf::RectangleShape floor(sf::Vector2f(windowWidth, floorHeight));
    floor.setPosition(0, windowHeight - floorHeight);
    floor.setFillColor(sf::Color(204, 0, 0));

    const float wallWidth = 15;
    sf::RectangleShape wall(sf::Vector2f(wallWidth, windowHeight - 200));
    wall.setPosition(0, 200);
    wall.setFillColor(sf::Color(204, 0, 0));

    float var1 = 0;
    float var2 = 0;
    float var3 = 0;
    float var4 = 0;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        smallSquare.Move();
        largeSquare.Move();

        if (smallSquare.xPosition <= wallWidth) {
            smallSquare.ReverseVelocity();
            smallSquare.SetPosition(0);
            ++countCollisions;
        }

        if (largeSquare.xPosition >= windowWidth && smallSquare.xVelocity > largeSquare.xVelocity) {
            largeSquare.SetPosition(smallSquare.xPosition + smallSquare.sideLength);
        }

        Collision(smallSquare, largeSquare);

        text.setString("Total collisions: " + std::to_string(countCollisions));
        text.setFillColor(sf::Color::White);
        Velocity1.setString("Velocity small: " + std::to_string(smallSquare.xVelocity));
        Velocity2.setString("Velocity large: " + std::to_string(largeSquare.xVelocity));

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::InputFloat("Weight of small block: ", &var1);
        ImGui::InputFloat("Weight of large block: ", &var2);
        ImGui::InputFloat("Velocity of small block: ", &var3);
        ImGui::InputFloat("Velocity of large block: ", &var4);
        if (ImGui::Button("Choose properties"))
            UpdateFunction(var1, var2, var3, var4, smallSquare, largeSquare);
        if (ImGui::Button("Reset"))
            ResetFunction(smallSquare, largeSquare, beginXPositionSmall, beginYPositionSmall,
                          beginXPositionLarge, beginYPositionLarge);

        window.clear();

        window.draw(text);
        smallSquare.SetPosition(std::max(smallSquare.xPosition, (float)wallWidth));
        largeSquare.SetPosition(std::max(largeSquare.xPosition, (float)smallSquare.sideLength + wallWidth));
        window.draw(smallSquare.square);
        window.draw(largeSquare.square);
        window.draw(Velocity1);
        window.draw(Velocity2);
        window.draw(floor);
        window.draw(wall);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}