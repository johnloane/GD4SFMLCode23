#include "SceneNode.hpp"
#include <cassert>
#include <memory>

SceneNode::SceneNode():m_children(), m_parent(nullptr)
{
}

void SceneNode::AttachChild(Ptr child)
{
    child->m_parent = this;
    //TODO Why is emplace_back more efficient than push_back
    m_children.emplace_back(std::move(child));
}

Ptr SceneNode::DetachChild(const SceneNode& node)
{
    auto found = std::find_if(m_children.begin(), m_children.end(), [&](Ptr& p) {return p.get() == &node; });
    assert(found != m_children.end());

    Ptr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);


    return result;
}

void SceneNode::Update(sf::Time dt)
{
    UpdateCurrent(dt);
    UpdateChildren(dt);
}

sf::Vector2f SceneNode::GetWorldPosition() const
{
    return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
    {
        transform = node->getTransform() * transform;
    }
    return transform;
}

void SceneNode::UpdateCurrent(sf::Time dt)
{
    //Do nothing here
}

void SceneNode::UpdateChildren(sf::Time dt)
{
    for (Ptr& child : m_children)
    {
        child->Update(dt);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Apply the transform of the current node
    states.transform *= getTransform();
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    //Do nothing here
}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Ptr& child : m_children)
    {
        child->draw(target, states);
    }
}
