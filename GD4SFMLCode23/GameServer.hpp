#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>

class GameServer
{
public:
	explicit GameServer(sf::Vector2f battlefield_size);
	~GameServer();
	void NotifyPlayerSpawn(sf::Int32 aircarft_identifer);
	void NotifyPlayerRealtimeChange(sf::Int32 aircraft_identifer, sf::Int32 action, bool action_enabled);
	void NotifyPlayerEvent(sf::Int32 aircraft_identifier, sf::Int32 action);

private:
	struct RemotePeer
	{
		RemotePeer();
		sf::TcpSocket m_socket;
		sf::Time m_last_packet_time;
		std::vector<sf::Int32> m_aircraft_identifier;
		bool m_ready;
		bool m_timed_out;
	};

	struct AircraftInfo
	{
		sf::Vector2f m_position;
		sf::Int32 m_hitpoints;
		sf::Int32 m_missile_ammo;
		std::map<sf::Int32, bool> m_realtime_actions;
	};

	typedef std::unique_ptr<RemotePeer> PeerPtr;

private:
	void SetListening(bool enable);
	void ExecutionThread();
	void Tick();
	sf::Time Now() const;

	void HandleIncomingPackets();
	void HandleIncomingPacket(sf::Packet& packet, RemotePeer& receiving_peer, bool& detected_timeout);

};

