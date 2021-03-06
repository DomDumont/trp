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

#include "Global.h"



#include "TextManager.h"
#include "Application.h"

#include "Utils.h"

#include "pugixml.hpp"


#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "SDL.h"

/*----------------------------------------------------------------------------*/

TextManager& TextManager::Get()
{
	static TextManager foo;
	return foo;
}

/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING
void RegisterTextManager()
{
	
	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		///sect:Text
		///glob:string TXT_GetString(string &id)
		ScriptManager::Get().RegisterGlobalFunction("string TXT_GetString(string &in id)", asFUNCTION(TXT_GetString), asCALL_CDECL);
		///glob:void TXT_Load(string &in file,int flags=13)
		ScriptManager::Get().RegisterGlobalFunction("void TXT_Load(string &in _file,int _flags=13)", asFUNCTION(TXT_Load), asCALL_CDECL);
		///glob:void TXT_UnLoad()
		ScriptManager::Get().RegisterGlobalFunction("void TXT_UnLoad()", asFUNCTION(TXT_UnLoad), asCALL_CDECL);
	}
	else
	{
		ScriptManager::Get().RegisterGlobalFunction("string TXT_GetString(string &in id)", WRAP_FN(TXT_GetString), asCALL_GENERIC);
		ScriptManager::Get().RegisterGlobalFunction("void TXT_Load(string &in _file,int _flags=13)", WRAP_FN(TXT_Load), asCALL_GENERIC);
		ScriptManager::Get().RegisterGlobalFunction("void TXT_UnLoad()", WRAP_FN(TXT_UnLoad), asCALL_GENERIC);

	}
	
}
#endif


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
std::string TXT_GetString(const std::string& _id)
{
	std::string toto = TextManager::Get().GetString(_id);
	UTI_Log(toto.c_str());
	return toto;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TXT_Load(const std::string& _file,int _flags=13)
{
	TextManager::Get().Load(_file, _flags);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void TXT_UnLoad()
{
	TextManager::Get().UnLoad();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextManager::TextManager()
{
	this->color.r = 0;
	this->color.g = 0;
	this->color.b = 0;
	this->color.a = 255;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


TextManager::~TextManager()
{

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextManager::Init()
  {
	

  }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextManager::Shutdown()
  {

  }


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string TextManager::GetString(const std::string& _id)
{
	std::map<std::string,std::string>::iterator it = mapStrings.find(_id);
	std::string result;
	if(it != mapStrings.end())
	{
		//element found;
		result = it->second;
		return result;
	}
	else
		return "NOT FOUND";
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextManager::Load(const std::string& _file,int _flags)
{
	// First clear everything before loading them again

	mapStrings.clear();

	// First load and create the Texture
	
	std::string fullPath;
	std::string loadedString;
	
	
	
	fullPath = _file + ".xml";
	
	loadedString = LoadTextFile(fullPath,_flags);
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(loadedString.c_str());
	pugi::xml_node root = doc.first_child();
	for (pugi::xml_node elem = root.first_child(); elem != NULL; elem = elem.next_sibling())
		{
		mapStrings[elem.attribute("id").value()] = elem.attribute("value").value();
		}
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Text Dico <%s> loaded successfully \n",_file.c_str());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextManager::UnLoad()
{
	mapStrings.clear();
}
