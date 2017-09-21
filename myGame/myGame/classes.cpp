#include "main.h"
#include "classes.h"
//temporär !AM SCHLUSS ENTFERNEN!
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
//

//Konstruktor
Player::Player(std::string n, int hp){
	//Variabeln initialisieren
	name = n;
	
	texture = NULL;

	posX = 0.0;
	posY = 0.0;
	
	width = 100;
	height = 125;

	speedX = 1.0;
	speedY = 2.0;
	speedGrav = 2.0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}
Player::Player(std::string n, int hp, float x, float y) {
	//Variabeln initialisieren
	name = n;

	texture = NULL;
	
	posX = x;
	posY = y;

	width = 0;
	height = 0;

	speedX = 0.0;
	speedY = 0.0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}
//

//Destruktor
Player::~Player(){

}

//Textur laden
void Player::setTexture(SDL_Texture* tex){
	// KOMMENTAR
	texture = tex;
}
//

//Position
void Player::setPosX(float x){
	posX = x;
}
void Player::setPosY(float y){
	posY = y;
}

float Player::getPosX(){
	return posX;
}
float Player::getPosY(){
	return posY;
}
//

//Dimensionen
void Player::setWidth(int w){
	width = w;
}
void Player::setHeight(int h){
	height = h;
}

int Player::getWidth(){
	return width;
}
int Player::getHeight(){
	return height;
}
//

//Geschwindigkeit
void Player::setSpeedX(float sx){
	speedX = sx;
}
void Player::setSpeedY(float sy){
	speedY = sy;
}
void Player::setSpeedGrav(float sg) {
	speedGrav = sg;
}

float Player::getSpeedX(){
	return speedX;
}
float Player::getSpeedY(){
	return speedY;
}
float Player::getSpeedGrav() {
	return speedGrav;
}
//

//Bewegung
void Player::moveLeft() {
	posX -= speedX;
}
void Player::moveRight() {
	posX += speedX;
}

void Player::jump() {
	posY -= speedY;
}
void Player::fall() {
	posY += speedGrav;
}
//

//Leben
void Player::looseHealth(int dmg){
	health -= dmg;
	if (health < 0) {
		health = 0;
	}
}
void Player::kill(){
	health = 0;
	std::cout << name << " died" << std::endl;
}
void Player::revive(){
	health = maxHealth;
	std::cout << name << " was revived" << std::endl;
}
//

//rendern
void Player::render(int x, int y, int w, int h, SDL_Rect * clip, SDL_RendererFlip flip){
	//Renderposition und -dimension 
	SDL_Rect renderQuad = { x, y, width, height };	//Der Quad der gerendert wird (wenn w&h leer bleiben, wird die width&height von der Klasse benutzt)

	//Wenn ein SDL_Rect clip angegeben wird, deren Dimensionen übernehmen
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Wenn w&h angegeben werden, deren Dimensionen übernehmen
	if (w != NULL && h != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//definitiv aufs Bild rendern
	SDL_RenderCopyEx(mainRenderer, texture, clip, &renderQuad, NULL, NULL, flip);
}
//



/* =-=-=-=-=-=-=-=-= CLASS ENEMIES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */


//Konstruktor
Enemies::Enemies(std::string n, int hp) {
	//Variabeln initialisieren
	name = n;

	texture = NULL;

	posX = 0.0;
	posY = 0.0;

	width = 100;
	height = 125;

	speedX = 1.0;
	speedY = 2.0;
	speedGrav = 2.0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}
Enemies::Enemies(std::string n, int hp, float x, float y) {
	//Variabeln initialisieren
	name = n;

	texture = NULL;

	posX = x;
	posY = y;

	width = 0;
	height = 0;

	speedX = 0.0;
	speedY = 0.0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}
//

//Destruktor
Enemies::~Enemies() {

}

//Textur laden
void Enemies::setTexture(SDL_Texture* tex) {
	// KOMMENTAR
	texture = tex;
}
//

//Position
void Enemies::setPosX(float x) {
	posX = x;
}
void Enemies::setPosY(float y) {
	posY = y;
}

float Enemies::getPosX() {
	return posX;
}
float Enemies::getPosY() {
	return posY;
}
//

//Dimensionen
void Enemies::setWidth(int w) {
	width = w;
}
void Enemies::setHeight(int h) {
	height = h;
}

int Enemies::getWidth() {
	return width;
}
int Enemies::getHeight() {
	return height;
}
//

//Geschwindigkeit
void Enemies::setSpeedX(float sx) {
	speedX = sx;
}
void Enemies::setSpeedY(float sy) {
	speedY = sy;
}
void Enemies::setSpeedGrav(float sg) {
	speedGrav = sg;
}

float Enemies::getSpeedX() {
	return speedX;
}
float Enemies::getSpeedY() {
	return speedY;
}
float Enemies::getSpeedGrav() {
	return speedGrav;
}
//

//Bewegung
void Enemies::moveLeft() {
	posX -= speedX;
	if (posX <= 0) {
		posX = 0;
	}
}
void Enemies::moveRight() {
	posX += speedX;
	if (posX >= SCREEN_WIDTH - width) {
		posX = SCREEN_WIDTH - width;
	}
}

void Enemies::jump() {
	posY -= speedY;
}
void Enemies::fall() {
	posY += speedGrav;
}
//

//Leben
void Enemies::looseHealth(int dmg) {
	health -= dmg;
	if (health < 0) {
		health = 0;
	}
}
void Enemies::kill() {
	health = 0;
	std::cout << name << " died" << std::endl;
}
void Enemies::revive() {
	health = maxHealth;
	std::cout << name << " was revived" << std::endl;
}
//

//rendern
void Enemies::render(int x, int y, int w, int h, SDL_Rect * clip, SDL_RendererFlip flip) {
	//Renderposition und -dimension 
	SDL_Rect renderQuad = { x, y, width, height };	//Der Quad der gerendert wird (wenn w&h leer bleiben, wird die width&height von der Klasse benutzt)

													//Wenn ein SDL_Rect clip angegeben wird, deren Dimensionen übernehmen
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Wenn w&h angegeben werden, deren Dimensionen übernehmen
	if (w != NULL && h != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//definitiv aufs Bild rendern
	SDL_RenderCopyEx(mainRenderer, texture, clip, &renderQuad, NULL, NULL, flip);
}

/* =-=-=-=-=-=-=-=-= END CLASS ENEMIES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */



/* =-=-=-=-=-=-=-=-= CLASS TEXTURES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */






Textures::Textures() {
	//Variabeln initialisieren
	texture = NULL;

	width = 0;
	height = 0;
}
Textures::Textures(std::string tex) {
	//Variabeln initialisieren
	loadTexture(tex);	//Textur vom Pfad "tex" laden

	width = 0;
	height = 0;
}

Textures::~Textures() {

}

//Textur laden
bool Textures::loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;		//Die finale Textur

										//die bereits existierende Textur freigeben
	freeTexture();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cerr << "!=!FEHLER!=! Bild konnte nicht geladen werden! error: " << IMG_GetError() << std::endl;
	}
	else {
		//colorKey der Textur
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 190, 170, 150));

		//Textur aus den Pixeln des Surfaces erstellen
		newTexture = SDL_CreateTextureFromSurface(mainRenderer, loadedSurface);
		if (newTexture == NULL) {
			std::cerr << "!=!FEHLER!=! Spielertextur konnte nicht erstellt werden! Error: " << SDL_GetError() << std::endl;
		}
		else {
			//Bilddimensionen aufrufen
			if (width == 0) {
				width = loadedSurface->w;
			}
			if (height == 0) {
				height = loadedSurface->h;
			}
		}
		//Surface wieder freigeben
		SDL_FreeSurface(loadedSurface);
	}

	texture = newTexture;
	return texture != NULL;
}
//

