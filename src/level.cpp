#include "level.h"
#include <fstream>

void Level::createWholeGround(GroundType ground_type)
{
	for (int i = (int)(ground_size_x / 2); i <= screen_width - ground_size_x / 2; i += (int)ground_size_x)
	{
		createGround(ground_type, 0.f + i, screen_height - ground_size_x / 2);
	}
	if (level_type == GroundType::Normal)
		objects.push_back(new Ground(world, screen_width, 0.1, sf::Color(147, 219, 36),
			screen_width / 2, screen_height - (ground_size_y + 0.5f)));
	else if (level_type == GroundType::Moon)
		objects.push_back(new Ground(world, screen_width, 0.1, sf::Color(243, 236, 220),
			screen_width / 2, screen_height - (ground_size_y + 0.5f)));
}
void Level::createBoundaries()
{
	// Left boundary
	objects.push_back(new Ground(world, 0, screen_height, GroundType::Blank, 0, screen_height / 2));
	// Up boundary
	objects.push_back(new Ground(world, screen_width, 0, GroundType::Blank, screen_width / 2, 0));
	// Right boundary
	objects.push_back(new Ground(world, 0, screen_height, GroundType::Blank, screen_width, screen_height / 2));
}
void Level::createThrowables(int throwable_amount, float radius)
{
	if (throwable_amount <= 1)
	{
		throwables[0] = new DefaultThrowable(world, radius);
		throwables[1] = new DefaultThrowable(world, radius);
		throwables[2] = new DefaultThrowable(world, radius);
	}
	else if (throwable_amount == 2)
	{
		throwables[0] = new DefaultThrowable(world, radius);
		throwables[1] = new DefaultThrowable(world, radius);
		throwables[2] = new Cluster(world, radius);
		thr_clus = reinterpret_cast<Cluster*>(throwables[2]);
	}

	else if (throwable_amount >= 3)
	{
		throwables[0] = new DefaultThrowable(world, radius);
		throwables[1] = new Cluster(world, radius);
		throwables[2] = new Bombarding(world, radius);
		thr_clus = reinterpret_cast<Cluster*>(throwables[1]);
		thr_bom = reinterpret_cast<Bombarding*>(throwables[2]);
	}

	active_throwable = throwables[0];
}

void Level::destroyAllObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->physics != nullptr)
		{
			objects[i]->deleteObject();
		}
		delete objects[i];
		objects[i] = nullptr;
	}
	objects.clear();
}

Level::Level(int level_number, int throwable_number, GroundType level_type, 
	int screen_width, int screen_height)
{
	this->screen_width = screen_width;
	this->screen_height = screen_height;
	this->level_number = level_number;
	this->throwable_number = throwable_number;
	this->level_type = level_type;
	if (level_type == GroundType::Normal)
	{
		gravity.Set(0.f, 9.8f);
		background_texture.loadFromFile("textures/background2_sky.png");
	}
	else if (level_type == GroundType::Moon)
	{
		gravity.Set(0.f, 1.6f);
		background_texture.loadFromFile("textures/moon_background.png");
	}
	world = new b2World(gravity);
	world->SetContactListener(&contact_listener_instance);
	background.setSize(sf::Vector2f(screen_width, screen_height));
	background.setTexture(&background_texture);
}
Level::~Level()
{
	destroyAllObjects();
	delete world;
	world = nullptr;
}

void Level::createGround(GroundType ground_type, float x, float y)
{
	objects.push_back(new Ground(world, ground_size_x, ground_size_y, ground_type, x, y));
}
void Level::createObstacle(ObstacleType obstacle_type, float x, float y,
	float box_size_x, float box_size_y)
{
	objects.push_back(new Obstacle(world, box_size_x, box_size_y, obstacle_type, x, y));
}
void Level::createEnemy(float x, float y, float box_size_x, float box_size_y)
{
	objects.push_back(new Enemy(world, box_size_x, box_size_y, x, y));
}

void Level::makeLevel()
{
	score = 0;
	createWholeGround(level_type);
	createBoundaries();
	createThrowables(throwable_number);
	// Switch for additional spawns, outside of file interpretation
	switch (level_number)
	{
	case 1:

		break;
	case 2:

		break;
	case 3:
		for (int i = 0; i < 15; i++)
		{
			createObstacle(ObstacleType::Wood,
				33 * 35.f + 15 + i % 8,
				15 * 35.f,
				15 + i % 8,
				15 + i % 8);
		}
		for (int i = 0; i < 4; i++)
		{
			createObstacle(ObstacleType::Stone,
				9 * 35.f + i % 10,
				19 * 35.f + 17.5f,
				15 + i % 10,
				15 + i % 8);
			createObstacle(ObstacleType::Stone,
				8 * 35.f + 6 + i % 10,
				20 * 35.f,
				12 + i % 10,
				12 + i % 8);
		}
		break;
	case 4:
		for (int i = 0; i < 60; i++)
		{
			createObstacle(ObstacleType::Stone,
				17 * 35.f + 7 + i % 4,
				12 * 35.f,
				7 + i % 4,
				7 + i % 4);
		}
		break;
	case 5:
		for (int i = 0; i < 70; i++)
		{
			createObstacle(ObstacleType::Metal,
				22 * 35.f + 8 + i % 6,
				12 * 35.f,
				10 + i % 4,
				10 + i % 4);
		}
		break;
	case 6:
		createGround(level_type, 500, 500);
		createObstacle(ObstacleType::Wood, 500, 475);
		createObstacle(ObstacleType::Stone, 600, 600, 100, 35);
		createObstacle(ObstacleType::Metal, 600, 600, 35, 100);
		createObstacle(ObstacleType::Wood, 1300, 750);
		createObstacle(ObstacleType::Wood, 1300, 725);
		createObstacle(ObstacleType::Wood, 1300, 690);
		createObstacle(ObstacleType::Wood, 1300, 655);
		createObstacle(ObstacleType::Wood, 1100, 750);
		createObstacle(ObstacleType::Wood, 1100, 725);
		createObstacle(ObstacleType::Wood, 1100, 690);
		createObstacle(ObstacleType::Wood, 1100, 655);
		createObstacle(ObstacleType::Metal, 1200, 620, 235);
		createEnemy(800, 750);
		createEnemy(1200, 750);
		break;
	default:
		break;
	}
}

