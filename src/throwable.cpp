#include "throwable.h"
#include <iostream>
#include "converter.h"
#include <cstdlib>

void Throwable::maximumVelocityLimitation(float& velocity, float max_velocity)
{
	if (velocity >= 0 && velocity >= max_velocity)
		velocity = max_velocity;
	if (velocity < 0 && velocity <= -max_velocity)
		velocity = -max_velocity;
}

void Throwable::createObject()
{
	b2BodyDef body_def;
	body_def.position = b2Vec2(Converter::pixelsToMeters(x), Converter::pixelsToMeters(y));
	body_def.type = b2_dynamicBody;
	body_def.angularDamping = 1.0f;
	physics = world->CreateBody(&body_def);
	b2CircleShape shape;
	shape.m_radius = Converter::pixelsToMeters(radius);
	b2FixtureDef fixture_def;
	if (throwable_type == ThrowableType::ClusterAddon)
		fixture_def.density = 2.0f;
	else
		fixture_def.density = 1.f;
	fixture_def.friction = 0.9f;
	fixture_def.restitution = 0.4f;
	fixture_def.shape = &shape;
	fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(this);
	physics->CreateFixture(&fixture_def);
}

float Throwable::getTrajectoryX(float thr_shifted_x, float n)
{
	float t = 1 / 60.0f; // Seconds per time step at 60fps
	float linear_velocity_x = Converter::pixelsToMeters(thr_start_x - thr_shifted_x) * 3;
	maximumVelocityLimitation(linear_velocity_x, max_velocity);
	float grav_x = world->GetGravity().x;
	float trajectory_x = Converter::pixelsToMeters(thr_start_x) + n * t * linear_velocity_x
		+ 0.5 * (n * n + n) * grav_x * t * t;
	return Converter::metersToPixels(trajectory_x);
}
float Throwable::getTrajectoryY(float thr_shifted_y, float n)
{
	float t = 1 / 60.0f; // Seconds per time step at 60fps
	float linear_velocity_y = Converter::pixelsToMeters(thr_start_y - thr_shifted_y) * 3;
	maximumVelocityLimitation(linear_velocity_y, max_velocity);
	float grav_y = world->GetGravity().y;
	float trajectory_y = Converter::pixelsToMeters(thr_start_y) + n * t * linear_velocity_y
		+ 0.5 * (n * n + n) * grav_y * t * t;
	return Converter::metersToPixels(trajectory_y);
}

void Throwable::launch(float thr_shifted_x, float thr_shifted_y)
{
	linear_velocity.x = Converter::pixelsToMeters(thr_start_x - thr_shifted_x) * 3;
	linear_velocity.y = Converter::pixelsToMeters(thr_start_y - thr_shifted_y) * 3;
	maximumVelocityLimitation(linear_velocity.x, max_velocity);
	maximumVelocityLimitation(linear_velocity.y, max_velocity);
	createObject();
	physics->SetLinearVelocity(linear_velocity);
}

void Throwable::checkIfMoving()
{
	if (physics != nullptr
		&& is_moving
		&& (abs(physics->GetLinearVelocity().x) + abs(physics->GetLinearVelocity().y)) < 0.2f)
	{
		is_moving = false;
	}
}

ThrowableState Throwable::getThrowableState()
{
	return state;
}
void Throwable::setThrowableState(ThrowableState throwable_state)
{
	this->state = throwable_state;
}

DefaultThrowable::DefaultThrowable(b2World* world, float radius)
{
	object_type = ObjectType::Throwable;
	throwable_type = ThrowableType::Default;
	initializePosition(world, thr_start_x, thr_start_y);
	this->radius = radius / 2;
	texture.loadFromFile("textures/thr_def.png");
	texture.setSmooth(true);
	graphics.setSize(sf::Vector2f(radius, radius));
	graphics.setTexture(&texture);
	graphics.setOrigin(radius / 2, radius / 2);
	graphics.setPosition(x, y);
}

Cluster::Cluster(b2World* world, float radius, bool is_add, float x, float y)
{
	object_type = ObjectType::Throwable;
	throwable_type = ThrowableType::Cluster;
	initializePosition(world, thr_start_x, thr_start_y);
	this->radius = radius / 2;
	texture.loadFromFile("textures/thr_clus.png");
	texture.setSmooth(true);
	graphics.setSize(sf::Vector2f(radius, radius));
	graphics.setTexture(&texture);
	graphics.setOrigin(radius / 2, radius / 2);
	graphics.setPosition(x, y);
	if (is_add)
	{
		throwable_type = ThrowableType::ClusterAddon;
		this->x = x;
		this->y = y;
		createObject();
	}
}

Bombarding::Bombarding(b2World* world, float radius, bool is_add, float x, float y)
{
	object_type = ObjectType::Throwable;
	throwable_type = ThrowableType::Bombarding;
	initializePosition(world, thr_start_x, thr_start_y);
	this->radius = radius / 2;
	texture.loadFromFile("textures/thr_bom.png");
	texture.setSmooth(true);
	graphics.setSize(sf::Vector2f(radius, radius));
	graphics.setTexture(&texture);
	graphics.setOrigin(radius / 2, radius / 2);
	graphics.setPosition(x, y);
	if (is_add)
	{
		this->x = x;
		this->y = y;
		createObject();
	}
}
