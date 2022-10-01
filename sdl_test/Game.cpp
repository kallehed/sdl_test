#include "Game.h"
#include "Npc.h"

void draw_circle(SDL_Renderer* renderer, float x, float y, float radius, SDL_Color c)
{
	constexpr int points = 31; // points on circle radius
	constexpr int total_vertices = points + 1;
	constexpr int total_indices = points * 3;
	
	SDL_Vertex verts[total_vertices];
	// first is middle
	verts[0].position = { x, y };
	verts[0].color = c;

	// rest of vertices, circle around
	{
		float deg = 0.f;
		constexpr float deg_increase = (2.f * (float)M_PI) / (float)points;
		for (int i = 1; i < total_vertices; ++i) {
			verts[i].position = { x + cosf(deg) * radius, y + sinf(deg) * radius };
			verts[i].color = c;
			deg += deg_increase;
		}
	}

	int indices[total_indices];
	// last first
	indices[0] = points;
	indices[1] = 0;
	indices[2] = 1;

	// rest of indices
	{
		int vert_index = 1;
		for (int i = 3; i < total_indices; i += 3) {
			indices[i] = vert_index;
			indices[i + 1] = 0;
			indices[i + 2] = vert_index + 1;
			++vert_index;
		}
	}

	SDL_RenderGeometry(renderer, NULL, verts, total_vertices, indices, total_indices);
}


SDL_Texture* Game::loadTexture(const char* path)
{
	SDL_Texture* newTexture = IMG_LoadTexture(_renderer, path);
	if (newTexture == NULL) {
		std::cout << "failed loading texture: " << path << "\n";
		std::cin.get();
	}
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

	return newTexture;
}

void Game::changeScale(int change)
{
	//_scale = std::min(12, std::max(_scale + change, 1));
	_scale = std::max(_scale + change, 1);

	float r_scale = _scale * _scale_granularity;
	SDL_SetWindowSize(_window, (int)(_WIDTH * r_scale), (int)(_HEIGHT * r_scale));
	SDL_RenderSetScale(_renderer, (float)r_scale, (float)r_scale);
}

Uint32 Game::getMouseState(int* x, int* y)
{
	float r_scale = _scale * _scale_granularity;
	Uint32 buttons = SDL_GetMouseState(x, y);
	(*x) = (int)((*x)/r_scale); // correctly place mouse position when window is bigger
	(*y) = (int)((*y)/r_scale);

	return buttons;
}

void Game::play_music(MUS::_ music)
{
	if (music == (MUS::_)(-1)) {
		Mix_HaltMusic();
		_current_music = music;
	}
	else {
		if (music != _current_music) {
			// music
			_current_music = music;
			Mix_PlayMusic(_music[_current_music], -1);
		}
	}
}

void Game::play_sound(SOUND::_ sound)
{
	Mix_PlayChannel(-1, _sound[sound], 0);
}

