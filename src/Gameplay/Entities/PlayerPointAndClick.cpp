#include "Gameplay/Entities/PlayerPointAndClick.h"
#include "Core/Physics.h"
#include "Core/Render.h"
#include "Core/Input.h"
#include "Core/App.h"
#include "Core/ParticleManager.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_mouse_joint.h>
#include <box2d/b2_world.h>

#include <imgui.h>

PlayerPointAndClick::PlayerPointAndClick() : Entity(EntityType::PLAYER)
{
	name.Create("PlayerPointAndClick");
}

PlayerPointAndClick::~PlayerPointAndClick() {

}

bool PlayerPointAndClick::Awake()
{
    return true;
}

bool PlayerPointAndClick::Start()
{
    position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
    pbody = app->physics->CreateRectangle(position.x, position.y, 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
    //pbody->body->GetFixtureList()->SetSensor(true);

    ground = app->physics->CreateRectangle(0, 0, 1, 1, bodyType::STATIC);

    // Define the mouse joint
    b2MouseJointDef jointDef;
    jointDef.bodyA = ground->body; // A static body
    jointDef.bodyB = pbody->body; // The body you want to move
    jointDef.target = pbody->body->GetPosition(); // Where you want to move it
    jointDef.maxForce = 1000.0f * pbody->body->GetMass(); // The maximum force
    jointDef.stiffness = 9999999999999.0f; // Optional: the response speed
    jointDef.damping = 0.0f; // Optional: how fast it will slow down

    // Create the mouse joint
    mouseJoint = (b2MouseJoint*)pbody->body->GetWorld()->CreateJoint(&jointDef);

    effects = new ParticleGenerator();
    effects->emiting = true;
    app->particleManager->AddGenerator(effects);

    mouseNoSelection = app->tex->Load("Assets/Textures/cursors/kenney_cursor-pack/Vector/Basic/hand_open.svg");
    mouseSelection = app->tex->Load("Assets/Textures/cursors/kenney_cursor-pack/Vector/Basic/hand_point.svg");

    return true;
}

bool PlayerPointAndClick::Update(float dt)
{
    SDL_ShowCursor(SDL_DISABLE);

    b2Vec2 mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };
    //pbody->body->SetTransform(mouseWorldPosition, 0.0f);

    // In your update function
    mouseJoint->SetTarget(mouseWorldPosition);


    effects->position = { position.x, position.y };

   if(hovering)
        app->render->DrawTextureLegacy(mouseSelection, app->input->GetMouseX(), app->input->GetMouseY(), NULL, 0.0f);
    else
        app->render->DrawTextureLegacy(mouseNoSelection, app->input->GetMouseX(), app->input->GetMouseY(), NULL, 0.0f);

    int leftCorner = ((app->render->camera.w / 2) + margin);
    int rightCorner = ((app->render->camera.w / 2) - margin);
    int topCorner = ((app->render->camera.h / 2) + margin);
    int bottomCorner = ((app->render->camera.h / 2) - margin);

    if(debug){   
        app->render->DrawLine(leftCorner, topCorner, rightCorner, topCorner, 255, 0, 0, 255, false);
        app->render->DrawLine(leftCorner, bottomCorner, rightCorner, bottomCorner, 255, 0, 0, 255, false);
        app->render->DrawLine(leftCorner, topCorner, leftCorner, bottomCorner, 255, 0, 0, 255, false);
        app->render->DrawLine(rightCorner, topCorner, rightCorner, bottomCorner, 255, 0, 0, 255, false);
    }else
    {
        if( app->input->GetMouseX() > leftCorner or
        app->input->GetMouseX() < rightCorner or
        app->input->GetMouseY() > topCorner or
        app->input->GetMouseY() < bottomCorner)
    {
        position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
    }
    }

    return true;
}

bool PlayerPointAndClick::CleanUp()
{
    return true;
}

void PlayerPointAndClick::DrawImGui()
{
    ImGui::Begin("PlayerPointAndClick");
    ImGui::Text("Position: %f, %f", position.x, position.y);
    ImGui::DragInt("Margin", &margin, 1);
    ImGui::End();
}

void PlayerPointAndClick::OnCollision(PhysBody* physA, PhysBody* physB)
{
    /* switch (physB->ctype)
    {
     case ColliderType::CAULDRON:
         cauldronIsOpen = true;
         break;
    } */

    if(physB)
    {
        LOG("Hovering true");
        hovering = true;

        if (physB->ctype == ColliderType::CAULDRON) {
            LOG("Cauldron open true");
            cauldronIsOpen = true; 
        }
    }
}

void PlayerPointAndClick::EndCollision(PhysBody* physA, PhysBody* physB)
{
    if(physB)
    {
        LOG("Hovering false");
        hovering = false;
    }
}

void PlayerPointAndClick::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
}

bool PlayerPointAndClick::SaveState(pugi::xml_node& node)
{
    return true;
}

bool PlayerPointAndClick::LoadState(pugi::xml_node& node)
{
    return true;
}