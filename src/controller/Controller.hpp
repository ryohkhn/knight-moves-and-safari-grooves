#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <string>

#include "../view/StateMachine.hpp"
#include "../view/InputManager.hpp"
#include "../view/ResourceManager.hpp"

#include "../settings/SETTINGS.hpp"
#include "../model/Game.hpp"
#include "../model/butin/Butin.hpp"
#include "../model/safari/Safari.hpp"
#include "../model/gounki/Gounki.hpp"
#include "../model/GameConfig.hpp"


using namespace std;

class Controller {
public:
    /**
     * est appelé une seule fois depuis main.cpp c'est à sa construction que le logiciel prend vie et se met à répondre
     * aux entrées clavier/souris de sorte à pouvoir jouer à des jeux
     * @param width la largeur de la fenêtre
     * @param height la hauteur de la fenêtre
     * @param title le titre de la fenêtre
     * la fenêtre est créé et msie en pleinne écran donc les valeurs passées en argument ne sont pas utile
     * */
	Controller(int width, int height, const string &title);
	virtual ~Controller();
    /**
     * Créer une copie du contrôleur voudrait dire avoir plusieurs version d'un même logiciel qui tourne en m^me temps
     * et on ne veut pas ça
     * */
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    /**
     * Différentes Structures pour manipuler des resources/Informations/États/jeux
     * voir les classes concernée pour des commentaires plus détaillés
     * */
	StateMachine *machine;
	RenderWindow *window;
	InputManager *input;
	ResourceManager *resource;
	Game *game;

    /**
     * un setteur comme un autre met qui prend le nom du jeu en paramètre pour pouvoir informer les États qui en auront
     * besoin pour faire des actions spécifiques en fonction du jeu en cours par exemple
     * @param name le nom du jeu "butin" dans le cas de Butin,...
     * */
	void setNewGame(Game *, string name);
    /**
     * Sera utilisé par les autres États pour savoir si une nouvelle partie peut être commencé, c-à-d qu'aucune partie
     * n'est en cours de jeu
     * @return vrai si une nouvelle partie peut commencer, faux sinon
     * */
	bool canStartNewGame() const;
    /**
     * Un getteur comme un autre pour récupérer la valeur prise par le setteur un peu plus haut
     * @return le nom du jeu en cours et "" si aucun ne l'est
     * */
	string getGameName() const;
    /**
     * Retire et delete proprement le jeu en cours
     * */
    void removeGame();
    /**
     * met en place le nom des joueurs d'une nouvelle partie
     * @param newNames un vector de string représentants le nom de tout les joueurs de la partie
     * */
	void setPlayerNames(vector<string> newNames);
    /**
     * retourne l'écran courant sous la forme d'une Texture similairement à un "screenshot", sera utilisé uniquement
     * par PauseState
     * @return la texture correspondante à ce qui est visible sur l'écran
     * */
	Texture *getWindowAsTexture();
private:
	// 100000.0 représente 1 seconde en micro seconde
	const float timePerUpdate = 1000000.0 / UPS;
	const float timePerFrame = 1000000.0 / FPS;
	Clock clock;

	string gameName = "";
	vector<string> playerNames;

    /**
     * Permet de charger tout pleins de Texture à la création du controlleur pour éviter que chque
     * jeux n'ai besoin de le faire avant de se mettre à jouer
     * */
	void loadTextures() const;
    /**
     * C'est cette m"thode qui va dicter quand se feront les mises à jour pouvant entrainer des changements
     * dans le modèle et les mise à jour de ce qui est visible à l'écran en plus de gérer
     * les différents changements d'États
     * */
	void run();
};

#endif