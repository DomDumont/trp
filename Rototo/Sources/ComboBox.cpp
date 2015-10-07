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
#include "ComboBox.h"
#include "Application_p.h"
#include "Font.h"

#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

ComboBox::ComboBox():
bgTexture(NULL),offsetBG(0),dragState(0),font(NULL),selectedIndex(-1),state(0)
{
	
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ComboBox Constructor\n");
	
#ifdef TRP_USE_BINDING
	this->onSelectionChangedHandler_script = NULL;
	this->sender_script.Set(NULL, NULL);
	this->userData_script.Set(NULL, NULL);
#endif
	
	this->on_selection_changed_handler = NULL;
	this->user_data = NULL;
	this->sender = NULL;

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
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


ComboBox::~ComboBox()
{
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ComboBox Destructor\n");
	MY_SAFE_RELEASE(this->font);
	
	if (this->bgTexture != NULL)
	{
		SDL_DestroyTexture(this->bgTexture); // et on la vire
		this->bgTexture = NULL;
	}

#ifdef TRP_USE_BINDING
	MY_SAFE_RELEASE(this->onSelectionChangedHandler_script);
#endif
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ComboBox::SetPosition(int _x,int _y)
{
	Widget::SetPosition(_x,_y);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void ComboBox::SetFont(Font & _font)
{
	
	MY_SAFE_RELEASE(this->font);
	this->font = &_font;
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::SetBackgroundColor(int _r,int _g,int _b,int _a)
{
	this->backgroundColor.r = _r;
	this->backgroundColor.g = _g;
	this->backgroundColor.b = _b;
	this->backgroundColor.a = _a;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::SetItemColor(int _r,int _g,int _b,int _a)
{
	this->itemColor.r = _r;
	this->itemColor.g = _g;
	this->itemColor.b = _b;
	this->itemColor.a = _a;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::SetSelectedItemColor(int _r,int _g,int _b,int _a)
{
	this->selectedItemColor.r = _r;
	this->selectedItemColor.g = _g;
	this->selectedItemColor.b = _b;
	this->selectedItemColor.a = _a;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::SetTextColor(int _r,int _g,int _b,int _a)
{
	this->textColor.r = _r;
	this->textColor.g = _g;
	this->textColor.b = _b;
	this->textColor.a = _a;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::Render()
{
	if (this->shown == false)
		return;
	if (this->state == 1)
		{
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
	else
		{
		// Closed State
		SDL_Rect	closedPosition;
		closedPosition  = this->position;
		closedPosition.h = this->sizeItemBG - 5; //Todo change this
			
		SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(g_app->sdlRenderer,this->backgroundColor.r,this->backgroundColor.g,this->backgroundColor.b,this->backgroundColor.a);
		SDL_RenderFillRect(g_app->sdlRenderer,&closedPosition);
		
		SDL_Rect sourceRect;
		sourceRect.x = 0;
		sourceRect.y = this->selectedIndex * this->sizeItemBG;
		sourceRect.h = this->sizeItemBG-5;
		sourceRect.w = this->position.w;
		
		SDL_RenderCopy(g_app->sdlRenderer, this->bgTexture, &sourceRect, &closedPosition); // On copy la textuer
		
		SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, SDL_BLENDMODE_NONE);
		}
}




/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::SetSize(int _w,int _h)
{
	Widget::SetSize(_w,_h);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int ComboBox::AddItem(const std::string& _text)
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

void ComboBox::RemoveItem(int _index)
{
	this->labels.erase(labels.begin()+_index);
	this->BuildInternalTexture();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::ResetContent()
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

void ComboBox::BuildInternalTexture()
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
	
	// If there is not enough items to fill the ComboBox space
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

int ComboBox::OnMouseButtonDown( SDL_Event * event)
{
	if (state == 1)
		{
		this->dragState = 1;
		return false;
		}
	else
		{
		this->state = 1; // Open the combobox
		return false;
		}
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int ComboBox::OnMouseButtonUp( SDL_Event * event)
{
	int ret = 0;
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ComboBox OnMouseButtonUp\n");
	if (this->dragState == 1)
		{

		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ComboBox Select on button up\n");
		//Only Drag not motion
		//Compute selected item
		
		this->selectedIndex = (int) ((this->offsetBG + event->button.y-this->position.y)/this->sizeItemBG);
		
		if (this->selectedIndex > (int)labels.size())
			this->selectedIndex = -1;
		
		//Rebuild the background texture
		this->BuildInternalTexture();

#ifdef TRP_USE_BINDING
		//Call Callback
		if (this->onSelectionChangedHandler_script != NULL)
			{

			this->sender_script.Set(this, g_app->scriptManager->engine->GetObjectTypeByName("ComboBox"));
			ret = g_app->scriptManager->RunCallback(this->onSelectionChangedHandler_script, &(this->sender_script), &(this->userData_script));
			this->sender_script.Set(NULL, NULL);

			}
#endif

		this->state = 0; //Close the comboBox
		
		}
	this->dragState = 0;
	return ret;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ComboBox::OnMouseMotion( SDL_Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"ComboBox OnMouseMotion\n");
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
void ComboBox::SetSelectedIndex(int _newIndex)
{
	this->selectedIndex = _newIndex; //TODO add check
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int  ComboBox::GetSelectedIndex()
{
	return this->selectedIndex;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string ComboBox::GetItemText(int _index)
{
	if ((_index != -1)&&(_index<(int)labels.size()))
		return labels[_index]->text;
	else
		return "";
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

ComboBox *ComboBox_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new ComboBox();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterComboBox()
{
	int r;

#ifndef __EMSCRIPTEN__

	///class:ComboBox
	r = g_app->scriptManager->engine->RegisterObjectType("ComboBox", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_FACTORY, "ComboBox@ f()", asFUNCTION(ComboBox_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_ADDREF, "void f()", asMETHOD(ComboBox,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_RELEASE, "void f()", asMETHOD(ComboBox,Release), asCALL_THISCALL); SDL_assert( r >= 0 );

	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void Render()", asMETHOD(ComboBox, Render), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSize(int _w,int _h)", asMETHODPR(ComboBox, SetSize,(int,int),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetPosition(int _x,int _y)", asMETHODPR(ComboBox, SetPosition,(int,int),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetFont(Font @ _font)", asMETHOD(ComboBox, SetFont), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","int AddItem(const string &in _newText)", asMETHOD(ComboBox, AddItem), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void RemoveItem(int _index)", asMETHOD(ComboBox, RemoveItem), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void ResetContent()", asMETHOD(ComboBox, ResetContent), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","int GetSelectedIndex()", asMETHOD(ComboBox, GetSelectedIndex), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSelectedIndex(int _index)", asMETHOD(ComboBox, SetSelectedIndex), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","string GetItemText(int _index)", asMETHOD(ComboBox, GetItemText), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetEnabled(bool _value)", asMETHOD(ComboBox, SetEnabled), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetBackgroundColor(int _r,int _g,int _b,int _a)", asMETHOD(ComboBox, SetBackgroundColor), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetItemColor(int _r,int _g,int _b,int _a)", asMETHOD(ComboBox, SetItemColor), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSelectedItemColor(int _r,int _g,int _b,int _a)", asMETHOD(ComboBox, SetSelectedItemColor), asCALL_THISCALL);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetTextColor(int _r,int _g,int _b,int _a)", asMETHOD(ComboBox, SetTextColor), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:float GetRotation()
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","float get_Rotation()", asMETHOD(ComboBox, GetRotation), asCALL_THISCALL);SDL_assert( r >= 0 );

	///prop:CallbackHandler @onSelectionChangedHandler
	g_app->scriptManager->RegisterObjectProperty("ComboBox", "CallbackHandler @onSelectionChangedHandler", asOFFSET(ComboBox, onSelectionChangedHandler_script));
	///prop:ref @userData
	g_app->scriptManager->RegisterObjectProperty("ComboBox", "ref @userData", asOFFSET(ComboBox, userData_script));
	//g_app->scriptManager->RegisterClassMethod("ComboBox","void Update(uint64 _elapsed)", asMETHOD(ComboBox, Update));
#else
	///class:ComboBox
	r = g_app->scriptManager->engine->RegisterObjectType("ComboBox", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_FACTORY, "ComboBox@ f()", WRAP_FN(ComboBox_Factory), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_ADDREF, "void f()", WRAP_MFN(ComboBox,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("ComboBox", asBEHAVE_RELEASE, "void f()", WRAP_MFN(ComboBox,Release), asCALL_GENERIC); SDL_assert( r >= 0 );
	
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void Render()", WRAP_MFN(ComboBox, Render), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSize(int _w,int _h)", WRAP_MFN_PR(ComboBox, SetSize,(int,int),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetPosition(int _x,int _y)", WRAP_MFN_PR(ComboBox, SetPosition,(int,int),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetFont(Font @ _font)", WRAP_MFN(ComboBox, SetFont), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","int AddItem(const string &in _newText)", WRAP_MFN(ComboBox, AddItem), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void RemoveItem(int _index)", WRAP_MFN(ComboBox, RemoveItem), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void ResetContent()", WRAP_MFN(ComboBox, ResetContent), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","int GetSelectedIndex()", WRAP_MFN(ComboBox, GetSelectedIndex), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSelectedIndex(int _index)", WRAP_MFN(ComboBox, SetSelectedIndex), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","string GetItemText(int _index)", WRAP_MFN(ComboBox, GetItemText), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetEnabled(bool _value)", WRAP_MFN(ComboBox, SetEnabled), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetBackgroundColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ComboBox, SetBackgroundColor), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetItemColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ComboBox, SetItemColor), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetSelectedItemColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ComboBox, SetSelectedItemColor), asCALL_GENERIC);SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","void SetTextColor(int _r,int _g,int _b,int _a)", WRAP_MFN(ComboBox, SetTextColor), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:float GetRotation()
	r = g_app->scriptManager->engine->RegisterObjectMethod("ComboBox","float get_Rotation()", WRAP_MFN(ComboBox, GetRotation), asCALL_GENERIC);SDL_assert( r >= 0 );

	///prop:CallbackHandler @onSelectionChangedHandler
	g_app->scriptManager->RegisterObjectProperty("ComboBox", "CallbackHandler @onSelectionChangedHandler", asOFFSET(ComboBox, onSelectionChangedHandler_script));
	///prop:ref @userData
	g_app->scriptManager->RegisterObjectProperty("ComboBox", "ref @userData", asOFFSET(ComboBox, userData_script));
	//g_app->scriptManager->RegisterClassMethod("ComboBox","void Update(uint64 _elapsed)", asMETHOD(ComboBox, Update));
	
#endif

}
#endif
