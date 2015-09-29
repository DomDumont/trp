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

#include "Global.h"
#include "Particles.h"

#include "Application_p.h"
#include "Utils.h"

#include "pugixml.hpp"



#include "Atlas.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Particle::Particle( AtlasEntry	* _entry)
{
    this->entry = _entry;
    SDL_Color tempColor = {255,255,255,255 };
    this->color = tempColor;
    this->angle = 0;
    this->velocityX = 0;
    this->velocityY = 0;
    this->angularVelocity = 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Particle::~Particle()
{

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Particle::Update(Uint64 _elapsed)
{
    TTL--;
    this->position.x += (int) this->velocityX;
    this->position.y += (int) this->velocityY;
    this->angle += this->angularVelocity;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Particle::Render()
{
    if ((this->entry) && (this->entry->atlas->texture))
        {
        SDL_SetTextureColorMod(this->entry->atlas->texture,this->color.r,this->color.g,this->color.b);
        SDL_SetTextureAlphaMod(this->entry->atlas->texture,this->color.a);
        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &this->frame, &this->position,this->angle,NULL,SDL_FLIP_NONE);
        }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Emitter *Emitter_Factory()
{
    // The class constructor is initializing the reference counter to 1
    return new Emitter();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterEmitter()
{
    int r;
    ///class:Emitter
    ///func:Emitter()
    r = g_app->scriptManager->engine->RegisterObjectType("Emitter", 0, asOBJ_REF); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Emitter", asBEHAVE_FACTORY, "Emitter@ f()", asFUNCTION(Emitter_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Emitter", asBEHAVE_ADDREF, "void f()", asMETHOD(Emitter,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Emitter", asBEHAVE_RELEASE, "void f()", asMETHOD(Emitter,Release), asCALL_THISCALL); SDL_assert( r >= 0 );

    ///func:void Load(Atlas @ atlas,string &in file,int flags=13)
    g_app->scriptManager->RegisterClassMethod("Emitter","void Load(Atlas @ _atlas,string &in _file,int _flags=13)", asMETHOD(Emitter, Load));
    ///func:void SetPosition(int x,int y,int from=0)
    g_app->scriptManager->RegisterClassMethod("Emitter","void SetPosition(int _x,int _y,int _from=0)", asMETHOD(Emitter, SetPosition));
    ///func:void Update(uint64 elapsed)
    g_app->scriptManager->RegisterClassMethod("Emitter","void Update(uint64 _elapsed)", asMETHOD(Emitter, Update));
    ///func:void Render()
    g_app->scriptManager->RegisterClassMethod("Emitter","void Render()", asMETHOD(Emitter, Render));

}
#endif


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Emitter::Emitter():refCount(1),atlas(NULL),entry(NULL)
{


    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Emitter Constructor \n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


Emitter::~Emitter()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Emitter Destructor\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Emitter::Emitter(const Emitter &other) 
{
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Emitter::Update(Uint64 _elapsed)
{
    for (int i = 0; i < this->emissionRate; i++)
        {
        //particles.Add(GenerateNewParticle());
        Particle *tempParticle = new Particle(this->entry);        
        //Generation is not called by script so we must add the reference manually
        this->atlas->AddRef();
        tempParticle->position = this->position;
        tempParticle->frame = this->frame;
        tempParticle->TTL = this->TTL;
        tempParticle->velocityX = UTI_Rand(1,10);
        tempParticle->velocityY = UTI_Rand(1,10);
        tempParticle->color.r = UTI_Rand(0,255);
        tempParticle->color.g = UTI_Rand(0,255);
        tempParticle->color.b = UTI_Rand(0,255);
        tempParticle->angularVelocity = UTI_Rand(1,2);
        particles.push_back(tempParticle);
        }

    for (particlesIT = particles.begin();  particlesIT != particles.end();/* ++particlesIT */)
        {
        Particle *pTemp;
        pTemp = *particlesIT;
        pTemp->Update(_elapsed);     

        if (pTemp->TTL <= 0)
            {
            delete pTemp;
            particlesIT = this->particles.erase(particlesIT);
            }
        else
            {
            ++particlesIT;
            }
        }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Emitter::Render()
{

    for (particlesIT = particles.begin();  particlesIT != particles.end();++particlesIT)
    {
        Particle *pTemp;
        pTemp = *particlesIT;
        pTemp->Render();     
    }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Emitter::SetPosition(int _x,int _y,int _from)
{
    switch(_from)
    {
    case 0: //Center
        this->position.x = _x - this->position.w/2;
        this->position.y = _y - this->position.h/2;
        break;

    case 1: //top left
        this->position.x = _x;
        this->position.y = _y;
        break;

    }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Emitter::Load(Atlas * _atlas,const std::string& _file,int _flags)
{

    this->atlas = _atlas;

    std::string fullPath;
    std::string loadedString;


    fullPath = _file + ".xml";

    loadedString = LoadTextFile(fullPath,_flags);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(loadedString.c_str());
    
    pugi::xml_node root = doc.first_child();
    for (pugi::xml_node elem = root.first_child(); elem != NULL; elem = elem.next_sibling())
    {
        if (strcmp(elem.name(),"emission-rate")==0)
          this->emissionRate = (float)atoi(elem.text().as_string());
        if (strcmp(elem.name(), "ttl") == 0)
          this->TTL = atoi(elem.text().as_string());

        if (strcmp(elem.name(), "texture") == 0)
            {
            this->textureName = elem.text().as_string();
            this->entry = _atlas->FindEntry(this->textureName);
            SDL_assert(this->entry != NULL);
            this->position = this->frame = this->entry->frame;
            this->position.x = 0;
            this->position.y = 0;
            }
        
    }

    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Emitter <%s> loaded successfully \n",_file.c_str());
}

