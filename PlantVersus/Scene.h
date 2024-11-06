#pragma once
// 防止被重复包含
#include<graphics.h>
#include"Camera.h"
class Scene
{
public:
	Scene() = default;

	~Scene() = default;

	// 进入场景
	virtual void on_enter() {}
	// 数据更新
	virtual void  on_update(int delta) {}
	// 绘制界面
	virtual void on_draw(const Camera& camera) {}
	// 处理玩家输入
	virtual void on_input(const ExMessage& msg){}
	// 离开场景
	virtual void on_exit() {}

private:


};
