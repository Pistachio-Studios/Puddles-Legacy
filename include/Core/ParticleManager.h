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
    bool active = false;
    float lifetime = 1.0f;
    iPoint spawnPosition = {0,0};
    int size = 5;
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

    float explosiveness = 1.0f;
    private:
    List<Particle*> particles;
    Timer* updateTimer;
    int updateRate = 1000;

    int emitedParticles = 0;
};

class ParticleManager : public Module
{
public:

    ParticleManager();

    ParticleManager(bool startEnabled);

    // Destructor
    virtual ~ParticleManager();

    // Called after Awake
    bool Start() override;

    // Called every frame
    bool PreUpdate() override;

    // Called every frame
    bool Update(float dt) override;

    // Called before quitting
    bool CleanUp() override;

    void DrawImGui() override;

    void AddGenerator(ParticleGenerator* generator);

    ParticleGenerator* CreateGenerator();//No se si esta o la de AddGenerator
private:
    List<ParticleGenerator*> generators;
};

#endif // __PARTICLEMANAGER_H__