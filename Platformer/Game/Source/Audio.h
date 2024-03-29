#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	void IncreaseVolume();

	void DecreaseVolume();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool SetMusicVolume(int volume);
	bool SetFxVolume(int volume);

	int GetMusicVolume();
	int GetFxVolume();

private:

	void LoadAllFx(pugi::xml_node&);

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;

	int musicVolume;
	int fxVolume;
	
};

#endif // __AUDIO_H__