#include "Menu.h"
#include "Player.h"
#include "App.h"
#include "Scene.h"




Menu::Menu(bool startEnabled) : Module(startEnabled)
{
	pbody.PushBack({ 618,175,22,17 });


}

Menu::~Menu()
{

}

// Load assets
bool Menu::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Textures/menu.png");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	int p1x = 10;
	int p2x = 10;
	int y = 50;

	//P1.selector = App->particles->P1ind;
	//App->particles->AddParticle(P1.selector, p1x, y, 0, P1.speed);

	P1.Locked = false;

	return ret;
}

Update_Status Menu::Update()
{
}

// Update: draw background
Update_Status Menu::PostUpdate()
{
	SDL_Rect rect = pbody.GetCurrentFrame();
	// Draw everything
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(pbody, pbody.x1, pbody.y1, &rect);


	return Update_Status::UPDATE_CONTINUE;
}

bool Menu::CleanUp()
{
	App->particles->CleanUp();
	return true;
}