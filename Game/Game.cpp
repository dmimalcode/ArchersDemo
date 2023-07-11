#include "../framework.h"

using namespace ArchersDemo;

Game::Game() : _arena(this), _botSystem(this), _projectileSystem(this), _spriteSystem(this)
{
}

Game::~Game()
{
	Destroy();
}

void Game::Create()
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		throw std::exception("SDL_Init failed.");

	_ticks = SDL_GetTicks();

	_pWindow = SDL_CreateWindow("ArchersDemo",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_RESIZABLE);
	if (!_pWindow)
		throw std::exception("SDL_CreateWindow failed.");

	_pWindowSurface = SDL_GetWindowSurface(_pWindow);

	_pBgSurface = SDL_LoadBMP("Data/Background.bmp");

	_arena.Create();
}

void Game::Loop()
{
	bool quit = false;
	bool resized = false;
	SDL_Event event = {};

	while (!quit) // Game loop.
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				quit = true;
			}
			break;
			case SDL_WINDOWEVENT:
			{
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					resized = true;
				}
				break;
				}
			}
			break;
			}
		}

		if (resized)
		{
			resized = false;
			_pWindowSurface = SDL_GetWindowSurface(_pWindow);
		}

		// Update every 0.05s.
		if (SDL_GetTicks() - _ticks < 50)
			continue;
		_ticks = SDL_GetTicks();

		Update();
		Draw();
	}
}

void Game::Destroy()
{
	if (_pBgSurface)
	{
		SDL_FreeSurface(_pBgSurface);
		_pBgSurface = nullptr;
	}

	if (_pWindowSurface)
	{
		SDL_FreeSurface(_pWindowSurface);
		_pWindowSurface = nullptr;
	}

	if (_pWindow)
	{
		SDL_DestroyWindow(_pWindow);
		_pWindow = nullptr;
	}

	SDL_Quit();
}

void Game::Update()
{
	// Spawn units every 0.1s. If update is every 0.05s, skip every second update.
	if (_skipSpawn)
	{
		_skipSpawn = false;
	}
	else
	{
		if (_spawnCount > 0)
		{
			_spawnCount--;
			_skipSpawn = true;

			CreateUnit(BotComponent::Team::teamA);
			CreateUnit(BotComponent::Team::teamB);
		}
	}

	_projectileSystem.Update(); // Physics
	_botSystem.Update(); // AI
	_spriteSystem.Update();
}

void Game::Draw()
{
	SDL_Rect rcSrc{ 0, 0, 256, 256 };
	SDL_Rect rcDst{ 0, 0, _pWindowSurface->w, _pWindowSurface->h };

	SDL_BlitScaled(_pBgSurface, &rcSrc, _pWindowSurface, &rcDst);

	_arena.Draw();
	_spriteSystem.Draw();

	SDL_UpdateWindowSurface(_pWindow); // Present
}

int Game::GetScreenWidth() const
{
	return _pWindowSurface->w;
}

int Game::GetScreenHeight() const
{
	return _pWindowSurface->h;
}

void Game::CreateUnit(BotComponent::Team team)
{
	float x = (team == BotComponent::Team::teamA) ? 10.f : 90.f;
	float y = (team == BotComponent::Team::teamA) ? 10.f : 90.f;

	const auto entity = _registry.create();
	_registry.emplace<BotComponent>(entity, team);
	_registry.emplace<HealthComponent>(entity, 100);
	_registry.emplace<PositionComponent>(entity, x, y);
	_registry.emplace<SpriteComponent>(entity, (team == BotComponent::Team::teamA) ? "Data/UnitA.bmp" : "Data/UnitB.bmp");
}

void Game::DestroyUnit(entt::entity eid)
{
	_registry.destroy(eid);
}

float Game::FindClosestUnit(BotComponent::Team team, float pos[2], float targetPos[2], entt::entity exclude)
{
	targetPos[0] = _arena.GetWidth() * 0.5f;
	targetPos[1] = _arena.GetHeight() * 0.5f;
	float minDistSq = FLT_MAX;
	auto view = _registry.view<BotComponent, PositionComponent>();
	view.each([this, team, pos, targetPos, exclude, &minDistSq](auto eid, auto& botComponent, auto& positionComponent)
		{
			if (eid == exclude)
				return; // It's me
			if (botComponent._team == team)
			{
				const float dx = positionComponent.x - pos[0];
				const float dy = positionComponent.y - pos[1];
				const float distSq = dx * dx + dy * dy;
				if (distSq < minDistSq)
				{
					minDistSq = distSq;
					targetPos[0] = positionComponent.x;
					targetPos[1] = positionComponent.y;
				}
			}
		});
	return minDistSq;
}

void Game::CreateArrow(float posFrom[2], float posTo[2])
{
	const float g = 9.8f;
	const float speed = 40;
	float dx = posTo[0] - posFrom[0];
	float dy = posTo[1] - posFrom[1];
	const float len = sqrt(dx * dx + dy * dy);
	dx /= len;
	dy /= len;

	const float angle = 0.5f * asin((g * len) / (speed * speed)); // Angle of reach

	const float hPart = sin(angle);
	const float vPart = cos(angle);
	const float vx = speed * hPart * dx;
	const float vy = speed * hPart * dy;
	const float vh = speed * vPart;

	const auto entity = _registry.create();
	_registry.emplace<PositionComponent>(entity, posFrom[0], posFrom[1]);
	_registry.emplace<ProjectileComponent>(entity, vx, vy, vh); // Initial velocity
	_registry.emplace<SpriteComponent>(entity, "Data/Arrow.bmp");
}

void Game::DestroyArrow(entt::entity eid)
{
	_registry.get<SpriteComponent>(eid).FreeSurface();
	_registry.destroy(eid);
}

void Game::Damage(float pos[2])
{
	entt::entity target = entt::null;
	float minDistSq = FLT_MAX;
	auto view = _registry.view<HealthComponent, PositionComponent>();
	view.each([this, pos, &minDistSq, &target](auto eid, auto& healthComponent, auto& positionComponent)
		{
			const float dx = positionComponent.x - pos[0];
			const float dy = positionComponent.y - pos[1];
			const float distSq = dx * dx + dy * dy;
			if (distSq < minDistSq)
			{
				minDistSq = distSq;
				target = eid;
			}
		});

	if (minDistSq > 1)
		return;

	if (_registry.valid(target))
	{
		if (_registry.get<HealthComponent>(target).Damage())
		{
			DestroyUnit(target);
		}
	}
}
