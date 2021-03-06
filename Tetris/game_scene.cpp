#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include "config_parser.h"
#include "png_image.h"
#include "string_operations.h"
#include "game_scene.h"

GameScene::GameScene(const std::string &configFile, const std::string &figuresFile) :
	generator(rd())
{
	loadConfigFromFile(configFile);
	loadFiguresFormFile(figuresFile);

	initGame();
}

GameScene::GameScene(size_t m, size_t n, const std::string &figuresFile) : 
	fieldLines(m),
	fieldColumns(n),
	field(m, std::vector<PointState>(n, PointState())),
	generator(rd()),
	latency(0.4)
{
	loadConfigFromFile(figuresFile);

	initGame();
}

GameScene::~GameScene() {

}

void GameScene::loadConfigFromFile(const std::string &path) {
	auto params {
		Config::getListParams(path, { "FieldWidth", "FieldHeight", "Latency" } )
	};
	auto width { Config::getParam<size_t>(params["FieldWidth"]) };
	auto height { Config::getParam<size_t>(params["FieldHeight"]) };
	latency = Config::getParam<float>(params["Latency"]);

	fieldLines = height;
	fieldColumns = width;
	
	field = std::vector<std::vector<PointState>>(
		fieldLines, std::vector<PointState>(fieldColumns)
	);
}

void GameScene::loadFiguresFormFile(const std::string &path) {
	std::ifstream input(path);
	std::string line;
	while(std::getline(input, line)) {
		if (line.length() == 0 || line[0] == '#')
			continue;
		auto lines { StringOperations::Split(line, " ") };
		std::vector<std::vector<PointState>> figure (
			lines.size(), 
			std::vector<PointState>(lines[0].size(), PointState())
		);
		for (size_t i = 0; i < lines.size(); i++)
			for (size_t j = 0; j < lines[0].size(); j++)
				if (lines[i][j] != '0')
					figure[i][j].isFilled = true;
		figures.push_back(figure);
	}
}

void GameScene::initGame() {
	score = 0;
	scorePart = 0.05;

	isSceneOver = false;

	initTextures();
	initFonts();

	generateFigure();
}

void GameScene::initTextures() {
	blockTexturePath = "Textures/blocks.png";
	backTexturePath = "Textures/back.png";
	
	// Load block textures
	auto textureSize { PNGImage::getSize(blockTexturePath) };
	
	if (textureSize.width == 0 || textureSize.height == 0)
		throw "";

	if (textureSize.width < textureSize.height 
	    && textureSize.width % textureSize.height != 0)
		throw "";

	blockTextureSize = textureSize.height;
	size_t count { textureSize.width / blockTextureSize };
	blockTextures.resize(count);

	for (size_t i = 0; i < count; i++) {
		auto isLoaded {
			blockTextures[i].loadFromFile(
				blockTexturePath,
				sf::IntRect(
					i * blockTextureSize, 
					0, 
					blockTextureSize,
					blockTextureSize
				)
			)
		};
		if (!isLoaded) {
			std::cerr << "Can't load texture" << std::endl;
			throw "";
		}
	}

	// Load background texture
	textureSize = PNGImage::getSize(backTexturePath);

	if (textureSize.width == 0 || textureSize.height == 0)
		throw "";

	backTextureSize = textureSize.height;
	backTexture.loadFromFile(backTexturePath);
}

void GameScene::initFonts() {
	scoreFontPath = "Fonts/Roboto-Black.ttf";
	if (!scoreFont.loadFromFile(scoreFontPath)) {
		std::cerr << "" << std::endl;
		throw "";
	}
}

std::vector<size_t> GameScene::getFigureBottonBorder() {
	std::vector<size_t> ret(figure.width);
	for (size_t j = 0; j < figure.width; j++) {
		size_t i = figure.height - 1;
		while (!figure.points[i][j].isFilled)
			i--;
		ret[j] = i;
	}
	return ret;
}

bool GameScene::figureCanPlaced(const std::vector<std::vector<PointState>> &points) {
	auto [width, height] { Matrix2::getSize(points) };
	for (size_t i = 0; i < height; i++)
		for (size_t j = 0; j < width; j++) {
			auto x { figure.x + j };
			auto y { figure.y + i };
			if (x >= fieldColumns || y >= fieldLines)
				return false;
			else if (points[i][j].isFilled && field[y][x].isFilled)
				return false;
		}
	return true;
}

void GameScene::generateFigure() {
	static std::uniform_int_distribution<size_t> distFigure(0, figures.size() - 1);
	static std::uniform_int_distribution<size_t> distTexture(0, blockTextures.size() - 1);
	auto figureIndex { distFigure(generator) };
	auto textureIndex { distTexture(generator) };
	figure.points = figures[figureIndex];
	figure.width = figure.points[0].size();
	figure.height = figure.points.size();
	figure.x = fieldColumns / 2 - figure.width / 2;
	figure.y = 0;
	figure.texture = textureIndex;
	figure.exist = figureCanPlaced(figure.points);
}

