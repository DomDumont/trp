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

	Button buttonStart;
	ListBox listBox;
	ComboBox comboBox;

	void Init();
};


#endif