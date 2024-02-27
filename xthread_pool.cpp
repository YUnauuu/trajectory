#include "xthread_pool.h"
#include<iostream>
using namespace std;


//��ʼ���̳߳�
//@para num �߳�����
void XThreadPool::Init(int num)
{
	unique_lock<mutex> lock(mux_);
	this->thread_num_ = num;
	cout << "Thread pool Init: " << num << endl;

	//�����߳�
	for (int i = 0; i < thread_num_; ++i)
	{
		//auto th = new thread(&XThreadPool::Run, this);
		auto th = make_shared<thread>(&XThreadPool::Run, this);
		threads_.push_back(th);
	}
}

//�̳߳��˳�
void XThreadPool::Stop()
{
	is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_)
	{
		th->join();
	}

	unique_lock<mutex> lock(mux_);
	threads_.clear();
}

//�̳߳� �̵߳���ں���
void XThreadPool::Run()
{
	while (!is_exit())
	{
		
		
		auto task = GetTask();
		if (!task) continue;
		++task_run_count_;
		/*{
			unique_lock<mutex> lock(mux_);
			if (task_run_count() == thread_num_)
			{
				cv_task_maxNum_.wait(lock);
				cout << "�߳��������" << endl;
			}
		}*/
		try
		{
			auto re = task->compute();
			//task->SetValue(re);
		}
		catch (...)
		{

		}
		--task_run_count_;
	}
}

//�������
void XThreadPool::AddTask(std::shared_ptr<XTask> task)
{
	unique_lock<mutex> lock(mux_);
	tasks_.push_back(task);
	task->is_exit = [this] {return is_exit(); };
	lock.unlock();
	cv_.notify_one();
}

//��ȡ����
std::shared_ptr<XTask> XThreadPool::GetTask()
{
	unique_lock<mutex> lock(mux_);
	if (tasks_.empty())
	{
		cv_.wait(lock);
	}
	if (is_exit())
		return nullptr;
	if (tasks_.empty())
	{
		return nullptr;
	}
	auto task = tasks_.front();
	tasks_.pop_front();
	cv_task_maxNum_.notify_one();
	return task;
}