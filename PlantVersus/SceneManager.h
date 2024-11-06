#pragma once
#include"Scene.h"

// 告诉这个类外部会出现这些变量
extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:

	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

	SceneManager() = default;
	~SceneManager() = default;

	// 场景切换时调用on_enter
	void set_current_scene(Scene* scene)
	{
		current_scene = scene;

		current_scene->on_enter();
	}

	void switch_to(SceneType type)
	{
		// 旧场景退出
		current_scene->on_exit();
		switch (type)
		{
		case SceneManager::SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneManager::SceneType::Selector:
			current_scene = selector_scene;
			break;
		default:
			break;
		}
		// 新场景进入
		current_scene->on_enter();
	}


	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}

	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}


	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}


private:
	Scene* current_scene = nullptr;
};