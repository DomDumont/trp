#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <string>

class Mix_Music;

class Music
{
public:

	Music();
	~Music();

	void	Load(const std::string & _file);
	void	Play(int _nbLoops = -1, int _timeFadeIn = 1000);
	void	UnLoad();
	void	Stop();

private:

	Mix_Music *music;
};

void ConstructMusic(Music *thisPointer);
void DestructMusic(Music *thisPointer);
void RegisterMusic();


#endif
