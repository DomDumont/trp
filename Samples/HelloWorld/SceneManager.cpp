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

void SceneManager::OnRender(Uint64 _delta)
	{	
		if (currentScene != NULL) 
			{
			currentScene->OnRender(_delta);
			}

	}