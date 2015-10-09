#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__


#include "SceneManager.h" //TODO à virer et à mettre dans Rototo

#include "Rototo.h"


class MenuScene : public Scene
{
public:

	int             windowX;
	int             windowY;


	Button 	buttonStart;
	ListBox listBox;
	ComboBox comboBox;
	Label  monLabel;
	Label  labelChosen;
    Label  helpLabel;	
    Music  myMusic;


	void Init();
	void OnRender(unsigned int _delta);
	void OnUpdate(unsigned int _delta);

};


#endif