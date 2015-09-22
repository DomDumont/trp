#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__


#include "SceneManager.h"


#include "Global.h"
#include "Utils.h"

#include "ListBox.h"
#include "ComboBox.h"
#include "Button.h"


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

	void Init();
	void OnRender(Uint64 _delta);
	void OnUpdate(Uint64 _delta);

};


#endif