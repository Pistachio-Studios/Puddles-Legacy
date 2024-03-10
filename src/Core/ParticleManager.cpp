#include "Core/ParticleManager.h"
#include "Core/App.h"
#include "Core/Physics.h"
#include "Core/Render.h"

#include "Utils/List.h"
#include "box2d/b2_body.h"

Particle::Particle() 
{
}

Particle::~Particle()
{
    // Release the particle's body
}

void Particle::Spawn(iPoint position, int size)
{
    // Set the particle's position
    this->position = position;
    this->size = size;

    // Set the particle's lifetime
    lifetime = 0.5f;

    // Set the particle's active state
    active = true;

    pbody = app->physics->CreateRectangleSensor(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.x), size, size, bodyType::DYNAMIC);

    // Start the particle's timer
    timer = new Timer();
}

void Particle::Update()
{
    if(active)
    {
        // Update the particle's lifetime
        if(timer->ReadMSec() <= lifetime * 1000)
        {
            pbody->body->ApplyForceToCenter({0, 9.8}, true);
            
            // Update the particle's position
            pbody->GetPosition(position.x, position.y);

            // Draw the particle
            app->render->DrawRectangle({position.x, position.y}, 255, 255, 255);
        }
        else
        {
            active = false;
            markedForDeletion = true;
        }
    }
}


ParticleGenerator::ParticleGenerator() 
{
    updateTimer = new Timer();
}

void ParticleGenerator::EmitParticles()
{
   if(emiting)
   {
       for (int i = 0; i < amount; i++)
       {
              // Create a new particle and add it to the particles array
              Particle* particle = new Particle();
              particle->Spawn(position, 10);
              particles.Add(particle);
       }
   }
}

void ParticleGenerator::PreUpdate()
{
    ListItem<Particle*>* item = particles.start;
    while(item != nullptr)
    {
        ListItem<Particle*>* nextItem = item->next; // Save the next item

        if(item->data->markedForDeletion)
        {
            app->physics->DestroyBody(item->data->pbody);
            particles.Del(item);
        }
        item = nextItem; // Go to the next item
    }
}

void ParticleGenerator::Update()
{
    if(updateTimer->ReadMSec() >= updateRate)
    {
        EmitParticles();
        ListItem<Particle*>* item = particles.start;
        while(item != nullptr)
        {
            item->data->Update();
            item = item->next;
        }
        updateTimer->Start();
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

bool ParticleManager::Awake(pugi::xml_node& conf)
{
    return true;
}

bool ParticleManager::Start()
{
    ParticleGenerator* generator = new ParticleGenerator();
    generator->emiting = true;
    generator->amount = 1;
    generator->position = {600, 1500};
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
    return true;
}

void ParticleManager::DrawImGui()
{
}