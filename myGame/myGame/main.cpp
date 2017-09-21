#include "main.h"
#include "classes.h"

#define WALKING_FRAMES_COUNT 4
#define WALKING_ANIMATION_SPEED 6
#define IDLE_FRAMES_COUNT 2
#define	JUMPING_FRAMES_COUNT 6
#define JUMPING_ANIMATION_SPEED 7
#define DUCKING_FRAMES_COUNT 1
#define DUCKING_ANIMATION_SPEED 6
#define JUMP_HEIGHT 20

//Grösse des Levels
const int LEVEL_WIDTH = 0;
const int LEVEL_HEIGHT = 0;
//Grösse des Fensters
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 750;

//Boden
const int GROUND = SCREEN_HEIGHT - 30;

enum State {sIdle, sWalking, sJumping, sFalling, sChange, sDead, sDuck};
State state = sIdle;

bool Init();		//startet SDL und erstellt Fenster, Renderer usw.
bool loadMedia();	//lädt alle Medien
void Close();		//gibt alle Medien frei und beendet SDL und SDL-Subsysteme
//globale Variabeln
SDL_Renderer* mainRenderer = NULL;
SDL_Window* mainWindow = NULL;
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
int hitBoxFlag = 0;

//FRAMES
SDL_Rect walkFrameClip[WALKING_FRAMES_COUNT];
SDL_Rect idleFrameClip[IDLE_FRAMES_COUNT];
SDL_Rect jumpFrameClip[JUMPING_FRAMES_COUNT];
SDL_Rect duckFrameClip[DUCKING_FRAMES_COUNT];
SDL_Rect currentFrame;

//flip-Variable
SDL_RendererFlip flipType = SDL_FLIP_NONE;

//Textur- Objekte erstellen
Textures malePlayer;
Textures malePlayerTest;
Textures femalePlayerTest;
Textures swampyPlayerTest;
Textures groundTexture;
Textures evilTexture;

//Objekte erstellen
Player player1 = { "TadeL", 1000 };
Enemies enemy1 = { "E1", 1000 };
//Hitboxen
SDL_Rect player1_hitbox;

Environment ground = { 1, 0, 730, SCREEN_WIDTH, 50 };
/*======================*/

Environment cube = { 1, 100, -290, 75, 75 };
Environment cube1 = { 1, 300, -200, 50, 50};
Environment cube2 = { 1, 600, -500, 80, 180};
Environment cube3 = {1, 800, 450, 300, 40};

//Kollisionserkennung
bool checkAllCollision();
//Kollision ganh
bool checkCollision(Player player, Environment env);
bool checkCollision(SDL_Rect rect, Environment env);
bool checkCollision(Environment env1, Environment env2);

bool checkCollisionLeft(SDL_Rect rect, Environment env);
bool checkCollisionRight(SDL_Rect rect, Environment env);

//Spielertexturen 
int playerTexture = 0; //0 = male, 1 = female, 2 = knight
const int TEXTURES_COUNT = 3;




