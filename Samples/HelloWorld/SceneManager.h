#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Global.h"
#include "Utils.h"

class Scene
{
public:
	virtual void Init(){};
	virtual void OnShutdown(){};
	virtual void OnRender(Uint64 _delta){};
	virtual void OnUpdate(Uint64 _delta){};
};

class SceneManager
{
public:

	void ChangeScene(Scene * _newScene);
	void OnRender(Uint64 _delta);
	void OnUpdate(Uint64 _delta);

private:
	Scene *currentScene;
};

#endif