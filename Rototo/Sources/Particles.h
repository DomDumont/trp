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

#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include <vector>

class Atlas;
class AtlasEntry;

//-------------------------------------------------------------
// Class Particle
//-------------------------------------------------------------

class Particle
{
public:
    double		angle;
    int         TTL;
    SDL_Color		color;
    SDL_Rect	frame;
    SDL_Rect	position;
    double      velocityX, velocityY;
    double      angularVelocity;
    AtlasEntry	*entry;

public:
    Particle( AtlasEntry	* _entry);
    ~Particle();

    void Update(Uint64 _elapsed);
    void Render();
};


//-------------------------------------------------------------
// Class Emitter
//-------------------------------------------------------------
class Emitter
{
public:

    Emitter();
    ~Emitter();
    Emitter(const Emitter &other);

    void AddRef()
    {
        // Increase the reference counter
        refCount++;
        //SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Emitter 0x%x AddRef : nb active Emitter = %d\n",(unsigned int)this,refCount);
    }

    void Release()
    {
        // Decrease ref count and delete if it reaches 0
        refCount--;
        if ( refCount == 0 )
            delete this;
        else
            if ( refCount > 0 )
            {
             //   SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Emitter 0x%x Release : nb active Emitter = %d\n",(unsigned int)this,refCount);
            }
            else
                SDL_assert(0);

    }

    void Load(Atlas * _atlas,const std::string& _file,int _flags = 13 /*GAMEDATA|BOTH*/);

    void SetPosition(int _x,int _y,int _from = 0);
    void Update(Uint64 _elapsed);
    void Render();

    std::vector<Particle *>  particles;
    std::vector<Particle *>::iterator particlesIT;

private:
    int         refCount;
    Atlas       *atlas;
    AtlasEntry	*entry;
    float       emissionRate;
    std::string textureName;
    SDL_Rect	frame;
    SDL_Rect	position;
    int         TTL;
};

Emitter *Emitter_Factory();
void RegisterEmitter();

#endif
