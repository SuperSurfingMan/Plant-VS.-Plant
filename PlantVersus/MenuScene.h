#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"
#include"Animation.h"
#include"Camera.h"
#include"Timer.h"

extern IMAGE img_menu_background;
extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	// 进入场景
	void on_enter() override{
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);


	}
	// 数据更新
	void  on_update(int delta) override{

	}
	// 绘制界面
	void on_draw(const Camera& camera) override{
		putimage(0, 0, &img_menu_background);
	}
	// 处理玩家输入
	void on_input(const ExMessage& msg) override{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
	// 离开场景
	void on_exit() override{

	}


private:
	Animation animation_peashooter_run_right;
	Camera camera;
	Timer timer;

};