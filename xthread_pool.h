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


//�̳߳�
class XThreadPool
{
private:
	//����ģʽ-��ֹ�ⲿ���죬����
	XThreadPool() {};
	XThreadPool(const XThreadPool&) = delete;
	const XThreadPool& operator=(const XThreadPool&) = delete;
	~XThreadPool() { Stop(); }
	//����ģʽ-��ֹ�ⲿ���죬����

	int thread_num_ = 0;//�߳�����
	std::vector<std::thread> threads_;//�߳�
	std::queue<std::function<void()>> tasks_;//�������, װ���� ����
	std::mutex tasks_mux_;//������еĻ�����
	std::mutex std_mtx;//���ʱ�Ļ�����

	std::condition_variable cv_;//��������-���һ������ʱ�����߳�
	std::condition_variable cv_tasks_reach_Max;//��������-�ﵽ��������������ȴ�һ���������
	bool is_exit_ = false;//�̳߳��˳����

	//�������е���������,�̰߳�ȫ
	std::atomic<int> task_run_count_ = { 0 };

	//�̳߳� �̵߳���ں���
	void Run();
public:
	//����ģʽ
	static XThreadPool& GetInstance()
	{
		static XThreadPool xthread_pool;
		return xthread_pool;
	}

	//��ʼ���̳߳� num �߳�����
	void Init(int num);

	//�̳߳��˳���������ս����������һ������ֵ
	void Stop();

	//�������������� tasks_
	template<class F, class... Args>
	void AddTask(F&& f, Args&&... args)
	{
		{
			std::function<void()> task =
				std::bind(std::forward<F>(f), std::forward<Args>(args)...); //����������󶨣��������Դ��� void()
			std::unique_lock<std::mutex> lock(tasks_mux_);
			tasks_.emplace(move(task));
		}
		//tasks_->is_exit = [this] {return is_exit(); };
		//lock.unlock();
		cv_.notify_one();
	}
	//void AddTask(std::shared_ptr<XTask> task);

	//��������� tasks_ ��ȡ����
	std::function<void()> GetTask();

	//�����̳߳��˳����
	bool is_exit() { return is_exit_; }

	//�����������е���������
	int task_run_count() { return task_run_count_; }
	
};

