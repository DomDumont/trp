#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__


class Scene
{
public:
	virtual void Init(){};
	virtual void OnShutdown(){};
	virtual void OnRender(unsigned int _delta){};
	virtual void OnUpdate(unsigned int _delta){};
};

class SceneManager
{
public:

	void ChangeScene(Scene * _newScene);
	void OnRender(unsigned int _delta);
	void OnUpdate(unsigned int _delta);

private:
	Scene *currentScene;
};

#endif