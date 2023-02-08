#pragma once
const unsigned short SERVER_PORT = 50000;

namespace Server
{
	//These are packets that come from the server
	enum class PacketType
	{
		kBroadcastMessage,
		kInitialState,
		kPlayerEvent,
		kPlayerRealtimeChange,
		kPlayerConnect,
		kPlayerDisconnect,
		kAcceptCoopPartner,
		kSpawnEnemy,
		kSpawnPickup,
		kSpawnSelf,
		kUpdateClientState,
		kMissionSuccess
	};
}

namespace Client
{
	//Messages sent from the client
	enum class PacketType
	{
		kPlayerEvent,
		kPlayerRealtimeChange,
		kRequestCoopPartner,
		kPositionUpdate,
		kGameEvent,
		kQuit
	};
}

namespace GameActions
{
	enum Type
	{
		kEnemyExplode
	};

	struct Action
	{
		Action() = default;
		Action(Type type, sf::Vector2f position) :type(type), position(position)
		{

		}

		Type type;
		sf::Vector2f position;
	};
}
