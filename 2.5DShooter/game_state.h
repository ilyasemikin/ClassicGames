#pragma once

#include <cmath>
#include "array2d.h"
#include "game_data.h"
#include "game_map_objects.h"
#include "state.h"

class GameState final : public State {
private:
	const size_t _mapSize = 10;

	const size_t _mapBlockSize = 15;

	const size_t _mainScreenOffset = _mapSize * _mapBlockSize;

	struct {
		float x;
		float y;
		float angle;
		const float fov = 3.14 / 3;
	} _player;

	GameDataRef _data;

	Array2d<GameMapObjects> _map;

	std::pair<size_t, size_t> _getGameScreenSize();
	std::tuple<float, float, float> _calcNearestWall(float angle);
	void _draw();
public:
	GameState(GameDataRef data);
	virtual ~GameState() {}

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
