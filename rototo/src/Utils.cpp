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
#include "Utils.h"
#include "Application.h"

#ifdef WIN32
#include <direct.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#ifdef TRP_OSX
#include "CoreFoundation/CoreFoundation.h"
#endif

FILE *rwLogFile = NULL;

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		{
		elems.push_back(item);
		}
	return elems;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void PadTo(std::string &str, const size_t num, const char paddingChar)
{
	if(num > str.size())
		//str.insert(0, num - str.size(), paddingChar);
		str.append(num - str.size(), paddingChar);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

bool StartsWith(const std::string& s1, const std::string& s2)
{
	return s2.size() <= s1.size() && s1.compare(0, s2.size(), s2) == 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void LogToOutputAndNetwork(const std::string &str)
{
	SDL_Log(str.c_str());
	g_app->networkManager->SendMessageToAllClients(str);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string UTI_GetLanguage()
{
	std::string tempLanguage = "";
	
#ifdef TRP_OSX
	CFLocaleRef loc = CFLocaleCopyCurrent();
	CFArrayRef langs = CFLocaleCopyPreferredLanguages();
	CFStringRef langCode = ( CFStringRef ) CFArrayGetValueAtIndex (langs, 0);
	CFStringRef langName = CFLocaleCopyDisplayNameForPropertyValue (loc, kCFLocaleLanguageCode, langCode);
	const char *bytes;
	bytes = CFStringGetCStringPtr(langName, CFStringGetSystemEncoding());
	if (bytes == NULL)
		{
		char localBuffer[30];
		Boolean success;
		success = CFStringGetCString(langName, localBuffer, 30, CFStringGetSystemEncoding());
		tempLanguage = localBuffer;
		}
	else
	tempLanguage = bytes;
#endif
	return tempLanguage;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GetBundlePath(char * _buf,int _length)
{
	#ifdef TRP_OSX
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
	resourcesURL = CFURLCreateCopyDeletingLastPathComponent(NULL, resourcesURL);
	CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle);
	
	CFRelease(resourcesURL);
	CFStringGetCString( str, _buf, _length, kCFStringEncodingASCII );
	CFRelease(str);
	#endif

	#ifdef WIN32
	_getcwd(_buf, _length);
	#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ScanGameData(const std::string & _path)
{
#ifdef WIN32

	WIN32_FIND_DATA FindFileData;
	std::string pathToScan = _path + "*";
	HANDLE hFind = FindFirstFile( pathToScan.c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	return;
	else
	{
	do
	{
		if ( ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
		if (( strcmp(FindFileData.cFileName,".") != 0 ) && (strcmp(FindFileData.cFileName,"..") != 0 ) )
			{
			g_app->networkManager->MakeDirToAllClients(FindFileData.cFileName);
			std::string newPath  = _path;
			newPath += FindFileData.cFileName;
			newPath += "//";
			SDL_Log(newPath.c_str());
			ScanGameData(newPath);
			}
		}
		else // file
		{
		/*s = sPath;
		s.Append( _T("\\") );
		s.Append( FindFileData.cFileName );
		*/
		SDL_Log(FindFileData.cFileName);
		}
	} while ( FindNextFile( hFind, &FindFileData) != 0 );
	FindClose( hFind );
	}
	return;
#endif
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string LoadTextFile(const std::string& _file, unsigned int _flags)
{
	std::string contents;
	
	
	SDL_RWops *rw;
	
	rw = g_app->resourceManager->Load(_file,_flags);
	
	if (rw != NULL)
	{
	/* Seek to 0 bytes from the end of the file */
	Sint64 length = SDL_RWseek(rw,0,SEEK_END);
	SDL_RWseek(rw,0,SEEK_SET);// ON retourne au début
	if (length <= 0)
		{
		SDL_Log("Could not seek inside %s\n",_file.c_str());
		return "";
		}
	else
		{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"script is %d bytes long\n",length);
		contents.resize((unsigned int)length);
		if (SDL_RWread(rw,&contents[0], contents.size(),1) != 1)
		{
		SDL_Log("Error reading file %s\n",_file.c_str());
		return "";
		}
		SDL_RWclose(rw);
		return contents;
		}
	}
	else
	{
#if defined WIN32 || defined TRP_OSX
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"TRP Error",SDL_GetError(),NULL);
#endif
	return "";
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_SetWindowTitle(const std::string& _title)
{
	SDL_SetWindowTitle(g_app->sdlWindow,_title.c_str());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_SetLogicalSize(int _w,int _h)
{
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(g_app->sdlRenderer, _w, _h);

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string UTI_GetVersion()
{
	std::string contents;
	contents = ROTOTO_VERSION;
	return contents;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void UTI_CreateLogFile()
{
	SDL_assert(rwLogFile == NULL);
	rwLogFile = fopen("trp.log", "w");
	fclose(rwLogFile);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void UTI_Log(const std::string& _toto)
{
	std::string tmp = _toto + "\n";
	//SDL_Log(_toto.c_str());
	LogToOutputAndNetwork(tmp.c_str());

	if (g_app->settings_logtofile != 0)
	{
	unsigned int tmpSize = (unsigned int) tmp.length();
	
	rwLogFile = fopen("trp.log", "a+");
	if (rwLogFile != NULL)
	{
	if (fwrite( tmp.c_str(), 1, tmpSize,rwLogFile) != tmpSize)
		{
		SDL_Log("Couldn't fully write string");
		}
	}
	UTI_CloseLogFile();
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void UTI_CloseLogFile()
{
	if (rwLogFile != NULL)
	{
	fclose(rwLogFile);
	rwLogFile = NULL;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void UTI_Exit()
{
	g_app->doneCode = DONECODE_REAL_QUIT;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void UTI_SRand(unsigned int _seed)
{
	srand(_seed);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  UTI_Rand(int _min,int _max)
{
//v3 = rand() % 30 + 1985;	// v3 in the range 1985-2014
// v1 = rand() % 100;		// v1 in the range 0 to 99
	
	return rand() % (_max-_min+1) + _min;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_GetLogicalSize(int &_w,int &_h)
{
	SDL_RenderGetLogicalSize(g_app->sdlRenderer,&_w,&_h);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_SetCapFPS(int _capFPS)
{
	g_app->capFPS = _capFPS;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_SetOrientation(int _orientation)
{
	if (_orientation == ORIENTATION_PAYSAGE)
		{
		g_app->orientation = ORIENTATION_PAYSAGE;
		SDL_SetWindowSize(g_app->sdlWindow,PHYSICAL_SIZE_X,PHYSICAL_SIZE_Y);
		SDL_RenderSetLogicalSize(g_app->sdlRenderer, LOGICAL_SIZE_X,LOGICAL_SIZE_Y);
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft" ); 
		}
	else
		{
		g_app->orientation = ORIENTATION_PORTRAIT;
		SDL_SetWindowSize(g_app->sdlWindow,PHYSICAL_SIZE_Y,PHYSICAL_SIZE_X);
		SDL_RenderSetLogicalSize(g_app->sdlRenderer, LOGICAL_SIZE_Y, LOGICAL_SIZE_X);
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait" ); 
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Uint64 IO_Open(const std::string& _name,const std::string& _mode)
{
	std::string modifiedFilename;

	modifiedFilename = "save_"+_name;
	if (_mode == "r")
		{
		SDL_RWops * handle;
		handle = g_app->resourceManager->Load(modifiedFilename,GAMEDATA|BOTH);
		return (Uint64)(handle);
		}
	else
		{
		SDL_RWops * handle;
		handle = g_app->resourceManager->Save(modifiedFilename,GAMEDATA|BOTH);
		return (Uint64)(handle);
		}
	return -1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  IO_WriteString(Uint64 _handle,const std::string& _string)
{
	unsigned int len = (unsigned int)_string.size();
	
	if (_handle == 0)
		return -1;
	if (SDL_RWwrite((SDL_RWops *) _handle, &len, 1, 4) != 4)
		return -1;

	if (SDL_RWwrite((SDL_RWops *) _handle, _string.c_str(), 1, len) != len)
		return -1;

	return 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  IO_ReadString(Uint64  _handle,std::string& _string)
{
	int len;
	char *pBuffer;
	
	if (_handle == 0)
		return -1;
	
	SDL_RWread((SDL_RWops *) _handle, &len, 4, 1);
	pBuffer = new char[len+1]; // Create the transmission buffer character array
	SDL_RWread((SDL_RWops *) _handle, pBuffer, len, 1);
	pBuffer[len] = 0;
	_string = pBuffer;
	delete []pBuffer;
	return -1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  IO_WriteInt(Uint64 _handle,int _value)
{
	if (_handle == 0)
		return -1;
	if (SDL_RWwrite((SDL_RWops *) _handle, &_value, 1, 4) != 4)
		return -1;
	return 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  IO_ReadInt(Uint64  _handle,int & _value)
{
	int len;
	
	if (_handle == 0)
		return -1;
	
	SDL_RWread((SDL_RWops *) _handle,&len, 4, 1);
	_value = len;
	return 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void IO_Close(Uint64 _handle)
{
	int ret;
	if (_handle != 0)
		{
		ret = SDL_RWclose((SDL_RWops *) _handle);
		(void) ret; //TODO change the prototype of the function to return int
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_Clear()
{
	SDL_SetRenderDrawColor(g_app->sdlRenderer, g_app->guiManager->background_color.r, g_app->guiManager->background_color.g, g_app->guiManager->background_color.b, g_app->guiManager->background_color.a);
	SDL_RenderClear(g_app->sdlRenderer);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_ClearRect(int _x,int _y,int _w,int _h)
{
	SDL_Rect tempRect;
	tempRect.x = _x;
	tempRect.y = _y;
	tempRect.w = _w;
	tempRect.h = _h;

	SDL_SetRenderDrawColor(g_app->sdlRenderer,255,255,224,255);
	SDL_RenderFillRect(g_app->sdlRenderer,&tempRect);
	SDL_SetRenderDrawColor(g_app->sdlRenderer,0,0,0,255);
	SDL_RenderDrawRect(g_app->sdlRenderer,&tempRect);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WND_ClearWithColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	SDL_SetRenderDrawColor(g_app->sdlRenderer, _r, _g, _b, _a);
	SDL_RenderClear(g_app->sdlRenderer);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterUtils()
{
	int r;

///sect:Utils
r = g_app->scriptManager->engine->RegisterFuncdef("bool CallbackHandler(ref @ _sender,ref @ _userData)"); SDL_assert( r >= 0 );
///glob:void WND_SetWindowTitle(string &in title)
g_app->scriptManager->RegisterGlobalFunction("void WND_SetWindowTitle(string &in _title)", asFUNCTION(WND_SetWindowTitle), asCALL_CDECL);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_SetLogicalSize(int _w,int _h)", asFUNCTION(WND_SetLogicalSize), asCALL_CDECL); SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_GetLogicalSize(int &out _w,int &out _h)", asFUNCTION(WND_GetLogicalSize), asCALL_CDECL); SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_SetOrientation(int _orientation)", asFUNCTION(WND_SetOrientation), asCALL_CDECL); SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_SetCapFPS(int _capFPS)", asFUNCTION(WND_SetCapFPS), asCALL_CDECL); SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void UTI_Log(string &in _toto)", asFUNCTION(UTI_Log), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void UTI_Exit()", asFUNCTION(UTI_Exit), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void UTI_SRand(uint _seed)", asFUNCTION(UTI_SRand), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("int  UTI_Rand(int _min,int _max)", asFUNCTION(UTI_Rand), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("uint64  IO_Open(string &in _name,string &in _mode)", asFUNCTION(IO_Open), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("int  IO_WriteString(uint64 _handle,string &in _string)", asFUNCTION(IO_WriteString), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("int  IO_ReadString(uint64 _handle,string &out _string)", asFUNCTION(IO_ReadString), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("int  IO_WriteInt(uint64 _handle,int _value)", asFUNCTION(IO_WriteInt), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("int  IO_ReadInt(uint64 _handle,int &out _value)", asFUNCTION(IO_ReadInt), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void IO_Close(uint64 _handle)", asFUNCTION(IO_Close), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_ClearWithColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asFUNCTION(WND_ClearWithColor), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_Clear()", asFUNCTION(WND_Clear), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("void WND_ClearRect(int _x,int _y,int _w,int _h)", asFUNCTION(WND_ClearRect), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("string UTI_GetLanguage()", asFUNCTION(UTI_GetLanguage), asCALL_CDECL);SDL_assert(r>0);
r = g_app->scriptManager->engine->RegisterGlobalFunction("string UTI_GetVersion()", asFUNCTION(UTI_GetVersion), asCALL_CDECL);SDL_assert(r>0);

}

