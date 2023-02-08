#pragma once
#include "Command.hpp"
#include "Action.hpp"
#include "MissionStatus.hpp"
#include <SFML/Window/Event.hpp>
#include <map>
#include "KeyBinding.hpp"
#include "CommandQueue.hpp"
#include <SFML/Network/TcpSocket.hpp>


class Player
{
public:
	Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding);
	void HandleEvent(const sf::Event& event, CommandQueue& command);
	void HandleRealtimeInput(CommandQueue& command);
	void HandleRealtimeNetworkInput(CommandQueue& commands);

	//React to events or realtime state changes recevied over the network
	void HandleNetworkEvent(Action action, CommandQueue& commands);
	void HandleNetworkRealtimeChange(Action action, bool action_enabled);

	void SetMissionStatus(MissionStatus status);
	MissionStatus GetMissionStatus() const;
	
	void DisableAllRealtimeActions();
	bool IsLocal() const;

private:
	void InitializeActions();

private:
	const KeyBinding* m_key_binding;
	std::map<Action, Command> m_action_binding;
	std::map<Action, bool> m_action_proxies;
	MissionStatus m_current_mission_status;
	int m_identifier;
	sf::TcpSocket* m_socket;
};

