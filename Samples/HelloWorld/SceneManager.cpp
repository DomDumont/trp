#include "SceneManager.h"



void SceneManager::ChangeScene(Scene * _newScene)
{
	if (currentScene != nullptr)
		{
		currentScene->OnShutdown();
		}
    _newScene->Init();
    currentScene = _newScene;
}

void SceneManager::OnRender(unsigned int _delta)
	{	
		if (currentScene != nullptr)
			{
			currentScene->OnRender(_delta);
			}

	}