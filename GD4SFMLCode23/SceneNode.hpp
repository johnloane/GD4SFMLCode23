#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "CommandQueue.hpp"

#include <memory>
#include <vector>
#include <set>

class Command;


class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	SceneNode();
	void AttachChild(Ptr child);
	Ptr DetachChild(const SceneNode& node);

	void Update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f GetWorldPosition() const;
	sf::Transform GetWorldTransform() const;

	void OnCommand(const Command& command, sf::Time dt);
	virtual sf::FloatRect GetBoundingRect() const;
	void DrawBoundingRect(sf::RenderTarget& target, sf::RenderStates states, sf::FloatRect& rect) const;

	void CheckSceneCollision(SceneNode& scene_graph, std::set<Pair>& collision_pairs);
	bool IsDestroyed() const;
	virtual unsigned int GetCategory() const;
	void RemoveWrecks();

private:
	virtual void UpdateCurrent(sf::Time dt, CommandQueue& commands);
	void UpdateChildren(sf::Time dt, CommandQueue& commands);

	//Note draw if from sf::Drawable hence the name
	//Do not be tempted to call this Draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void CheckNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
	virtual bool IsMarkedForRemoval();
	
	

private:
	std::vector<Ptr> m_children;
	SceneNode* m_parent;

};
float Distance(const SceneNode& lhs, const SceneNode& rhs);
bool Collision(const SceneNode& lhs, const SceneNode& rhs);