int main(int argc, char* args[]) {	
	
	if(!Init()) {
		std::cerr << "!=!FEHLER!=! Initialisierung fehlgeschlagen!" << std::endl;
	}
	else {
		if(!loadMedia()) {
			std::cerr << "!=!FEHLER!=! Laden der Medien fehlgeschlagen!" << std::endl;
		}
		else {
			//Texturen setzen

			ground.setTexture(groundTexture.getTexture());
			cube.setTexture(groundTexture.getTexture());
			cube1.setTexture(groundTexture.getTexture());
			cube2.setTexture(groundTexture.getTexture());
			cube3.setTexture(groundTexture.getTexture());
			
			//--- ---//
			bool quit = false;
			SDL_Event e;
			//state-changer Variabeln
			bool switchTo_sChange = false;
			//veränderbare Dimensionsvariabeln für den Spieler
			int playerRenderWidth,
				playerRenderHeight;
			float	playerRenderPosX,
					playerRenderPosY;
			//Jumpflag (Damit Space losgelassen werden muss bevor erneut gesprungen wird)
			bool canJump = true;
			bool walkAnimation = true;
			int jumpCooldown = 20; //nicht hier ändern

			//FRAMES
			int idleFrame = 0;
			int walkFrame = 0;
			int jumpFrame = 0;
			int duckFrame = 0;
			
			/*=====SPIELER 1=====*/
			player1.setPosX(450);
			player1.setPosY(400);
			player1.setSpeedX(4);	//JumpSpeed
			player1.setSpeedY(7);
			player1.setSpeedGrav(7);
			int jumpHeight = JUMP_HEIGHT;
			/*=====ENEMY 1=====*/
			enemy1.setPosX(1000);
			enemy1.setPosY(SCREEN_HEIGHT - 30 - enemy1.getHeight());
			enemy1.setTexture(evilTexture.getTexture());
			SDL_Rect enemy1_frame;
				enemy1_frame.x = 0;
				enemy1_frame.y = 0;
				enemy1_frame.w = 200;
				enemy1_frame.h = 250;
			/*===================*/

			/* === MAIN LOOP === */
			while(!quit) {
				walkAnimation = true;
				//Eventhandler
				while(SDL_PollEvent(&e)!=0) {
					switch (e.type) {
					//User requested quit
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_KEYDOWN:
						//welche Taste wurde gedrückt?
						switch (e.key.keysym.sym) {
						//User requested quit mit der Escape-Taste
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_e:
							switchTo_sChange = !switchTo_sChange;
							break;
						case SDLK_UP:
							if (state == sChange) {
								playerTexture--;
								if (playerTexture < 0) {
									playerTexture = TEXTURES_COUNT - 1;
								}
							}
							else{
								
							}
							break;
						case SDLK_DOWN:
							//Wenn im sChange- Modus						
							if (state == sChange) {
								playerTexture++;
								if (playerTexture == 3) {
									playerTexture = 0;
								}
							}
							else {
								//state = sDuck;
							}
						}
						break;
					case SDL_KEYUP:
						//welche Taste wurde losgelassen?
						switch (e.key.keysym.sym) {
							//
						}
					}
				}
				const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
				
				if ((currentKeyState[SDL_SCANCODE_DOWN] || currentKeyState[SDL_SCANCODE_S]) && state != sJumping) {
					state = sDuck;
				}

				//Moonwalk deaktivieren
				if ((currentKeyState[SDL_SCANCODE_RIGHT] || currentKeyState[SDL_SCANCODE_D]) && (currentKeyState[SDL_SCANCODE_LEFT] || currentKeyState[SDL_SCANCODE_A])) {
					walkAnimation = false;
				}
				
				if ((currentKeyState[SDL_SCANCODE_RIGHT] || currentKeyState[SDL_SCANCODE_D]) && state != sChange) {
					if (!checkCollisionLeft(player1_hitbox, cube1)&& !checkCollisionLeft(player1_hitbox, cube2)&& !checkCollisionLeft(player1_hitbox, cube)&& !checkCollisionLeft(player1_hitbox, cube3)) {
						player1.moveRight();
					}

					if (player1_hitbox.x+player1_hitbox.w > SCREEN_WIDTH) {
						player1.moveLeft();
					}
					if (walkAnimation) {
						flipType = SDL_FLIP_NONE;
					}else {
						walkFrame = 0;
					}
					if (state != sJumping && state != sFalling) {
						state = sWalking;
					}
					if (currentKeyState[SDL_SCANCODE_LSHIFT]) {
						player1.moveRight();
					}
				}
				if ((currentKeyState[SDL_SCANCODE_LEFT] || currentKeyState[SDL_SCANCODE_A]) && state != sChange) {
					if (!checkCollisionRight(player1_hitbox, cube)&&!checkCollisionRight(player1_hitbox, cube1)&& !checkCollisionRight(player1_hitbox, cube2)&& !checkCollisionRight(player1_hitbox, cube3)) {
						player1.moveLeft();
					}

					if (player1_hitbox.x < 0) {
						player1.moveRight();
					}
					if(walkAnimation){
						flipType = SDL_FLIP_HORIZONTAL;
					}else {
						walkFrame = 0;
					}
					if (state != sJumping && state != sFalling) {
						state = sWalking;
					}
					if (currentKeyState[SDL_SCANCODE_LSHIFT]) {
						player1.moveLeft();
					}
				}
				if (currentKeyState[SDL_SCANCODE_SPACE] && jumpCooldown == 0 && (state == sIdle || state == sWalking)){
					state = sJumping;
					canJump = false;	
					jumpCooldown = 30;
				}

				if (checkAllCollision()) {
					canJump = true;	//damit SPACE losgelassen werden muss bevor erneut gesprungen werden kann
					jumpFrame = 0;
				}

				//State-changer
				if (!(currentKeyState[SDL_SCANCODE_LEFT] || currentKeyState[SDL_SCANCODE_A]) && !(currentKeyState[SDL_SCANCODE_RIGHT] || currentKeyState[SDL_SCANCODE_D]) && !(currentKeyState[SDL_SCANCODE_DOWN] || currentKeyState[SDL_SCANCODE_S]) && state != sJumping && state != sFalling) {
					state = sIdle;
				}
				if (switchTo_sChange) {
					state = sChange;
				}
				else if (state != sJumping && !checkAllCollision()) {
					state = sFalling;
					playerRenderWidth = player1.getWidth();
					playerRenderHeight = player1.getHeight();					
				}
				else if(state != sJumping && state != sFalling && state != sWalking && state != sDuck) {
					state = sIdle;
				}

				
				//STATE EVENT

				switch (state) {
				case sIdle:
					playerRenderWidth = player1.getWidth();
					playerRenderHeight = player1.getHeight();

					playerRenderPosX = player1.getPosX();
					playerRenderPosY = player1.getPosY();

					//AnimationsFrame
					idleFrame++;
					currentFrame = idleFrameClip[idleFrame/25];

					if ((idleFrame+1) / 25 > (IDLE_FRAMES_COUNT-1)) {
						idleFrame = 0;
					}

					walkFrame = 0;
					jumpFrame = 0;

					break;
				case sDuck:
					//Hitbox anpassen
					player1_hitbox.h = 90;
					player1_hitbox.y = player1.getPosY() + 35;

					currentFrame = duckFrameClip[duckFrame];					
					break;
				case sWalking:
					playerRenderWidth = player1.getWidth();
					playerRenderHeight = player1.getHeight();

					playerRenderPosX = player1.getPosX();
					playerRenderPosY = player1.getPosY();

					//Hitbox anpassen
					player1_hitbox.h = player1.getHeight() -100;
					player1_hitbox.y = player1.getPosY() + 10;

					if (walkAnimation) {
						walkFrame++;
					}else {
						walkFrame = 0;
					}

					if (walkFrame > (WALKING_FRAMES_COUNT)*WALKING_ANIMATION_SPEED) {
						walkFrame = 0;
					}
					if (walkFrame / WALKING_ANIMATION_SPEED == WALKING_FRAMES_COUNT) {
						walkFrame = 0;
					}
					currentFrame = walkFrameClip[walkFrame / WALKING_ANIMATION_SPEED];

					jumpFrame = 0;
					break;
				case sJumping:
					
					if (jumpHeight > 0) {
						player1.jump();
						jumpHeight--;
					}
					else{
						state = sFalling;
						jumpHeight = JUMP_HEIGHT;
					}
					//renderVariable aktualisieren
					playerRenderPosX = player1.getPosX();
					playerRenderPosY = player1.getPosY();

					//jump animation
					jumpFrame++;

					if (jumpFrame > (JUMPING_FRAMES_COUNT)*JUMPING_ANIMATION_SPEED) {
						jumpFrame = (JUMPING_FRAMES_COUNT)*JUMPING_ANIMATION_SPEED;
					}
					currentFrame = jumpFrameClip[jumpFrame / JUMPING_ANIMATION_SPEED];
					break;
				case sFalling:
					if (checkAllCollision()) {
						state = sIdle;
						break;
					}
					else {
						player1.fall();
					}
					//renderVariable aktualisieren
					playerRenderPosX = player1.getPosX();
					playerRenderPosY = player1.getPosY();

					//fallanimation

					
					if (jumpFrame < (JUMPING_FRAMES_COUNT - 2)* JUMPING_ANIMATION_SPEED) {
						//dritten Frame ansteuern
						jumpFrame = ((JUMPING_FRAMES_COUNT - 2)* JUMPING_ANIMATION_SPEED);
					}
					jumpFrame++;

					if (jumpFrame > (JUMPING_FRAMES_COUNT)*JUMPING_ANIMATION_SPEED) {
						jumpFrame = (JUMPING_FRAMES_COUNT)*JUMPING_ANIMATION_SPEED;
					}
					currentFrame = jumpFrameClip[(jumpFrame / JUMPING_ANIMATION_SPEED)-1];

					break;
				case sChange:
					playerRenderWidth = player1.getWidth() * 2;
					playerRenderHeight = player1.getHeight() * 2;

					playerRenderPosX = (SCREEN_WIDTH / 2) - (playerRenderWidth / 2);
					playerRenderPosY = (SCREEN_HEIGHT/2) - (playerRenderHeight / 2);
					break;
				}

				//HITBOXEN IM SPRUNG

				//Hitbox anpassen case JUMPING || FALLING
				/*if (state == sJumping || state == sFalling) {
					if (jumpFrame < 1 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight() - 35;
						hitBoxFlag = 30;
					}
					else if (jumpFrame >= 1 * JUMPING_ANIMATION_SPEED && jumpFrame < 2 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight();
						hitBoxFlag = -5;
					}
					else if (jumpFrame >= 2 * JUMPING_ANIMATION_SPEED && jumpFrame < 3 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight() - 60;
						hitBoxFlag = 0;
					}
					else if (jumpFrame >= 3 * JUMPING_ANIMATION_SPEED && jumpFrame < 4 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight() - 40;
						hitBoxFlag = 0;
					}
					else if (jumpFrame >= 4 * JUMPING_ANIMATION_SPEED && jumpFrame < 5 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight() - 40;
						hitBoxFlag = 0;
					}
					else if (jumpFrame >= 5 * JUMPING_ANIMATION_SPEED && jumpFrame < 6 * JUMPING_ANIMATION_SPEED) {
						player1_hitbox.h = player1.getHeight() - 40;
						hitBoxFlag = 0;
					}
					else {
						player1_hitbox.h = player1.getHeight();
						hitBoxFlag = 0;
					}
				}*/

				//TEMPORÄR BIS COLLISIONSERKENNUNG IM SPRUNG FERTIG IST
				if (state != sDuck) {
					player1_hitbox.h = player1.getHeight()-3;
					player1_hitbox.y = player1.getPosY() + hitBoxFlag;
				}

				//STATE EVENT END
				enemy1.moveLeft();

				if (!checkCollision(cube, ground)) {
					cube.moveDown(5);
				}
				if (!checkCollision(cube1, ground)) {
					cube1.moveDown(4);
				}
				if (!checkCollision(cube2, ground)) {
					cube2.moveDown(7);
				}

				if (jumpCooldown > 0) {
					jumpCooldown--;
				}
				
				//TEST AREA

				if (checkCollision(player1_hitbox, cube2)) {
					if (player1_hitbox.y + player1_hitbox.h == cube2.getPosY()) {
						player1_hitbox.y = cube2.getPosY() + player1_hitbox.h;
					}
				}

				//
				//std::cerr << jumpFrame << std::endl;
				//SDL_Delay(100);
				//system("cls");
				//

				//Textur des Spielers wählen 
				switch (playerTexture) {
				case 0:
					player1.setTexture(malePlayer.getTexture());
					break;
				case 1:
					player1.setTexture(femalePlayerTest.getTexture());
					break;
				case 2:
					player1.setTexture(swampyPlayerTest.getTexture());
					break;
				}

				//Hitboxen aktualisieren
				if (flipType == SDL_FLIP_NONE) {
					player1_hitbox.x = player1.getPosX() + (player1.getWidth() / 4);
				}
				else {
					player1_hitbox.x = player1.getPosX() + (player1.getWidth() / 3);
				}
				player1_hitbox.w = 40;

				//Bild claeren (nur solange kein Hintergrund vorhanden ist)
				SDL_RenderClear(mainRenderer);
				
				//aktuellen Frame rendern
				if (state != sChange) {
					ground.render(ground.getPosX(), ground.getPosY(), ground.getWidth(), ground.getHeight(), NULL, flipType);
					cube.render(cube.getPosX(), cube.getPosY(), cube.getWidth(), cube.getHeight(), NULL, flipType);
					cube1.render(cube1.getPosX(), cube1.getPosY(), cube1.getWidth(), cube1.getHeight(), NULL, flipType);
					cube2.render(cube2.getPosX(), cube2.getPosY(), cube2.getWidth(), cube2.getHeight(), NULL, flipType);
					cube3.render(cube3.getPosX(), cube3.getPosY(), cube3.getWidth(), cube3.getHeight(), NULL, flipType);
				}
				enemy1.render(enemy1.getPosX(), enemy1.getPosY()+10, enemy1.getWidth(), enemy1.getHeight(), &enemy1_frame, SDL_FLIP_HORIZONTAL);
				player1.render(playerRenderPosX, playerRenderPosY, playerRenderWidth, playerRenderHeight, &currentFrame, flipType);
	
				if (checkAllCollision()) {
					SDL_SetRenderDrawColor(mainRenderer, 255, 0, 0, 1);
				}
				else {
					SDL_SetRenderDrawColor(mainRenderer, 0, 0, 255, 1);
				}
				SDL_RenderDrawRect(mainRenderer, &player1_hitbox);
				SDL_SetRenderDrawColor(mainRenderer, 200, 200, 255, 1);

				//Bild updaten
				SDL_RenderPresent(mainRenderer);
			}
		}
	}
	return 0;
}