//Textur freigeben
void Textures::freeTexture() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}
//

//Textur abrufen
SDL_Texture* Textures::getTexture() {
	return texture;
}
//



/* =-=-=-=-=-=-=-=-= CLASS ENVIRONMENT =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

Environment::Environment(int hp) {
	posX = 0.0;
	posY = 0.0;

	width = 0;
	height = 0;

	speedX = 0.0;
	speedY = 0.0;
	speedGrav = 0.0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}

Environment::Environment(int hp, float x, float y) {
	posX = x;
	posY = y;

	width = 0;
	height = 0;

	speedX = 0;
	speedY = 0;
	speedGrav = 0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}

Environment::Environment(int hp, int x, int y, int w, int h) {
	posX = x;
	posY = y;

	width = w;
	height = h;

	speedX = 0;
	speedY = 0;
	speedGrav = 0;

	maxHealth = hp;
	health = 0;
	isDead = false;
}

Environment::~Environment() {
	isDead = true;
}

//Textur setzen
void Environment::setTexture(SDL_Texture* tex) {
	//KOMMENTAR
	texture = tex;
}

//Position
void Environment::setPosX(float x) {
	posX = x;
}
void Environment::setPosY(float y) {
	posY = y;
}

float Environment::getPosX() {
	return posX;
}
float Environment::getPosY() {
	return posY;
}

//Dimensionen
void Environment::setWidth(int w) {
	width = w;
}
void Environment::setHeight(int h) {
	height = h;
}

int Environment::getWidth() {
	return width;
}
int Environment::getHeight() {
	return height;
}

//Leben
void Environment::looseHealth(int dmg) {
	health -= dmg;
	//damit health nicht unter 0 sinkt
	if (health < 0) {
		health = 0;
	}
}

void Environment::kill() {
	health = 0;
}

void Environment::revive() {
	//maxHealth wiederherstellen
	health = maxHealth;
}

//Bewegung 
void Environment::moveDown(int speed) {
	posY += (1 * speed);
}

//rendern
void Environment::render(int x, int y, int w, int h, SDL_Rect * clip, SDL_RendererFlip flip) {
	//Renderposition und -dimension 
	SDL_Rect renderQuad = { x, y, width, height };	//Der Quad der gerendert wird (wenn w&h leer bleiben, wird die width&height von der Klasse benutzt)

													//Wenn ein SDL_Rect clip angegeben wird, deren Dimensionen übernehmen
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Wenn w&h angegeben werden, deren Dimensionen übernehmen
	if (w != NULL && h != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//definitiv aufs Bild rendern
	SDL_RenderCopyEx(mainRenderer, texture, clip, &renderQuad, NULL, NULL, flip);
}
void Environment::render(float x, float y, int w, int h, SDL_Rect * clip, SDL_RendererFlip flip) {
	//Renderposition und -dimension 
	SDL_Rect renderQuad = { x, y, width, height };	//Der Quad der gerendert wird (wenn w&h leer bleiben, wird die width&height von der Klasse benutzt)

													//Wenn ein SDL_Rect clip angegeben wird, deren Dimensionen übernehmen
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Wenn w&h angegeben werden, deren Dimensionen übernehmen
	if (w != NULL && h != NULL) {
		renderQuad.w = w;
		renderQuad.h = h;
	}

	//definitiv aufs Bild rendern
	SDL_RenderCopyEx(mainRenderer, texture, clip, &renderQuad, NULL, NULL, flip);
}
//