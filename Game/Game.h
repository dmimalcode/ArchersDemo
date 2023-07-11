#pragma once

namespace ArchersDemo
{
	class Game
	{
		SDL_Window* _pWindow = nullptr;
		SDL_Surface* _pWindowSurface = nullptr;
		SDL_Surface* _pBgSurface = nullptr;

		entt::registry _registry;

		Arena _arena;

		BotSystem _botSystem;
		ProjectileSystem _projectileSystem;
		SpriteSystem _spriteSystem;

		Uint32 _ticks = 0;
		float _fixedDt = 0.05f;
		int _spawnCount = 20;
		bool _skipSpawn = false;

	public:
		Game();
		~Game();

		void Create();
		void Loop();
		void Destroy();

		void Update();
		void Draw();

		float GetFixedDt() const { return _fixedDt; }

		SDL_Surface* GetWindowSurface() { return _pWindowSurface; }
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		entt::registry& GetRegistry() { return _registry; }

		Arena& GetArena() { return _arena; }

		void CreateUnit(BotComponent::Team team);
		void DestroyUnit(entt::entity eid);

		float FindClosestUnit(BotComponent::Team team, float pos[2], float targetPos[2], entt::entity exclude);

		void CreateArrow(float posFrom[2], float posTo[2]);
		void DestroyArrow(entt::entity eid);

		void Damage(float pos[2]);
	};
}
