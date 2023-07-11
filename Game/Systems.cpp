#include "../framework.h"

using namespace ArchersDemo;

BotSystem::BotSystem(Game* pGame) : _pGame(pGame)
{
}

BotSystem::~BotSystem()
{
}

void BotSystem::Update()
{
	auto view = _pGame->GetRegistry().view<BotComponent, PositionComponent>();
	view.each([this](entt::entity eid, auto& botComponent, auto& positionComponent)
		{
			botComponent.Update(_pGame, eid, positionComponent);
		});
}


SpriteSystem::SpriteSystem(Game* pGame) : _pGame(pGame)
{
}

SpriteSystem::~SpriteSystem()
{
}

void SpriteSystem::Update()
{
	auto view = _pGame->GetRegistry().view<PositionComponent, SpriteComponent>();
	view.each([this](auto& positionComponent, auto& spriteComponent)
		{
			float x = positionComponent.x;
			float y = positionComponent.y;
			_pGame->GetArena().ArenaSpaceToScreenSpace(x, y);
			spriteComponent._rcDst.x = static_cast<int>(x);
			spriteComponent._rcDst.y = static_cast<int>(y);
			spriteComponent._rcDst.y -= static_cast<int>(_pGame->GetArena().GetTileScreenSpaceHeight() * positionComponent.h); // Altitude

			spriteComponent._rcDst.w = static_cast<int>(spriteComponent._rcSrc.w * _pGame->GetArena().GetTileScreenSpaceHeight() / 6.4f);
			spriteComponent._rcDst.h = static_cast<int>(spriteComponent._rcSrc.h * _pGame->GetArena().GetTileScreenSpaceHeight() / 6.4f);

			// Bottom-center point is the pivot
			spriteComponent._rcDst.x -= spriteComponent._rcDst.w / 2;
			spriteComponent._rcDst.y -= spriteComponent._rcDst.h;
		});
}

void SpriteSystem::Draw()
{
	auto view = _pGame->GetRegistry().view<SpriteComponent>();
	view.each([this](auto& spriteComponent)
		{
			SDL_BlitScaled(spriteComponent._pSurface, &spriteComponent._rcSrc, _pGame->GetWindowSurface(), &spriteComponent._rcDst);
		});
}


ProjectileSystem::ProjectileSystem(Game* pGame) : _pGame(pGame)
{
}

ProjectileSystem::~ProjectileSystem()
{
}

void ProjectileSystem::Update()
{
	auto view = _pGame->GetRegistry().view<PositionComponent, ProjectileComponent>();
	view.each([this](entt::entity eid, auto& positionComponent, auto& projectileComponent)
		{
			projectileComponent.Update(_pGame, eid, positionComponent);
		});
}
