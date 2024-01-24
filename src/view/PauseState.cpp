#include "PauseState.hpp"
#include "MenuState.hpp"

PauseState::PauseState(Controller *controller, Texture *texture): controller{controller} {
    backgroundTexture = *texture;
    delete(texture);
}

PauseState::~PauseState() {
    cout << "destroying PauseMenu" << endl;
}

void PauseState::init() {
    background.setTexture(backgroundTexture);
    resumeButton.setTexture(controller -> resource -> getTexture("resumeButton"));
    returnMenuButton.setTexture(controller -> resource -> getTexture("menuButton"));
    exitButton.setTexture(controller -> resource -> getTexture("exitButton"));

    background.setPosition(0, 0);

    Vector2f center = controller -> machine -> getCenter();
    resumeButton.setPosition(
        center.x - resumeButton.getGlobalBounds().width / 2,
        center.y - TILE_SIZE * 3
    );
    returnMenuButton.setPosition(
        center.x - returnMenuButton.getGlobalBounds().width / 2,
        center.y
    );
    exitButton.setPosition(
        center.x - exitButton.getGlobalBounds().width / 2,
        center.y + TILE_SIZE * 2
    );
}

void PauseState::handleInput() {
    Event event;

    while(controller -> window -> pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                controller -> window -> close();
                break;
            case Event::KeyPressed:
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    controller -> machine -> removeState();
                }
                break;
            case Event::MouseButtonPressed:
                if (controller -> input -> isSpriteClicked(resumeButton, Mouse::Left, *controller -> window)) {
                    controller -> machine -> removeState();
                    break;
                } else if (controller -> input -> isSpriteClicked(returnMenuButton, Mouse::Left, *controller -> window)) {
                    controller -> machine -> addState(new MenuState(controller), true);
                    break;
                } else if (controller -> input ->isSpriteClicked(exitButton, Mouse::Left, *controller -> window)) {
                    controller -> window -> close();
                    break;
                }
            default:
                break;
        }
    }
}

void PauseState::draw() {
    controller -> window -> clear();
    controller -> window -> draw(background);
    controller -> window -> draw(resumeButton);
    controller -> window -> draw(returnMenuButton);
    controller -> window -> draw(exitButton);
    controller -> window -> display();
}

void PauseState::update() {

}