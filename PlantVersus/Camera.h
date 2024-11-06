#pragma once
#include"Vector2.h"
#include"Timer.h"
class Camera
{
public:
	Camera() {
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]() {
			is_shaking = false;
			reset();
		});
	}
	~Camera() = default;

	void set_size(const Vector2 size) {
		this->size = size;
	}

	const Vector2& get_size() const {
		return size;
	}

	void set_position(const Vector2 position) {
		this->position = position;
	}

	const Vector2& get_position() const {
		return position;
	}

	void look_at(const Vector2& target) {
		position = target - size / 2.0f;
	}

	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		// 更新计时器
		timer_shake.on_update(delta);
		if (is_shaking)
		{
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength;
		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}
private:
	Vector2 size;
	// 自己的位置
	Vector2 position;
	// 摄像机抖动定时器
	Timer timer_shake;
	// 是否正在抖动
	bool is_shaking = false;
	// 摄像机的抖动幅度
	float shaking_strength = 0;

};