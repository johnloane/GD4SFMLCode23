#include "State.hpp"
#include "StateStack.hpp"



State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player, MusicPlayer& music, SoundPlayer& sounds)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player(&player)
	, music(&music)
	, sounds(&sounds)
{
}

State::State(StateStack& stack, Context context)
	: m_stack(&stack)
	, m_context(context)
{
}

State::~State()
{
}

void State::RequestStackPush(StateID state_id)
{
	m_stack->PushState(state_id);
}

void State::RequestStackPop()
{
	m_stack->PopState();
}

void State::RequestStackClear()
{
	m_stack->ClearState();
}

State::Context State::GetContext() const
{
	return m_context;
}


