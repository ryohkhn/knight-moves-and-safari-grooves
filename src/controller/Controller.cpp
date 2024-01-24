#include "Controller.hpp"
#include "../view/MenuState.hpp"

Controller::Controller(int width, int height, const string& title) {
    cout << "creating window" << endl;
    machine = new StateMachine();
    input = new InputManager();
    resource = new ResourceManager();

	window = new RenderWindow(VideoMode(width, height), title, Style::Fullscreen | Style::Close);
    sf::Mouse::setPosition(Vector2i(0, 0), *window);


    machine -> addState(new MenuState(this));
    game = nullptr;

    clock = Clock();

    loadTextures();

	this -> run();
}

void Controller::loadTextures() const {
    resource -> loadFont("pixel", "./resources/Minecraft.ttf");
    resource -> loadTexture("closeButton", "./resources/button/CloseButton.png");
    resource -> loadTexture("quitButin", "./resources/button/QuitButinButton.png");
    resource -> loadTexture("quitGounki", "./resources/button/QuitGounkiButton.png");
    resource -> loadTexture("quitSafari", "./resources/button/QuitSafariButton.png");
	resource -> loadTexture("butinLaunch", "./resources/button/ButinButton.png");
	resource -> loadTexture("gounkiLaunch", "./resources/button/GounkiButton.png");
	resource -> loadTexture("safariLaunch", "./resources/button/SafariButton.png");
    resource -> loadTexture("background", "./resources/Background.png");
    resource -> loadTexture("resumeButton", "./resources/button/ResumeButton.png");
    resource -> loadTexture("menuButton", "./resources/button/MenuButton.png");
    resource -> loadTexture("exitButton", "./resources/button/ExitButton.png");
    resource -> loadTexture("enabledStartButton", "./resources/button/EnabledStartButton.png");
    resource -> loadTexture("disabledStartButton", "./resources/button/DisabledStartButton.png");
    resource -> loadTexture("enabledAddButton", "./resources/button/EnabledAddButton.png");
    resource -> loadTexture("disabledAddButton", "./resources/button/DisabledAddButton.png");
    resource -> loadTexture("yellowPiece", "./resources/piece/YellowPiece.png");
    resource -> loadTexture("redPiece", "./resources/piece/RedPiece.png");
    resource -> loadTexture("blackPiece", "./resources/piece/BlackPiece.png");
    resource -> loadTexture("redTile", "./resources/board/RedTile.png");
    resource -> loadTexture("blueTile", "./resources/board/BlueTile.png");
    resource -> loadTexture("crocodilePiece", "./resources/piece/CrocodilePiece.png");
    resource -> loadTexture("elephantPiece", "./resources/piece/ElephantPiece.png");
    resource -> loadTexture("lionPiece", "./resources/piece/LionPiece.png");
    resource -> loadTexture("redVerticalFence", "./resources/piece/RedVerticalFence.png");
    resource -> loadTexture("redHorizontalFence", "./resources/piece/RedHorizontalFence.png");
    resource -> loadTexture("verticalFence", "./resources/piece/VerticalFence.png");
    resource -> loadTexture("horizontalFence", "./resources/piece/HorizontalFence.png");
    resource -> loadTexture("enabledEndTurnButton", "./resources/button/EnabledEndTurnButton.png");
    resource -> loadTexture("disabledEndTurnButton", "./resources/button/DisabledEndTurnButton.png");
    resource -> loadTexture("whiteSquare", "./resources/piece/WhiteSquarePiece.png");
    resource -> loadTexture("blackSquare", "./resources/piece/BlackSquarePiece.png");
    resource -> loadTexture("whiteCircle", "./resources/piece/WhiteCirclePiece.png");
    resource -> loadTexture("blackCircle", "./resources/piece/BlackCirclePiece.png");
}

void Controller::run() {
    int updates = 0;
    double updateTracker = 0;

    int frames = 0;
    double frameTracker = 0;

    long lastCheck = clock.getElapsedTime().asMilliseconds();
    long previousTime = clock.getElapsedTime().asMicroseconds();

    while (window -> isOpen()) {
        machine -> processStateChanges();
        long currentTime = clock.getElapsedTime().asMicroseconds();

        updateTracker += (currentTime - previousTime) / timePerUpdate;
        if (updateTracker >= 1) {
            if (!machine -> getActiveState() -> isPaused) {
                machine -> getActiveState() -> handleInput();
                machine -> getActiveState() -> update();
                updates++;
            }
            updateTracker--;
        }
        frameTracker += (currentTime - previousTime) / timePerFrame;
        if (frameTracker >= 1) {
            machine -> getActiveState() -> draw();

            frames++;
            frameTracker--;
        }

        if (clock.getElapsedTime().asMilliseconds() - lastCheck >= 1000) {
            lastCheck = clock.getElapsedTime().asMilliseconds();
//            cout << "FPS: " << frames << "\nUPS: " << updates << endl;
            updates = 0;
            frames = 0;
        }
        previousTime = currentTime;
    }
}

bool Controller::canStartNewGame() const {
    return game == nullptr || game -> isGameDone();
}

// Les vérifs necessaires seront faites avant d'appeler setNewGame
void Controller::setNewGame(Game *newGame, string name) {
    if (game != nullptr) {
        delete(game);
        game = nullptr;
    }
    gameName = name;
    game = newGame;
}

void Controller::removeGame() {
    delete(game);
    game = nullptr;
}

void Controller::setPlayerNames(vector<std::string> newNames) {
    playerNames = newNames;
}

Texture* Controller::getWindowAsTexture() {
    Vector2u windowSize = window -> getSize();
    Texture *texture = new Texture();
    texture -> create(windowSize.x, windowSize.y);
    texture -> update(*window);
    return texture;
}

string Controller::getGameName() const {
    return gameName;
}

Controller::~Controller() {
    delete machine;
    delete window;
    delete input;
    delete resource;
    delete game;
}