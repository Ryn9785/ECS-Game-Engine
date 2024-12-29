#include "GameEngine.h"
#include "Assets.hpp"
#include "Scene_play.h"
#include "Scene_Menu.h"
#include "Scene.h"

#include <iostream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::init(const std::string& path)
{
	m_assets.loadFromFile(path);
	m_window.create(sf::VideoMode(1280, 768), "Gmae");
	m_window.setFramerateLimit(60);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			quit();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "Screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "Screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
			{
				continue;
			}

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
			//std::cout << event.key.code << " " << actionType << std::endl;
			currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
			//currentScene()->
		}
	}
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	// If endCurrentScene is true, remove the current scene from the scene map
	if (endCurrentScene && !m_currentScene.empty()) {
		m_sceneMap.erase(m_currentScene);
	}

	// Set the new scene in the scene map
	m_sceneMap[sceneName] = scene;

	// Update the current scene to the new one
	m_currentScene = sceneName;
}

void GameEngine::quit() {
	// Set the running flag to false, which will stop the game loop
	m_running = false;
}

const Assets& GameEngine::assets() const {
	// Return the assets manager
	return m_assets;
}


void GameEngine::update() {
	// Calculate the delta time since the last frame
	sf::Time deltaTime = m_deltaClock.restart();

	// Check if the game is still running and the current scene is valid
	if (m_running && currentScene()) {
		// Call the update function for the current scene, passing scaled delta time
		currentScene()->update();

		// Handle any user inputs
		sUserInput();

		// Clear the window
		m_window.clear();

		// Render the current scene
		currentScene()->sRender();

		// Display the rendered frame
		m_window.display();
	}
}
