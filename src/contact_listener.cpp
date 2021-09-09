#include "contact_listener.h"

void ContactListener::BeginContact(b2Contact* contact)
{
    object_a = reinterpret_cast<SimulatedObject*>(contact->GetFixtureA()->GetUserData().pointer);
    object_b = reinterpret_cast<SimulatedObject*>(contact->GetFixtureB()->GetUserData().pointer);
    if (object_a != nullptr && object_b != nullptr &&
        object_a->physics != nullptr && object_b != nullptr)
    {
        calculateContactDamage(object_a, object_b);
        applyDamage(object_a);
        applyDamage(object_b);
        if (object_a->object_type == ObjectType::Throwable && !cluster_contact)
        {
            Throwable* thr = reinterpret_cast<Throwable*>(object_a);
            cluster_contact = checkIfClusterContact(thr);
        }
        if (object_b->object_type == ObjectType::Throwable && !cluster_contact)
        {
            Throwable* thr = reinterpret_cast<Throwable*>(object_b);
            cluster_contact = checkIfClusterContact(thr);
        }
    }
}

void ContactListener::calculateContactDamage(SimulatedObject* object_a, SimulatedObject* object_b)
{
    float damage_a = std::abs(object_a->physics->GetLinearVelocity().x)
        + std::abs(object_a->physics->GetLinearVelocity().y);
    float damage_b = std::abs(object_b->physics->GetLinearVelocity().x)
        + std::abs(object_b->physics->GetLinearVelocity().y);
    if (object_a->object_type == ObjectType::Throwable)
        damage_a = damage_a * object_a->physics->GetMass();
    if (object_b->object_type == ObjectType::Throwable)
        damage_b = damage_b * object_b->physics->GetMass();
    damage = damage_a + damage_b;
}

void ContactListener::applyDamage(SimulatedObject* object)
{
    if (object->object_type == ObjectType::Obstacle || object->object_type == ObjectType::Enemy)
        object->hp = object->hp - damage;
}

bool ContactListener::checkIfClusterContact(Throwable* object)
{
    if (object->throwable_type == ThrowableType::Cluster)
        return true;
    else
        return false;
}