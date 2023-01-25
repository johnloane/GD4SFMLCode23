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
{
}

void MultiplayerGameState::Draw()
{
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
