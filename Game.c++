#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;
const int GRAVITY = 1;
const int JUMP_STRENGTH = -15;
const int PIPE_WIDTH = 80;
const int PIPE_GAP = 200;
const float PIPE_SPEED = 3.0f;

struct Pipe {
    sf::RectangleShape upper, lower;
    float x;
    
    Pipe(float startX) {
        x = startX;
        float gapY = rand() % (HEIGHT - PIPE_GAP - 100) + 50;
        
        upper.setSize(sf::Vector2f(PIPE_WIDTH, gapY));
        upper.setFillColor(sf::Color::Green);
        upper.setPosition(x, 0);
        
        lower.setSize(sf::Vector2f(PIPE_WIDTH, HEIGHT - gapY - PIPE_GAP));
        lower.setFillColor(sf::Color::Green);
        lower.setPosition(x, gapY + PIPE_GAP);
    }
    
    void update() {
        x -= PIPE_SPEED;
        upper.setPosition(x, 0);
        lower.setPosition(x, upper.getSize().y + PIPE_GAP);
    }
    
    bool isOffScreen() { return x + PIPE_WIDTH < 0; }
};

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Floppy Bird");
    sf::RectangleShape bird(sf::Vector2f(40, 40));
    bird.setFillColor(sf::Color::Yellow);
    bird.setPosition(100, HEIGHT / 2);
    
    float velocity = 0;
    std::vector<Pipe> pipes;
    pipes.emplace_back(WIDTH);
    
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                velocity = JUMP_STRENGTH;
        }
        
        velocity += GRAVITY;
        bird.move(0, velocity);
        
        if (pipes.back().x < WIDTH - 300)
            pipes.emplace_back(WIDTH);
        
        for (auto &pipe : pipes)
            pipe.update();
        
        if (!pipes.empty() && pipes[0].isOffScreen())
            pipes.erase(pipes.begin());
        
        window.clear();
        window.draw(bird);
        for (auto &pipe : pipes) {
            window.draw(pipe.upper);
            window.draw(pipe.lower);
        }
        window.display();
    }
    return 0;
}
