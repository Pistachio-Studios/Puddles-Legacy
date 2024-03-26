#include "Core/ParticleManager.h"
#include "Core/App.h"
#include "Core/DebugUI.h"
#include "Core/Physics.h"
#include "Core/Render.h"

#include "Utils/List.h"
#include "box2d/b2_body.h"
#include <SDL_render.h>
#include <box2d/b2_math.h>
#include <cstdlib>
#include <imgui.h>
#include <tracy/Tracy.hpp>
#include <Utils/Log.h>


#include <Core/Animation.h>//BORRAR
#include <Core/Textures.h>


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

void Particle::Update(float dt)//alomejor seria mejor llamarle draw
{
    // Update the particle's lifetime
    if(lifetimeTimer->ReadMSec() <= lifetime * 1000)
    { 
        // Get the particle's position
        b2Vec2 pos = pbody->body->GetPosition();
        position.x = METERS_TO_PIXELS(pos.x);
        position.y = METERS_TO_PIXELS(pos.y);

        // Calculate the ratio of the elapsed time to the total lifetime
        float ratio = static_cast<float>(lifetimeTimer->ReadMSec()) / (lifetime * 1000);

        // Interpolate the opacity based on the ratio
        if(opacityFade < 0)
        {
            // Fade out from full opacity to none
            color.a = static_cast<Uint8>((1.0f - ratio) * 255);
        }
        else if(opacityFade > 0)
        {
            // Fade in from no opacity to full
            color.a = static_cast<Uint8>(ratio * 255);
        }
        
        int newSize = size;
        // Interpolate the size based on the ratio
        if(sizeFade < 0)
        {
            // Shrink from full size to none
            newSize = static_cast<int>((1.0f - ratio) * size);
        }
        else if(sizeFade > 0)
        {
            // Grow from no size to full
            newSize = static_cast<int>(ratio * size);
        }

        // Draw the particle
        if (anim != nullptr and anim->texture != nullptr)
        {
            if(color.r < 255 and color.g < 255 and color.b < 255)
            SDL_SetTextureColorMod(anim->texture, color.r, color.g, color.b);
            
            if(color.a < 255)
            SDL_SetTextureAlphaMod(anim->texture, color.a);

            app->render->DrawTexture(anim->texture, position.x - anim->GetCurrentFrame().w / 2, position.y - anim->GetCurrentFrame().h / 2, &anim->GetCurrentFrame(), 1.0f, angle);
            size *= lifetimeTimer->ReadMSec();
        }
        else
        {
            app->render->DrawRectangle({ position.x - newSize / 2, position.y - newSize / 2, newSize, newSize }, color.r, color.g, color.b, color.a);
        }
    }
    else
    {
        active = false;
    }
}

ParticleGenerator::ParticleGenerator() 
{
    updateTimer = new Timer();
    app->particleManager->AddGenerator(this);
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

    //float interval = static_cast<float>(updateRate) / amount;
    //while(updateTimer->ReadMSec() >= interval * emitedParticles) {
    //    if (emitedParticles < particles.Count()) {
    //        Particle* particle = particles[emitedParticles];
    //        if(!particle->active){
    //            particle->lifetime = lifetime;
    //            particle->size = size;
    //            particle->position = position;
    //            particle->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
    //            /* particle->pbody->body->SetAwake(false);
    //            particle->pbody->body->SetAwake(true); */

    //            float angle = atan2(direction.y, direction.x);
    //            float randomFactor = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
    //            float angle1_rad = angle + DEGTORAD * (spread * randomFactor);
    //            b2Vec2 direction = b2Vec2(cos(angle1_rad), sin(angle1_rad));
    //            particle->pbody->body->SetLinearVelocity(b2Vec2(direction.x * initialVelocity, direction.y * initialVelocity));
    //            particle->Spawn();
    //        }
    //    }
    //    emitedParticles++;
    //}
    //iterar 
    //la particula no tiene que estar activa y ademas 
    float interval = static_cast<float>(lifetime * 1000) / amount;

    if (emitedParticles >= amount)emitedParticles = 0;

    for(; emitedParticles < amount; emitedParticles++) {
        Particle* particle = particles[emitedParticles];
        if (!particle->active and updateTimer->ReadMSec() >= interval * emitedParticles * (1.0f - explosiveness)) {
            particle->lifetime = lifetime;
            particle->size = size;
            particle->color = color;
            particle->opacityFade = opacityFade;
            particle->sizeFade = sizeFade;
            
            float randomAngle = static_cast<float>(rand()) / RAND_MAX * 360.0f;
            particle->angle = randomAngle * angleRandomness;
            
            if(anim != nullptr)
                particle->anim = anim; //PONER EN UN SITIO QUE SOLO SE HAGA UNA VEZ

            iPoint spawnPosition = { 0,0 };

            spawnPosition = {
                static_cast<int>(position.x + SDL_cos(rand()) * spawnRadius),
                static_cast<int>(position.y + SDL_sin(rand()) * spawnRadius)
            };

            particle->position = spawnPosition;
            particle->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawnPosition.x), PIXEL_TO_METERS(spawnPosition.y)), 0);
            /* particle->pbody->body->SetAwake(false);
            particle->pbody->body->SetAwake(true); */

            float angle = atan2(direction.y, direction.x);
            float randomFactor = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
            float angle1_rad = angle + DEGTORAD * (spread * randomFactor);
            b2Vec2 direction = b2Vec2(cos(angle1_rad), sin(angle1_rad));
            particle->pbody->body->SetLinearVelocity(b2Vec2(direction.x * initialVelocity, direction.y * initialVelocity));
            particle->Spawn();
            emitedParticles++;
        }
    }
}

