#include "Scene_Play.h"
#include "Physics.hpp"
#include "Assets.hpp"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"
#include "Scene_Menu.h"
#include <iostream>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	:Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::P, "PAUSE");
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
	registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
	registerAction(sf::Keyboard::G, "TOGGLE_GRID");

	//TODO: Register all other game Actions
	registerAction(sf::Keyboard::W, "JUMP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Space, "SHOOT");

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assets().getFont("Roboto-Bold"));
	//m_game->assets();

	loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridx, float gridy, std::shared_ptr<Entity>entity)
{
	//TODO: takes grid x,y and entity
	//		return vec2 where center postion of entity should be
	//		
	Vec2f halfSize = entity->get<CAnimation>().animation.getSize();
	return Vec2f(gridx * m_gridSize.x - halfSize.x, gridy * m_gridSize.y - halfSize.y);
}
void Scene_Play::loadLevel(const std::string& filename)
{
	//reset EM
	m_entityManager = EntityManager();
	std::string f = filename;
	f.pop_back();
	f.pop_back();
	f.pop_back();
	f.pop_back();
	const std::string layer0File = f + "_Layer0.csv";
	const std::string layer1File = f + "_Layer1.csv";
	//TODO: read level file and add appropraite entities
	//		use playerconfig struct to store player properties
	// Helper lambda to process a single layer
	auto processLayer = [&](const std::string& filename, int layer) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Error: Unable to open file " << filename << std::endl;
			return;
		}

		std::string line;
		int row = 0;

		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string cell;
			int col = 0;

			while (std::getline(iss, cell, ',')) {
				// Skip empty or invalid cells
				if (cell.empty() || cell == "0") {
					++col;
					continue;
				}

				// Animation ID (assumed to be integer as a string in the CSV)
				int animationID = std::stoi(cell);

				// Fetch the animation name from the asset manager using animation ID
				//std::string animationName = m_game->assets().getAnimationNameByID(animationID);
				/*if (animationName.empty()) {
					std::cerr << "Warning: Animation ID " << animationID << " not found in assets!" << std::endl;
					++col;
					continue;
				}*/

				// Create an entity for this animation
				auto entity = m_entityManager.addEntity("layer" + std::to_string(layer));
				//entity->add<CAnimation>(m_game->assets().getAnimation(animationName), true);
				entity->add<CAnimation>(m_game->assets().getAnimation(cell), true);
				entity->add<CTransform>(gridToMidPixel(col, row, entity));

				++col;
			}
			++row;
		}
		};

	// Process both layers
	processLayer(layer0File, 0);
	processLayer(layer1File, 1);

	std::cout << "Level loaded successfully from CSV files: " << layer0File << " and " << layer1File << std::endl;
	spawnPlayer();

	//std::ifstream file(filename);
	//std::string line;

	//while (std::getline(file, line))
	//{
	//	// Skip empty lines and comments
	//	if (line.empty() || line[0] == '#') continue;

	//	std::istringstream iss(line);
	//	std::string type, animationName;
	//	int gridX, gridY;

	//	// Parse basic entity data
	//	iss >> type >> gridX >> gridY >> animationName;

	//	// Create entity based on type
	//	auto entity = m_entityManager.addEntity(type);
	//	entity->add<CAnimation>(m_game->assets().getAnimation(animationName), true);
	//	entity->add<CTransform>(gridToMidPixel(gridX, gridY, entity));

	//	// Handle additional properties
	//	std::string property;
	//	while (iss >> property)
	//	{
	//		auto delimiterPos = property.find('=');
	//		if (delimiterPos != std::string::npos)
	//		{
	//			std::string key = property.substr(0, delimiterPos);
	//			std::string value = property.substr(delimiterPos + 1);

	//			// Example: Add gravity or state components
	//			if (key == "gravity") entity->add<CGravity>(std::stoi(value));
	//			if (key == "state") entity->add<CState>(value);
	//			if (key == "lifespan") entity->add<CLifespan>(std::stoi(value),m_currentFrame);
	//			if (key == "boundingBox") entity->add<CBoundingBox>();
	//		}
	//	}
	//}
}
void Scene_Play::spawnPlayer()
{
	auto player =  m_entityManager.addEntity("player");
	player->add<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	player->add<CTransform>(gridToMidPixel(3,17,player));

	player->add<CBoundingBox>(Vec2f(64,64));
	//TODO: add all componet to player
	player->add<CInput>();
	player->add<CGravity>(1);
	player->add<CState>(std::string("Stand"));
}



