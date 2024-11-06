#pragma once
#include "Bullet.h"
#include "Animation.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;
extern Camera main_camera;


class SunBullet : public Bullet
{
public:
	SunBullet()
	{
		size.x = 96; size.y = 96;
		damage = 20;

		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);


		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() { can_remove = true; });

		// 计算偏差
		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();

		// 宽高分别做差后/2
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();
		main_camera.shake(5, 250);
		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);

	}

	void on_update(int delta)
	{	

		// 炸弹没有爆炸时更新位置
		if (valid)
		{
			// 更新竖直方向速度
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		// 根据是否有效判断更新哪个动画
		if (!valid)
			animation_explode.on_update(delta);
		else
			animation_idle.on_update(delta);

		if (check_if_exceeds_screen())
			can_remove = true;
			
	}


	void on_draw(const Camera& camera) const
	{
		// valid主要用来检查播放爆炸还是普通动画
		if (valid)
		{
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		}
		else
		{
			animation_explode.on_draw(camera,
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));
		}

		Bullet::on_draw(camera);
	}

private:
	// 默认的闪烁动画
	Animation animation_idle;
	Animation animation_explode;

	// 爆炸动画的渲染偏移 为了使较大的爆炸动画的中心与正常的日光动画中心重叠
	Vector2 explode_render_offset;

	// 日光炸弹的重力
	const float gravity = 1e-3f;  
};