void Level::throwableUsed()
{
	objects.push_back(active_throwable);
	active_throwable->setThrowableState(ThrowableState::Used);
}
void Level::createClusterAdds()
{
	if (thr_clus != nullptr
		&& contact_listener_instance.cluster_contact == true
		&& thr_clus->collided == false)
	{
		thr_clus->collided = true;
		Cluster* cluster_add[8];
		for (int i = 0; i < 8; i++)
		{
			cluster_add[i] = new Cluster(world, 15.f, true,
				thr_clus->graphics.getPosition().x, thr_clus->graphics.getPosition().y);
			objects.push_back(cluster_add[i]);
		}
		cluster_add[0]->physics->SetLinearVelocity(b2Vec2(20, 20));
		cluster_add[1]->physics->SetLinearVelocity(b2Vec2(20, 20));
		cluster_add[2]->physics->SetLinearVelocity(b2Vec2(-20, 20));
		cluster_add[3]->physics->SetLinearVelocity(b2Vec2(-20, 20));
		cluster_add[4]->physics->SetLinearVelocity(b2Vec2(20, -20));
		cluster_add[5]->physics->SetLinearVelocity(b2Vec2(20, -20));
		cluster_add[6]->physics->SetLinearVelocity(b2Vec2(-20, -20));
		cluster_add[7]->physics->SetLinearVelocity(b2Vec2(-20, -20));
	}
}
void Level::createBombardingAdds()
{
	if (thr_bom->adds_used < 3 && thr_bom->is_moving == true)
	{
		thr_bom->adds_used++;
		Bombarding* bombarding_add = new Bombarding(world, 20.f, true,
			thr_bom->graphics.getPosition().x, thr_bom->graphics.getPosition().y + 30);
		objects.push_back(bombarding_add);
		bombarding_add->physics->SetLinearVelocity(b2Vec2(0, 50));
	}
}
void Level::checkThrowableMovement()
{
	for (int i = 0; i < 3; i++)
	{
		if (throwables[i] != nullptr && throwables[i]->getThrowableState() == ThrowableState::Used)
			throwables[i]->checkIfMoving();
	}
}

void Level::destroyFlaggedObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->physics != nullptr)
		{
			if (objects[i]->hp <= 0)
			{
				score += objects[i]->max_hp * 10;
				objects[i]->deleteObject();

				objects.erase(std::remove(objects.begin(), objects.end(),
					objects[i]), objects.end());
			}
			else if (objects[i]->hp <= (objects[i]->max_hp / 2))
			{
				objects[i]->setAsDamaged();
			}
		}
	}
}

bool Level::isLevelWon()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->physics != nullptr)
		{
			if (objects[i]->object_type == ObjectType::Enemy)
				return false;
		}
	}
	return true;
}
bool Level::isLevelLost()
{
	for (int i = 0; i < 3; i++)
	{
		if (throwables[i]->is_moving == true)
			return false;
	}
	return true;
}

void Level::addPointsForUnusedThrows()
{
	for (int i = 0; i < 3; i++)
	{
		if (throwables[i]->getThrowableState() != ThrowableState::Used)
		{
			if (throwables[i]->throwable_type == ThrowableType::Default)
				score += 3000;
			else // Cluster and Bombarding
				score += 5000;
		}
	}
}

b2World* Level::getWorldPointer()
{
	return world;
}
std::vector<SimulatedObject*>* Level::getObjects()
{
	return &objects;
}
Throwable* Level::getActiveThrowable()
{
	return active_throwable;
}
sf::RectangleShape Level::getBackgroud()
{
	return background;
}
int Level::getScore()
{
	return score;
}
int Level::getLevelNumber()
{
	return level_number;
}
GroundType Level::getLevelType()
{
	return level_type;
}
Throwable* Level::getThrowableByNumber(int throwable_number)
{
	return throwables[throwable_number];
}

void Level::setActiveThrowable(int throwable_number)
{
	if (throwables[throwable_number] != nullptr
		&& throwables[throwable_number]->getThrowableState() != ThrowableState::Used)
		active_throwable = throwables[throwable_number];
}