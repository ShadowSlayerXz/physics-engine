#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

constexpr float GRAVITY = 9.81f;
constexpr float RESTITUTION = 0.8f;
constexpr float DT = 1.0f/480.0f;
constexpr unsigned int WINDOW_WIDTH = 1000u;
constexpr unsigned int WINDOW_HEIGHT = 800u;

struct vector2
{
    float x;
    float y;
};

struct Particle
{
    vector2 position;
    vector2 velocity;
    float mass;
    float radius;
};

int main()
{
   // Create a window for rendering 
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),"Physics Engine");
    
    // Create a particle representing a ball    
    Particle ball;
    ball.position = {500.0f, 400.0f};
    ball.velocity = {0.0f, 0.0f};
    ball.mass = 1.0f;
    ball.radius = 5.0f; 

    //Create a circle shape to represent the ball visually
    sf::CircleShape ballshape(ball.radius);
    ballshape.setFillColor(sf::Color::White);

    // Set origin to center for proper positioning
    ballshape.setOrigin(sf::Vector2f(ball.radius, ball.radius)); 

    

    cout <<"Ball created at position: [" << ball.position.x << ", " << ball.position.y << "] with radius: " << ball.radius << endl; 
    float logTimer = 0.0f;

    while(window.isOpen())
    {
        

        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

    // Physics simulation loop
    ball.velocity.y +=GRAVITY * DT;


    ball.position.x += ball.velocity.x * DT;
    ball.position.y += ball.velocity.y * DT;

    // Check for collision with the ground
    if(ball.position.y + ball.radius >= WINDOW_HEIGHT)
    {
        ball.position.y = WINDOW_HEIGHT - ball.radius; // Reset position to ground level
        ball.velocity.y *= -RESTITUTION; // Reverse and reduce velocity based on restitution
    }

    //sync the visual representation with the particle's position
    ballshape.setPosition(sf::Vector2f(ball.position.x, ball.position.y));

    logTimer += DT;
    if(logTimer >= 1.0f && ball.position.y + ball.radius <= WINDOW_HEIGHT)
    {
        cout << "Ball position: [" << ball.position.x << ", " << ball.position.y << "]" << " Velocity: [" << ball.velocity.x << ", " << ball.velocity.y << "]" << endl;
        logTimer = 0.0f;
    }

        window.clear();
        window.draw(ballshape);
        window.display();
    }

    return 0;
}
