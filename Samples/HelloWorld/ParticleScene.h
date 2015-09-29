#ifndef __PARTICLESCENE_H__
#define __PARTICLESCENE_H__


#include "SceneManager.h"

#include "Rototo.h"


class ParticleScene : public Scene
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
	void OnRender(unsigned int _delta);
	void OnUpdate(unsigned int _delta);

};


#endif