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
#include "ListBox.h"
#include "Application.h"
#include "Font.h"


#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "ListBox_p.h"


/*----------------------------------------------------------------------------*/
ListBox_p::ListBox_p()
{
#ifdef TRP_USE_BINDING
	this->onSelectionChangedHandler_script = NULL;
	this->sender_script.Set(NULL,NULL);
	this->userData_script.Set(NULL,NULL);
#endif

}

/*----------------------------------------------------------------------------*/

ListBox::ListBox():
listbox_p(new ListBox_p),bgTexture(NULL), offsetBG(0), dragState(0), font(NULL), selectedIndex(-1)
{


	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox Constructor\n");


	this->on_selection_changed_handler = NULL;

	this->backgroundColor.r = 0;
	this->backgroundColor.g = 0;
	this->backgroundColor.b = 0;
	this->backgroundColor.a = 255;

	this->itemColor.r = 50;
	this->itemColor.g = 0;
	this->itemColor.b = 0;
	this->itemColor.a = 255;

	this->selectedItemColor.r = 150;
	this->selectedItemColor.g = 150;
	this->selectedItemColor.b = 0;
	this->selectedItemColor.a = 255;

	this->textColor.r = 255;
	this->textColor.g = 255;
	this->textColor.b = 255;
	this->textColor.a = 255;

	this->sizeXBG = 0;
	this->sizeYBG = 0;
	this->sizeItemBG = 0;
	this->sender = NULL;
	this->user_data = NULL;
}

/*----------------------------------------------------------------------------*/

ListBox::~ListBox()
{

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox Destructor\n");
	MY_SAFE_RELEASE(this->font);

	if (this->bgTexture != NULL)
		{
		SDL_DestroyTexture(this->bgTexture); // et on la vire
		this->bgTexture = NULL;
		}

	
#ifdef TRP_USE_BINDING
	MY_SAFE_RELEASE(this->listbox_p->onSelectionChangedHandler_script);
#endif
	
	this->on_selection_changed_handler = NULL;

}

/*----------------------------------------------------------------------------*/

void ListBox::SetFont(Font & _font)
{

	MY_SAFE_RELEASE(this->font);
	this->font = &_font;

}

/*----------------------------------------------------------------------------*/

void ListBox::SetBackgroundColor(int _r,int _g,int _b,int _a)
{
	this->backgroundColor.r = _r;
	this->backgroundColor.g = _g;
	this->backgroundColor.b = _b;
	this->backgroundColor.a = _a;
}

/*----------------------------------------------------------------------------*/

