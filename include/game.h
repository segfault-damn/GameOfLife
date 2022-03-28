#pragma once
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <vector>
#include <map>  
#include <utility>

using namespace std;
using int_pair = pair<int, int>;

namespace life_game {
	class Game {
	public:
		Game(int gameWidth, int gameHeight);
		~Game();
		void Game::render_game();

		void Game::update_game();

		void Game::set_status(int_pair pos, bool status);
		void Game::set_status(map<int_pair, bool> & gamepad, int_pair pos, bool status);
		bool Game::get_status(int_pair pos);
		vector<int_pair> Game::get_live_cells();

	private:
		map<int_pair, bool> gamepad;
		vector<int_pair> live_cells;
		int gameWidth;
		int gameHeight;
	};

}

#endif GAME_H