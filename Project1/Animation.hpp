#pragma once

#include "vec2.hpp"

#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
	sf::Sprite	m_sprite;
	size_t		m_frameCount = 1;//total no of frames of animation
	size_t		m_currentframe = 0;
	size_t		m_speed = 0;
	Vec2f		m_size;
	std::string m_name = "none";

public:
	Animation() = default;
	Animation(const std::string& name, const sf::Texture& t)
		:Animation(name, t, 1, 0){	}
	Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
		:m_name(name),
		m_sprite(t),
		m_frameCount(frameCount),
		m_currentframe(0),
		m_speed(speed)
	{
		m_size = Vec2f((float)(t.getSize().x / frameCount), (float)t.getSize().y);
		m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
		m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentframe) * m_size.x, 0, m_size.x, m_size.y));
	}
	void update()
	{
		////add the speed variable to current frame
		m_currentframe++;
		if (m_speed == 0 || m_frameCount <= 1)
		{
			return; // No animation to update
		}
		size_t displayedFrame = (m_currentframe / m_speed) % m_frameCount;

		m_sprite.setTextureRect(sf::IntRect(
			static_cast<int>(displayedFrame * m_size.x), // X offset
			0,                                          // Y offset
			static_cast<int>(m_size.x),                // Frame width
			static_cast<int>(m_size.y)                 // Frame height
		));
	}
	bool hasEnded() const
	{
		//TODO: detect when animation has ended(last frame was played) and return true
		//return false;
		//return m_currentframe >= m_frameCount; //?
		return m_currentframe >= (m_frameCount * m_speed) - 1;
	}
	const std::string& getName() const
	{
		return m_name;
	}
	const Vec2f& getSize() const
	{
		return m_size;
	}
	sf::Sprite& getSprite()
	{
		return m_sprite;
	}
};