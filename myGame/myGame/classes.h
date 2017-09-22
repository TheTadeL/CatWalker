#pragma once

#include <iostream>
#include "../../SDL2-2.0.5/include/SDL.h"
#include "../../SDL2_image-2.0.1/include/SDL_image.h"
#include <string>

class Player {
public:
	//Konstruktor
	Player(std::string n, int hp);
	Player(std::string n, int hp,float x, float y);
	
	//Destruktor
	~Player();

	//Textur setzen
	void	setTexture(SDL_Texture* tex);

	//Position
	void	setPosX(float x),
			setPosY(float y);
	float	getPosX(),
			getPosY();

	//Dimesionen
	void	setWidth(int w),
			setHeight(int h);
	int		getWidth(),
			getHeight();

	//Geschwindigkeit
	void	setSpeedX(float sx),
			setSpeedY(float sy),
			setSpeedGrav(float sg);
		
	float	getSpeedX(),
			getSpeedY(),
			getSpeedGrav();

	//Bewegung
	void moveLeft();
	void moveRight();
	void jump();
	void fall();

	//Leben
	void	looseHealth(int dmg);
	void	kill();
	void	revive();

	//rendern
	void render(int x, int y, int w, int h, SDL_Rect* clip, SDL_RendererFlip flip);

protected:
	//Name
	std::string name;
	
	//Position
	float posX,
		  posY;

	//Dimension
	int	width,
		height;

	//Geschwindigkeit
	float	speedX,
			speedY,
			speedGrav;	//gravitation

	//Leben
	int maxHealth,
		health;
	bool isDead;

	//Textur
	SDL_Texture* texture;
};		
/* =-=-=-=-=-=-=-=-= END Player =-=-=-=-=-=-=-=-= */

class Enemies {
public:
	//Konstruktor
	Enemies(std::string n, int hp);
	Enemies(std::string n, int hp, float x, float y);

	//Destruktor
	~Enemies();

	//Textur setzen
	void	setTexture(SDL_Texture* tex);

	//Position
	void	setPosX(float x),
			setPosY(float y);
	float	getPosX(),
			getPosY();

	//Dimesionen
	void	setWidth(int w),
			setHeight(int h);
	int		getWidth(),
			getHeight();

	//Geschwindigkeit
	void	setSpeedX(float sx),
			setSpeedY(float sy),
			setSpeedGrav(float sg);

	float	getSpeedX(),
			getSpeedY(),
			getSpeedGrav();

	//Bewegung
	void moveLeft();
	void moveRight();
	void jump();
	void fall();

	//Leben
	void	looseHealth(int dmg);
	void	kill();
	void	revive();

	//Kollisionserkennung

	//rendern
	void render(int x, int y, int w, int h, SDL_Rect* clip, SDL_RendererFlip flip);

protected:
	//Name
	std::string name;

	//Position
	float	posX,
			posY;

	//Dimension
	int	width,
		height;

	//Geschwindigkeit
	float	speedX,
			speedY,
			speedGrav;	//gravitation

	//Leben
	int maxHealth,
		health;
	bool isDead;

	//Textur
	SDL_Texture* texture;
};

/* =-=-=-=-=-=-=-=-= Enemies Klasse =-=-=-=-=-=-=-=-= */
/* =-=-=-=-=-=-=-=-= END Enemies =-=-=-=-=-=-=-=-= */

/* =-=-=-=-=-=-=-=-= Textures Klasse =-=-=-=-=-=-=-=-= */
class Textures {
public:
	//Konstruktor
	Textures();
	Textures(std::string tex);

	//Destruktor
	~Textures();
	
	//Dimesionen
	void	setWidth(int w),
			setHeight(int h);
	int		getWidth(),
			getHeight();
	
	//Textur
	bool	loadTexture(std::string path);
	void	freeTexture();
	SDL_Texture* getTexture();

protected:
	SDL_Texture* texture;
	
	int width,
		height;
};
/* =-=-=-=-=-=-=-=-= END Textures =-=-=-=-=-=-=-=-= */

/* =-=-=-=-=-=-=-=-= Environment Klasse =-=-=-=-=-=-=-=-= */
class Environment {
public:
	//Konstruktor
	Environment(int hp);
	Environment(int hp, float x, float y);
	Environment(int hp, int x, int y, int w, int h);
	
	//Destruktor
	~Environment();

	//Textur setzen
	void	setTexture(SDL_Texture* tex);

	//Position
	void	setPosX(float x),
			setPosY(float y);
	float	getPosX(),
			getPosY();

	//Dimesionen
	void	setWidth(int w),
			setHeight(int h);
	int		getWidth(),
			getHeight();


	//Leben
	void	looseHealth(int dmg);
	void	kill();
	void	revive();

	//Bewegung
	void moveDown(int speed);

	//rendern
	void render(int x, int y, int w, int h, SDL_Rect* clip, SDL_RendererFlip flip);
	void render(float x, float y, int w, int h, SDL_Rect* clip, SDL_RendererFlip flip);

protected:
	//Name
	std::string name;

	//Position
	float	posX,
			posY;

	//Dimension
	int	width,
		height;

	//Geschwindigkeit
	float	speedX,
			speedY,
			speedGrav;	//gravitation

	//Leben
	int maxHealth,
		health;
	bool isDead;

	//Textur
	SDL_Texture* texture;
};
/* =-=-=-=-=-=-=-=-= END Environment =-=-=-=-=-=-=-=-= */
