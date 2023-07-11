#pragma once

namespace ArchersDemo
{
	class BotSystem
	{
		Game* _pGame = nullptr;

	public:
		BotSystem(Game* pGame);
		~BotSystem();

		void Update();
	};

	class SpriteSystem
	{
		Game* _pGame = nullptr;

	public:
		SpriteSystem(Game* pGame);
		~SpriteSystem();

		void Update();
		void Draw();
	};

	class ProjectileSystem
	{
		Game* _pGame = nullptr;

	public:
		ProjectileSystem(Game* pGame);
		~ProjectileSystem();

		void Update();
	};
}
