#ifndef __CHARS_H__
#define __CHARS_H__

#include "PugiXml/src/pugixml.hpp"
#include "Module.h"
#include "List.h"

struct SDL_Texture;

struct UIChar
{
	SString c;
	SDL_Rect rect;
};


class Chars : public Module
{
public:

	Chars();

	// Destructor
	virtual ~Chars();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

public:
	List<UIChar*> chars;
	SDL_Texture* tex;

	bool DrawText(int posX, int posY, SString text);

private:
	const char* texPath;
};

#endif // __CHARS_H__
