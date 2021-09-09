#include "simulated_object.h"
#include <iostream>
#include "converter.h"

void SimulatedObject::initializePosition(b2World* world, float x, float y)
{
	this->x = x;
	this->y = y;
	this->world = world;
	try
	{
		if (world == nullptr)
			throw 1;
	}
	catch (int e)
	{
		std::cerr << "World pointer can't be nullptr." << std::endl;
		return;
	}
}
void SimulatedObject::updatePosition()
{
	graphics.setPosition(Converter::metersToPixels(physics->GetPosition().x),
		Converter::metersToPixels(physics->GetPosition().y));
	graphics.setRotation(Converter::radToDeg(physics->GetAngle()));
}
void SimulatedObject::deleteObject()
{
	if (physics != nullptr)
	{
		world->DestroyBody(physics);
		physics = nullptr;
	}
}
void SimulatedObject::setAsDamaged()
{
	graphics.setTexture(&texture_damaged);
}

Obstacle::Obstacle(b2World* world, float block_size_x, float block_size_y,
	ObstacleType obstacle_type, float x, float y)
{
	object_type = ObjectType::Obstacle;
	if (obstacle_type == ObstacleType::Wood)
	{
		this->obstacle_type = ObstacleType::Wood;
	}
	else if (obstacle_type == ObstacleType::Stone)
	{
		this->obstacle_type = ObstacleType::Stone;
	}
	else // Metal
	{
		this->obstacle_type = ObstacleType::Metal;
	}
	initializePosition(world, x, y);
	this->block_size_x = block_size_x;
	this->block_size_y = block_size_y;
	setType(obstacle_type);
	createObject();
}
void Obstacle::createObject()
{
	b2BodyDef body_def;
	body_def.position = b2Vec2(Converter::pixelsToMeters(x), Converter::pixelsToMeters(y));
	body_def.type = b2_dynamicBody;
	physics = world->CreateBody(&body_def);
	b2PolygonShape shape;
	shape.SetAsBox(Converter::pixelsToMeters(block_size_x / 2),
		Converter::pixelsToMeters(block_size_y / 2));
	b2FixtureDef fixture_def;
	if (obstacle_type == ObstacleType::Wood)
	{
		fixture_def.density = 0.7f;
	}
	else if (obstacle_type == ObstacleType::Stone)
	{
		fixture_def.density = 1.3f;
	}
	else // Metal
	{
		fixture_def.density = 1.7f;
	}
	fixture_def.friction = 0.5f;
	fixture_def.shape = &shape;
	fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);
	physics->CreateFixture(&fixture_def);
}
void Obstacle::setType(ObstacleType obstacle_type)
{
	if (obstacle_type == ObstacleType::Wood)
	{
		max_hp = 20;
		texture.loadFromFile("textures/box.png");
		texture_damaged.loadFromFile("textures/box_damaged.png");

	}
	else if (obstacle_type == ObstacleType::Stone)
	{
		max_hp = 35;
		texture.loadFromFile("textures/stone.png");
		texture_damaged.loadFromFile("textures/stone_damaged.png");
	}
	else // Metal
	{
		max_hp = 60;
		texture.loadFromFile("textures/metal.png");
		texture_damaged.loadFromFile("textures/metal_damaged.png");
	}
	hp = max_hp;
	texture.setSmooth(true);
	texture_damaged.setSmooth(true);
	graphics.setSize(sf::Vector2f(block_size_x, block_size_y));
	graphics.setTexture(&texture);
	graphics.setOrigin(block_size_x / 2, block_size_y / 2);
}

Ground::Ground(b2World* world, float block_size_x, float block_size_y,
	GroundType ground_type, float x, float y)
{
	object_type = ObjectType::Ground;
	initializePosition(world, x, y);
	this->block_size_x = block_size_x;
	this->block_size_y = block_size_y;
	if (ground_type == GroundType::Normal)
	{
		texture.loadFromFile("textures/grass.png");
		texture.setSmooth(true);
	}
	else if (ground_type == GroundType::Moon)
	{
		texture.loadFromFile("textures/moon3.png");
		texture.setSmooth(true);
	}
	else if (ground_type == GroundType::Blank)
	{

	}
	graphics.setSize(sf::Vector2f(block_size_x, block_size_y));
	graphics.setTexture(&texture);
	graphics.setOrigin(block_size_x / 2, block_size_y / 2);
	createObject();
}
Ground::Ground(b2World* world, float block_size_x, float block_size_y,
	sf::Color color, float x, float y)
{
	object_type = ObjectType::Ground;
	initializePosition(world, x, y);
	this->block_size_x = block_size_x;
	this->block_size_y = block_size_y;
	graphics.setFillColor(color);
	graphics.setSize(sf::Vector2f(block_size_x, block_size_y));
	graphics.setOrigin(block_size_x / 2, block_size_y / 2);
	createObject();
}
void Ground::createObject()
{
	b2BodyDef body_def;
	body_def.position = b2Vec2(Converter::pixelsToMeters(x), Converter::pixelsToMeters(y));
	body_def.type = b2_staticBody;
	physics = world->CreateBody(&body_def);
	b2PolygonShape shape;
	shape.SetAsBox(Converter::pixelsToMeters(block_size_x / 2),
		Converter::pixelsToMeters(block_size_y / 2));
	b2FixtureDef fixture_def;
	fixture_def.density = 0.f;
	fixture_def.shape = &shape;
	fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);
	physics->CreateFixture(&fixture_def);
}

Enemy::Enemy(b2World* world, float block_size_x, float block_size_y, float x, float y)
{
	max_hp = 5;
	hp = max_hp;
	object_type = ObjectType::Enemy;
	initializePosition(world, x, y);
	this->block_size_x = block_size_x;
	this->block_size_y = block_size_y;
	texture.loadFromFile("textures/angry_block.png");
	texture.setSmooth(true);
	texture_damaged.loadFromFile("textures/angry_block_damaged.png");
	texture_damaged.setSmooth(true);
	graphics.setSize(sf::Vector2f(block_size_x, block_size_y));
	graphics.setTexture(&texture);
	graphics.setOrigin(block_size_x / 2, block_size_y / 2);
	createObject();
}
void Enemy::createObject()
{
	b2BodyDef body_def;
	body_def.position = b2Vec2(Converter::pixelsToMeters(x), Converter::pixelsToMeters(y));
	body_def.type = b2_dynamicBody;
	physics = world->CreateBody(&body_def);
	b2PolygonShape shape;
	shape.SetAsBox(Converter::pixelsToMeters(block_size_x / 2),
		Converter::pixelsToMeters(block_size_y / 2));
	b2FixtureDef fixture_def;
	fixture_def.density = 1.f;
	fixture_def.friction = 0.7f;
	fixture_def.shape = &shape;
	fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);
	physics->CreateFixture(&fixture_def);
}