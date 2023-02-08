#pragma once
#include "StateID.hpp"
#include "ResourceIdentifiers.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
	class RenderWindow;
}
class Player;
class StateStack;
class KeyBinding;



class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys1, KeyBinding& keys2);
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		MusicPlayer* music;
		SoundPlayer* sounds;
		KeyBinding* keys1;
		KeyBinding* keys2;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();
	virtual void Draw() = 0;
	virtual bool Update(sf::Time dt) = 0;
	virtual bool HandleEvent(const sf::Event& event) = 0;
	virtual void OnActivate();
	virtual void OnDestroy();

protected:
	void RequestStackPush(StateID state_id);
	void RequestStackPop();
	void RequestStackClear();

	Context GetContext() const;

private:
	StateStack* m_stack;
	Context m_context;


};

