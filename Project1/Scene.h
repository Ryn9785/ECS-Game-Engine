#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"
//#include "GameEngine.h"
#include <memory>

class GameEngine;

typedef std::map<int, std::string>ActionMap;

class Scene
{
protected:
	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	ActionMap m_actionMap;
	bool m_paused = true;
	bool m_hasEnded = false;
	size_t m_currentFrame = 0;

	size_t m_width = 1280; // Default width, can be overridden
	size_t m_height = 768; // Default height, can be overridden


	virtual void onEnd() = 0;
	void setPaused(bool paused) { m_paused = paused; }

public:
	Scene() = default;
	Scene(GameEngine* gameEngine) :m_game(gameEngine) {};

	virtual void update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	// Perform action based on the current state of the game
	virtual void doAction(const Action& action) {
		//// Example: Check if action is in the map and handle it
		//if (m_actionMap.find(action) != m_actionMap.end()) {
		//	sDoAction(action);
		//}
	}
	//void simulate(const size_t frames);
	// Simulate for a specific number of frames
	void simulate(const size_t frames) {
		for (size_t i = 0; i < frames; ++i) {
			if (m_paused || m_hasEnded) break;
			update();
			++m_currentFrame;
		}
	}
	void registerAction(int inputKey, const std::string& actionName)
	{
		m_actionMap[inputKey] = actionName;
	}

	// Getter for scene width
	size_t width() const { return m_width; }

	// Getter for scene height
	size_t height() const { return m_height; }
	size_t currentFrame()const
	{
		return m_currentFrame;
	}

	bool hasEnded()const { return m_hasEnded; }
	const ActionMap& getActionMap()const
	{
		return m_actionMap;
	}
	//void drawLine(const Vec2f& p1, const Vec2f& p2)
	//{
	//	sf::Vertex line[] = { sf::Vector2f(p1.x,p1.y),sf::Vector2f(p2.x,p2.y) };
	//	m_game->window().draw(line, 2, sf::Lines);
	//}
};