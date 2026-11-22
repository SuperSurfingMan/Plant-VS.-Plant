#pragma once
#include<iostream>
#include"player/Player.h"
#include"bullet/sun_bullet_ex.h"
#include"bullet/sun_bullet.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;

extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sun_text;

extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;

extern player* player1;
extern player* player2;

class Sunflower : public player
{
public:
	Sunflower(bool facing_right = true) : player(facing_right)
	{
		animation_idel_right.set_atlas(&atlas_sunflower_idle_right);
		animation_idel_left.set_atlas(&atlas_sunflower_idle_left);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);

		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);

		animation_sun_text.set_atlas(&atlas_sun_text);


		animation_idel_right.set_interval(75);
		animation_idel_left.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);

		animation_sun_text.set_interval(100);

		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);

		animation_sun_text.set_loop(false);

		animation_attack_ex_left.set_callback([&]() {

			is_attacking_ex = false;
			is_sun_text_visible = false;
		});

		animation_attack_ex_right.set_callback([&]() {

			is_attacking_ex = false;
			is_sun_text_visible = false;
		});


		animation_die_left.set_atlas(&atlas_sunflower_die_left);
		animation_die_right.set_atlas(&atlas_sunflower_die_right);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);
		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		// 设置尺寸
		size.x = 96;
		size.y = 96;

		attack_cd = 250;
	}
	~Sunflower() = default;

	void on_update(int delta) override 
	{
		player::on_update(delta);
		if (is_sun_text_visible)
		{
			animation_sun_text.on_update(delta);
		}
	}

	void on_draw(const Camera& camera) override
	{
		player::on_draw(camera);
		// current_animation->on_draw(camera, (int)position.x, (int)position.y);
		if (is_sun_text_visible)
		{
			Vector2 text_position;
			IMAGE* frame = animation_sun_text.get_frame();
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;
			text_position.y = position.y - frame->getheight();
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);
		}

	}
	// 制造子弹的逻辑
	void on_attack()
	{
		Bullet* bullet = new SunBullet();
		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		// 发射时的坐标
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y;

		// 设置子弹初始位置与初速度
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);

		bullet->set_callback([&]() { mp += 35; });

		bullet_list.push_back(bullet);
	}

	// 生成大日光的逻辑
	void on_attack_ex() override
	{
		is_attacking_ex = true;
		is_sun_text_visible = true;
		animation_sun_text.reset();

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new SunBulletEx();


		player* target_player = (id == PlayerID::P1 ? player2 : player1);

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_position();
		// x方向上使日光正好位于目标上方
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		// y方向上 为自身玩家y坐标的负值 在窗口之外
		bullet_position.y = -size.y;
		bullet_velocity.x = 0;
		bullet_velocity.y = speed_sun_ex;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		bullet->set_callback([&]() { mp += 50; });

		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
	}

private:

	// 大型日光炸弹下落速度
	const float speed_sun_ex = 0.15f;
	// 小型日光炸弹抛出的初速度
	const Vector2 velocity_sun = { 0.25f, -0.5f };


	// 头顶 日 字的文本动画
	Animation animation_sun_text;
	//是否显示头顶的文本
	bool is_sun_text_visible = false;


};