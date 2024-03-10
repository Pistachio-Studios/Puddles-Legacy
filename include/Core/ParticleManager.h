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
    void Spawn(iPoint position, int size);
	void Update();
public:
    float lifetime = 5;
    bool active = true;
    iPoint spawnPosition;
    int size;
    bool markedForDeletion = false;
    PhysBody* pbody;
private:
    iPoint position;
    Timer* timer;
};

class ParticleGenerator
{
public:
    ParticleGenerator();
    void PreUpdate();
    void Update();
    void EmitParticles();
public:
    bool emiting;
    int amount;
    iPoint position;
    private:
    List<Particle*> particles;
    Timer* updateTimer;
    int updateRate = 1000;
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

    // Called every frame
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