void ListBox::SetItemColor(int _r,int _g,int _b,int _a)
{
	this->itemColor.r = _r;
	this->itemColor.g = _g;
	this->itemColor.b = _b;
	this->itemColor.a = _a;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetSelectedItemColor(int _r,int _g,int _b,int _a)
{
	this->selectedItemColor.r = _r;
	this->selectedItemColor.g = _g;
	this->selectedItemColor.b = _b;
	this->selectedItemColor.a = _a;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetTextColor(int _r,int _g,int _b,int _a)
{
	this->textColor.r = _r;
	this->textColor.g = _g;
	this->textColor.b = _b;
	this->textColor.a = _a;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::Render()
{
	if (this->shown == false)
		return;

	//Draw Background Normal
	//SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, (this->backgroundColor.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_app->sdlRenderer,this->backgroundColor.r,this->backgroundColor.g,this->backgroundColor.b,this->backgroundColor.a);
	SDL_RenderFillRect(g_app->sdlRenderer,&this->position);

	SDL_Rect sourceRect;
	sourceRect.x = 0;
	sourceRect.y = this->offsetBG;
	sourceRect.h = this->position.h;
	sourceRect.w = this->position.w;
	
	SDL_RenderCopy(g_app->sdlRenderer, this->bgTexture, &sourceRect, &this->position); // On copy la textuer

	SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, SDL_BLENDMODE_NONE);
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetSize(int _w,int _h)
{
	Widget::SetSize(_w,_h);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ListBox::SetPosition(int _x,int _y)
{
	Widget::SetPosition(_x,_y);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetUserData(void* userdata)
{
	this->user_data = userdata;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetSender(void * sender)
{
	this->sender = sender;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::SetSelectionClickHandler(on_selection_changed_handler_type handler)
{
	this->on_selection_changed_handler = handler;
}


/*----------------------------------------------------------------------------*/
void ListBox::SetUserDataScript(CScriptHandle userdata)
{
	this->listbox_p->userData_script =  userdata;
}


/*----------------------------------------------------------------------------*/
void ListBox::SetSelectionChangedHandlerScript(asIScriptFunction * handler)
{
	if ((this->listbox_p->onSelectionChangedHandler_script) && (this->listbox_p->onSelectionChangedHandler_script != handler))
	{
		this->listbox_p->onSelectionChangedHandler_script->Release();
	}
	if (handler != NULL)
	{
		this->listbox_p->onSelectionChangedHandler_script = (asIScriptFunction *)handler;
		handler->AddRef(); //Hyper important. Nous gardons un pointeur sur la callback donc on augmente son compteur de référence
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int ListBox::AddItem(const std::string& _text)
{
	Label * tempLabel = new Label();

	if (this->font == NULL)
		{
		this->font = g_app->guiManager->font;
		this->font->AddRef();
		}
	tempLabel->SetFont(*this->font);


	this->font->AddRef(); //Specific addref here because SetFont is not call by script
	//TODO Check this tempLabel->SetColor(this->textColor.r,this->textColor.g,this->textColor.b,this->textColor.a);
	tempLabel->SetText(_text);
	this->labels.push_back(tempLabel);
	
	//Rebuild the background texture
	this->BuildInternalTexture();
	return (int) this->labels.size()-1;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::RemoveItem(int _index)
{
	this->labels.erase(labels.begin()+_index);
	this->BuildInternalTexture();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::ResetContent()
{
	for (labelsIT = labels.begin();  labelsIT != labels.end(); ++labelsIT )
		{
		Label *pTemp;
		pTemp = *labelsIT;
	
		delete pTemp; //Sprites created by me (as an animation)
		//pTemp->UnLoad();
		//pTemp->Release();
		}
	labels.clear();
	this->selectedIndex = -1;
	//Rebuild the background texture
	this->BuildInternalTexture();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::BuildInternalTexture()
{
	if (this->bgTexture != NULL)
		{
		SDL_DestroyTexture(this->bgTexture); // et on la vire
		}



	//TODO Check this :  Assign themes textures, should certainly be elsewhere

	if (this->sprite_item.entry == NULL)
		if (g_app->guiManager->list_normal_item != NULL)
			this->sprite_item = *(g_app->guiManager->list_normal_item);

	if (this->sprite_selected.entry == NULL)
		if (g_app->guiManager->list_selected_item != NULL)
			this->sprite_selected = *(g_app->guiManager->list_selected_item);

	//Compute the size and then create the texture
	sizeYBG = 0;
	for (labelsIT = labels.begin();  labelsIT != labels.end(); ++labelsIT )
		{
		Label *pTemp;
		pTemp = *labelsIT;
		sizeYBG += pTemp->frame.h+5;
		}
	sizeXBG = this->position.w;

	// If there is not enough items to fill the listbox space
	// Make the backgroung texture at least the same size
	if (sizeYBG<this->position.h)
		{
		sizeYBG = this->position.h;
		}
	this->bgTexture = SDL_CreateTexture(g_app->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, sizeXBG, sizeYBG);
	SDL_SetTextureBlendMode(this->bgTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(g_app->sdlRenderer, this->bgTexture);



	SDL_SetRenderDrawColor(g_app->sdlRenderer,0,0,0,0);
	SDL_RenderClear(g_app->sdlRenderer);

	int posX = 0; // Change this
	int posY = 0;


	int i=0;
	for (labelsIT = labels.begin();  labelsIT != labels.end(); ++labelsIT)
		{
		Label *pTemp;
		pTemp = *labelsIT;

		pTemp->SetAnchor(ANCHOR_TOPLEFT);
		pTemp->SetPosition(posX + 20 ,posY); 
		//Draw the item rectangle
		SDL_Rect tempRect;
		tempRect.x = posX;
		tempRect.y = posY;
		tempRect.w = this->position.w;
		tempRect.h = pTemp->frame.h;
		if (i != this->selectedIndex)
			{
			//Normal Item
			if (this->sprite_item.entry != NULL)
				{
				this->sprite_item.SetAnchor(ANCHOR_TOPLEFT);
				this->sprite_item.SetPosition(tempRect.x,tempRect.y);
				this->sprite_item.SetSize(tempRect.w,tempRect.h);
				this->sprite_item.Render();
				}
			else
				{
				SDL_SetRenderDrawColor(g_app->sdlRenderer,this->itemColor.r,this->itemColor.g,this->itemColor.b,this->itemColor.a);
				SDL_RenderFillRect(g_app->sdlRenderer,&tempRect);
				}
			}
		else
			{
		//Selected Item
		if (this->sprite_selected.entry != NULL)
			{
			this->sprite_selected.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_selected.SetPosition(tempRect.x,tempRect.y);
			this->sprite_selected.SetSize(tempRect.w,tempRect.h);
			this->sprite_selected.Render();
			}
		else
			{
			SDL_SetRenderDrawColor(g_app->sdlRenderer,this->selectedItemColor.r,this->selectedItemColor.g,this->selectedItemColor.b,this->selectedItemColor.a);
			SDL_RenderFillRect(g_app->sdlRenderer,&tempRect);
			}
			}
		

		pTemp->Render();

		this->sizeItemBG = pTemp->frame.h+5;
		posY += this->sizeItemBG; //Size of the current Label

		i++;
		}

	SDL_RenderCopy(g_app->sdlRenderer, this->bgTexture, NULL, NULL); // On fait le rendu de la texture
	SDL_SetRenderTarget(g_app->sdlRenderer, NULL); // On repasse en target normale

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int ListBox::OnMouseButtonDown( SDL_Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox OnMouseButtonDown\n");
	this->dragState = 1;
	return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int ListBox::OnMouseButtonUp( SDL_Event * event)
{
	int ret = 0;
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox OnMouseButtonUp\n");
	if (this->dragState == 1)
		{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox Select on button up\n");
		//Only Drag not motion
		//Compute selected item
		
	this->selectedIndex = (int) ((this->offsetBG + event->button.y-this->position.y)/this->sizeItemBG);

	if (this->selectedIndex > (int)labels.size())
		this->selectedIndex = -1;

		//Rebuild the background texture
		this->BuildInternalTexture();

		//Call Callback
		
#ifdef TRP_USE_BINDING
		if (this->listbox_p->onSelectionChangedHandler_script != NULL)
			{			
				this->listbox_p->sender_script.Set(this, ScriptManager::Get().engine->GetObjectTypeByName("ListBox"));
				ret = ScriptManager::Get().RunCallback(this->listbox_p->onSelectionChangedHandler_script,&(this->listbox_p->sender_script), &(this->listbox_p->userData_script));
				this->listbox_p->sender_script.Set(NULL, NULL);
			}
#endif
		
		if (this->on_selection_changed_handler != NULL)
			{
			bool ret = this->on_selection_changed_handler(this->sender,this->user_data);
			return ret;
			}


		}
	this->dragState = 0;
	return ret ;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ListBox::OnMouseMotion( SDL_Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ListBox OnMouseMotion\n");
	if (this->dragState >= 1)
		{
		this->dragState = 2; //Drag + motion
		offsetBG -= event->motion.yrel;
		if (this->offsetBG<0) this->offsetBG = 0;
		if (this->offsetBG>(sizeYBG-this->position.h)) this->offsetBG = (sizeYBG-this->position.h);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"offsetBG = %d\n",this->offsetBG);
		}
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  ListBox::GetSelectedIndex()
{
	return this->selectedIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string ListBox::GetItemText(int _index)
{
	if ((_index != -1)&&(_index<(int)labels.size()))
		return labels[_index]->text;
	else
		return "";
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

ListBox *ListBox_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new ListBox();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING

void RegisterListBox()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{

		///class:ListBox
		r = ScriptManager::Get().engine->RegisterObjectType("ListBox", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_FACTORY, "ListBox@ f()", asFUNCTION(ListBox_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_ADDREF, "void f()", asMETHOD(ListBox, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_RELEASE, "void f()", asMETHOD(ListBox, Release), asCALL_THISCALL); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void Render()", asMETHOD(ListBox, Render), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSize(int _w,int _h)", asMETHODPR(ListBox, SetSize, (int, int), void), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetPosition(int _x,int _y)", asMETHODPR(ListBox, SetPosition, (int, int), void), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetFont(Font @ _font)", asMETHOD(ListBox, SetFont), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "int AddItem(const string &in _newText)", asMETHOD(ListBox, AddItem), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void RemoveItem(int _index)", asMETHOD(ListBox, RemoveItem), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void ResetContent()", asMETHOD(ListBox, ResetContent), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "int GetSelectedIndex()", asMETHOD(ListBox, GetSelectedIndex), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "string GetItemText(int _index)", asMETHOD(ListBox, GetItemText), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetEnabled(bool _value)", asMETHOD(ListBox, SetEnabled), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetBackgroundColor(int _r,int _g,int _b,int _a)", asMETHOD(ListBox, SetBackgroundColor), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetItemColor(int _r,int _g,int _b,int _a)", asMETHOD(ListBox, SetItemColor), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSelectedItemColor(int _r,int _g,int _b,int _a)", asMETHOD(ListBox, SetSelectedItemColor), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetTextColor(int _r,int _g,int _b,int _a)", asMETHOD(ListBox, SetTextColor), asCALL_THISCALL);
		SDL_assert(r >= 0);
		///func:float GetRotation()
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "float get_Rotation()", asMETHOD(ListBox, GetRotation), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetUserData( ref @userdata)", asMETHOD(ListBox, SetUserDataScript), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSelectionChangedHandler( CallbackHandler @)", asMETHOD(ListBox, SetSelectionChangedHandlerScript), asCALL_THISCALL);
		SDL_assert(r >= 0);
	}
	
	else
	{
		r = ScriptManager::Get().engine->RegisterObjectType("ListBox", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_FACTORY, "ListBox@ f()", WRAP_FN(ListBox_Factory), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_ADDREF, "void f()", WRAP_MFN(ListBox, AddRef), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("ListBox", asBEHAVE_RELEASE, "void f()", WRAP_MFN(ListBox, Release), asCALL_GENERIC); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void Render()", WRAP_MFN(ListBox, Render), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSize(int _w,int _h)", WRAP_MFN_PR(ListBox, SetSize, (int, int), void), asCALL_GENERIC); //TODO Check this , should be virtual ? _PR ?
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetPosition(int _x,int _y)", WRAP_MFN_PR(ListBox, SetPosition, (int, int), void), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetFont(Font @ _font)", WRAP_MFN(ListBox, SetFont), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "int AddItem(const string &in _newText)", WRAP_MFN(ListBox, AddItem), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void RemoveItem(int _index)", WRAP_MFN(ListBox, RemoveItem), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void ResetContent()", WRAP_MFN(ListBox, ResetContent), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "int GetSelectedIndex()", WRAP_MFN(ListBox, GetSelectedIndex), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "string GetItemText(int _index)", WRAP_MFN(ListBox, GetItemText), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetEnabled(bool _value)", WRAP_MFN(ListBox, SetEnabled), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetBackgroundColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ListBox, SetBackgroundColor), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetItemColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ListBox, SetItemColor), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSelectedItemColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ListBox, SetSelectedItemColor), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetTextColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ListBox, SetTextColor), asCALL_GENERIC);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "float get_Rotation()", WRAP_MFN(ListBox, GetRotation), asCALL_GENERIC);
		SDL_assert(r >= 0);


		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetUserData( ref @userdata)", WRAP_MFN(ListBox, SetUserDataScript), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("ListBox", "void SetSelectionChangedHandler( CallbackHandler @)", WRAP_MFN(ListBox, SetSelectionChangedHandlerScript), asCALL_GENERIC);
		SDL_assert(r >= 0);



	}

}
#endif
