#include "../framework.h"

using namespace ArchersDemo;

void BotComponent::Update(Game* pGame, entt::entity eid, PositionComponent& positionComponent)
{
	float pos[2] = { positionComponent.x, positionComponent.y };

	_reloadCooldown -= pGame->GetFixedDt();
	_reloadCooldown = std::max(_reloadCooldown, 0.f);

	// Spread apart
	const float r2 = _radius * 2;
	float minDistSq = pGame->FindClosestUnit(_team, pos, _targetPos, eid);
	if (minDistSq < r2 * r2)
	{
		const float rdX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1;
		const float rdY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1;

		float dx = _targetPos[0] - positionComponent.x + rdX;
		float dy = _targetPos[1] - positionComponent.y + rdY;
		const float len = sqrt(dx * dx + dy * dy);
		dx /= len;
		dy /= len;
		positionComponent.x -= _speed * dx * pGame->GetFixedDt();
		positionComponent.y -= _speed * dy * pGame->GetFixedDt();
	}
	else // Move or Attack
	{
		float minDistSq = pGame->FindClosestUnit((_team == Team::teamA) ? Team::teamB : Team::teamA, pos, _targetPos, eid);
		if (minDistSq <= _fireRange * _fireRange)
		{
			if (_reloadCooldown <= 0)
			{
				_reloadCooldown = 1;
				pGame->CreateArrow(pos, _targetPos);
			}
		}
		else // Move to target
		{
			float dx = _targetPos[0] - positionComponent.x;
			float dy = _targetPos[1] - positionComponent.y;
			const float len = sqrt(dx * dx + dy * dy);
			dx /= len;
			dy /= len;
			positionComponent.x += _speed * dx * pGame->GetFixedDt();
			positionComponent.y += _speed * dy * pGame->GetFixedDt();
		}
	}
}


bool HealthComponent::Damage(int hp)
{
	if (_hp > 0)
	{
		_hp -= hp;
		if (_hp <= 0)
		{
			return true;
		}
	}
	return false;
}


void ProjectileComponent::Update(Game* pGame, entt::entity eid, PositionComponent& positionComponent)
{
	const float g = 9.8f;

	vh -= g * pGame->GetFixedDt();

	positionComponent.x += vx * pGame->GetFixedDt();
	positionComponent.y += vy * pGame->GetFixedDt();
	positionComponent.h += vh * pGame->GetFixedDt();

	if (positionComponent.h < 0) // Reached the ground
	{
		float pos[2] = { positionComponent.x, positionComponent.y };
		pGame->Damage(pos);
		pGame->DestroyArrow(eid);
	}
}


SpriteComponent::SpriteComponent(const char* path)
{
	_pSurface = SDL_LoadBMP(path);
	_rcSrc.x = 0;
	_rcSrc.y = 0;
	_rcSrc.w = _pSurface->w;
	_rcSrc.h = _pSurface->h;
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::FreeSurface()
{
	if (_pSurface)
	{
		SDL_FreeSurface(_pSurface);
		_pSurface = nullptr;
	}
}