void Scene_Play::spawnBullet(std::shared_ptr<Entity> e)
{
	float bulletSpeed = 5.0f;
	//TODO: shoot bullet in direction the player is facing
	auto bullet = m_entityManager.addEntity("bullet");
	Vec2f direction = (e->get<CTransform>().scale.x > 0) ? Vec2f(1, 0) : Vec2f(-1, 0); // Right if facing right, left otherwise
	bullet->add<CTransform>(e->get<CTransform>().pos);
	//bullet->add<CTransform>(e->get<CTransform>().pos, direction * bulletSpeed);//orignal error due to more argumentss
	bullet->add<CBoundingBox>(Vec2f(10, 10));
	bullet->add<CLifespan>(60,m_currentFrame); // Bullet exists for 60 frames
	bullet->add<CAnimation>(m_game->assets().getAnimation("Bullet"), true);
}

void Scene_Play::update()
{
	m_entityManager.update();

	//TODO: implement pause
	if (player()->get<CTransform>().pos.y > 1280)
	{
		player()->destroy();
		spawnPlayer();
	}
	sMovement();
	//sCollision();
	sLifeSpan();
	sAnimation();
	sRender();

}
void Scene_Play::sMovement()
{
	auto& pi = player()->get<CInput>();
	auto& pt = player()->get<CTransform>();
	auto& pg = player()->get<CGravity>();
	auto& ps = player()->get<CState>();
	pt.prevPos = pt.pos;

	// Set initial horizontal velocity based on input
	//pt.velocity.x = 0;
	if (pi.left)
	{
		pt.velocity.x = -m_playerConfig.SPEED;
	}
	else if (pi.right)
	{
		pt.velocity.x = m_playerConfig.SPEED;
	}
	else
	{
		pt.velocity.x = 0;
	}
	if (pi.up)
	{
		pt.velocity.y = -m_playerConfig.SPEED;
	}
	else if (pi.down)
	{
		pt.velocity.y = m_playerConfig.SPEED;
	}
	else
	{
		pt.velocity.y = 0;
	}
	if (pt.velocity.x != 0)
	{
		ps.state = "Run";
	}
	else
	{
		ps.state = "Stand";
	}
	// Jump logic: apply an initial upward velocity when jump input is triggered
	if (pi.up && pi.canJump)
	{
		pt.velocity.y -= m_playerConfig.JUMP;
		pi.canJump = false;
	}

	// Update player position
	pt.pos.x += pt.velocity.x;
	pt.pos.y += pt.velocity.y;

	// Apply gravity to vertical velocity, whether jumping or falling
	
	//pt.velocity.y += pg.gravity;
}

void Scene_Play::sLifeSpan()
{
	for (auto& e : m_entityManager.getEntities())
	{
		if (e->has<CLifespan>())
		{
			auto& lifespan = e->get<CLifespan>();
			if (m_currentFrame - lifespan.frameCreated >= lifespan.lifespan)
			{
				e->destroy();
			}
		}
	}
}