bool Init() {
	bool success = true;
	
	//SDL Subsysteme initialisieren
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "!=!FEHLER!=! SDL konnte nicht initialisiert werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		//SDL Textur filtering auf linear setzen
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			std::cerr << "Warnung: lineare Textur filterung nicht aktiviert!" << std::endl;
		}
		//Fenster erstellen
		mainWindow = SDL_CreateWindow("  =-=-=-=-=-= Tadel's Game =-=-=-=-=-=  ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mainWindow == NULL) {
			std::cerr << "!=!FEHLER!=! Fenster konnte nicht erstellt werden! Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			//Vsynced Renderer erstellen
			mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mainRenderer == NULL) {
				std::cerr << "!=!FEHLER!=! Renderer konnte nicht erstellt werden! Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				//Renderfarbe initialisieren
				SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//.png- Lader initialisieren
				int imgFlag = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlag)&imgFlag)) {
					std::cerr << "!=!FEHLER!=! SDL_image konnte nicht initialisiert werden! Error: " << IMG_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	if (success == false) {
		SDL_Delay(3000);
	}
	return success;
}

bool loadMedia() {
	bool success = true;

	if (!groundTexture.loadTexture("groundTexture.png")) {
		std::cerr << "!=!FEHLER!=! groundTexture konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	//Spieler 1
	if (!malePlayer.loadTexture("playerManAnimated-v2.2.1.png")) {
		std::cerr << "!=!FEHLER!=! malePlayer Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	//FRAMECLIPS festlegen
	idleFrameClip[0].x = 0;
	idleFrameClip[0].y = 0;
	idleFrameClip[0].w = 200;
	idleFrameClip[0].h = 250;
	
	idleFrameClip[1].x = 200;
	idleFrameClip[1].y = 0;
	idleFrameClip[1].w = 200;
	idleFrameClip[1].h = 250;


	walkFrameClip[0].x = 0;
	walkFrameClip[0].y = 251;
	walkFrameClip[0].w = 200;
	walkFrameClip[0].h = 250;

	walkFrameClip[1].x = 200;
	walkFrameClip[1].y = 251;
	walkFrameClip[1].w = 200;
	walkFrameClip[1].h = 250;

	walkFrameClip[2].x = 400;
	walkFrameClip[2].y = 251;
	walkFrameClip[2].w = 200;
	walkFrameClip[2].h = 250;

	walkFrameClip[3].x = 600;
	walkFrameClip[3].y = 251;
	walkFrameClip[3].w = 200;
	walkFrameClip[3].h = 250;
	

	jumpFrameClip[0].x = 0;
	jumpFrameClip[0].y = 502;
	jumpFrameClip[0].w = 200;
	jumpFrameClip[0].h = 250;

	jumpFrameClip[1].x = 200;
	jumpFrameClip[1].y = 502;
	jumpFrameClip[1].w = 200;
	jumpFrameClip[1].h = 250;

	jumpFrameClip[2].x = 400;
	jumpFrameClip[2].y = 502;
	jumpFrameClip[2].w = 200;
	jumpFrameClip[2].h = 250;

	jumpFrameClip[3].x = 600;
	jumpFrameClip[3].y = 502;
	jumpFrameClip[3].w = 200;
	jumpFrameClip[3].h = 250;

	jumpFrameClip[4].x = 800;
	jumpFrameClip[4].y = 502;
	jumpFrameClip[4].w = 200;
	jumpFrameClip[4].h = 250;

	jumpFrameClip[5].x = 1000;
	jumpFrameClip[5].y = 502;
	jumpFrameClip[5].w = 200;
	jumpFrameClip[5].h = 250;

	duckFrameClip[0].x = 600;
	duckFrameClip[0].y = 0;
	duckFrameClip[0].w = 200;
	duckFrameClip[0].h = 250;


	if(!evilTexture.loadTexture("evilPlayer.png")) {
		std::cerr << "!=!FEHLER!=! evilPlayer Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	//TESTS
	if (!malePlayerTest.loadTexture("playerMan.png")) {
		std::cerr << "!=!FEHLER!=! malePlayerTest Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	if (!femalePlayerTest.loadTexture("playerManAnimated.png")) {
		std::cerr << "!=!FEHLER!=! femalePlayerTest Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	if (!swampyPlayerTest.loadTexture("playerSwamp.png")) {
		std::cerr << "!=!FEHLER!=! swampyPlayerTest Textur konnte nicht geladen werden! Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	
	if (success == false) {
		SDL_Delay(3000);
	}
	return success;
}

void Close() {
	//geladene Bilder freigeben

	//Fenster und Renderer zerstören
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	mainWindow = NULL;
	mainRenderer = NULL;
}


bool checkCollision(Player player, Environment env) {
	bool collision = false;

	if (player.getPosX() + player.getWidth() > env.getPosX() && player.getPosX() < env.getPosX() + env.getWidth()) {
		collision = true;
	}
	return collision;
}
//
bool checkCollision(SDL_Rect rect, Environment env) {
	bool collisionX = false;
	bool collisionY = false;
	bool collision = false;

	if (rect.x + rect.w > env.getPosX() && rect.x < env.getPosX() + env.getWidth()) {
		collisionX = true;
	}
	if (rect.y+rect.h-15 <= env.getPosY()) {
		if (rect.y + rect.h > env.getPosY() && rect.y < env.getPosY() + env.getHeight()) {
			collisionY = true;
		}
	}
	if (collisionX && collisionY) {
		collision = true;
	}
	return collision;
}
//
bool checkCollision(Environment env1, Environment env2) {
	bool collisionX = false;
	bool collisionY = false;
	bool collision = false;

	if (env1.getPosX() + env1.getWidth() > env2.getPosX() && env1.getPosX() < env2.getPosX() + env2.getWidth()) {
		collisionX = true;
	}
	if (env1.getPosY() + env1.getHeight() > env2.getPosY() && env1.getPosY() < env2.getPosY() + env2.getHeight()) {
		collisionY = true;
	}
	if (collisionX && collisionY) {
		collision = true;
	}
	return collision;
}
bool checkCollisionLeft(SDL_Rect rect, Environment env){
	bool collision = false;
	
	if (rect.x < env.getPosX()+env.getWidth()/2) {
		if ((rect.y + rect.h - 15 > env.getPosY() && rect.y < env.getPosY() + env.getHeight()) && (rect.x + rect.w > env.getPosX() && rect.x < env.getPosX() + env.getWidth())) {
			collision = true;
		}
	}
	
	return collision;
}
bool checkCollisionRight(SDL_Rect rect, Environment env) {
	bool collision = false;

	if (rect.x > env.getPosX() + env.getWidth() / 2) {
		if ((rect.y + rect.h - 15 > env.getPosY() && rect.y < env.getPosY() + env.getHeight()) && (rect.x < env.getPosX() + env.getWidth() && rect.x + rect.w > env.getPosX())) {
			collision = true;
		}
	}

	return collision;
}
//

bool checkAllCollision() {
	bool collision = false;
	if (checkCollision(player1_hitbox, ground) ||
		checkCollision(player1_hitbox, cube) ||
		checkCollision(player1_hitbox, cube1) ||
		checkCollision(player1_hitbox, cube2) ||
		checkCollision(player1_hitbox, cube3)
		//
		) {
		collision = true;
	}
	return collision;
}