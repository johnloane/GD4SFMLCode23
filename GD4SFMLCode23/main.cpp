#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "ResourceHolder.hpp"
#include <iostream>

int main()
{
	ResourceHolder<sf::Texture, Texture> game_textures;
	try
	{
		game_textures.Load(Texture::kAircraft, "Media/Textures/Eagle.png");
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	Game game(game_textures);
	game.Run();

}