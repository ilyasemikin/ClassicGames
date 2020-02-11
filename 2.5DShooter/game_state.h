#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "array2d.h"
#include "game_data.h"
#include "game_map_objects.h"
#include "state.h"

class GameState final : public State {
private:
	struct {
		const size_t size = 10;
		const size_t blockSize = 15;
	} _mapProperties;

	const size_t _mainScreenOffset = _mapProperties.size * _mapProperties.blockSize;

	struct {
		float x;
		float y;
		float angle;
		const float fov = M_PI / 3;
	} _player;

	GameDataRef _data;

	Array2d<GameMapObjects> _map;

	void _rotatePlayer(float angle);
	void _movePlayer(float c);

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
