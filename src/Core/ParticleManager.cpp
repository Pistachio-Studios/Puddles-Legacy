#include "Core/ParticleManager.h"
#include "Core/App.h"
#include "Core/Physics.h"
#include "Core/Render.h"

#include "Utils/List.h"
#include "box2d/b2_body.h"
#include <imgui.h>

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
    this->spawnPosition = position;
    this->size = size;

    // Set the particle's lifetime
    lifetime = 2.0f;

    // Set the particle's active state
    active = true;

    pbody = app->physics->CreateRectangleSensor(spawnPosition.x, spawnPosition.y, size, size, bodyType::DYNAMIC);

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
            
            // Get the particle's position
            b2Vec2 pos = pbody->body->GetPosition();
            position.x = METERS_TO_PIXELS(pos.x);
            position.y = METERS_TO_PIXELS(pos.y);

            // Draw the particle
            app->render->DrawRectangle({position.x, position.y, size, size}, 255, 255, 255);
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
       /* for (int i = 0; i < amount; i++)
       {
              // Create a new particle and add it to the particles array
              Particle* particle = new Particle();
              particle->Spawn(position, 10);
              particles.Add(particle);
       } */

        if (emitedParticles < amount)
        {
            float interval = static_cast<float>(updateRate) / amount;
            if (updateTimer->ReadMSec() >= interval * emitedParticles * explosiveness)
            {
                // Create a new particle and add it to the particles array
                Particle* particle = new Particle();
                particle->Spawn(position, 10);
                particles.Add(particle);
                emitedParticles++;
            }
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
    if(updateTimer->ReadMSec() <= updateRate)
    {
        EmitParticles();
    }
    else
    {
        emitedParticles = 0;
        updateTimer->Start();
    }

    ListItem<Particle*>* item = particles.start;
    while(item != nullptr)
    {
        item->data->Update();
        item = item->next;
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
    generator->amount = 1;
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
            }
        }
    }
    ImGui::End();

}

void ParticleManager::AddGenerator(ParticleGenerator* generator)
{
    generators.Add(generator);
}