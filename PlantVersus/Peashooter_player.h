#pragma once
#include"Player.h"
#include"pea_bullet.h"
#include<iostream>

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;

extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_attack_ex_left;

extern Atlas atlas_peashooter_die_left;
extern Atlas atlas_peashooter_die_right;


class Peashooter : public player
{
public:
	Peashooter(bool facing_right = true) : player(facing_right)
	{
		animation_idel_right.set_atlas(&atlas_peashooter_idle_right);
		animation_idel_left.set_atlas(&atlas_peashooter_idle_left);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);

		animation_idel_right.set_interval(75);
		animation_idel_left.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);

		animation_die_left.set_atlas(&atlas_peashooter_die_left);
		animation_die_right.set_atlas(&atlas_peashooter_die_right);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);
		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		// 设置尺寸
		size.x = 96;
		size.y = 96;

		// 初始化攻击状态定时器
		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]() { is_attacking_ex = false; });
		// 特殊攻击  定时器默认可以触发多次
		timer_spawn_pea_ex.set_wait_time(100);
		timer_spawn_pea_ex.set_callback([&]() { spawn_pea_bullet(speed_pea_ex); });

		attack_cd = 100;
	}
	~Peashooter() = default;

	void on_update(int delta) override
	{
		player::on_update(delta);
		
		if (is_attacking_ex)
		{
			main_camera.shake(5, 100);
			timer_attack_ex.on_update(delta);
			timer_spawn_pea_ex.on_update(delta);
		}

	}

	void on_draw(const Camera& camera) override
	{
		player::on_draw(camera);

	}

	void on_attack() override
	{
		spawn_pea_bullet(speed_pea);
		// 随机播放音效
		switch (rand()%2)
		{
		case 0:
			mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
			break;
		default:
			break;
		}

	}

	void on_attack_ex()
	{
		is_attacking_ex = true;
		timer_attack_ex.restart();

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);

	}
private:
	const float speed_pea = 0.75f;
	const float speed_pea_ex = 1.5f;
	const int attack_ex_duration = 2500;  // 特殊攻击状态持续时间

	// 生成豌豆子弹             子弹速度
	void spawn_pea_bullet(float speed)
	{
		Bullet* bullet = new PeaBullet();

		// 子弹的位置和速度
		Vector2 bullet_position, bullet_velocity;
		// 控制子弹生成的位置
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = is_facing_right ? position.x + size.x - bullet_size.x / 2 : position.x - bullet_size.x / 2;
		bullet_position.y = position.y;

		bullet_velocity.x = is_facing_right ? speed : -speed;
		bullet_velocity.y = 0;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);
		// 设置子弹的碰撞目标为对方角色
		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		// 打中后加25mp
		bullet->set_callback([&]() { mp += 25; });

		bullet_list.push_back(bullet);
	}

private:
	Timer timer_attack_ex;                // 特殊攻击状态定时器
	Timer timer_spawn_pea_ex;             // 豌豆子弹发射定时器 
	 
};