#include "GameBoard.h"

#include "GameEngine\config.h"
#include "GameEngine\GameEngineMain.h"
#include "GameEngine\EntitySystem\Components\CollidableComponent.h"
#include "GameEngine\EntitySystem\Components\SpriteRenderComponent.h"
#include "GameEngine\Helper\MathHelpers.h"
#include "GameEngine\Util\CameraManager.h"
#include "GameEngine\Util\StateManager.h"
#include "Game\GameEntities\PlayerEntity.h"
#include "Game\GameEntities\ProjectileEntity.h"
#include "GameEngine\EntitySystem\Components\TextComponent.h"
#include "Game\GameEntities\TextEntity.h"

using namespace Game;

GameBoard::GameBoard()
	: m_lastProjectileSpawnTimer(0.f)
	, m_isGameOver(false)
	, m_player(nullptr)
	, m_backGround(nullptr)
{
	m_player = new PlayerEntity();
	m_text = new TextEntity();
	
	GameEngine::StateManager::GetInstance()->state
		->AddEntity(m_player);
	m_player->SetPos(sf::Vector2f(GameEngine::WINDOW_WIDTH / 2.0, GameEngine::WINDOW_HEIGHT / 2.0));
	m_player->SetSize(sf::Vector2f(219.0f, 218.0f));
	
	//sf::String testString = "I hope this works.";
	//GameEngine::StateManager::GetInstance()->state->AddEntity(m_text);
	//m_text->SetPos(sf::Vector2f(400.f, 400.f));
	//m_text->SetSize(sf::Vector2f(272.5f, 162.5f));
		

	CreateBackGround();
	//Debug
	for (int a = 0; a < 3; ++a)
	{
		//SpawnNewRandomObstacles();
	}
}


GameBoard::~GameBoard()
{

}


void GameBoard::Update()
{	
	float dt = GameEngine::GameEngineMain::GetInstance()->GetTimeDelta();
	if (!m_isGameOver)
	{
		m_lastProjectileSpawnTimer -= dt;
		if (m_lastProjectileSpawnTimer <= 0.f)
		{
			//SpawnNewRandomObstacles();
			//SpawnNewRandomTiledObstacles();
			SpawnNewRandomTrash();
		}

		UpdateProjectiles(dt);
		UpdateBackGround();
		UpdatePlayerDying();
	}		
}


void GameBoard::UpdateProjectiles(float dt)
{
	
	for (std::vector<GameEngine::Entity*>::iterator it = m_projectiles.begin(); it != m_projectiles.end();)
	{
		GameEngine::Entity* projectile = (*it);
		projectile->Update();
		//sf::Vector2f currPos = obstacle->GetPos();
		//currPos.x -= obstacleSpeed * dt;
		//obstacle->SetPos(currPos);
		//If we are to remove ourselves
		/*if (projectile->GetPos().x + 60 < GameEngine::WINDOW_WIDTH / 2 &&
			projectile->GetPos().x - 60 < GameEngine::WINDOW_WIDTH / 2 &&
			projectile->GetPos().y + 60 < GameEngine::WINDOW_HEIGHT / 2 &&
			projectile->GetPos().x - 60 < GameEngine::WINDOW_HEIGHT/ 2)
		{
			GameEngine::StateManager::GetInstance()->state
				->RemoveEntity(projectile);
			it = m_projectiles.erase(it);
		}
		else
		{
			it++;
		}*/
		it++;
	}
}


void GameBoard::UpdatePlayerDying()
{	
	bool noGameOver = GameEngine::CameraManager::IsFollowCameraEnabled();

	if (noGameOver)
		return;

	static float xToPlayerDie = 0.f;
	if (m_player->GetPos().x < xToPlayerDie)
	{
		m_isGameOver = true;
	}
}


