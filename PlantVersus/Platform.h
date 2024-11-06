#pragma once
#include"graphics.h"
#include"Camera.h"
#include"Util.h"

extern bool is_debug;
class Platform
{
public:
	Platform() = default;
	~Platform() = default;

	// 碰撞检测线
	struct CollisionShape
	{
		float y;
		float left, right;
	};
public:
	// 实际发生碰撞的检测位置
	CollisionShape shape;
	// 平台的图片
	IMAGE* img = nullptr;
	// 碰撞平台的实际渲染位置
	POINT render_position = { 0 };


public:
	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);
		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}



private:



};