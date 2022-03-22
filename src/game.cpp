#include "game.h"

#include <iostream>
#include <vector>

using namespace std;

namespace life_game {
	Game::Game(int gameWidth, int gameHeight) {
		this->gameWidth = gameWidth;
		this->gameHeight = gameHeight;

		for (int i = 0; i < this->gameWidth; i++) {
			for (int j = 0; j < this->gameHeight; j++) {
				int_pair pos(i,j);
				this->set_status(pos, false);
			}
		}
	}

	Game::~Game() {
	}

	void Game::render_game() {
		for (int i = 0; i < this->gameHeight; i++) {
			for (int j = 0; j < this->gameWidth; j++) {
				int_pair pos(j, i);
				bool cell_is_live = get_status(pos);
				cout << cell_is_live << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << endl;
	}

	// 1． 每个细胞的状态由该细胞及周围八个细胞上一次的状态所决定；
	// 2. 如果一个细胞周围有3个细胞为生，则该细胞为生，即该细胞若原先为死，则转为生，若原先为生，则保持不变；
	// 3. 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
	// 4. 在其它情况下，该细胞为死，即该细胞若原先为生，则转为死，若原先为死，则保持不变
	void Game::update_game() {
		map<int_pair, bool> temp_gamepad = this->gamepad;
		for (int i = 0; i < this->gameWidth; i++) {
			for (int j = 0; j < this->gameHeight; j++) {
				int_pair pos(i, j);
				bool curr_cell_status = this->get_status(pos);
				
				int near_live_cells_num = 0;

				// count near cell status
				for (int x = i - 1; x <= i + 1; x++) {
					for (int y = j - 1;y <= j + 1; y++) {
						if (x >= 0 && y >= 0 && x < gameWidth && y < gameHeight && !(x == i && y == j)) {							
							int_pair near_cell_pos(x, y);
							bool near_cell_status = this->get_status(near_cell_pos);
							if (near_cell_status) near_live_cells_num++;						
						}
					}
				}
				//-------------
				// determine current cell status
				if (near_live_cells_num == 3) {
					set_status(temp_gamepad, make_pair(i, j), true);
				}
				else if (near_live_cells_num == 2) {
					// nothing happens
				}
				else {
					set_status(temp_gamepad, make_pair(i, j), false);
				}				
			}
		}
		this->gamepad = temp_gamepad;
	}

	bool Game::get_status(int_pair pos) {
		bool cell_is_live = gamepad[pos];
		return cell_is_live;
	}

	void Game::set_status(int_pair pos, bool status) {
		this->gamepad[pos] = status;
		
		if (status) {
			if (find(live_cells.begin(), live_cells.end(), pos) == live_cells.end())
				live_cells.push_back(pos);
		}
		else {
			if (find(live_cells.begin(), live_cells.end(), pos) != live_cells.end())
				live_cells.erase(remove(live_cells.begin(), live_cells.end(), pos), live_cells.end());
		}
		
	}

	void Game::set_status(map<int_pair, bool> & gamepad, int_pair pos, bool status) {
		gamepad[pos] = status;
		
		if (status) {
			if (find(live_cells.begin(), live_cells.end(), pos) == live_cells.end())
				live_cells.push_back(pos);
		}
		else {
			if (find(live_cells.begin(), live_cells.end(), pos) != live_cells.end())
				live_cells.erase(remove(live_cells.begin(), live_cells.end(), pos), live_cells.end());
		}
		
	}

	vector<int_pair> Game::get_live_cells() {
		return live_cells;
	}
}