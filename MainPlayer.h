#pragma once

#include "Enemy.h"
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <SFML/Audio.hpp>

class MainPlayer
{
private:
	sf::Sprite Sprite;
	sf::Texture Texture;

	float attackCooldown;
	float attackCooldownMax;

	int HP;
	int HPMAX;

public:
	MainPlayer();
	virtual ~MainPlayer();

	const bool canAttack();

	void setPosition(const sf::Vector2f pos); // functions overloaded intentionally
	void setPosition(const float x, const float y);
	void setHP(const int hp);
	void loseHP(const int losthp);

	const sf::Vector2f &getPos() const;
	const sf::FloatRect getBounds() const;
	const int &getHP() const;
	const int &getHPMax() const;

	void initializeVariables();
	void initializeTexture();
	void initializeSprite();

	void Update();
	void updateMovement();
	void updateCooldown();

	void Render(sf::RenderTarget &Target);
};
