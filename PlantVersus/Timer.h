#pragma once
#include<functional>
class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void on_update(float delta)
	{
		if (paused) return;
		// 已经过去的时间
		pass_time += delta;

		if (pass_time >= wait_time)
		{
			bool can_shot = (!one_shot || (one_shot && !shotted));
			shotted = true;
			if (can_shot && callback)
				callback();
			pass_time -= wait_time;
		}
	}

	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(float val)
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}


	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

private:
	// 回调函数
	std::function<void()> callback;
	// 从上一次更新后经过的时间
	float pass_time = 0;
	// 设定的等待时间
	float wait_time = 0;
	// 是否暂停
	bool paused = false;
	// 是否触发了
	bool shotted = false;
	// 是否只能触发一次
	bool one_shot = false;

};