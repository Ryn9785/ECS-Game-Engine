#pragma once

#include "Scene.h"
#include <map>
#include <memory>

#include "EntityManager.hpp"

class Scene_Play :public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED=5, MAXSPEED=10, JUMP=25, GRAVITY;
		std::string WEAPON;
	};

protected:
	std::string					m_levelPath;
	PlayerConfig				m_playerConfig;
	bool						m_drawTextures = true;
	bool						m_drawCollision = false;
	bool						m_drawGrid = false;
	const Vec2f					m_gridSize = { 64,64 };
	sf::Text					m_gridText;


	void init(const std::string& levelPath);

	void loadLevel(const std::string& filename);

	void update();
	void onEnd(); //?added not sure
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<Entity> e);

	Vec2f gridToMidPixel(float gridx, float gridy, std::shared_ptr<Entity>entity);
	std::shared_ptr<Entity>		player();

	void sDoAction(const Action& action);
public:
	Scene_Play(GameEngine* gameEngine = nullptr, const std::string& levelPath = "");
	//void sRender();
	void sMovement();
	void sLifeSpan();
	void sCollision();
	void sAnimation();
	void sRender();
	//void EnemySpawner();
	//void sDebug();
	void drawLine(const Vec2f& p1, const Vec2f& p2);//add by me not sure
};