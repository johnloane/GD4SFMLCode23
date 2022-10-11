#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <memory>
#include <vector>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

public:
	SceneNode();
	void AttachChild(Ptr child);
	Ptr DetachChild(const SceneNode& node);

	void Update(sf::Time dt);

	sf::Vector2f GetWorldPosition() const;
	sf::Transform GetWorldTransform() const;

private:
	virtual void UpdateCurrent(sf::Time dt);
	void UpdateChildren(sf::Time dt);

	//Note draw if from sf::Drawable hence the name
	//Do not be tempted to call this Draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr> m_children;
	SceneNode* m_parent;

};

