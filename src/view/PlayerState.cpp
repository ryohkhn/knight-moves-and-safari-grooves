#include <iostream>
#include "PlayerState.hpp"
#include "butin/ButinState.hpp"
#include "safari/SafariState.hpp"
#include "gounki/GounkiState.hpp"

PlayerState::PlayerState(Controller *controller): controller{controller}, minPlayers{controller -> game -> getMinMaxPlayers().first}, maxPlayers{controller -> game -> getMinMaxPlayers().second} {
    playerDisplayNames.insert(playerDisplayNames.begin(), Text());
    //Do stuff in init

}
PlayerState::~PlayerState() {
    cout << "Destroying PlayerState !!!!!" << endl;
}

void PlayerState::init() {
    title.setFont(controller -> resource -> getFont("pixel"));
    playerDisplayNames[0].setFont(controller -> resource -> getFont("pixel"));
    background.setTexture(controller -> resource -> getTexture("background"));
    addPlayerButton.setTexture(controller -> resource -> getTexture("enabledAddButton"));
    startButton.setTexture(controller -> resource ->getTexture("disabledStartButton"));

    Vector2f center = controller -> machine -> getCenter();

    background.setPosition(0, 0);
    addPlayerButton.setPosition(
            center.x - addPlayerButton.getGlobalBounds().width * 1.5, center.y + addPlayerButton.getGlobalBounds().height * 2
    );
    startButton.setPosition(
        center.x + startButton.getGlobalBounds().width * 0.5, center.y + startButton.getGlobalBounds().height * 2
    );

    title.setCharacterSize(TEXT_SIZE);
    title.setFillColor(Color::Black);
    string titleString = "Enter at least "
            + to_string(controller -> game -> getMinMaxPlayers().first)
            + " and at most "
            + to_string(controller -> game -> getMinMaxPlayers().second)
            + " player names";
    title.setString(titleString);
    title.setPosition(
        center.x - title.getGlobalBounds().width / 2, center.y - TILE_SIZE * 5
    );

    playerDisplayNames[0].setCharacterSize(TEXT_SIZE);
    playerDisplayNames[0].setFillColor(Color::Black);
    playerDisplayNames[0].setString("");
}

void PlayerState::handleInput() {
    Event event{};

    while (controller -> window -> pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                controller -> window -> close();
                break;
            case sf::Event::TextEntered:
                if ((int) playerNames.size() < maxPlayers && playerDisplayNames[0].getString().getSize() < MAX_NAME_LENGTH) {
                    if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
                        playerDisplayNames[0].setString(
                                playerDisplayNames[0].getString() + (char)event.text.unicode
                        );
                        repositionNameDisplay();
                    }
                }

                break;
            case sf::Event::KeyPressed:
                if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
                    playerDisplayNames[0].setString(
                        playerDisplayNames[0].getString().substring(0, playerDisplayNames[0].getString().getSize() - 1)
                    );
                    repositionNameDisplay();
                } else if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    if ((int) playerNames.size() == maxPlayers) {
                        goToGameState();
                    } else {
                        saveName();
                    }
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (controller -> input -> isSpriteClicked(addPlayerButton, Mouse::Left, *controller -> window)) {
                    saveName();
                } else if (controller -> input -> isSpriteClicked(startButton, Mouse::Left, *controller -> window)) {
                    if ((int) playerNames.size() >= minPlayers) {
                        goToGameState();
                    }
                }
            default:
                break;
        }
    }
}
void PlayerState::goToGameState() {
    string gameToLaunch = controller -> getGameName();
    controller -> game -> initPlayers(playerNames);
    if (gameToLaunch == "butin") {
        controller -> machine -> addState(new ButinState(controller), true);
    } else if (gameToLaunch == "gounki") {
        controller -> machine -> addState(new GounkiState(controller), true);
    } else if (gameToLaunch == "safari") {
        controller -> machine -> addState(new SafariState(controller), true);
    } else {
        cout << "no game is running" << endl;
    }
}
void PlayerState::saveName() {
    if (playerDisplayNames[0].getString().getSize() > 0) {
        string newPlayerName = playerDisplayNames[0].getString();
        if ((int) playerNames.size()  < maxPlayers && find(playerNames.begin(), playerNames.end(), newPlayerName) == playerNames.end()) {
            playerNames.push_back(playerDisplayNames[0].getString());
            if ((int) playerNames.size() < maxPlayers) {
                playerDisplayNames.insert(playerDisplayNames.begin(), Text());
                playerDisplayNames[0].setFont(controller -> resource -> getFont("pixel"));
                playerDisplayNames[0].setCharacterSize(TEXT_SIZE);
                playerDisplayNames[0].setFillColor(Color::Black);
                playerDisplayNames[0].setString("");
                repositionNameDisplay();
            }
        }
    }
}

void PlayerState::repositionNameDisplay() {
    Vector2f center = controller -> machine -> getCenter();

    for (int i = 0; i < (int) playerDisplayNames.size(); i++) {
        playerDisplayNames[i].setPosition(
            center.x - playerDisplayNames[i].getGlobalBounds().width / 2, center.y - (TILE_SIZE * i)
        );
    }
}

void PlayerState::update() {
    if ((int) playerNames.size() >= minPlayers) {
        startButton.setTexture(controller -> resource -> getTexture("enabledStartButton"));
    }
    if ((int) playerNames.size() == maxPlayers) {
        addPlayerButton.setTexture(controller -> resource -> getTexture("disabledAddButton"));
    }
}

void PlayerState::draw() {
    controller -> window -> clear();

    controller -> window -> draw(background);
    controller -> window -> draw(title);
    controller -> window -> draw(addPlayerButton);
    controller -> window -> draw(startButton);
    for (Text const &t : playerDisplayNames) {
        controller -> window -> draw(t);
    }
    controller -> window -> display();
}