/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2014 - Dominique Dumont

   Permission is granted to use this software under the terms of either:
   a) the GPL v3 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   T.R.P. is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   -----------------------------------------------------------------------------

   To release a closed-source product which uses T.R.P., commercial licenses are
   available: visit veed.fr for more information.

  ==============================================================================
*/

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__


#include <string>

class Mix_Chunk;
class Mix_Music;

class Sound
{
public:
	Sound();
	~Sound();
	void	Load(const std::string & _file);
	void	Play(int _nbLoops=0);
	void	UnLoad();
	void	Stop();
	void	SetVolume(int _newVolume);

private:

	Mix_Chunk*	sample;
	int		channel;
	int		volume;

};

void ConstructSound(Sound *thisPointer);
void DestructSound(Sound *thisPointer);
void RegisterSound();



class SoundManager
{
public:
	static SoundManager& Get();
	
	~SoundManager();

	void	Init();
	void	Shutdown();
	
	void	SetMusicVolume(int _newVolume);
	void	SetSFXVolume(int _newVolume);

	
public:
	
	int	volumeSFX;
	int	volume;

private:
	SoundManager();
private:
	
	int				rate;
	unsigned short 	format;
	int				channels;
	int				buffers;
	int				initialized;



};

void RegisterSoundManager();
void SND_SetMusicVolume(int _newVolume);
void SND_SetSFXVolume(int _newVolume);

#endif
