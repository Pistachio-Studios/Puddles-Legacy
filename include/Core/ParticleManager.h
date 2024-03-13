#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "Core/Module.h"
#include "Utils/DynArray.h"
#include "Utils/List.h"
#include "Utils/Point.h"
#include "Utils/Timer.h"

#include <SDL_pixels.h>
#include <box2d/b2_math.h>

class PhysBody;
class Animation;

class Particle
{
public:
    Particle();
    ~Particle();
    void Spawn();
	void Update();
public:
    bool active = false;

    // Particle's properties
    float lifetime = 1.0f;
    int size = 5;
    float angle;
    SDL_Color color = {255,255,255,255};

    PhysBody* pbody;

    iPoint position;

    Animation* anim = nullptr;
private:
    
    Timer* lifetimeTimer;
};

class ParticleGenerator
{
public:
    ParticleGenerator();
    void PreUpdate();
    void Update();
    void EmitParticles();
    void ResetParticles();
public:
    bool emiting = true;
    int amount = 8;
    iPoint position = {0,0};

    // Particle's properties
    float explosiveness =0.0f;
    float lifetime = 1.0f;
    int size = 5;
    b2Vec2 direction = {1,0};
    float spread = 45.0f;
    float Damping = 0.0f;

    SDL_Color color = {255,255,255,255};

    float angleRandomness = 0.0f;

    float initialVelocity = 1.0f;

    bool oneShoot = false;

    float spawnRadius = 1.0f;

    Animation* anim = nullptr;

    List<Particle*> particles; //poner en privado en algun momento, esta para mostrar el num de part. en imgui
    private:
    
    Timer* updateTimer;
    int updateRate = 1000;//revisar este numero (antes estaba a 1000)

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