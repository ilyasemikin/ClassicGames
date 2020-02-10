#pragma once

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
	} _player;

	GameDataRef _data;

	Array2d<GameMapObjects> _map;

	std::pair<float, float> _calcNearestWall();
	void _drawMap();
public:
	GameState(GameDataRef data);
	virtual ~GameState() {}

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