void ParticleGenerator::PreUpdate()
{
    ZoneScoped;
    int particlesCount = particles.Count();

    if(amount < particlesCount or !emiting)//TODO!! cuando cambias el lifetime con imgui deberia de entrar aqui tambi�n
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

void ParticleGenerator::Update(float dt)
{
    ZoneScoped;
    if (emiting and particles.Count() > 0)
    {
        if (updateTimer->ReadMSec() <= lifetime * 1000)
        {
            EmitParticles();
        }
        else
        {
            updateTimer->Start();
            if (oneShoot) emiting = false;
        }

        if(anim != nullptr)
        {
            anim->Update(dt);
        }

        //creo que aqui deberia de actualizar las propiedades de las particulas y aplicar las fuerzas
        ListItem<Particle*>* item = particles.start;
        while (item != nullptr)
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
            item->data->Update(dt);
            item = item->next;
        }
    }
    else
    {
        updateTimer->Start();//para solucionar los problemas al desactivar y activar emission
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
    /* ParticleGenerator* generator = new ParticleGenerator();
    generator->emiting = true;
    generator->amount = 10;
    generator->position = {600, 300};

    generator->anim = test;
    generators.Add(generator); */

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
        item->data->Update(dt);
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
    if(app->debugUI->particleManagerGenerators)
    {
        if(ImGui::Begin("Particle Manager"))
    {
        for (int i = 0; i < generators.Count(); i++)
        {
            ParticleGenerator* generator = generators[i];
            if (ImGui::CollapsingHeader("Generator %d", i))
            {
                ImGui::Text("Si algo hace como pulsaciones al cambiar\nun valor, activa y desactiva Emiting!!!");

                ImGui::Checkbox("Emitting", &generator->emiting);
                ImGui::Checkbox("One Shoot", &generator->oneShoot);
                ImGui::DragInt("Amount", &generator->amount);
                ImGui::DragFloat("Spawn Radius", &generator->spawnRadius);
                ImGui::DragInt2("position", &generator->position.x, 1);
                ImGui::DragFloat("lifetime", &generator->lifetime, 0.1f);
                ImGui::SliderFloat("Explosiveness", &generator->explosiveness, 0, 1);
                ImGui::Text("Particles: %d", generator->particles.Count());
                ImGui::DragFloat2("Direction", &generator->direction.x, 1);
                ImGui::DragInt("Size", &generator->size, 1, 1);
                ImGui::DragFloat("Initial Velocity", &generator->initialVelocity);
                ImGui::DragFloat("Damping", &generator->Damping);
                ImGui::SliderFloat("Spread", &generator->spread, 0, 180);
                ImGui::DragFloat("Angle Randomness", &generator->angleRandomness, .1, -3, 3);
                ImGui::DragFloat("Opacity Fade", &generator->opacityFade, 1, -1, 1);
                ImGui::DragFloat("Size Fade", &generator->sizeFade, 1, -1, 1);

                ImVec4 imguiColor;
                imguiColor.x = generator->color.r / 255.0f;
                imguiColor.y = generator->color.g / 255.0f;
                imguiColor.z = generator->color.b / 255.0f;
                imguiColor.w = generator->color.a / 255.0f;

                ImGui::ColorEdit4("Color", (float*)&imguiColor);

                generator->color.r = static_cast<Uint8>(imguiColor.x * 255);
                generator->color.g = static_cast<Uint8>(imguiColor.y * 255);
                generator->color.b = static_cast<Uint8>(imguiColor.z * 255);
                generator->color.a = static_cast<Uint8>(imguiColor.w * 255);
                
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
    
}

void ParticleManager::AddGenerator(ParticleGenerator* generator)
{
    generators.Add(generator);
}