#include "../framework.h"

using namespace ArchersDemo;

Arena::Arena(Game* pGame) : _pGame(pGame)
{
}

Arena::~Arena()
{
}

void Arena::Create()
{
	_pTileSurface = SDL_LoadBMP("Data/Tile.bmp");
}

void Arena::Destroy()
{
	if (_pTileSurface)
	{
		SDL_FreeSurface(_pTileSurface);
		_pTileSurface = nullptr;
	}
}

void Arena::Draw()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; ++j)
		{
			// Top-left corner:
			float tilePosTL[2] = { j * 10.f, i * 10.f }; // In meters.
			tilePosTL[0] += 5;
			tilePosTL[1] -= 5;
			ArenaSpaceToScreenSpace(tilePosTL[0], tilePosTL[1]);

			// Bottom-right corner:
			float tilePosBR[2] = { j * 10.f, i * 10.f };
			tilePosBR[0] += 5;
			tilePosBR[1] += 15;
			ArenaSpaceToScreenSpace(tilePosBR[0], tilePosBR[1]);

			SDL_Rect rcSrc{ 0, 0, 256, 128 };
			SDL_Rect rcDst{
				static_cast<int>(tilePosTL[0]),
				static_cast<int>(tilePosTL[1]),
				static_cast<int>(tilePosBR[0] - tilePosTL[0]),
				static_cast<int>(tilePosBR[1] - tilePosTL[1]) };

			SDL_BlitScaled(_pTileSurface, &rcSrc, _pGame->GetWindowSurface(), &rcDst);
		}
	}
}

void Arena::ArenaSpaceToScreenSpace(float& x, float& y) const
{
	const float u = x / _width;
	const float v = y / _height;

	const float aspectRatio = static_cast<float>(_pGame->GetScreenWidth()) / _pGame->GetScreenHeight();

	float rc[2] = {};
	float origin[2] = {};

	if (aspectRatio <= 2)
	{
		rc[0] = static_cast<float>(_pGame->GetScreenWidth());
		rc[1] = static_cast<float>(_pGame->GetScreenWidth() / 2);

		origin[0] = 0;
		origin[1] = static_cast<float>(_pGame->GetScreenHeight() / 2);
	}
	else
	{
		rc[0] = static_cast<float>(_pGame->GetScreenHeight() * 2);
		rc[1] = static_cast<float>(_pGame->GetScreenHeight());

		origin[0] = static_cast<float>((_pGame->GetScreenWidth() - rc[0]) / 2);
		origin[1] = static_cast<float>(_pGame->GetScreenHeight() / 2);
	}

	float basisX[2] = { rc[0] / 2 , -rc[1] / 2 };
	float basisY[2] = { rc[0] / 2 , rc[1] / 2 };

	x = origin[0] + basisX[0] * u + basisY[0] * v;
	y = origin[1] + basisX[1] * u + basisY[1] * v;
}

float Arena::GetTileScreenSpaceHeight() const
{
	const float aspectRatio = static_cast<float>(_pGame->GetScreenWidth()) / _pGame->GetScreenHeight();

	if (aspectRatio <= 2)
	{
		const float h = static_cast<float>(_pGame->GetScreenWidth() / 2);
		return h / _height;
	}
	else
	{
		const float h = static_cast<float>(_pGame->GetScreenHeight());
		return h / _height;
	}
}
