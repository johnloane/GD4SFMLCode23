#include "NetworkNode.hpp"
NetworkNode::NetworkNode()
	: SceneNode()
	, m_pending_actions()
{
}

void NetworkNode::NotifyGameAction(GameActions::Type type, sf::Vector2f position)
{
	m_pending_actions.push(GameActions::Action(type, position));
}

bool NetworkNode::PollGameAction(GameActions::Action& out)
{
	if (m_pending_actions.empty())
	{
		return false;
	}
	else
	{
		out = m_pending_actions.front();
		m_pending_actions.pop();
		return true;
	}
}

unsigned NetworkNode::GetCategory() const
{
	return static_cast<int>(ReceiverCategories::kNetwork);
}