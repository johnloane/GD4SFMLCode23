#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include "ResourceHolder.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
	try
	{
		Application app;
		app.Run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

}