#pragma once
#include <vector>
#include <memory>
#include <map>
#include <algorithm>   // Added for std::remove_if
#include "Entity.hpp"

typedef  std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;           // Stores all entities
    EntityVec m_entitiestoAdd;      // Entities waiting to be added
    EntityMap m_entityMap;          // Maps tag to vector of entities
    size_t m_totalEntities = 0;

    // Helper function to remove all inactive entities from a vector
    void removeDeadEntities(EntityVec& vec)
    {
        // Removes dead entities from the vector using std::remove_if
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }),
            vec.end());
    }

public:
    EntityManager() = default;

    //------------------------
    // Updates the entity manager, adding new entities and removing inactive ones
    void update()
    {
        // Add entities from m_entitiestoAdd to m_entities
        for (auto& entity : m_entitiestoAdd)
        {
            m_entities.push_back(entity);
        }
        m_entitiestoAdd.clear();

        // Remove dead entities from the main vector
        removeDeadEntities(m_entities);

        // Remove dead entities from each vector in the entity map
        for (auto& pair : m_entityMap)
        {
            auto& tag = pair.first;
            auto& entityVec = pair.second;
            removeDeadEntities(entityVec);
        }
    }

    // Adds a new entity with a specific tag and returns a shared pointer to it
    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));


        // Add the entity to the entities-to-add list
        m_entitiestoAdd.push_back(entity);

        // Ensure the tag exists in the map, then add the entity to the map
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        m_entityMap[tag].push_back(entity);

        return entity;
    }

    // Get all entities
    const EntityVec& getEntities() const
    {
        return m_entities;
    }

    // Get entities by tag
    const EntityVec& getEntities(const std::string& tag)
    {
        // If the tag doesn't exist, return an empty vector
        if (m_entityMap.find(tag) == m_entityMap.end())
        {
            m_entityMap[tag] = EntityVec();
        }
        return m_entityMap[tag];
    }

    // Get the entity map (tag to entity vectors)
    const std::map<std::string, EntityVec>& getEntityMap() const
    {
        return m_entityMap;
    }
};
