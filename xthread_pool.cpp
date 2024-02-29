#include "xthread_pool.h"
#include<iostream>
using namespace std;


//初始化线程池 num 线程数量
void XThreadPool::Init(int num)
{
	is_exit_ = false;
	//unique_lock<mutex> lock(tasks_mux_);
	this->thread_num_ = num;
	cout << "Thread pool Init: " << num << endl;

	//开启线程 使用 Run() 作为线程入口，即运行 Run() 函数
	for (int i = 0; i < thread_num_; ++i)
	{
		//auto th = new thread(&XThreadPool::Run, this);
		//auto th = make_shared<thread>(&XThreadPool::Run, this);
		
		threads_.emplace_back(&XThreadPool::Run, this);
	}
}
//void XThreadPool::Init(int num)
//{
//	is_exit_ = false;
//	unique_lock<mutex> lock(mux_);
//	this->thread_num_ = num;
//	cout << "Thread pool Init: " << num << endl;
//
//	//启动线程
//	for (int i = 0; i < thread_num_; ++i)
//	{
//		//auto th = new thread(&XThreadPool::Run, this);
//		auto th = make_shared<thread>(&XThreadPool::Run, this);
//		threads_.emplace_back(th);
//	}
//}

//线程池退出
void XThreadPool::Stop()
{
	//is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_)
	{
		//th->join();
		th.join();
	}
	
	unique_lock<mutex> lock(tasks_mux_);
	cout << "Stop" << endl;
	threads_.clear();
}

//线程池 线程的入口函数，开启线程就执行这个函数直到 is_exit_ = true
void XThreadPool::Run()
{
	//cout << this_thread::get_id << endl;
	//cout << tasks_.size() << endl;
	//cout << is_exit_ << endl;
	while (!is_exit())
	{
		auto task = GetTask();
		if (!task) continue;
		++task_run_count_;
		/*{
			std::unique_lock<std::mutex> lock(std_mtx);
			std::cout << this->task_run_count() << std::endl;
		}*/
		/*{
			unique_lock<mutex> lock(mux_);
			if (task_run_count() == thread_num_)
			{
				cv_task_maxNum_.wait(lock);
				cout << "线程数达最大" << endl;
			}
		}*/
		try
		{
			task();
			//task->SetValue(re);
		}
		//catch (...)
		catch(const std::exception& e)
		{
			//std::cout <<  std::endl;
			std::cerr << "捕获到异常: " << e.what() << std::endl;
		}
		--task_run_count_;
	}
}

//添加任务,使用模版，bind 可以将函数与参数绑定
//template<class F, class... Args>
//void XThreadPool::AddTask(F&& f, Args&&... args)
//{
//	{
//		unique_lock<mutex> lock(mux_);
//		std::function<void()> task =
//			std::bind(std::forward<F>(f), std::forward<Args>(args)...); //函数与参数绑定，这样可以传入 void()
//		tasks_.emplace(move(task));
//	}
//	//tasks_->is_exit = [this] {return is_exit(); };
//	lock.unlock();
//	cv_.notify_one();
//}
//void XThreadPool::AddTask(std::shared_ptr<XTask> task)
//{
//	unique_lock<mutex> lock(mux_);
//	tasks_.emplace(task);
//	task->is_exit = [this] {return is_exit(); };
//	lock.unlock();
//	cv_.notify_one();
//}

//获取任务
std::function<void()> XThreadPool::GetTask()
{
	unique_lock<mutex> lock(tasks_mux_);
	if (tasks_.empty())
	{
		cv_.wait(lock);
	}

	if (tasks_.empty())
	{
		return nullptr;
	}
	std::function<void()> task = move(tasks_.front());
	tasks_.pop();
	return task;
}