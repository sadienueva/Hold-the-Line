#include "Game.h"
#include <SFML/Audio.hpp>

void Game::initializeTextures()
{
	this->Textures["BULLET"] = new sf::Texture();
	this->Textures["BULLET"]->loadFromFile("Textures/bullet.png");
	this->Textures["ENEMY"] = new sf::Texture();
	this->Textures["ENEMY"]->loadFromFile("Textures/Enemy.png");
}
Game::Game()
{
	this->initializeWindow();
	this->initializeBackground();
	this->initializePlayerChar();
	this->initializeTextures();
	this->initializeSound();
	this->initializeGUI();
}
Game::~Game()
{

	delete this->GameWindow;
	delete this->Player1;

	for (auto &i : this->Textures)
	{
		delete i.second;
	}

	for (auto *i : this->bullets)
	{
		delete i;
	}

	for (auto *e : this->enemies)
	{
		delete e;
	}

	delete this->buffer;
	delete this->sound;
	delete this->lvlupbuffer;
	delete this->lvlupsound;
	delete this->explosionbuffer;
	delete this->explosionsound;
}

void Game::Run()
{

	while (this->GameWindow->isOpen())
	{
		this->Render();

		this->updatePollEvents();
	}
}

void Game::updatePollEvents()
{
	sf::Event sfmlEvent;
	while (this->GameWindow->pollEvent(sfmlEvent))
	{
		if (sfmlEvent.Event::type == sf::Event::Closed)
		{
			this->GameWindow->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			this->GameWindow->close();
		}
	}

	if (this->Player1->getHP() <= 0)
	{
		return;
	}

	this->Player1->Update();
	this->updateCollision();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->Player1->canAttack())
	{
		this->sound->play();
		this->bullets.push_back(new Bullets(this->Textures["BULLET"], this->Player1->getPos().x + 25.f, this->Player1->getPos().y, 0.f, -1.f, 5.f));
	}
	this->updateBullets();

	this->initializeEnemies();

	for (auto &i : enemies)
	{
		i->Update();
	}

	this->updateEnemies();
	this->updateCombat();
	this->updateGUI();
}

void Game::updateCollision()
{
	// left world collision
	if (this->Player1->getBounds().left < 0.f)
	{
		this->Player1->setPosition(0.f, this->Player1->getBounds().top);
	}
	// right world collision
	else if (this->Player1->getBounds().left + this->Player1->getBounds().width >= this->GameWindow->getSize().x)
	{
		this->Player1->setPosition(this->GameWindow->getSize().x - this->Player1->getBounds().width, this->Player1->getBounds().top);
	}
	// barrier collision
	if (this->Player1->getBounds().top < 400.f)
	{
		this->Player1->setPosition(this->Player1->getBounds().left, 400.f);
	}
	// bottom world collision
	else if (this->Player1->getBounds().top + this->Player1->getBounds().height >= this->GameWindow->getSize().y)
	{
		this->Player1->setPosition(this->Player1->getBounds().left, this->GameWindow->getSize().y - this->Player1->getBounds().height);
	}
}

void Game::updateBullets()
{

	unsigned counter = 0;
	for (auto *bullet : this->bullets)
	{
		bullet->Render(this->GameWindow);
		bullet->Update();

		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{

			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateEnemies()
{

	unsigned counter = 0;
	for (auto *enemy : this->enemies)
	{

		if (enemy->getBounds().top > 325.f)
		{
			this->explosionsound->play();
			this->Player1->loseHP(1.f);

			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}
		++counter;
	}
}

void Game::updateCombat()
{
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				this->enemies[i]->takeDamage();
				if (enemies[i]->getHP() == 0.f)
				{
					this->Points += 1;
					delete this->enemies[i];
					this->enemies.erase(this->enemies.begin() + i);
					enemy_deleted = true;
				}
			}
		}
	}
}

void Game::updateGUI()
{
	std::stringstream ss;
	ss << "Points: " << this->Points;
	this->pointText.setString(ss.str());

	float hpPercent = static_cast<float>(this->Player1->getHP()) / this->Player1->getHPMax();
	this->playerHPBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHPBar.getSize().y));
}

