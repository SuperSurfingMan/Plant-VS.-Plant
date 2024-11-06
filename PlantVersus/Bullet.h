#pragma once
#include"Vector2.h"
#include"functional"
#include"PlayerID.h"
#include<graphics.h>
#include"Camera.h"

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	int get_damage() const 
	{
		return damage;
	}

	void set_damage(int damage)
	{
		this->damage = damage;
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

	void set_velocity(float x, float y) 
	{
		velocity.x = x;
		velocity.y = y;
	}

	PlayerID get_collide_target() const
	{
		return target_id;
	}

	void set_collide_target(PlayerID p_id)
	{
		target_id = p_id;
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	void set_valid(bool flag)
	{
		valid = flag;
	}

	bool get_valid() const
	{
		return valid;
	}

	bool check_can_remove() const
	{
		return can_remove;
	}

	virtual void on_collide()
	{
		if (callback)
			callback();
	}
	// 碰撞检测
	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta)
	{

	}

	virtual void on_draw(const Camera& camera) const
	{
		if (is_debug)
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}

protected:
	// 子弹的尺寸
	Vector2 size;
	// 子弹的位置
	Vector2 position;
	// 子弹的速度
	Vector2 velocity;
	// 子弹的伤害
	int damage = 10;
	// 子弹是否有效
	bool valid = true;
	// 子弹是否可以被删除
	bool can_remove;
	// 子弹破碎后的回调函数
	std::function<void()> callback;
	// 子弹可以伤害的玩家编号
	PlayerID target_id = PlayerID::P1;
	// 检测子弹位置是否已经超出屏幕范围
	bool check_if_exceeds_screen() const
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			|| position.y + size.y <= 0 || position.y >= getheight());
	}
};
