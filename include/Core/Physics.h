#pragma once
#include "Utils/DynArray.h"
#include "Core/Module.h"
#include "Gameplay/Entities/Entity.h"

//#include <box2d/box2d.h>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_math.h>
class b2Body;
class b2Contact;
class b2World;
class b2RevoluteJoint;
class b2PrismaticJoint;
class b2WeldJoint;
class b2MouseJoint;

#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
	DYNAMIC,
	STATIC,
	KINEMATIC
};

enum class ColliderType {
	PLAYER,
	SWORD,
	PLANT,
	POTION,
	SCOREITEM,
	CHECKPOINT,
	ENEMYWASP,
	ENEMYSPIDER,
	ENEMYBOSS,
	BULLET,
	MAGIC,
	OBSTACLE,
	CHANGESCENE,
	DEATH,
	LIMITS,
	WIN,
	NPC,
	UNKNOWN
	// ..
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL), ctype(ColliderType::UNKNOWN)
	{}

	~PhysBody() {}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Entity* listener;
	ColliderType ctype;
};

class RaycastCallback : public b2RayCastCallback {
public:
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                       const b2Vec2& normal, float32 fraction) override;

	Entity* listener;
};

struct Raycast {
    b2RayCastCallback* callback;
	b2Vec2 rayStart;
	b2Vec2 rayEnd;
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener // TODO
{
public:

	// Constructors & Destructors
	Physics();

	Physics(bool startEnabled);

	~Physics();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	// Create basic physics objects
	PhysBody* CreateRectangle(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateCircle(int x, int y, int radious, bodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, bodyType type);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float lowerAngle, float upperAngle, float referenceAngle, bool collideConnected, bool enableLimit);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, b2Vec2 axys, float maxHeight, bool collideConnected, bool enableLimit);
	b2WeldJoint* CreateWeldJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle, bool collideConnected, bool enableLimit);
	PhysBody* CreateRope(int x, int y, int length, float segmentWidth = 0.1f, float segmentHeight = 0.25f);
	PhysBody* CreateRope(b2Vec2 startPos, int length, float segmentWidth = 0.1f, float segmentHeight = 0.25f);
	PhysBody* CreateRope(b2Vec2 startPos, b2Vec2 endPos, int length, float segmentWidth = 0.1f);

	PhysBody* CreateParticle(int x, int y, int width, int height);

	Raycast* CreateRaycast(Entity* listener, b2Vec2 rayStart, b2Vec2 rayEnd);

	// Destroy physics objects
	bool DestroyBody(PhysBody* body);
	bool DestroyAllWorldBodies();
	
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	float lookAt(b2Vec2 source, b2Vec2 target);

private:

	// Box2D World
	b2World* world;
	b2Body* ground;

	// Mouse joint
	b2MouseJoint* mouseJoint;
	b2Body* mouseBody;

	//Raycast
	DynArray<Raycast*> raycasts;

	DynArray<PhysBody*> bodiesToBeDeleted;
};