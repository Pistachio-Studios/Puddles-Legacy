#include "Core/ParticleManager.h"
#include "Core/App.h"
#include "Core/Physics.h"
#include "Core/Render.h"

#include "Utils/List.h"
#include "box2d/b2_body.h"
#include <box2d/b2_math.h>
#include <cstdlib>
#include <imgui.h>
#include <tracy/Tracy.hpp>


Particle::Particle()
{
    pbody = app->physics->CreateParticle(position.x, position.y, size, size);

    lifetimeTimer = new Timer();
    
}

Particle::~Particle()
{
    app->physics->DestroyBody(pbody);
}

void Particle::Spawn()
{
    active = true;
    lifetimeTimer->Start();
}

void Particle::Update()//alomejor seria mejor llamarle draw
{
    // Update the particle's lifetime
    if(lifetimeTimer->ReadMSec() <= lifetime * 1000)
    { 
        // Get the particle's position
        b2Vec2 pos = pbody->body->GetPosition();
        position.x = METERS_TO_PIXELS(pos.x);
        position.y = METERS_TO_PIXELS(pos.y);

        // Draw the particle
        app->render->DrawRectangle({position.x - size / 2, position.y - size / 2, size, size}, 255, 255, 255);
    }
    else
    {
        active = false;
    }
}

ParticleGenerator::ParticleGenerator() 
{
    updateTimer = new Timer();
}

void ParticleGenerator::EmitParticles()
{
    ZoneScoped;
    /* if (emitedParticles <= amount)
    {
        float interval = static_cast<float>(updateRate) / amount;

        while(updateTimer->ReadMSec() >= interval * emitedParticles) {
            if (emitedParticles < particles.Count()) {
                Particle* particle = particles[emitedParticles];
                particle->lifetime = lifetime;
                particle->initialVelocity = initialVelocity;
                particle->Spawn(position, direction, size, lifetime);
            }
            emitedParticles++;
        }
    } */

/*     for(int i = 0; i < amount - 1; i++)
    {
        Particle* particle = particles[i];
        if(!particle->active)
        {
            particle->lifetime = lifetime;
            particle->size = size;
            particle->position = position;
            particle->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
            particle->pbody->body->SetLinearVelocity(b2Vec2(direction.x * initialVelocity, direction.y * initialVelocity));
            particle->Spawn();
        }
    } */

    float interval = static_cast<float>(updateRate) / amount;
    while(updateTimer->ReadMSec() >= interval * emitedParticles * (1.1f - explosiveness)) {
        if (emitedParticles < particles.Count()) {
            Particle* particle = particles[emitedParticles];
            if(!particle->active){
                particle->lifetime = lifetime;
                particle->size = size;
                particle->position = position;
                particle->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
                /* particle->pbody->body->SetAwake(false);
                particle->pbody->body->SetAwake(true); */

                float angle = atan2(direction.y, direction.x);
                float randomFactor = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
                float angle1_rad = angle + DEGTORAD * (spread * randomFactor);
                b2Vec2 direction = b2Vec2(cos(angle1_rad), sin(angle1_rad));
                particle->pbody->body->SetLinearVelocity(b2Vec2(direction.x * initialVelocity, direction.y * initialVelocity));
                particle->Spawn();
            }
        }
        emitedParticles++;
    }
}

void ParticleGenerator::PreUpdate()
{
    ZoneScoped;
    int particlesCount = particles.Count();

    if(amount < particlesCount)
    {
        for(int i = 0; i < particlesCount; i++)
        {
            delete particles.end->data;
            particles.Del(particles.end);
        }
        ResetParticles();
    }
    else if(amount > particlesCount)
    {
        for(int i = 0; i < amount - particlesCount; i++)
        {
            particles.Add(new Particle());
        }
        ResetParticles();
    }
}

