#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Chars.h"
#include "Textures.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

Chars::Chars() : Module()
{
	name.Create("chars");
}

// Destructor
Chars::~Chars()
{}



// Called before render is available
bool Chars::Awake(pugi::xml_node& config)
{
	LOG("Loading Chars");
	bool ret = true;

	texPath = config.attribute("path").as_string();

	for (pugi::xml_node charNode = config.child("char"); charNode && ret; charNode = charNode.next_sibling("char"))
	{
		//Load the layer
		UIChar* uiChar = new UIChar();
		uiChar->c = charNode.attribute("c").as_string();
		uiChar->rect = { charNode.attribute("x").as_int(),charNode.attribute("y").as_int() ,charNode.attribute("w").as_int() ,charNode.attribute("h").as_int() };
		chars.Add(uiChar);
	}
	return ret;
}

// Called before the first frame
bool Chars::Start()
{
	tex = app->tex->Load(texPath);
	return true;
}

bool Chars::DrawText(int x, int y, SString text)
{
	uint w, h;
	app->win->GetWindowSize(w, h);

	bool ret = true;
	for (int i = 0; i < text.Length(); i++)
	{
		ListItem<UIChar*>* item = chars.start;
		UIChar* uiChar = NULL;

		while (item)
		{
			uiChar = item->data;
			if (uiChar->c.GetString()[0] == text.GetString()[i])
			{
				ret = app->render->DrawTexture(tex, (x - app->render->camera.x) / app->win->GetScale() + i * 8, (y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect {8,8,8,8});
				ret = app->render->DrawTexture(tex, (x - app->render->camera.x) / app->win->GetScale() + i * 8,(y - app->render->camera.y) / app->win->GetScale(), & uiChar->rect);
			}
			item = item->next;
		}
		
	}
	return ret;
}


