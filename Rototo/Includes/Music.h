/*
==============================================================================

This file is part of the T.R.P. Engine
Copyright (c) 2015 - Dominique Dumont

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
