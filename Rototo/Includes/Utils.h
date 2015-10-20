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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

void PadTo(std::string &str, const size_t num, const char paddingChar = ' ');
bool StartsWith(const std::string& s1, const std::string& s2) ;
void LogToOutputAndNetwork(const std::string &str);

#define MYMIN(a,b) (((a)<(b))?(a):(b))



std::vector<std::string> split(const std::string &s, char delim);

void GetBundlePath(char * _buf,int _length);

std::string UTI_GetLanguage();


void ScanGameData(const std::string & _path);

// Example REF_CAST behaviour
template<class A, class B>
B* refCast(A* a)
{
	// If the handle already is a null handle, then just return the null handle
	if( !a ) return 0;
	// Now try to dynamically cast the pointer to the wanted type
	B* b = dynamic_cast<B*>(a);
	if( b != 0 )
		{
		// Since the cast was made, we need to increase the ref counter for the returned handle
		b->AddRef();
		}
	return b;
}

std::string LoadTextFile(const std::string& _file, unsigned int _flags);


void WND_ClearWithColor(unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);
void WND_Clear();
void WND_ClearRect(int _x,int _y,int _w,int _h);
void WND_SetWindowTitle(const std::string& _title);
void WND_SetOrientation(int _orientation);
void WND_SetCapFPS(int _capFPS);
void WND_SetLogicalSize(int _w,int _h);
void WND_GetLogicalSize(int &_w,int &_h);
void UTI_CreateLogFile();
void UTI_Log(const std::string& _toto);
void UTI_CloseLogFile();
void UTI_Exit();
void UTI_SRand(unsigned int _seed);
int  UTI_Rand(int _min,int _max);
std::string UTI_GetVersion();

unsigned long long IO_Open(const std::string& _name, const std::string& _mode);
int  IO_WriteString(unsigned long long  _handle, const std::string& _string);
int  IO_ReadString(unsigned long long  _handle, std::string& _string);
int  IO_WriteInt(unsigned long long _handle, int _value);
int  IO_ReadInt(unsigned long long _handle, int& _value);
void IO_Close(unsigned long long  _handle);


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_RWops;
//TODO should definitely not be here
SDL_Texture * IMG_LoadTexture_RW(SDL_Renderer * _renderer, SDL_RWops * _flow, bool _freesrc);



void RegisterUtils();
#endif
