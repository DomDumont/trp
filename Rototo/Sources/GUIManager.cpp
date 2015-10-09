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



#include "GUIManager.h"
#include "Application_p.h"
#include "Utils.h"
#include "pugixml.hpp"
#include "Font.h"
#include "Sprite.h"
#include "Atlas.h"

#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void GUI_LoadTheme(const std::string& _file)
{
	g_app->guiManager->LoadTheme(_file);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void GUI_UnLoadTheme()
{
	g_app->guiManager->UnLoadTheme();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void GUI_AddWidget(Widget *_widget)
{
	g_app->guiManager->AddWidget(_widget);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void GUI_RemoveWidget(Widget *_widget)
{
	g_app->guiManager->RemoveWidget(_widget);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

GUIManager::GUIManager(): font(NULL), atlas(NULL), button_up(NULL),button_down(NULL), button_disable(NULL),
	list_normal_item(NULL),list_selected_item(NULL),checkbox_up(NULL),checkbox_down(NULL),checkbox_disable(NULL),
	radiobox_up(NULL),radiobox_down(NULL),radiobox_disable(NULL)

{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"CGUIManager Constructor\n");
	this->primary_text_color.r = 0;
	this->primary_text_color.g = 0;
	this->primary_text_color.b = 0;
	this->primary_text_color.a = 255;

	this->disable_text_color.r = 0;
	this->disable_text_color.g = 0;
	this->disable_text_color.b = 0;
	this->disable_text_color.a = 255;

	this->background_color.r = 0;
	this->background_color.g = 0;
	this->background_color.b = 0;
	this->background_color.a = 255;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

GUIManager::~GUIManager()
{
	UnLoadTheme(); //TODO Check this. Just in case ????
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::Init()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"CGUIManager::Init\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::Shutdown()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"CGUIManager::Shutdown\n");
	widgets.clear(); //TODO perhaps release also the widgets, to check.
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::HandleEvent( SDL_Event * event)
{
	switch (event->type) 
	{
	case SDL_MOUSEBUTTONDOWN:
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Nb Widgets = %d\n",widgets.size());
		for (widgetsIT = widgets.begin();  widgetsIT != widgets.end(); ++widgetsIT )
			{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Pouet Pouet %d\n",widgets.size());
			Widget *pTemp;
			pTemp = *widgetsIT;
			if (pTemp->Touched(event->button.x,event->button.y))
				{
				if (pTemp->OnMouseButtonDown(event) != 0)
					break;
				}

			}

		break;
	case SDL_MOUSEBUTTONUP:
		for (widgetsIT = widgets.begin();  widgetsIT != widgets.end(); ++widgetsIT )
		{
		Widget *pTemp;
		pTemp = *widgetsIT;
		if (pTemp->OnMouseButtonUp(event) != 0)
			break;

		/*
		if (pTemp->Touched(event->button.x,event->button.y))
			{
			
			}
		*/
		}
		break;

	case SDL_MOUSEMOTION:
		for (widgetsIT = widgets.begin();  widgetsIT != widgets.end(); ++widgetsIT )
		{
		Widget *pTemp;
		pTemp = *widgetsIT;
		pTemp->OnMouseMotion(event);

		/*
		if (pTemp->Touched(event->button.x,event->button.y))
			{
			
			}
		*/
		}
		break;

	case SDL_KEYUP:

		for (widgetsIT = widgets.begin();  widgetsIT != widgets.end(); ++widgetsIT )
		{
		Widget *pTemp;
		pTemp = *widgetsIT;
		pTemp->OnKeyUp(event);
		}
		break;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::AddWidget(Widget *_widget)
{
	if( _widget )
		{
		widgets.push_back(_widget);
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::RemoveWidget(Widget *_widget)
{

	if( _widget )
		{
		int total_widgets = (int) widgets.size();
		widgetsIT = widgets.begin();
		for (int i=0; i <  total_widgets; i++,widgetsIT++)
			{
			if (_widget == (*widgetsIT))
				{
			
				//param->Release();
				_widget->Release();
				widgets.erase(widgetsIT);
				break;
				}
			}
		_widget->Release();
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sprite * GUIManager::LoadSprite(void* _elem)
{

  pugi::xml_node * el = (pugi::xml_node *) _elem;


	Sprite * temp_sprite;
	temp_sprite= new Sprite();
	temp_sprite->Load(this->atlas,el->attribute("name").as_string());
	if (strcmp(el->attribute("nine").as_string(),"yes")== 0)
	{
	temp_sprite->SetNinePatch(true);
  temp_sprite->SetNinePatchRect(atoi(el->attribute("x").as_string()), atoi(el->attribute("y").as_string()), atoi(el->attribute("w").as_string()), atoi(el->attribute("h").as_string()));
	}
	else
	{
	temp_sprite->SetNinePatch(false);
	}

	this->atlas->AddRef(); // Load is not done through script so add the reference manually
	return temp_sprite;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::LoadTheme(const std::string& _file)
{
	std::string fullPath;
	std::string loadedString;

	//First load Atlas

	MY_SAFE_RELEASE(this->atlas);

	fullPath = "themes//"+_file+"//images//"+_file;
	this->atlas = new Atlas();
	this->atlas->Load(fullPath,FULLPATH|BOTH);
	


	//Then load the theme xml

	fullPath = "themes//"+_file+"//"+_file+".xml";
	loadedString = LoadTextFile(fullPath,FULLPATH|BOTH);
	if (loadedString.empty())
	{
	SDL_Log("cannot find xml definition for theme %s",_file.c_str());
	return;
	}

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(loadedString.c_str());
  pugi::xml_node root = doc.first_child();
  for (pugi::xml_node elem = root.first_child(); elem != NULL; elem = elem.next_sibling())
	{
	SDL_Log(elem.name());
		
  if (strcmp(elem.name(), "font") == 0)
		{
		std::string tempFontName;
		tempFontName  = "themes//"+_file+"//fonts//" + elem.attribute("name").as_string();

		int tempSize   = atoi(elem.attribute("size").as_string());
		SDL_assert(this->font == NULL);
		this->font = new Font();
		this->font->Load(tempFontName,tempSize,FULLPATH|BOTH);
		}
	else
    if (strcmp(elem.name(), "button_up") == 0)
		{
		MY_SAFE_RELEASE(this->button_up);
		this->button_up = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "button_down") == 0)
		{
		MY_SAFE_RELEASE(this->button_down);
		this->button_down = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "button_disabled") == 0)
		{
		MY_SAFE_RELEASE(this->button_disable);
		this->button_disable = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "list_normal") == 0)
		{
		MY_SAFE_RELEASE(this->list_normal_item);
		this->list_normal_item = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "list_selected") == 0)
		{
		MY_SAFE_RELEASE(this->list_selected_item);
		this->list_selected_item = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "checkbox_up") == 0)
		{
		MY_SAFE_RELEASE(this->checkbox_up);
		this->checkbox_up = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "checkbox_down") == 0)
		{
		MY_SAFE_RELEASE(this->checkbox_down);
		this->checkbox_down = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "checkbox_disabled") == 0)
		{
		MY_SAFE_RELEASE(this->checkbox_disable);
		this->checkbox_disable = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "radiobox_up") == 0)
		{
		MY_SAFE_RELEASE(this->radiobox_up);
		this->radiobox_up = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "radiobox_down") == 0)
		{
		MY_SAFE_RELEASE(this->radiobox_down);
		this->radiobox_down = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "radiobox_disabled") == 0)
		{
			MY_SAFE_RELEASE(this->radiobox_disable);
			this->radiobox_disable = LoadSprite(&elem);
		}
	else
    if (strcmp(elem.name(), "primary_text_color") == 0)
		{
		primary_text_color.r = atoi(elem.attribute("r").as_string());
    primary_text_color.g = atoi(elem.attribute("g").as_string());
    primary_text_color.b = atoi(elem.attribute("b").as_string());
    primary_text_color.a = atoi(elem.attribute("a").as_string());
		}
	else
    if (strcmp(elem.name(), "disable_text_color") == 0)
		{
    disable_text_color.r = atoi(elem.attribute("r").as_string());
    disable_text_color.g = atoi(elem.attribute("g").as_string());
    disable_text_color.b = atoi(elem.attribute("b").as_string());
    disable_text_color.a = atoi(elem.attribute("a").as_string());
		}
	else
    if (strcmp(elem.name(), "background_color") == 0)
		{
    background_color.r = atoi(elem.attribute("r").as_string());
    background_color.g = atoi(elem.attribute("g").as_string());
    background_color.b = atoi(elem.attribute("b").as_string());
    background_color.a = atoi(elem.attribute("a").as_string());
		}

	}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void GUIManager::UnLoadTheme()
{
	
	MY_SAFE_RELEASE(this->font);
	MY_SAFE_RELEASE(this->atlas);
	MY_SAFE_RELEASE(this->button_up);
	MY_SAFE_RELEASE(this->button_down);
	MY_SAFE_RELEASE(this->button_disable);
	MY_SAFE_RELEASE(this->list_normal_item);
	MY_SAFE_RELEASE(this->list_selected_item);
	MY_SAFE_RELEASE(this->checkbox_up);
	MY_SAFE_RELEASE(this->checkbox_down);
	MY_SAFE_RELEASE(this->checkbox_disable);
	MY_SAFE_RELEASE(this->radiobox_up);
	MY_SAFE_RELEASE(this->radiobox_down);
	MY_SAFE_RELEASE(this->radiobox_disable);
	
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterGUI()
{

#ifndef __EMSCRIPTEN__ //TODO Pout this elsewhese.

	///sect:GUI
	///glob:void GUI_AddWidget(Widget @)
	g_app->scriptManager->RegisterGlobalFunction("void GUI_AddWidget(Widget @)", asMETHOD(GUIManager,AddWidget), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);
	///glob:void GUI_RemoveWidget(Widget @)
	g_app->scriptManager->RegisterGlobalFunction("void GUI_RemoveWidget(Widget @)", asMETHOD(GUIManager,RemoveWidget), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);
	///glob:void GUI_LoadTheme(string &in file)
	g_app->scriptManager->RegisterGlobalFunction("void GUI_LoadTheme(string &in _file)", asMETHOD(GUIManager,LoadTheme), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);
	///glob:void GUI_UnLoadTheme()
	g_app->scriptManager->RegisterGlobalFunction("void GUI_UnLoadTheme()", asMETHOD(GUIManager,UnLoadTheme), asCALL_THISCALL_ASGLOBAL, g_app->guiManager);
#else

	g_app->scriptManager->RegisterGlobalFunction("void GUI_AddWidget(Widget @)", WRAP_FN(GUI_AddWidget), asCALL_GENERIC);
	g_app->scriptManager->RegisterGlobalFunction("void GUI_RemoveWidget(Widget @)", WRAP_FN(GUI_RemoveWidget), asCALL_GENERIC);
	g_app->scriptManager->RegisterGlobalFunction("void GUI_LoadTheme(string &in _file)", WRAP_FN(GUI_LoadTheme), asCALL_GENERIC);
	g_app->scriptManager->RegisterGlobalFunction("void GUI_UnLoadTheme()", WRAP_FN(GUI_UnLoadTheme), asCALL_GENERIC);

#endif	
}