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
#include "Settings.h"

#include "ResourceManager.h"
#include "Utils.h"
#include "pugixml.hpp"

#include "SDL.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Settings::Read()
{
	std::string loadedString;

	loadedString = LoadTextFile("settings.xml", FULLPATH | BOTH);

	if (loadedString.empty())
		{
		UTI_Log("Error : Empty settings file");
		return;
		}
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(loadedString.c_str());
    if (result.status != pugi::status_ok)
    {
        UTI_Log("Error : parsing xml file");
        return;
    }
	pugi::xml_node root = doc.first_child();
	for (pugi::xml_node elem = root.first_child(); elem != NULL; elem = elem.next_sibling())
	{
		SDL_Log(elem.name());
		if (strcmp(elem.name(), "position") == 0)
		{
			this->winpos_x = atoi(elem.attribute("x").as_string());
			this->winpos_y = atoi(elem.attribute("y").as_string());
		}
		else
			if (strcmp(elem.name(), "size") == 0)
			{
				this->winsize_w = atoi(elem.attribute("w").as_string());
				this->winsize_h = atoi(elem.attribute("h").as_string());
			}
			else
				if (strcmp(elem.name(), "editor") == 0)
				{
					this->editorURL = elem.attribute("url").as_string();
					this->editorArgs = elem.attribute("args").as_string();
				}
				else
					if (strcmp(elem.name(), "datafolder") == 0)
					{
						this->gamedataURL = elem.attribute("directory").as_string();
						UTI_Log(this->gamedataURL.c_str());
					}
					else
						if (strcmp(elem.name(), "autorestart") == 0)
						{
							this->autorestart = atoi(elem.attribute("value").as_string());
						}
						else
							if (strcmp(elem.name(), "allowdebug") == 0)
							{
								this->allowdebug = atoi(elem.attribute("value").as_string());
							}
							else
								if (strcmp(elem.name(), "verbose") == 0)
								{
									this->verbose = atoi(elem.attribute("value").as_string());
								}
								else
									if (strcmp(elem.name(), "logtofile") == 0)
									{
										this->logtofile = atoi(elem.attribute("value").as_string());
									}
									else
										if (strcmp(elem.name(), "server") == 0)
										{
											this->serverIP = elem.attribute("ip").as_string();
										}

	} // for(tinyxml2::XMLElement* elem ...

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Settings::Write()
{

}

