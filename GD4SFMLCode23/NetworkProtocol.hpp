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
		kPlayerRealTimeChange,
		kRequestCoopPartner,
		kPositionUpdate,
		kGameEvent,
		kQuit
	};
}
