#pragma once
#include"Camera.h"
#include<graphics.h>
#include"Vector2.h"
#include"Animation.h"
#include"PlayerID.h"
#include"Platform.h"
#include"Bullet.h"
#include"particle.h"

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;
extern bool is_debug;

class player
{
public:
	player(bool facing_right = true) : is_facing_right(facing_right)
	{
		// 起跳动画初始化
		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]() {
			is_jump_effect_visible = false;
		});

		// 落地动画初始化
		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]() {
			is_land_effect_visible = false;
		});

		current_animation = is_facing_right ? &animation_idel_right : &animation_idel_left;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() { can_attack = true; });

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() { is_invulnerable = false; });

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]() { 
			// 反转标志位
			is_showing_sketch_frame = !is_showing_sketch_frame;
		});


		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]() {
			Vector2 particle_position;
			IMAGE* frame = atlas_run_effect.get_image(0);
			particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
			particle_position.y = position.y + size.y - frame->getheight();
			particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
		});

		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]() { is_cursor_visible = false; });
	}
	~player() = default;

	virtual void on_update(int delta) {
		// 方向信息
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			// 攻击时不允许左右转向
			if(!is_attacking_ex)
				is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			//  距离与时间相关 使得即使在不同帧率下也能实现相似的移动距离 避免由帧率决定移速
			float distance = direction * run_velocity * delta;  // delta时间内玩家走过的距离(水平方向)
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idel_right : &animation_idel_left;
			timer_run_effect_generation.pause();
		}

		if (is_attacking_ex)
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

		if (hp <= 0)
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;

		current_animation->on_update(delta);
		animation_jump_effect.on_update(delta);
		animation_land_effect.on_update(delta);
		// 普通攻击计时器更新
		timer_attack_cd.on_update(delta);

		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);


		// 死亡时的动效生成
		if (hp <= 0)
			timer_die_effect_generation.on_update(delta);

		// 粒子移除
		particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(),
			[](const Particle& particle) {
				return !particle.check_valid();
			})
			, particle_list.end()
		);

		// 遍历剩余的粒子
		for (Particle& particle : particle_list)
			particle.on_update(delta);

		if (is_showing_sketch_frame)
			sketch_image(current_animation->get_frame(), &img_sketch);


		// 每次update都要 模拟重力下坠
		move_and_collide(delta);
	}

	virtual void on_run(float distance)
	{
		if (is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}

	virtual void on_jump()
	{
		// 简单起见 当竖直方向速度不为0时直接返回 不执行速度的增加
		if (velocity.y != 0 || is_attacking_ex)
			return;

		// 起跳的一瞬间速度立刻达到jump_velocity
		velocity.y += jump_velocity;

		// 起跳动画设置
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_draw(const Camera& camera)
	{
		// 可见时绘制脚底的跃起特效
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if(is_land_effect_visible)
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);

		for (const Particle& particle : particle_list)
			particle.on_draw(camera);

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y);

		// 是否显示头顶的光标
		if (is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2), (int)(position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2), (int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}


		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)position.x, position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
	}

	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable.restart();
	}

	// 落地时的逻辑
	virtual void on_land()
	{
		is_land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_land_effect.get_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 键盘A
				case 0x41:
					is_left_key_down = true;
					break;
					// 键盘D
				case 0x44:
					is_right_key_down = true;
					break;
					// 键盘W 
				case 0x57:
					on_jump();
					break;
					// "F"键按下触发攻击
				case 0x46:
					if (can_attack)
					{
						// 生成了对应的子弹 加入到子弹列表中
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// "G"键按下
				case 0x47:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					is_left_key_down = true;
					break;
				case VK_RIGHT:
					is_right_key_down = true;
					break;
				case VK_UP:
					on_jump();
					break;
				case VK_OEM_PERIOD:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case VK_OEM_2:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 键盘A
				case 0x41:
					is_left_key_down = false;
					break;
					// 键盘D
				case 0x44:
					is_right_key_down = false;
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					is_left_key_down = false;
					break;
				case VK_RIGHT:
					is_right_key_down = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	virtual void on_attack()
	{

	}

	virtual void on_attack_ex()
	{

	}

	int get_hp() const
	{
		return hp;
	}

	void set_hp(int val) 
	{
		hp = val;
	}

	int get_mp() const
	{
		return mp;
	}
protected:
	// 蓝条和血条计数
	int mp = 0;
	int hp = 100;

	Vector2 size;        // 角色的尺寸 用于计算碰撞
	Vector2 position;    // 玩家角色的位置
	Vector2 velocity;    // 玩家的速度向量

	// 玩家可能用到的四种动画
	Animation animation_idel_left;
	Animation animation_idel_right;
	Animation animation_run_left;
	Animation animation_run_right;
	// 特殊攻击动画 左/右
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;

	Animation animation_jump_effect;                       // 跳跃特效动画
	Animation animation_land_effect;                       // 落地特效动画

	Animation animation_die_left;                          // 朝左的死亡动画
	Animation animation_die_right;                         // 朝右的死亡动画
		
	bool is_jump_effect_visible = false;                   // 跳跃动画是否可见
	bool is_land_effect_visible = false;                   // 落地动画是否可见

	Vector2 position_jump_effect;                          // 跳跃动画播放位置
	Vector2 position_land_effect;                          // 落地动画播放位置

	Animation* current_animation = nullptr;

	// 初始化玩家序号
    PlayerID id = PlayerID::P1;

	// 记录玩家当前的键位是否被按下
	bool is_left_key_down = false;
	bool is_right_key_down = false;
	// 记录玩家当前朝向 左/右
	bool is_facing_right = true;
	// 记录玩家当前是否可以发动普通攻击 
	bool can_attack = true;
	// 普通攻击冷却计时器
	Timer timer_attack_cd;
	// 设置攻击的冷却时间
	int attack_cd = 500;
	// 是否正在释放特殊攻击
	bool is_attacking_ex = false;

	// 无敌帧显示与设置
	IMAGE img_sketch;                               // 动画帧剪影图片
 	bool is_invulnerable = false;                   // 角色是否处于无敌状态
	bool is_showing_sketch_frame = false;           // 是否应该显示角色的剪影
	Timer timer_invulnerable;                       // 无敌状态定时器
	Timer timer_invulnerable_blink;                 // 无敌状态闪烁定时器

	std::vector<Particle> particle_list;            // 粒子对象列表
	Timer timer_run_effect_generation;              // 跑动特效粒子发射定时器
	Timer timer_die_effect_generation;              // 死亡特效粒子发射定时器

	// 玩家刚进入游戏时的头顶光标设置
	bool is_cursor_visible = true;                  // 是否可见
	Timer timer_cursor_visibility;                  // 可见性定时器

	// 死亡弹飞动作
	Vector2 last_hurt_direction;                    // 致死攻击的方向 

protected:
	// 玩家的重力
	const float gravity = 1.6e-3f;
	// 玩家水平方向上的移动速度 初始直接设为const固定值
	const float run_velocity = 0.55f;
	// 跳跃速度 向上跳->为负值
	const float jump_velocity = -0.85f;


	// 移动与相交
	// 此方法计算玩家位置与地图线段位置的关系 模拟碰撞效果
	void move_and_collide(int delta)
	{
		float last_velocity_y = velocity.y;

		// 更新垂直方向速度  
		velocity.y += gravity * delta;
		// 更新位置
		position += velocity * (float)delta;

		// 角色死亡后不再计算碰撞
		if (hp <= 0)
			return;

		if (velocity.y > 0)
		{
			// 将玩家与场景内的可碰撞物体逐个比较
			for (const Platform& platform : platform_list)
			{
				// 获取平台的碰撞外形
				const Platform::CollisionShape& shape = platform.shape;
				// 是否在水平方向上发生了碰撞
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
				// 是否在竖直方向上相交
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);
				// 水平和竖直方向上均有重叠说明二者的碰撞模型相交了  
				// 相交了 && 上一帧脚底在平台上 -> 脚底修正为平台表面
				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;   // 玩家上一帧脚底的位置
					// 玩家上一帧脚底的位置 位于 线段之上
					if (last_tick_foot_pos_y <= shape.y)
					{
						// 位置更新为 恰好站在平台上(矩形底部与平台碰撞线重叠)
						position.y = shape.y - size.y;
						velocity.y = 0;  // 竖直方向速度重置为0
						// 发生了碰撞而且y方向速度不为0
						if (last_velocity_y != 0)
							on_land();
						break;
					}
				}
			}
		}

		if (!is_invulnerable)
		{
			// 遍历子弹列表 将玩家与列表中的子弹一一比较判断能否发生碰撞
			for (Bullet* bullet : bullet_list)
			{
				// 子弹无效 或 子弹的目标不是自身  则直接跳过对此子弹的统计
				if (!bullet->get_valid() || bullet->get_collide_target() != id)
					continue;

				if (bullet->check_collision(position, size))
				{
					make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
					// 更新最后一次受击方向
					last_hurt_direction = bullet->get_position() - position;
					if (hp <= 0)
					{
						// 设置起飞的水平和竖直速度
						velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
						velocity.y = -1.0f;
					}
				}
			}
		}

	}
};