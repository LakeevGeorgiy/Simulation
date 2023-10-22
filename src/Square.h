#pragma once

class Square {
public:
    int sideLength;
    float xPosition;
    float yPosition;
    float xVelocity;
    float yVelocity;
    float weight;
    sf::RectangleShape square;
    sf::Vector2f size;

    Square(int side, float xPos, float yPos, float xVel, float yVel, float w) :
            sideLength(side), xPosition(xPos), yPosition(yPos), xVelocity(xVel), yVelocity(yVel), size(sideLength,sideLength), weight(w)  {

        square.setSize(size);
        square.setPosition(xPosition, yPosition);
    }

    void SetColor(uint8_t red, uint8_t green, uint8_t blue){
        square.setFillColor(sf::Color(red, green, blue));
    }

    void SetPosition(float xPos) {
        xPosition = xPos;
        square.setPosition(xPos, yPosition);
    }

    void Move() {
        xPosition += xVelocity;
        square.setPosition(xPosition, yPosition);
    }

    void ReverseVelocity() {
        xVelocity *= -1;
    }

    void ChangeSpeedAfterCollision(float otherW, float otherVelocity) {
        xVelocity = ((weight - otherW) * xVelocity + 2 * otherW * otherVelocity) / (weight + otherW);
    }

    void InputData(float w, float velocity){
        weight = w;
        xVelocity = velocity;
    }

    void Reset(int xPos, int yPos){
        xPosition = xPos;
        yPosition = yPos;
        xVelocity = 0;
        yVelocity = 0;
        weight = 1;
    }

    ~Square() = default;
};