void Scene_Play::sCollision()
{
	for (auto& e : m_entityManager.getEntities("tile"))
	{

		auto& playerTransform = player()->get<CTransform>();
		auto& playerBox = player()->get<CBoundingBox>();

		auto& brickTransform = e->get<CTransform>();
		auto& brickBox = e->get<CBoundingBox>();

		// Calculate current and previous overlap
		Vec2f overlap = Physics::GetOverLap(player(), e);
		Vec2f prevOverlap = Physics::GetPreviousOverlap(player(), e);
		if (overlap.x <= 0 || overlap.y <= 0)
		{
			continue;
		}
		if (overlap.x > 0 && overlap.y > 0) // Collision detected
		{
			// Determine collision direction
			if (prevOverlap.y>0 && overlap.x>0) // Horizontal collision
			{
				if (playerTransform.pos.x > brickTransform.pos.x) // brick is on the left of player
				{
					playerTransform.pos.x = brickTransform.pos.x + brickBox.size.x; // Move player to the right of the brick
				}
				else // Right collision
				{
					playerTransform.pos.x = brickTransform.pos.x - playerBox.size.x; // Move player to the left of the brick
				}

				playerTransform.velocity.x = 0; // Stop horizontal movement
			}else if (prevOverlap.x>0 && overlap.y > 0) // Vertical collision
			{
				playerTransform.velocity.y = 0;
				if (playerTransform.pos.y > brickTransform.pos.y) // brick is on top of player
				{
					playerTransform.pos.y = brickTransform.pos.y + brickBox.size.y;
				}
				else // bottom collision
				{
					playerTransform.pos.y -= (overlap.y);
					player()->get<CInput>().canJump = true;
				}
			}
		}

	}
}



void Scene_Play::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
		else if (action.name() == "PAUSE") { setPaused(!m_paused); }
		else if (action.name() == "QUIT") { onEnd(); }
		else if (action.name() == "JUMP")
		{
			player()->get<CInput>().up = true;
		}
		else if (action.name() == "LEFT")
		{
			player()->get<CTransform>().scale.x = -1;
			player()->get<CInput>().left = true;
		}
		else if (action.name() == "RIGHT")
		{
			player()->get<CTransform>().scale.x = 1;
			player()->get<CInput>().right = true;
		}
		else if (action.name() == "SHOOT")
		{
			player()->get<CInput>().shoot = true;
		}
		else if (action.name() == "DOWN")
		{
			player()->get<CInput>().down = true;
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "JUMP")
		{
			player()->get<CInput>().up = false;
		}
		else if (action.name() == "LEFT")
		{
			//asggggggggg
			player()->get<CInput>().left = false;
		}
		else if (action.name() == "RIGHT")
		{
			
			//asggggggggg
			player()->get<CInput>().right = false;
		}
		else if (action.name() == "SHOOT")
		{
			//asggggggggg
			player()->get<CInput>().shoot = false;
		}
		else if (action.name() == "DOWN")
		{
			player()->get<CInput>().down = false;
		}
	}
}

void Scene_Play::sAnimation() //not done yet
								// animation happening but not flipping
{
	// Iterate through all entities
	for (auto& entity : m_entityManager.getEntities())
	{
		
		// Get the CAnimation component of the entity
		auto& animation = entity->get<CAnimation>();
	
		// Update the animation
		animation.animation.update();

		// Check if the animation is not repeated and has ended
		if (!animation.repeat && animation.animation.hasEnded())
		{
			// Destroy the entity
			entity->destroy();
		}
	}
	// Handle player animations based on state
	auto& playerState = player()->get<CState>().state;
	auto& playerAnimation = player()->get<CAnimation>();
	// Change animation only if the state has changed
	if (playerAnimation.animation.getName() != playerState)
	{
		if (playerState == "air")
		{
			playerAnimation.animation = CAnimation(m_game->assets().getAnimation("air"), true).animation;
		}
		else if (playerState == "Run")
		{
			playerAnimation.animation = CAnimation(m_game->assets().getAnimation("Run"), true).animation;
		}
		else if(playerState =="Stand")
		{
			playerAnimation.animation = CAnimation(m_game->assets().getAnimation("Stand"), true).animation;
		}
	}
}




void Scene_Play::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}