Game::Game()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		std::cin.get();
	}
	_window = SDL_CreateWindow("Legend of the Banana Man",
		DEV::DEV ? 50 : SDL_WINDOWPOS_UNDEFINED, DEV::DEV ? 50 : SDL_WINDOWPOS_UNDEFINED,
		_WIDTH, _HEIGHT,
		SDL_WINDOW_SHOWN);
	SDL_SetWindowSize(_window, _WIDTH, _HEIGHT);
	
	//_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	TTF_Init();
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		std::cin.get();
	}

	_font = TTF_OpenFont("data/fonts/8bitoperator.ttf", 12);
	if (!_font) {
		std::cout << "Error loading font: " << TTF_GetError() << std::endl;
		std::cin.get(); // stop
	}

	{ // load images
		constexpr const char* paths[TEX::TOTAL] = {
			"data/images/FireMagic.png",
			"data/images/BombExplosion.png",
			"data/images/Bomb.png",
			"data/images/BombRed.png",
			"data/images/Bullet.png",
			"data/images/MagicOrb.png",
			"data/images/BlueSlime.png",
			"data/images/Wizard.png",
			"data/images/GreenSlime1.png",
			"data/images/GreenSlime2.png",
			"data/images/RedBear.png",
			"data/images/RedHuman.png",
			"data/images/RedHumanWalk1.png",
			"data/images/RedHumanWalk2.png",
			"data/images/RedHumanBackwards.png",
			"data/images/RedHumanBackwardsWalk1.png",
			"data/images/RedHumanBackwardsWalk2.png",
			"data/images/Coin.png",
			"data/images/Container.png",
			"data/images/DialogueBox.png",

			"data/images/Bush.png",
			"data/images/Bush2.png",
			"data/images/TreeStump.png",
			"data/images/SmallTree1.png",
			"data/images/SmallTree2.png",
			"data/images/SquareGreen.png",
			"data/images/SquareStone.png",
			"data/images/SquareSwamp.png",
			"data/images/CobbleStone.png",
			"data/images/CobbleWall.png",
			"data/images/FlowerRed.png",
			"data/images/FlowerBlue1.png",
			"data/images/FlowerBlue2.png",

			"data/images/Portal.png",
			"data/images/Heart.png",
			"data/images/Bonfire.png",
			"data/images/Chest.png",
			"data/images/Gnome.png",
			"data/images/GuyBlonde.png",
			"data/images/FullGreen.png",
			"data/images/SlimeSad.png",
			"data/images/BushBerry.png",
			"data/images/BossBody.png",
			"data/images/BossHead.png",
			"data/images/Needle.png",
			"data/images/BossHeadBite.png",
			"data/images/GirlGreenDress.png",
			"data/images/Cactus.png",
			"data/images/CactusAngry.png",
			"data/images/BombMan.png",
			"data/images/TreeDude.png",
			"data/images/HeartContainer.png",
			"data/images/BombMan2.png",
			"data/images/BananaMan.png",
			"data/images/Mosquito.png",
			"data/images/GreenSlime12.png",
			
		};
		for (int i = 0; i < TEX::TOTAL; ++i) {
			_textures[i] = loadTexture(paths[i]);
		}
	}

	// Load music
	{
		constexpr const char* paths[MUS::TOTAL] = {
			"data/music/WeirdPiece.wav",
			"data/music/FirstBossPiece.wav",
			"data/music/FinalBossPiece.wav",
			"data/music/NicePiece.wav",
			"data/music/RestaurantPiece.wav",
			"data/music/ChillPiece.wav",
			"data/music/FirstPiece.wav",
			"data/music/WinPiece.wav",
			"data/music/HarderPiece.wav",
			"data/music/SolemnPiece.wav",
			"data/music/DiePiece.wav",
		};
		for (int i = 0; i < MUS::TOTAL; ++i) {
			_music[i] = Mix_LoadMUS(paths[i]);
			if (_music[i] == NULL) {
				printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
				std::cin.get();
			}
		}
	}
	// load sound effects
	{
		constexpr const char* paths[SOUND::TOTAL] = {
			"data/music/sound/Coin.wav",
			"data/music/sound/Heal.wav",
			"data/music/sound/Fire.wav",
			"data/music/sound/Bonfire.wav",
			"data/music/sound/Shoot.wav",
			"data/music/sound/GetItem.wav",
			"data/music/sound/EnemyHit.wav",
			"data/music/sound/PlayerHit.wav",
			"data/music/sound/Speak.wav",
			"data/music/sound/Explosion.wav",
			"data/music/sound/Buy.wav",
			"data/music/sound/BuyFail.wav",
			"data/music/sound/Portal.wav",
			"data/music/sound/Fuse.wav",
		};

		for (int i = 0; i < SOUND::TOTAL; ++i) {
			_sound[i] = Mix_LoadWAV(paths[i]);
			if (_sound[i] == NULL) {
				printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
				std::cin.get();
			}
		}

	}
	Mix_VolumeMusic(90);

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	_tile_handler.TileHandler_construct(*this);
	_cam.construct(*this);

	// Press E texture
	SDL_Surface* surface = TTF_RenderText_Solid(_font, "Press E", {0,0,0,255});
	_press_e_texture = SDL_CreateTextureFromSurface(_renderer, surface);
	_press_e_w_and_h = { surface->w, surface->h };
	SDL_FreeSurface(surface);
	
	// at 1 or 2, makes text very blurry.
	
	//SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1");
	changeScale(0);

	_cam.load_from_file(*this, _level);
}

void Game::start_game()
{
	game_loop();
}