void GameBoard::SpawnNewRandomProjectiles()
{
	if (m_projectiles.size() > 0) return;
	static float minNextSpawnTime = 0.3f;
	static float maxNextSpawnTime = 0.7f;

	static float minObstacleXPos = 50.f;
	static float maxObstacleXPos = 450.f;
	static float minObstacleYPos = 20.f;
	static float maxObstacleYPos = 450.f;
	
	static float minObstacleHeight = 50.f;
	static float maxObstacleHeight = 170.f;
	static float minObstacleWidth = 20.f;
	static float maxObstacleWidth = 40.f;

	sf::Vector2f pos = sf::Vector2f(MathHelpers::RandFloatIn(minObstacleXPos, maxObstacleXPos), MathHelpers::RandFloatIn(minObstacleYPos, maxObstacleYPos));
	sf::Vector2f size = sf::Vector2f(MathHelpers::RandFloatIn(minObstacleWidth, maxObstacleWidth), MathHelpers::RandFloatIn(minObstacleHeight, maxObstacleHeight));

	SpawnNewProjectile(pos, size);

	m_lastProjectileSpawnTimer = MathHelpers::RandFloatIn(minNextSpawnTime, maxNextSpawnTime);
}


void GameBoard::SpawnNewRandomTrash()
{
	static int minTrashCount = 1;
	static int maxTrashCount = 3;

	static float minNextSpawnTime = 0.8f;
	static float maxNextSpawnTime = 1.5f;

	static float leftTrashXPos = 60.f;
	static float rightTrashXPos = 800.f;
	static float topTrashYPos = 0.f;
	static float bottomTrashYPos = 800.f;

	sf::Vector2f pos = sf::Vector2f(0.f, 0.f);
	sf::Vector2f size = sf::Vector2f(0.f, 0.f);

	int trashCount = (int)MathHelpers::RandFloatIn((float)minTrashCount, (float)maxTrashCount);
	for (int a = 0; a < trashCount; ++a)
	{
		int spawnOnLRSide = (rand() > RAND_MAX / 2) ? 0 : 1;

		if (spawnOnLRSide) {
			pos = sf::Vector2f((rand() > RAND_MAX / 2) ? leftTrashXPos : rightTrashXPos, MathHelpers::RandFloatIn(topTrashYPos, bottomTrashYPos));
			size = sf::Vector2f(50.f, 50.f);
		}
		else if (!spawnOnLRSide) {
			pos = sf::Vector2f(MathHelpers::RandFloatIn(leftTrashXPos, rightTrashXPos), (rand() > RAND_MAX / 2) ? topTrashYPos : bottomTrashYPos);
			size = sf::Vector2f(50.f, 50.f);
		}
		SpawnNewProjectile(pos, size);
	}
	m_lastProjectileSpawnTimer = MathHelpers::RandFloatIn(minNextSpawnTime, maxNextSpawnTime);
}

void GameBoard::SpawnNewProjectile(const sf::Vector2f& pos, const sf::Vector2f& size)
{
	int trashIndex = rand() % (12-4 + 1)+4;
	ProjectileEntity* projectile = new ProjectileEntity(trashIndex);
	GameEngine::StateManager::GetInstance()->state
		->AddEntity(projectile);
	projectile->SetPos(pos);
	projectile->SetSize(sf::Vector2f(size.x, size.y));

	m_projectiles.push_back(projectile);
}


void GameBoard::CreateBackGround()
{
	GameEngine::Entity* bgEntity = new GameEngine::Entity();
	GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>
		(bgEntity->AddComponent<GameEngine::SpriteRenderComponent>());
	render->SetTexture(GameEngine::eTexture::BG);
	render->SetZLevel(0);
	bgEntity->SetPos(sf::Vector2f(400.f, 400.f));
	bgEntity->SetSize(sf::Vector2f(800.f, 800.f));
	GameEngine::StateManager::GetInstance()->state
		->AddEntity(bgEntity);

	m_backGround = bgEntity;
}


void GameBoard::UpdateBackGround()
{
	if (!m_backGround || !m_player)
		return;

	if (!GameEngine::CameraManager::IsFollowCameraEnabled())
		return;

	m_backGround->SetPos(m_player->GetPos());
}