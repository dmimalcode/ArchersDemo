#pragma once

namespace ArchersDemo
{
	class Game;

	class Arena
	{
		Game* _pGame = nullptr;
		SDL_Surface* _pTileSurface = nullptr;
		float _width = 100;
		float _height = 100;

	public:
		Arena(Game* pGame);
		~Arena();

		void Create();
		void Destroy();

		void Draw();

		void ArenaSpaceToScreenSpace(float& x, float& y) const;
		float GetTileScreenSpaceHeight() const;

		float GetWidth() const { return _width; };
		float GetHeight() const { return _height; };
	};
}