bool GameScene::thereIsBarrier(Direction dir) {
	switch (dir) {
		case Direction::LEFT:
			if (figure.x == 0)
				return true;
			for (size_t i = 0; i < figure.height; i++) {
				size_t j = 0;
				while (!figure.points[i][j].isFilled)
					j++;
				if (field[figure.y + i][figure.x + j - 1].isFilled)
					return true;
			}
			return false;
		case Direction::RIGHT:
			if (figure.x + figure.width == fieldColumns)
				return true;
			for (size_t i = 0; i < figure.height; i++) {
				size_t j = figure.width - 1;
				while (!figure.points[i][j].isFilled)
					j--;
				if (field[figure.y + i][figure.x + j + 1].isFilled)
					return true;
			}
			return false;
		case Direction::DOWN:
			if (figure.y + figure.height == fieldLines)
				return true;
			for (size_t j = 0; j < figure.width; j++) {
				size_t i = figure.height - 1;
				while (!figure.points[i][j].isFilled)
					i--;
				if (field[figure.y + i + 1][figure.x + j].isFilled)
					return true;
			}
			return false;
	}
	return true;
}

void GameScene::moveFigure(Direction dir) {
	switch (dir) {
		case Direction::LEFT:
			if (figure.exist && !thereIsBarrier(dir))
				figure.x--;
			break;
		case Direction::RIGHT:
			if (figure.exist && !thereIsBarrier(dir))
				figure.x++;
			break;
		case Direction::DOWN:
			if (!thereIsBarrier(dir))
				figure.y++;
			else if (figure.exist) {
				fixFigure();
				clearFieldLines();
				generateFigure();
			}
			break;
	}
}

void GameScene::dropFigure() {
	if (figure.exist) {
		auto border { getFigureBottonBorder() };
		size_t offset = 0;
		auto check {
			[&](const std::vector<std::vector<PointState>> &points, const std::vector<size_t> &borders, size_t i0, size_t j0) {
				auto [width, height] { Matrix2::getSize(points) };
				for (size_t j = 0; j < borders.size(); j++) {
					size_t x = j + j0;
					size_t y = borders[j] + i0;
					if (x >= width || y + 1 >= height)
						return false;
					if (points[y + 1][x].isFilled)
						return false;
				}
				return true;
			}
		};

		while (check(field, border, figure.y + offset, figure.x))
			offset++;

		figure.y += offset;

		fixFigure();
		clearFieldLines();
		generateFigure();
	}
}

void GameScene::fixFigure() {
	for (size_t i = 0; i < figure.height; i++)
		for (size_t j = 0; j < figure.width; j++) {
			auto jField { figure.x + j };
			auto iField { figure.y + i };
			if (figure.points[i][j].isFilled) {
				field[iField][jField].isFilled = true;
				field[iField][jField].texture = figure.texture;
			}
		}
}

void GameScene::rotateFigure(Matrix2::RotateDirection dir) {
	std::vector<std::vector<PointState>> rotatePoints(figure.points);
	Matrix2::rotate(rotatePoints, dir);

	if (figureCanPlaced(rotatePoints)) {
		figure.points = rotatePoints;
		figure.width = rotatePoints[0].size();
		figure.height = rotatePoints.size();
	}
}

void GameScene::clearFieldLines() {
	auto pred { [](PointState x) { return x.isFilled; } };
	std::list<size_t> deleted;
	for (size_t i = 0; i < fieldLines; i++) {
		auto isCompletelyFilled { std::all_of(field[i].begin(), field[i].end(), pred) };
		if (isCompletelyFilled) {
			score++;
			deleted.push_back(i);
			std::fill(field[i].begin(), field[i].end(), PointState());
		}
	}

	// TODO: redo further
	for (auto &line : deleted) {
		for (size_t i = line; i > 0; i--)
			field[i] = field[i - 1];
		std::fill(field[0].begin(), field[0].end(), PointState());
	}
}

Rectangle GameScene::getScoreWindowPlace(const sf::Vector2u &wndSize) {
	Rectangle ret;
	ret.x = 0;
	ret.y = 0;
	ret.width = wndSize.x;
	ret.height = wndSize.y * scorePart;
	return ret;
}

Rectangle GameScene::getGameWindowPlace(const sf::Vector2u &wndSize) {
	Rectangle ret;
	ret.x = 0;
	ret.y = wndSize.y * scorePart + 1;
	ret.width = wndSize.x;
	ret.height = wndSize.y - ret.y;
	return ret;
}

