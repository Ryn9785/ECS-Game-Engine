#pragma once
#include <string>
#include <tuple>
#include "Components.hpp"

class EntityManager;

using ComponentTuple = std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
>;

class Entity
{
    friend class EntityManager;

    ComponentTuple   m_components;
    bool             m_active = true;
    std::string      m_tag = "default";
    size_t           m_id = 0;

    // Refactored constructor for readability
    Entity(const size_t& id, const std::string& tag)
        : m_id(id), m_tag(tag) {
    }

public:
    // Check if the entity is active
    bool isActive() const
    {
        return m_active;
    }

    // Mark the entity as destroyed (inactive)
    void destroy()
    {
        m_active = false;
    }

    // Get the entity's ID
    size_t id() const
    {
        return m_id;
    }

    // Get the entity's tag (type/category identifier)
    const std::string& tag() const
    {
        return m_tag;
    }

    // Check if the entity has a specific component
    template<typename T>
    bool has() const
    {
        return get<T>().exists;
    }

    // Add a component to the entity and mark it as existing
    template<typename T, typename... TArgs>
    T& add(TArgs&&... mArgs)
    {
        auto& component = get<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.exists = true;
        return component;
    }

    // Get a mutable reference to a component of the entity
    template<typename T>
    T& get()
    {
        return std::get<T>(m_components);
    }

    // Get a constant reference to a component of the entity
    template<typename T>
    const T& get() const
    {
        return std::get<T>(m_components);
    }

    // Remove a component from the entity (reset it)
    template<typename T>
    void remove()
    {
        get<T>() = T();  // Reset the component to its default state
    }

};
