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



#include "TextManager.h"
#include "Application_p.h"

#include "Utils.h"

#include "pugixml.hpp"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING
void RegisterTextManager()
{
	
	///sect:Text
	///glob:string TXT_GetString(string &id)
	g_app->scriptManager->RegisterGlobalFunction("string TXT_GetString(string &in id)", asMETHOD(TextManager,GetString), asCALL_THISCALL_ASGLOBAL, g_app->textManager);
	///glob:void TXT_Load(string &in file,int flags=13)
	g_app->scriptManager->RegisterGlobalFunction("void TXT_Load(string &in _file,int _flags=13)", asMETHOD(TextManager,Load), asCALL_THISCALL_ASGLOBAL, g_app->textManager);
	g_app->scriptManager->RegisterGlobalFunction("string TXT_UnLoad()", asMETHOD(TextManager,UnLoad), asCALL_THISCALL_ASGLOBAL, g_app->textManager);
	
}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextManager::TextManager()
{

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
