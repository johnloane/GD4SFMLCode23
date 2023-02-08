#pragma once
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	kPlayerAircraft = 1 << 1,
	kAlliedAircraft = 1 << 2,
	kEnemyAircraft = 1 << 3,
	kPickup = 1 << 4,
	kAlliedProjectile = 1 << 5,
	kEnemyProjectile = 1 << 6,
	kParticleSystem = 1 << 7,
	kSoundEffect = 1 << 8,
	kNetwork = 1 << 9,

	kAircraft = kPlayerAircraft | kAlliedAircraft | kEnemyAircraft,
	kProjectile = kAlliedProjectile | kEnemyProjectile,
};

//A message that would be sent to all aircraft
//unsigned int all_aircraft = ReceiverCategories::kPlayerAircraft | ReceiverCategories::kAlliedAircraft | ReceiverCategories::kEnemyAircraft;