void ParticleGenerator::Update()
{
    ZoneScoped;
    if(emiting)
    {
        if(updateTimer->ReadMSec() <= updateRate)
        {
            EmitParticles();
        }
        else
        {
            emitedParticles = 0;
            updateTimer->Start();
        }
    }
    
    //creo que aqui deberia de actualizar las propiedades de las particulas y aplicar las fuerzas
    ListItem<Particle*>* item = particles.start;
    while(item != nullptr)
    {
        PhysBody* pbody = item->data->pbody;

        //Update Particles

        //Apply impulse to the particle
        /* direction.Normalize();
        float mass = pbody->body->GetMass();
        float impulseMagnitude = mass * initialVelocity;
        b2Vec2 impulseForce = impulseMagnitude * direction;
        pbody->body->ApplyForceToCenter(impulseForce, false); */

        //Apply damping to the particle
        pbody->body->SetLinearDamping(Damping);

        //if(item->data->active)item->data->Update();
        item->data->Update();
        item = item->next;
    }
}

void ParticleGenerator::ResetParticles()
{
    for(int i = 0; i < particles.Count(); i++)
    {
        particles[i]->lifetime = lifetime;
        particles[i]->size = size;
        particles[i]->position = position;
        particles[i]->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
        particles[i]->pbody->body->SetAwake(false);
        particles[i]->pbody->body->SetAwake(true);
        //particles[i]->pbody->body->SetLinearVelocity(b2Vec2(direction.x * initialVelocity, direction.y * initialVelocity));
    }
}

ParticleManager::ParticleManager() : Module()
{
    name.Create("particleManager");
}

ParticleManager::ParticleManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("particleManager");
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::Start()
{
    //BORRAR!!!
    ParticleGenerator* generator = new ParticleGenerator();
    generator->emiting = true;
    generator->amount = 10;
    generator->position = {600, 300};
    generators.Add(generator);

    return true;
}

bool ParticleManager::PreUpdate()
{
    ListItem<ParticleGenerator*>* item = generators.start;
    while(item != nullptr)
    {
        item->data->PreUpdate();
        item = item->next;
    }
    return true;
}

bool ParticleManager::Update(float dt)
{
    ListItem<ParticleGenerator*>* item = generators.start;
    while(item != nullptr)
    {
        item->data->Update();
        item = item->next;
    }
    return true;
}

bool ParticleManager::CleanUp()
{
    //TODO: Clean up the generators and particles
    return true;
}

void ParticleManager::DrawImGui()
{
    if(ImGui::Begin("Particle Manager"))
    {
        for (int i = 0; i < generators.Count(); i++)
        {
            ParticleGenerator* generator = generators[i];
            if (ImGui::CollapsingHeader("Generator %d", i))
            {
                ImGui::Checkbox("Emitting", &generator->emiting);
                ImGui::DragInt("Amount", &generator->amount);
                ImGui::DragInt2("position", &generator->position.x, 1);
                ImGui::SliderFloat("Explosiveness", &generator->explosiveness, 0, 1);
                ImGui::DragFloat("lifetime", &generator->lifetime, 0.1f);
                ImGui::Text("Particles: %d", generator->particles.Count());
                ImGui::DragFloat2("Direction", &generator->direction.x, 1);
                ImGui::DragInt("Size", &generator->size, 1, 1);
                ImGui::DragFloat("initialVelocity", &generator->initialVelocity);
                ImGui::DragFloat("Damping", &generator->Damping);
                ImGui::SliderFloat("Spread", &generator->spread, 0, 180);

                ImGui::SetNextWindowBgAlpha(0.0f); // Set window background alpha to 0 for transparency
                ImGui::Begin("Spawner Position", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove); // Set ImGuiWindowFlags to remove background, title bar, resize, and move functionality
                ImGui::SetWindowPos(ImVec2(generator->position.x, generator->position.y)); // Set window position to top-left corner
                ImGui::Bullet();
                ImGui::End();
            }
        }
    }
    ImGui::End();
    
}

void ParticleManager::AddGenerator(ParticleGenerator* generator)
{
    generators.Add(generator);
}