float GameScene::getBlockSize(const Rectangle &wndPlace) {
	auto widthRatio { 
		static_cast<float>(fieldColumns * blockTextureSize) / wndPlace.width 
	};
	auto heightRation { 
		static_cast<float>(fieldLines * blockTextureSize) / wndPlace.height 
	};
	if (widthRatio < heightRation)
		return static_cast<float>(wndPlace.height) / fieldLines;
	else
		return static_cast<float>(wndPlace.width) / fieldColumns;
}

void GameScene::printDebugInfo() {
	std::cout << "Field:" << std::endl;
	std::cout << "Count lines = " << fieldLines << "; count columns = " << fieldColumns << std::endl;
	for (size_t i = 0; i < fieldLines; i++) {
		for (size_t j = 0; j < fieldColumns; j++) {
			if (figure.x <= j && j < figure.x + figure.width &&
			    figure.y <= i && i < figure.y + figure.height &&
			    figure.points[i - figure.y][j - figure.x].isFilled)
				std::cout << 'F';
			else
				std::cout << field[i][j].isFilled;
		}
		std::cout << std::endl;
	}
	std::cout << "Figure:" << std::endl;
	std::cout << "Exist = " << (figure.exist ? "true" : "false") << std::endl;
	std::cout << "X = " << figure.x << "; y = " << figure.y << std::endl;
	std::cout << "Width = " << figure.width << "; height = " << figure.height << std::endl;
	for (size_t i = 0; i < figure.height; i++) {
		for (size_t j = 0; j < figure.width; j++)
			std::cout << figure.points[i][j].isFilled;
		std::cout << std::endl;
	}
}

bool GameScene::isOver() {
	return isSceneOver;
}

void GameScene::handleKey(sf::Event::KeyEvent event) {
	switch (event.code) {
		case sf::Keyboard::Key::Up:
			rotateFigure(Matrix2::RotateDirection::CLOCKWISE);
			break;
		case sf::Keyboard::Key::Down:
			dropFigure();
			break;
		case sf::Keyboard::Key::Right:
			moveFigure(Direction::RIGHT);
			break;
		case sf::Keyboard::Key::Left:
			moveFigure(Direction::LEFT);
			break;
		default:
			break;
	}
}

void GameScene::update() {
	static sf::Clock deltaClock;
	if (deltaClock.getElapsedTime().asSeconds() > latency) {
		moveFigure(Direction::DOWN);
		deltaClock.restart();
	}
}

void GameScene::display(sf::RenderWindow &target) {
	auto scoreWndPlace { getScoreWindowPlace(target.getSize()) };
	auto gameWndPlace { getGameWindowPlace(target.getSize()) };

	sf::Text text;
	const float scoreTextScale = 0.85;
	text.setFont(scoreFont);
	text.setCharacterSize(scoreWndPlace.height * scoreTextScale);
	auto strScore { "Score: " + std::to_string(score) };
	text.setString(strScore);
	text.setFillColor(sf::Color::White);

	target.draw(text);

	auto blockSize { getBlockSize(gameWndPlace) };
	auto blockScale { blockSize / blockTextureSize };
	auto backScale { blockSize / backTextureSize };
	sf::Sprite sprite;
	sprite.setScale(blockScale, blockScale);

	sf::Sprite backSprite;
	backSprite.setTexture(backTexture);
	backSprite.setScale(backScale, backScale);

	// Drawing figure
	if (figure.exist)
		for (size_t i = 0; i < figure.height; i++)
			for (size_t j = 0; j < figure.width; j++)
				if (figure.points[i][j].isFilled) {
					sprite.setPosition(
						gameWndPlace.x + (figure.x + j) * blockSize,
						gameWndPlace.y + (figure.y + i) * blockSize
					);
					sprite.setTexture(blockTextures[figure.texture]);
					target.draw(sprite);
				}

	// Drawing field
	for (size_t i = 0; i < fieldLines; i++)
		for (size_t j = 0; j < fieldColumns; j++) {
			if (field[i][j].isFilled) {
				sprite.setPosition(
					gameWndPlace.x + j * blockSize,
					gameWndPlace.y + i * blockSize
				);
				sprite.setTexture(blockTextures[field[i][j].texture]);
				target.draw(sprite);
				continue;
			}
			
			if (figure.exist &&
			    figure.x <= j && j < figure.x + figure.width &&
			    figure.y <= i && i < figure.y + figure.height &&
			    figure.points[i - figure.y][j - figure.x].isFilled)
				continue;

			// Drawing empty cells
			backSprite.setPosition(
				gameWndPlace.x + j * blockSize,
				gameWndPlace.y + i * blockSize
			);
			target.draw(backSprite);
		}
}
