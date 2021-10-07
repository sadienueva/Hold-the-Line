#pragma once
#include "MainPlayer.h"
#include <map>
#include "Bullets.h"
#include <SFML/Audio.hpp>

class Game
{
private:
	sf::RenderWindow *GameWindow;

	MainPlayer *Player1;

	std::map<std::string, sf::Texture *> Textures;
	std::vector<Bullets *> bullets;
	std::vector<Enemy *> enemies;

	sf::SoundBuffer *buffer;
	sf::Sound *sound;
	sf::SoundBuffer *lvlupbuffer;
	sf::Sound *lvlupsound;
	sf::SoundBuffer *explosionbuffer;
	sf::Sound *explosionsound;

	sf::Sprite Background;
	sf::Texture BackgroundTexture;
	float MovementSpeed;
	float SpawnTimerMax = 20.f;
	float SpawnTimer = SpawnTimerMax;

public:
	Game();
	virtual ~Game();

	void Run();

	void updatePollEvents();
	void updateCollision();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void updateGUI();

	sf::Font font;
	sf::Text pointText;
	sf::Text gameoverText;

	sf::RectangleShape playerHPBar;
	sf::RectangleShape playerHPBarBack;

	unsigned Points = 0;

	void Render();
	void renderBackground();
	void renderGUI();

	void initializeTextures();
	void initializeVariable();
	void initializeWindow();
	void initializePlayerChar();
	void initializeBackground();
	void initializeEnemies();
	void initializeSound();
	void initializeGUI();
};