void Game::Render()
{
	/*
		Renders under:
		1) clear window, 2) draw items, 3) display window
	*/

	this->GameWindow->clear();

	this->renderBackground();

	this->Player1->Render(*this->GameWindow);

	for (auto *bullet : this->bullets)
	{
		bullet->Render(this->GameWindow);
	}

	for (auto *enemy : this->enemies)
	{
		enemy->Render(this->GameWindow);
	}

	this->renderGUI();

	if (this->Player1->getHP() <= 0)
	{
		this->GameWindow->draw(this->gameoverText);
	}

	this->GameWindow->display();
}

void Game::renderBackground()
{
	this->GameWindow->draw(this->Background);
}

void Game::renderGUI()
{
	this->GameWindow->draw(this->pointText);
	this->GameWindow->draw(this->playerHPBarBack);
	this->GameWindow->draw(this->playerHPBar);
}

void Game::initializeVariable()
{
	this->SpawnTimerMax = 10.f;
	this->SpawnTimer = SpawnTimerMax;
}

void Game::initializeWindow()
{
	this->GameWindow = new sf::RenderWindow(sf::VideoMode(576, 576), "Hold The Line!", sf::Style::Close | sf::Style::Titlebar);
	this->GameWindow->setFramerateLimit(144);
	this->GameWindow->setVerticalSyncEnabled(false);
}

void Game::initializePlayerChar()
{
	this->Player1 = new MainPlayer();
}

void Game::initializeBackground()
{
	this->BackgroundTexture.loadFromFile("Textures/BackgroundGame.png");
	this->Background.setTexture(this->BackgroundTexture);
}

void Game::initializeEnemies()
{
	// spawn enemies
	if (this->Points % 10 == 0 && this->Points != 0)
	{
		this->lvlupsound->play();
		this->Points += 1.f;
		this->SpawnTimerMax -= 0.5f;
	}

	this->SpawnTimer += 0.1f;
	if (this->SpawnTimer >= this->SpawnTimerMax)
	{
		enemies.push_back(new Enemy(this->Textures["ENEMY"], (rand() % this->GameWindow->getSize().x - 20.f), -100.f));
		this->SpawnTimer = 0.f;
	}
}

void Game::initializeSound()
{
	this->buffer = new sf::SoundBuffer;
	this->sound = new sf::Sound;
	this->lvlupbuffer = new sf::SoundBuffer;
	this->lvlupsound = new sf::Sound;
	this->explosionbuffer = new sf::SoundBuffer;
	this->explosionsound = new sf::Sound;

	this->explosionbuffer->loadFromFile("Sounds/Explosion.wav");
	this->explosionsound->setBuffer(*explosionbuffer);

	this->lvlupbuffer->loadFromFile("Sounds/Levelup.wav");
	this->buffer->loadFromFile("Sounds/LaserGun.wav");
	this->lvlupsound->setBuffer(*lvlupbuffer);
	this->sound->setBuffer(*buffer);
}

void Game::initializeGUI()
{
	this->font.loadFromFile("Fonts/AmaticSC-Regular.ttf");

	this->pointText.setPosition(0.f, 400.f);
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(34);
	this->pointText.setFillColor(sf::Color::White);

	this->gameoverText.setFont(this->font);
	this->gameoverText.setCharacterSize(72);
	this->gameoverText.setFillColor(sf::Color::Red);
	this->gameoverText.setString("The Zerg Have Won! Game Over!");
	this->gameoverText.setPosition(
		this->GameWindow->getSize().x / 2.f - this->gameoverText.getGlobalBounds().width / 2.f,
		this->GameWindow->getSize().y / 2.f - this->gameoverText.getGlobalBounds().height / 2.f);

	this->playerHPBar.setSize(sf::Vector2f(400.f, 25.f));
	this->playerHPBar.setFillColor(sf::Color::Cyan);
	this->playerHPBar.setPosition(sf::Vector2f(20.f, 20.f));
	this->playerHPBarBack = this->playerHPBar;
	this->playerHPBarBack.setFillColor(sf::Color(25, 25, 25, 200));
	this->playerHPBarBack.setPosition(sf::Vector2f(20.f, 20.f));
}
