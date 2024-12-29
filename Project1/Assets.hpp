#pragma once
#include	<map>
#include	<string>
#include	<SFML/Graphics.hpp>
#include	"Animation.hpp"
#include	<cassert>
#include	<iostream>
#include	<fstream>
//done! i believe
class Assets
{
	std::map<std::string, sf::Texture> m_textureMap;
	std::map<std::string, Animation> m_animationMap;
	std::map<std::string, sf::Font> m_fontMap;
	void addTexture(const std::string& name, const  std::string& path,bool smooth=1)
	{
		m_textureMap[name] = sf::Texture();
		if (!m_textureMap[name].loadFromFile(path))
		{
			std::cerr << "could not load Texture file: " << path << "\n";
			m_textureMap.erase(name);
		}
		else
		{
			m_textureMap[name].setSmooth(smooth);
			std::cout << "Loaded Texture:	" << path << "\n";
		}
	}
	void addAnimation(const std::string& name, const std::string& textureName, size_t& frames, size_t& speed)
	{
		m_animationMap[name] = Animation(name, m_textureMap[textureName], frames, speed);
		std::cout << "Added Animation:	" << name << "\n";
	}
	

	void addFont(const std::string& name, const  std::string& path)
	{
		m_fontMap[name] = sf::Font();
		if (!m_fontMap[name].loadFromFile(path))
		{
			std::cerr << "could not load font file: "<<path<<"\n";
			m_fontMap.erase(name);
		}
		else
		{
			std::cout << "Loaded Font:	" << path << "\n";
		}
	}

public:
	Assets() = default;
	
	void loadFromFile(const std::string& path)
	{
		std::ifstream file(path);

		std::string str;
		while (file.good())
		{
			file >> str;
			if (str == "Texture")
			{
				std::string name, path;
				file >> name >> path;
				addTexture(name, path);
			}
			else if (str == "Animation")
			{
				std::string name, texture;
				size_t frames, speed;
				file >> name >> texture >> frames >> speed;
				addAnimation(name, texture, frames, speed);
			}
			else if (str == "Font")
			{
				std::string name, path;
				file >> name >> path;
				addFont(name, path);
			}
			else
			{
				std::cout << "Unknown Asset type: " << str << "\n";
			}
		}
	}


	const sf::Font& getFont(const std::string& name)const
	{
		//assert(m_fontMap.find(name) != m_fontMap.end());
		//return m_fontMap.at(name);
		auto it = m_fontMap.find(name);  // fontMap is the map containing fonts
		if (it != m_fontMap.end())
		{
			return it->second;  // Return the found font
		}
		else
		{
			std::cerr << "Font not found: " << name << std::endl;
			// Optionally, return a default font or throw an exception here
			throw std::runtime_error("Font not found: " + name);
		}
	}
	const Animation& getAnimation(const std::string name)const
	{
		assert(m_animationMap.find(name) != m_animationMap.end());
		return m_animationMap.at(name); 
	}
	const sf::Texture& getTexture(const std::string name)const
	{
		assert(m_textureMap.find(name) != m_textureMap.end());
		return m_textureMap.at(name);
	}
	const std::map<std::string, sf::Texture>& getTextures()const
	{
		return m_textureMap;
	}
	const std::map<std::string, Animation>& getAnimations()const
	{
		return m_animationMap;
	}
	
};