void Scene_Play::sRender()
{
	if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
	else { m_game->window().clear(sf::Color(50, 50, 150)); }

	//auto& pPos = player()->get<CTransform>().pos;

	//// World dimensions
	//const float worldWidth = 1536.0f;
	//const float worldHeight = 1216.0f;
	//// Get the size of the window
	//float windowWidth = m_game->window().getSize().x;
	//float windowHeight = m_game->window().getSize().y;

	//// Calculate the center of the view, clamping it within world boundaries
	//float windowCenterX = std::max(windowWidth / 2.0f, std::min(pPos.x, worldWidth - windowWidth / 2.0f));
	//float windowCenterY = std::max(windowHeight / 2.0f, std::min(pPos.y, worldHeight - windowHeight / 2.0f));

	//sf::View view = m_game->window().getView();
	//view.setCenter(windowCenterX, windowCenterY);
	//m_game->window().setView(view);
	// Camera smoothing factor (between 0 and 1)
	const float smoothingFactor = 0.1f;

	// Store the previous view center (initialize it if necessary)
	static sf::Vector2f previousViewCenter = sf::Vector2f(640, 832);

	// World dimensions
	const float worldWidth = 1536.0f;
	const float worldHeight = 1216.0f;

	// Get the player's position
	auto& pPos = player()->get<CTransform>().pos;

	// Get the size of the window
	float windowWidth = m_game->window().getSize().x;
	float windowHeight = m_game->window().getSize().y;

	// Calculate the target center of the view (clamped to world boundaries)
	float targetCenterX = std::max(windowWidth / 2.0f, std::min(pPos.x, worldWidth - windowWidth / 2.0f));
	float targetCenterY = std::max(windowHeight / 2.0f, std::min(pPos.y, worldHeight - windowHeight / 2.0f));

	// Smoothly interpolate between the previous center and the target center
	float smoothedCenterX = previousViewCenter.x + (targetCenterX - previousViewCenter.x) * smoothingFactor;
	float smoothedCenterY = previousViewCenter.y + (targetCenterY - previousViewCenter.y) * smoothingFactor;

	// Update the previous view center for the next frame
	previousViewCenter = sf::Vector2f(smoothedCenterX, smoothedCenterY);

	// Set the smoothed center to the view
	sf::View view = m_game->window().getView();
	view.setCenter(smoothedCenterX, smoothedCenterY);
	m_game->window().setView(view);

	//draw all Entity
	if (m_drawTextures)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			auto& transform = e->get<CTransform>();

			if (e->has<CAnimation>())
			{
				auto& animation = e->get<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x,transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}
	}

	//draw all entity boundingbox
	if (m_drawCollision)
	{
		for (auto& e : m_entityManager.getEntities())
		{
			if (e->has<CBoundingBox>())
			{
				auto& box = e->get<CBoundingBox>();
				auto& t = e->get<CTransform>();

				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(t.pos.x, t.pos.y);
				rect.setFillColor(sf::Color(255, 0, 0, 100));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

	//draw grid
	if (m_drawGrid)
	{
		float leftx = m_game->window().getView().getCenter().x - (float)(width() / 2);
		float rightx = leftx + width() + m_gridSize.x;
		float nextgridx = leftx;
		//float nextgridx = leftx - ((int)leftx % (int)m_gridSize.x);

		// Draw vertical lines
		for (float x = nextgridx; x < rightx; x += m_gridSize.x)
		{
			drawLine(Vec2f(x, 0), Vec2f(x, height()));
		}

		// Draw horizontal lines and grid text
		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			// Corrected y-coordinate calculation for grid lines
			float actualY = height() - y;

			drawLine(Vec2f(leftx, actualY), Vec2f(rightx, actualY));

			for (float x = nextgridx; x < rightx; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);

				// Position text inside the grid cell
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x+3, actualY - m_gridSize.y+2);
				m_game->window().draw(m_gridText);
			}
		}
	}
}

void Scene_Play::drawLine(const Vec2f& p1, const Vec2f& p2)
{
	//sf::Vertex line[] = { sf::Vector2f(p1.x,p1.y),sf::Vector2f(p2.x,p2.y) };
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y), sf::Color(0, 0, 0, 255)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y), sf::Color(0, 0, 0, 255))
	};
	m_game->window().draw(line, 2, sf::Lines);
}

std::shared_ptr<Entity> Scene_Play::player()
{
	auto& players = m_entityManager.getEntities("player");
	if (players.empty())
	{
		return nullptr;
	}

	return players.front();
}