#include "Game.h"

//Key press surfaces constants
enum TEXTURES
{
	TEXTURE_1,
	TEXTURE_2,
	TEXTURE_3,
	TEXTURE_TOTAL
};

//bool loadMedia();

//SDL_Texture* loadTexture(std::string path);

/*
bool loadMedia() {
	bool success = true;

	gTextures[TEXTURE_1] = loadTexture("images/image1.bmp");
	gTextures[TEXTURE_2] = loadTexture("images/image2.bmp");
	gTextures[TEXTURE_3] = loadTexture("images/image3.png");

	gFont = TTF_OpenFont("fonts/8bitoperator.ttf", 12);
	if (gFont == NULL) {
		std::cout << "font file failed\n";
		success = false;
	}
	else {
		// load text
		SDL_Color textColor = { 0,0,0 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, "The quick brown fox jumps over the lazy dog", textColor);
		gText = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		SDL_FreeSurface(textSurface);
	}

	return success;

}

SDL_Texture* loadTexture(std::string path) {

	SDL_Texture* newTexture = IMG_LoadTexture(gRenderer, path.c_str());
	if (newTexture == NULL) {
		std::cout << "failed loading texture: " << path << "\n";
	}


	return newTexture;
}*/
int main(int argc, char* args[])
{
	Game game;
	game.start_game();
	game.close_game();

	return 0;
}