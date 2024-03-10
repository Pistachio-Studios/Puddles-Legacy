#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "Core/Module.h"
#include "Utils/DynArray.h"
#include "Utils/List.h"
#include "Utils/Point.h"
#include "Utils/Timer.h"

class PhysBody;
class Animation;

class Particle
{
public:
    Particle();
    ~Particle();
	void Update();
public:
    int lifetime = 5;
    bool active = true;
    iPoint spawnPosition;
private:
	PhysBody* pbody;
    iPoint position;
    Timer* timer;
    int size;
};

class ParticleGenerator
{
public:
    ParticleGenerator();
    void Update();
    void EmitParticles();
public:
    bool emiting;
    int amount;
    iPoint position;
    private:
    List<Particle*> particles;
    int spawnRate;
};

class ParticleManager : public Module
{
public:

    ParticleManager();

    ParticleManager(bool startEnabled);

    // Destructor
    virtual ~ParticleManager();

    // Called before render is available
    bool Awake(pugi::xml_node& conf) override;

    // Called after Awake
    bool Start() override;

    bool PreUpdate() override;

    // Called every frame
    bool Update(float dt) override;

    // Called before quitting
    bool CleanUp() override;

    void DrawImGui() override;

private:
    List<ParticleGenerator*> generators;
};

#endif // __PARTICLEMANAGER_H__