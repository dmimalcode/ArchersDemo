#pragma once

namespace ArchersDemo
{
	struct PositionComponent;

	struct BotComponent
	{
		enum class Team
		{
			teamA,
			teamB
		};

		Team _team;
		float _speed = 10;
		float _fireRange = 40;
		float _reloadCooldown = 0;
		float _targetPos[2];
		float _radius = 0.25f;

		void Update(Game* pGame, entt::entity eid, PositionComponent& positionComponent);
	};

	struct HealthComponent
	{
		int _hp = 100;

		bool Damage(int hp = 20);
	};

	struct PositionComponent
	{
		float x = 0;
		float y = 0;
		float h = 0; // Altitude.
	};

	struct ProjectileComponent
	{
		float vx = 0;
		float vy = 0;
		float vh = 0;

		void Update(Game* pGame, entt::entity eid, PositionComponent& positionComponent);
	};

	struct SpriteComponent
	{
		SDL_Surface* _pSurface = nullptr;
		SDL_Rect _rcSrc;
		SDL_Rect _rcDst;

		SpriteComponent(const char* path);
		~SpriteComponent();

		void FreeSurface();
	};
}
