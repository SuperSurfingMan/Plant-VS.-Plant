#pragma once
#include"Util.h"
// 角色局内状态条 蓝条/血条
class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_position(int x, int y) {
		position.x = x;
		position.y = y;
	}


	void set_avatar(IMAGE* img)
	{
		this->img_avatar = img;
	}

	void set_hp(int val)
	{
		hp = val;
	}

	void set_mp(int val)
	{
		mp = val;
	}


	void on_draw()
	{
		putimage_alpha(position.x, position.y, img_avatar);
		
		// 圆角矩形绘制两次 实现带阴影的状态条效果
		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);

		setfillcolor(RGB(67, 47, 47));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3, position.y + 68, 8, 8);
		

		// 实际长度
		float hp_bar_width = width * max(0, hp) / 100.0f;
		float mp_bar_width = width * min(100, mp) / 100.0f;

		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_bar_width + 3, position.y + 68, 8, 8);

	}

private:
	// 状态条的宽度
	const int width = 275;



	int hp = 0;
	int mp = 0;
	// 在窗口中显示的位置
	POINT position = { 0 };
	// 角色头像图片
	IMAGE* img_avatar = nullptr;
};