#include "MenuState.hpp"
#include "PlayerState.hpp"
#include <iostream>



using namespace std;

MenuState::MenuState(Controller *controller): controller{controller} {
	// do stuff in init rather then here
}

State::~State() {
    cout << "StateDestruction" << endl;
}
MenuState::~MenuState() {
    cout << "destroying menu" << endl;
}

void MenuState::buttonFactory() {
    Sprite buttonFrameSprite = Sprite();
    Texture frameButton = Texture();
    frameButton.loadFromFile("./resources/buttonParts/HalfButtonFrame.png");
    buttonFrameSprite.setTexture(frameButton);
    Text butinText;
    butinText.setFont(controller -> resource -> getFont("pixel"));
    butinText.setString("quit gounki");
    butinText.setStyle(Text::Bold);
    butinText.setCharacterSize((float)TEXT_SIZE * (2.0 / 3.0));
    butinText.setFillColor(Color::Black);
    buttonFrameSprite.setPosition(0, 0);
    butinText.setPosition(
        buttonFrameSprite.getGlobalBounds().width / 2 - butinText.getGlobalBounds().width / 2,
        buttonFrameSprite.getGlobalBounds().height / 2 - butinText.getGlobalBounds().height / 2 - 5
    );

    Image colorBg;
    colorBg.create(220, 120, Color(172, 50, 50));
    Texture bg;
    bg.loadFromImage(colorBg);
    Sprite bgSprite;
    bgSprite.setTexture(bg);
    bgSprite.setPosition(0, 0);

    rd.create(220, 60);
    rd.clear();
    rd.draw(bgSprite);
    rd.draw(buttonFrameSprite);
    rd.draw(butinText);
    rd.display();
//    rd.getTexture().copyToImage().saveToFile("resources/button/QuitGounkiButton.png");
}

void MenuState::init() {
//    buttonFactory();

    closeButton.setTexture(controller -> resource -> getTexture("closeButton"));
    quitButin.setTexture(controller -> resource ->getTexture("quitButin"));
    quitGounki.setTexture(controller -> resource ->getTexture("quitGounki"));
    quitSafari.setTexture(controller -> resource ->getTexture("quitSafari"));
    butinButton.setTexture(controller -> resource -> getTexture("butinLaunch"));
	gounkiButton.setTexture(controller -> resource -> getTexture("gounkiLaunch"));
	safariButton.setTexture(controller -> resource -> getTexture("safariLaunch"));
    background.setTexture(controller -> resource -> getTexture("background"));
    gameTitle.setFont(controller -> resource -> getFont("pixel"));

    gameTitle.setString("Knight Moves and Safari Grooves");
    gameTitle.setCharacterSize(TITLE_SIZE);
    gameTitle.setFillColor(Color::Black);

    closeButton.setPosition(1770, 90);

    Vector2f center = controller -> machine -> getCenter();

	butinButton.setPosition(
		center.x - butinButton.getGlobalBounds().width * 1.5 - TILE_SIZE, center.y - TILE_SIZE / 2
	);
    quitButin.setPosition(
		center.x - butinButton.getGlobalBounds().width * 1.5 - TILE_SIZE + 10, center.y + (double) TILE_SIZE * (1.5) - 10
	);
	gounkiButton.setPosition(
		center.x - gounkiButton.getGlobalBounds().width / 2, center.y - TILE_SIZE / 2
	);
    quitGounki.setPosition(
		center.x - gounkiButton.getGlobalBounds().width / 2 + 10, center.y + (double) TILE_SIZE * (1.5) - 10
	);
	safariButton.setPosition(
		center.x + safariButton.getGlobalBounds().width / 2 + TILE_SIZE, center.y - TILE_SIZE / 2
	);
    quitSafari.setPosition(
		center.x + safariButton.getGlobalBounds().width / 2 + 10 + TILE_SIZE, center.y + (double) TILE_SIZE * (1.5) - 10
	);
    background.setPosition(0, 0);
    gameTitle.setPosition(
        center.x - gameTitle.getGlobalBounds().width / 2, TILE_SIZE * 6
    );
}

void MenuState::handleInput() {
	Event event;

	while (controller -> window -> pollEvent(event)) {
		if (event.type == Event::Closed) {
			controller -> window -> close();
		} else if (controller -> input -> isSpriteClicked(closeButton, Mouse::Left, *controller -> window)) {
            controller -> window -> close();
        } else if (controller -> input -> isSpriteClicked(butinButton, Mouse::Left, *controller -> window)) {
            if (controller -> canStartNewGame()) {
                controller -> setNewGame(new Butin(), "butin");
                controller -> machine -> addState(new PlayerState(controller), true);
            }
            else if (controller -> getGameName() == "butin") {
                controller -> machine -> removeState();
            }
        } else if (controller -> input -> isSpriteClicked(gounkiButton, Mouse::Left, *controller -> window)) {
            if (controller -> canStartNewGame()) {
                controller ->setNewGame(new Gounki(), "gounki");
                controller -> machine -> addState(new PlayerState(controller), true);
            } else if (controller -> getGameName() == "gounki") {
                controller -> machine -> removeState();
            }
        } else if (controller -> input -> isSpriteClicked(safariButton, Mouse::Left, *controller -> window)) {
            if (controller -> canStartNewGame()) {
                controller -> setNewGame(new Safari(), "safari");
                controller -> machine -> addState(new PlayerState(controller), true);
            } else if (controller -> getGameName() == "safari") {
                controller -> machine -> removeState();
            }
        } else if (!controller -> canStartNewGame()
            && (controller -> input -> isSpriteClicked(quitButin, Mouse::Left, *controller -> window)
            || controller -> input -> isSpriteClicked(quitGounki, Mouse::Left, *controller -> window)
            || controller -> input -> isSpriteClicked(quitSafari, Mouse::Left, *controller -> window))
        ) {
            controller -> machine -> removeBackendState();
        }
	}
}

void MenuState::draw() {
	controller -> window -> clear();
    controller -> window -> draw(background);
    controller -> window -> draw(closeButton);
    controller -> window -> draw(gameTitle);

    if (!controller -> canStartNewGame()) {
        if (controller -> getGameName() == "butin") {controller -> window -> draw(quitButin);}
        else if (controller -> getGameName() == "gounki") {controller -> window -> draw(quitGounki);}
        else if (controller -> getGameName() == "safari") {controller -> window -> draw(quitSafari);}
    }

	controller -> window -> draw(butinButton);
	controller -> window -> draw(gounkiButton);
	controller -> window -> draw(safariButton);


    Sprite tmp;
    tmp.setTexture(rd.getTexture());
    tmp.setPosition(250, 250);
    controller -> window -> draw(tmp);

	controller -> window -> display();
}

void MenuState::update() {
	// std::cout << "updating the menu" << std::endl;
}