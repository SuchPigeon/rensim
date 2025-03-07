#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define HEIGHT 480
#define WIDTH 620

#define CHUNKS 8
#define CHUNK_SIZE 100

int ABSMOD(int x, int y) {
	return ((x%y)+y)%y;
}

struct {
	double x;
	double y;
} player;

struct {
	double x;
	double y;
} camera;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
TTF_Font *font = nullptr;

int initGame() {
	// SDL Init Error
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    window = SDL_CreateWindow("[TUTORIAL STAGE]RenSim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("FiraCode-Regular.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
		return 1;
	}

	return 0;
}

void renderGame() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Print GRID
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for(int ih = 0; ih < HEIGHT; ih++) {
		for(int iw = 0; iw < WIDTH; iw++) {
			if( ABSMOD(ih+5+static_cast<int>(camera.y), CHUNK_SIZE) <= 5 &&
				ABSMOD(iw+5+static_cast<int>(camera.x), CHUNK_SIZE) <= 5)
				SDL_RenderDrawPoint(renderer, iw, ih);
		}
	}

	for(int wChunk = 0; wChunk < 30; wChunk++) {
		for(int hChunk = 0; hChunk < 30; hChunk++) {
			int wChunkBeg = wChunk*CHUNK_SIZE;
			int hChunkBeg = hChunk*CHUNK_SIZE;

			SDL_Surface *textSurface = TTF_RenderText_Solid(font, (std::to_string(wChunk)+"@"+std::to_string(hChunk)).c_str(), {255,0,255});
			SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			SDL_Rect textRect = {wChunkBeg-camera.x, hChunkBeg-camera.y, textSurface->w, textSurface->h};
			SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		}
	}

	SDL_RenderPresent(renderer);
}

int main() {
	// Close if initialization exited with error
	if(initGame() != 0) return 1;

	int status = 1;
	SDL_Event event;

	player.x = player.y = 0.;

	while(status) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				status = 0;
			} else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_w:
						camera.y += 1.;
						break;
					case SDLK_s:
						camera.y -= 1.;
						break;
					case SDLK_a:
						camera.x -= 1.;
						break;
					case SDLK_d:
						camera.x += 1.;
						break;
					default:
						break;
				}
			}
		}

		renderGame();

		SDL_Delay(1000. / 30.); // 60 FPS cap
		
	}

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
