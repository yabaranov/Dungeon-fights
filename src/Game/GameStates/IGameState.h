#pragma once

class IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 8;
	static constexpr unsigned int SCREEN_WIDTH = 14 * BLOCK_SIZE * 2;
	static constexpr unsigned int SCREEN_HEIGHT = 14 * BLOCK_SIZE * 2;

	virtual void render() const = 0;
	virtual void update(const double delta) = 0;
	virtual ~IGameState() = default;
	virtual void processInput(const std::array<bool, 349>& keys, const std::array<bool, 349>& keysPressed) {};
	virtual unsigned int getStateWidth() const = 0;
	virtual unsigned int getStateHeight() const = 0;
};