#pragma once
#include<thread>
#include<mutex>
#include<vector>
#include<list>
#include<tuple>
#include<condition_variable>
#include<functional>
#include<atomic>
#include<future>
#include"xtask.h"


//线程池
class XThreadPool
{
public:
	//初始化线程池
	//@para num 线程数量
	void Init(int num);

	//启动所有线程，请先调用Init
	//void Start();

	//线程池退出
	void Stop();

	//添加任务
	//void AddTask(XTask* task);
	void AddTask(std::shared_ptr<XTask> task);


	//获取任务
	//XTask* GetTask();
	std::shared_ptr<XTask> GetTask();

	//返回线程池退出标记
	bool is_exit() { return is_exit_; }

	//返回正在运行的任务数量
	int task_run_count() { return task_run_count_; }
	~XThreadPool() { Stop(); }
private:
	int thread_num_ = 0;//线程数量
	//std::vector<std::thread*> threads_;//线程
	std::vector<std::shared_ptr<std::thread>> threads_;//线程智能指针
	std::mutex mux_;//互斥锁
	//std::list<XTask*> tasks_;//任务列表
	std::list<std::shared_ptr<XTask>> tasks_;//任务列表
	std::condition_variable cv_;//信号量-添加一个任务时唤醒一个线程
	std::condition_variable cv_task_maxNum_;//信号量-达到最大任务数量，等待一个任务结束
	bool is_exit_ = false;//线程池退出标记

	//正在运行的任务数量,线程安全
	std::atomic<int> task_run_count_ = { 0 };

	//线程池 线程的入口函数
	void Run();

};

