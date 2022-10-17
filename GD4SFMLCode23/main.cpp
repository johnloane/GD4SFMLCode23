#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "ResourceHolder.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
	try
	{
		Game game;
		game.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

}