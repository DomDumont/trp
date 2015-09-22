#include "SceneManager.h"



void SceneManager::ChangeScene(Scene * _newScene)
{
	if (currentScene != NULL) 
		{
		currentScene->OnShutdown();
		}
    _newScene->Init();
    currentScene = _newScene;
}
