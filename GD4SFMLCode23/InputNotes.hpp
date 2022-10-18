#pragma once
/*
* sf::Event event;
while(window.pollEvent(event))
{
//Handle the event
}

Window, Joystick, Keyboard, Mouse

Window Events

sf::Event::Closed
x button alt f4

sf::Event::Resized
sf::Event:SizeEvent event.size
Can disable resizing, and you probably should

sf::Event::LostFocus, sf::Event::GainedFocus
Should handle
Probably want to pause on lostfocus and unpause on gained

Joystick events
ID
sf::Event::JoystickButtonPressed
sf::Event::JoystickMoved
event.joystickMove

sf::Event::JoytickConnected
event.joystickConnect

Keyboard events
sf::Event::KeyPressed
event.key.code
event.key.control 
sf::Window::setKeyRepeatEnabled()

sf::Event::TextEntered
event.text

Mouse Events
sf::Event::MouseEntered
sf::Event::MouseMoved
sf::Event::MouseButtonPressed
sf::Event::MouseWheelMoved

Real time events
sf::Joystick can say whether it is connected based on ID
Can ask how many buttons it has

sf::Keyboard has isKeyPressed()

sf::Mouse isButtonPressed()
sf::Mouse::getPosition()


Real time event handling
void Game Update(sf:::Time dt)
{
	sf::Vector2f movement(0.f, 0.f);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)
		movement.y -= PlayerSpeed;
}

Events vs realtime
If a state has changed, you should use events
e.g. firing a missle, space bar pressed
If you want to know the current state then use real time
e.g Keyboard movement input

Mouse
sf::Vector2i mouse_position = sf::Mouse::getPosition(mWindow);
sf::Vector2i delta = m_last_mouse_position - mouse_position;
m_last_mouse_position = mouse_position;

sf::Vector2i window_centre(mWindow.getSize()/2);
sf::Vector2i mouse_position = sf::Mouse::getPosition(mWindow);
sf::Vector2i delta = window_centre - mouse_position;
sf::Mouse::setPosition(window_centre, mWindow);

void Game::proccessEvents()
{
	sf::Event event;
	while(mWindow.pollEvent(event))
	{
		if(event.type == sf::Event:GainedFocus)
			m_paused = false;
		else if(event.type == sf::Event::LostFocus)
			m_paused = true;
	}
}

void Game::Run()
{
	while(mWindow.isOpen())
	{
		if(!m_paused)
			update();
		render();
		processEvents();
	}
}

sf::Event event;
while(window.pollEvent(event))
{
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		m_playeraircraft->LaunchMissile();
}

struct Command
{
	std::function<void(SceneNode&, sf::Time)> action;
}

std::function
int add(int a, in b){return a + b;}
std::function<int(int, int)> adder1 = &add;
std::function<int(int, int)> adder2 
= [](int a, int b){return a+b;};
int sum = adder1(3, 5);

std::function<int(int)> increaser = std::bind(&add, _1, 1);
int increased = increaser(5);

void MoveLeft(SceneNode& node, sf::Time dt)
{
	node.move(-30.f*dt.asSeconds(), 0.f);
}

Command c;
c.action = &MoveLeft;

c.action = [](SceneNode& node, sf::Time dt)
{
	node.move(-30.f*dt.asSeconds(), 0.f);
};

