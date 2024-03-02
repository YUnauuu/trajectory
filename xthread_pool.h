#pragma once
#include<thread>
#include<mutex>
#include<vector>
#include<queue>
#include<condition_variable>
#include<functional>
#include<atomic>
#include<future>
#include"method.h"


//线程池
class XThreadPool
{
private:
	//单例模式-禁止外部构造，析构
	XThreadPool() {};
	XThreadPool(const XThreadPool&) = delete;
	const XThreadPool& operator=(const XThreadPool&) = delete;
	~XThreadPool() { Stop(); }
	//单例模式-禁止外部构造，析构

	int thread_num_ = 0;//线程数量
	std::vector<std::thread> threads_;//线程
	std::queue<std::function<void()>> tasks_;//任务队列, 装的是 函数
	std::mutex tasks_mux_;//任务队列的互斥锁
	std::mutex std_mtx;//输出时的互斥锁

	std::condition_variable cv_;//条件变量-添加一个任务时唤醒线程
	std::condition_variable cv_tasks_reach_Max;//条件变量-达到最大任务数量，等待一个任务结束
	bool is_exit_ = false;//线程池退出标记

	//正在运行的任务数量,线程安全
	std::atomic<int> task_run_count_ = { 0 };

	//线程池 线程的入口函数
	void Run();
public:
	//单例模式
	static XThreadPool& GetInstance()
	{
		static XThreadPool xthread_pool;
		return xthread_pool;
	}

	//初始化线程池 num 线程数量
	void Init(int num);

	//线程池退出，输出最终结果，包含归一化、阈值
	void Stop();

	//添加任务到任务队列 tasks_
	template<class F, class... Args>
	void AddTask(F&& f, Args&&... args)
	{
		{
			std::function<void()> task =
				std::bind(std::forward<F>(f), std::forward<Args>(args)...); //函数与参数绑定，这样可以传入 void()
			std::unique_lock<std::mutex> lock(tasks_mux_);
			tasks_.emplace(move(task));
		}
		//tasks_->is_exit = [this] {return is_exit(); };
		//lock.unlock();
		cv_.notify_one();
	}
	//void AddTask(std::shared_ptr<XTask> task);

	//从任务队列 tasks_ 获取任务
	std::function<void()> GetTask();

	//返回线程池退出标记
	bool is_exit() { return is_exit_; }

	//返回正在运行的任务数量
	int task_run_count() { return task_run_count_; }
	
};

