#include "MultiplayerGameState.hpp"
#include "MusicPlayer.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <fstream>
#include "PickupType.hpp"

sf::IpAddress GetAddressFromFile()
{
	//Try to open existing file
	std::ifstream input_file("ip.txt");
	std::string ip_address;
	if (input_file >> ip_address)
	{
		return ip_address;
	}

	//If the open/read failed, create a new file
	std::ofstream output_file("ip.txt");
	std::string local_address = "127.0.0.1";
	output_file << local_address;
	return local_address;

}

MultiplayerGameState::MultiplayerGameState(StateStack& stack, Context context, bool is_host)
	:State(stack, context)
	, m_world(*context.window, *context.fonts, *context.sounds, is_host)
	, m_window(*context.window)
	, m_texture_holder(*context.textures)
	, m_connected(false)
	, m_game_server(nullptr)
	, m_active_state(true)
	, m_has_focus(true)
	, m_host(is_host)
	, m_game_started(false)
	, m_client_timeout(sf::seconds(2.f))
	, m_time_last_packet(sf::Time::Zero)
{
	m_broadcast_text.setFont(context.fonts->Get(Font::kMain));
	m_broadcast_text.setPosition(1024.f/2, 100.f);
	
	m_player_invitation_text.setFont(context.fonts->Get(Font::kMain));
	m_player_invitation_text.setCharacterSize(20);
	m_player_invitation_text.setColor(sf::Color::White);
	m_player_invitation_text.setString("Press Enter to spawn player 2");
	m_player_invitation_text.setPosition(1000 - m_player_invitation_text.getLocalBounds().width, 760 - m_player_invitation_text.getLocalBounds().height);
	
	//Use this for "Attempt to connect" and "Failed to connect" messages
	m_failed_connection_text.setFont(context.fonts->Get(Font::kMain));
	m_failed_connection_text.setCharacterSize(35);
	m_failed_connection_text.setColor(sf::Color::White);
	m_failed_connection_text.setString("Attempting to connect...");
	Utility::CentreOrigin(m_failed_connection_text);
	m_failed_connection_text.setPosition(m_window.getSize().x/2.f, m_window.getSize().y/2.f);
	
	//Render an establishing connection frame for user feedback
	m_window.clear(sf::Color::Black);
	m_window.draw(m_failed_connection_text);
	m_window.display();
	m_failed_connection_text.setString("Failed to connect to server");
	Utility::CentreOrigin(m_failed_connection_text);

	//If this is the host, create a server
	sf::IpAddress ip;
	
	if (m_host)
	{
		m_game_server.reset(new GameServer(sf::Vector2f(m_window.getSize())));
		ip = "127.0.0.1";
	}
	else
	{
		ip = GetAddressFromFile();
	}

	if (m_socket.connect(ip, SERVER_PORT, sf::seconds(5.f)) != sf::Socket::Done)
	{
		m_connected = true;
	}
	else
	{
		m_failed_connection_clock.restart();
	}
	
	//Set socket to non-blocking
	m_socket.setBlocking(false);
	
	//Play the game music
	context.music->Play(MusicThemes::kMissionTheme);	
}

void MultiplayerGameState::Draw()
{
	if (m_connected)
	{
		m_world.Draw();
		
		//Show the broadcast message in default view
		m_window.setView(m_window.getDefaultView());
		
		if (!m_broadcasts.empty())
		{
			m_window.draw(m_broadcast_text);
		}
		
		if (m_local_player_identifiers.size() < 2 && m_player_invitation_time < sf::seconds(0.5f))
		{
			m_window.draw(m_player_invitation_text);
		}
	}
	else
	{
		m_window.draw(m_failed_connection_text);
	}
}

void MultiplayerGameState::Update(sf::Time dt)
{
}

void MultiplayerGameState::HandleEvent(const sf::Event& event)
{
}

void MultiplayerGameState::OnActivate()
{
}

void MultiplayerGameState::OnDestroy()
{
}

void MultiplayerGameState::DisableAllRealtimeActions()
{
}

void MultiplayerGameState::UpdateBroadcastMessage(sf::Time elpased_time)
{
}

void MultiplayerGameState::HandlePacket(sf::Int32 packet_type, sf::Packet& packet)
{
}