void Game::close_game()
{
	for (int i = 0; i < SOUND::TOTAL; ++i) {
		Mix_FreeChunk(_sound[i]);
		_sound[i] = NULL;
	}
	for (int i = 0; i < MUS::TOTAL; ++i) {
		Mix_FreeMusic(_music[i]);
		_music[i] = NULL;
	}

	TTF_CloseFont(_font);
	_font = NULL;

	for (int i = 0; i < TEX::TOTAL; ++i) {
		SDL_DestroyTexture(_textures[i]);
	}
	SDL_DestroyTexture(_press_e_texture);

	SDL_DestroyRenderer(_renderer);
	_renderer = NULL;
	SDL_DestroyWindow(_window);
	_window = NULL;

	Mix_Quit();
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
		_mouse_scroll = 0;
		for (int i = 0; i < _keys_frame.size(); ++i) _keys_frame[i] = false;

		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode < _KEY_BOOLS) {
					if (_keys_down[e.key.keysym.scancode] == false) {
						_keys_frame[e.key.keysym.scancode] = true;
					}
					_keys_down[e.key.keysym.scancode] = true;
				}
			}
			else if (e.type == SDL_KEYUP) {
				if (e.key.keysym.scancode < _KEY_BOOLS) {
					_keys_down[e.key.keysym.scancode] = false;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				
				int x, y;
				Uint32 buttons;
				buttons = getMouseState(&x, &y);
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

		// IMPORTANT EVENTS, FROM CLICKING
		//if (_keys_frame[SDL_SCANCODE_ESCAPE]) { running = false; }

		if constexpr (DEV::DEV) {
			if (_keys_frame[SDL_SCANCODE_K]) {
				if (_edit_mode == false) {
					_edit_mode = true;
					_save.reset();
					_cam.load_from_file(*this, _level);
				}
				else {
					_edit_mode = false;
					_cam.save_to_file(*this);
				}
			}
		}
		if (_keys_frame[SDL_SCANCODE_O]) { // scale change
			changeScale(-1);
		}
		if (_keys_frame[SDL_SCANCODE_P]) {
			changeScale(1);
		}

		game_logic();

		if (_edit_mode) game_draw<true>();
		else game_draw<false>();
		
		// change level possibly
		if (_change_level) {
			_cam.load_from_file(*this, _level_to_change_to);
			_change_level = false;
		}
		// possibly enter state of "YOU DIED"
		if (_entity_handler._p._respawn) 
		{
			play_music(MUS::Die_Piece);
			Mix_PlayMusic(_music[MUS::Die_Piece], 0);
			for (int _ = 0; _ < 175; ++_) {
				SDL_Delay(20);
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				SDL_RenderClear(_renderer);
				int text_x = _WIDTH / 4;
				int text_y = _HEIGHT / 4;
				if (_ > 25)
				{
					_cam.draw_text(*this, "You died.", { 0,0,0,255 }, text_x, text_y, 5);
					if (_ > 60)
					{
						_cam.draw_text(*this, "Sad.", { 0,0,0,255 }, text_x+100, text_y+100, 4);
						if (_ > 90)
						{
							_cam.draw_text(*this, "OH and also you lose half of your money now :(", { 0,0,0,255 }, text_x-260, text_y + 200, 3);
						}
					}
				} 

				SDL_RenderPresent(_renderer);
			}
			// actually respawn
			_cam.load_from_file(*this, _entity_handler._p._respawn_level);
			_entity_handler._p.set_x(_entity_handler._p._respawn_x);
			_entity_handler._p.set_y(_entity_handler._p._respawn_y);
			
			_entity_handler._p._respawn = false;
			_entity_handler._p._alive = true;
			_entity_handler._p._hp = _entity_handler._p._max_hp;
			_entity_handler._p._coins /= 2;

			_change_level = false; // JUST IN CASE
		}

		//std::cout << std::endl << "frame time MS " << _dt << std::endl;
		if (_ticks % 60 == 0) {
			//if (_dt != 0.f) std::cout << "fps: " << (int)(1000 / (_dt)) << std::endl;
		}
		_dt = 1000.f * (SDL_GetPerformanceCounter() - start_time) / ((float)SDL_GetPerformanceFrequency());
		SDL_Delay(std::min((Uint32)30,std::max((Uint32)0,(Uint32)floor(16.666f - _dt))));
		//_dt = std::min(_MAX_DT, _dt); // no less than 50 fps simulated.
		_dt = _MAX_DT;
		_dt *= _slow_motion_factor;
		_slow_motion_factor += 0.02f;
		_slow_motion_factor = std::min(1.f, _slow_motion_factor);

		++_ticks;
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

template <bool EDIT>
void Game::game_draw()
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	if constexpr (EDIT) { // exclusive edit-things-to-draw
		_cam.draw_grid(*this); // grid
	}
	_tile_handler.draw(*this);

	_entity_handler.draw(*this);

	if constexpr (!EDIT) {
		_cam.draw_play(*this);
	}

	if constexpr (EDIT) { // exclusive edit-things-to-draw
		_cam.draw_edit_text(*this); // grid
	}

	SDL_RenderPresent(_renderer);
}