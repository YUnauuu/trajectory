#include "xthread_pool.h"
#include<iostream>
#include<chrono>

extern const double threshold_value = 0.5;//输出阈值
extern std::chrono::steady_clock::time_point start;//记录开始时间
extern std::atomic<bool> all_tasks_have_add;//标记全部任务已添加到任务队列
extern std::vector<Data> data_vector;//存放所有读取的数据
extern std::vector<std::vector<double>> result_vv;//结果数组，二维数组

//初始化线程池 num 线程数量
void XThreadPool::Init(int num)
{
	is_exit_ = false;
	//unique_lock<mutex> lock(tasks_mux_);
	this->thread_num_ = num;
	//std::cout << "Thread pool Init: " << num << std::endl;

	//开启线程 使用 Run() 作为线程入口，即运行 Run() 函数
	for (int i = 0; i < thread_num_; ++i)
	{
		//auto th = new thread(&XThreadPool::Run, this);
		//auto th = make_shared<thread>(&XThreadPool::Run, this);
		
		threads_.emplace_back(&XThreadPool::Run, this);
	}
}

//线程池退出，输出最终结果
void XThreadPool::Stop()
{
	//is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_)
	{
		//th->join();
		th.join();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();//记录结束时间
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "用时：" << duration.count() / 60 << "分" << duration.count() % 60 << "秒" << std::endl;
	double max = result_vv[1][0];
	double min = result_vv[1][0];
	for (int i = 0; i < result_vv.size(); ++i)
	{
		for (int j = i + 1; j < result_vv.size(); ++j)
		{
			//std::cout << data_vector[i].GetName() << " 与 " << data_vector[j].GetName() << " 间距离：" << result_vv[i][j] << std::endl;
			if (max < result_vv[j][i])
				max = result_vv[j][i];
			if (min > result_vv[j][i])
				min = result_vv[j][i];
		}
	}
	for (int i = 0; i < result_vv.size(); ++i)
	{
		for (int j = i + 1; j < result_vv.size(); ++j)
		{
			result_vv[j][i] = 1.0 - (result_vv[j][i] - min) / (max - min);//使用Min-Max 归一化
			//std::cout << data_vector[i].GetName() << " 与 " << data_vector[j].GetName() << " 相似度：" << result_vv[i][j] << std::endl;
			if (result_vv[j][i] > threshold_value);
				//std::cout << data_vector[i].GetName() << " 与 " << data_vector[j].GetName() << " 相似度：" << result_vv[j][i] << std::endl;
		}
	}
	std::unique_lock<std::mutex> lock(tasks_mux_);
	//std::cout << "Stop" << std::endl;
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
		//++task_run_count_;
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
		//--task_run_count_;
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
	std::unique_lock<std::mutex> lock(tasks_mux_);
	if (tasks_.empty())
	{
		if (all_tasks_have_add)
		{
			is_exit_ = true;
			return nullptr;
		}
		else
		{
			cv_.wait(lock);
		}
	}

	if (tasks_.empty())
	{
		return nullptr;
	}
	std::function<void()> task = move(tasks_.front());
	tasks_.pop();
	return task;
}