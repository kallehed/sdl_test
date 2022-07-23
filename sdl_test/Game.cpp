#include "Game.h"

Game::Game()
{
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow("SDLtest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _WIDTH, _HEIGHT, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
}

void Game::start_game()
{
	game_loop();
}

void Game::close_game()
{
	TTF_CloseFont(_font);
	_font = NULL;

	//for (int i = 0; i < TEXTURE_TOTAL; ++i) {
	//	SDL_DestroyTexture(gTextures[i]);
	//	gTextures[i] = NULL;
	//}

	SDL_DestroyRenderer(_renderer);
	_renderer = NULL;
	SDL_DestroyWindow(_window);
	_window = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::game_loop()
{
	bool running = true;
	SDL_Event e;
	while (running) {
		Uint64 start_time = SDL_GetPerformanceCounter();

		// reset variables only relevant for the frame
		_mouse_btn_pressed_this_frame = { false, false, false };

		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_k: // change edit mode bool
					_edit_mode = !_edit_mode;
					break;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				
				int x, y;
				Uint32 buttons;
				buttons = SDL_GetMouseState(&x, &y);
				_mouse_pos_on_latest_press = { x, y };

				if ((buttons & SDL_BUTTON_LMASK) != 0) { // left mouse pressed
					
					std::cout << " left mouse pressed \n";
					_mouse_btn_pressed_this_frame[0] = true;
				}
				else if ((buttons & SDL_BUTTON_MMASK) != 0) { // middle mouse pressed

					std::cout << " middle mouse pressed MMMMMMMM \n";
					_mouse_btn_pressed_this_frame[1] = true;

				}
				else if ((buttons & SDL_BUTTON_RMASK) != 0) { // right mouse pressed
					
					std::cout << " right mouse pressed \n";
					_mouse_btn_pressed_this_frame[2] = true;	
				}
			}
			if (e.type == SDL_MOUSEWHEEL)
			{
				_mouse_scroll = e.wheel.y;
			}
		}

		game_logic();

		game_draw();

		//std::cout << std::endl << "frame time MS " << _dt << std::endl;
		if (_dt != 0.f) std::cout << "fps: " << (int)(1000 / (_dt)) << std::endl;
		_dt = 1000.f*(SDL_GetPerformanceCounter() - start_time)/((float)SDL_GetPerformanceFrequency());
		_dt = std::min(_MAX_DT, _dt); // no less than 30 fps simulated.
		
	}
}

void Game::game_logic()
{
	if (!_edit_mode) { // play mode

		_entity_handler.logic(*this);

		_cam.play_logic(*this);
	}
	else { // edit mode
		_cam.edit_logic(*this);
	}
}

void Game::game_draw()
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	if (_edit_mode) { // exclusive edit-things-to-draw
		_cam.draw_grid(*this); // grid
	}

	_tile_handler.draw(*this);

	_entity_handler.draw(*this);

	if (!_edit_mode) {
		_cam.draw_hud(*this);
	}

	/*SDL_Rect srcrect = {0, 0 , 300, 300};
	SDL_RenderCopy(gRenderer, gTextures[image_to_display], &srcrect, &dstrect);
	*/

	SDL_RenderPresent(_renderer);
}
