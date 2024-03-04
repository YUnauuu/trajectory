#include "xthread_pool.h"
#include<iostream>
#include<chrono>

extern const double threshold_value = 0.5;//�����ֵ
extern std::chrono::steady_clock::time_point start;//��¼��ʼʱ��
extern std::atomic<bool> all_tasks_have_add;//���ȫ����������ӵ��������
extern std::vector<Data> data_vector;//������ж�ȡ������
extern std::vector<std::vector<double>> result_vv;//������飬��ά����

//��ʼ���̳߳� num �߳�����
void XThreadPool::Init(int num)
{
	is_exit_ = false;
	//unique_lock<mutex> lock(tasks_mux_);
	this->thread_num_ = num;
	//std::cout << "Thread pool Init: " << num << std::endl;

	//�����߳� ʹ�� Run() ��Ϊ�߳���ڣ������� Run() ����
	for (int i = 0; i < thread_num_; ++i)
	{
		//auto th = new thread(&XThreadPool::Run, this);
		//auto th = make_shared<thread>(&XThreadPool::Run, this);
		
		threads_.emplace_back(&XThreadPool::Run, this);
	}
}

//�̳߳��˳���������ս��
void XThreadPool::Stop()
{
	//is_exit_ = true;
	cv_.notify_all();
	for (auto& th : threads_)
	{
		//th->join();
		th.join();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();//��¼����ʱ��
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "��ʱ��" << duration.count() / 60 << "��" << duration.count() % 60 << "��" << std::endl;
	double max = result_vv[1][0];
	double min = result_vv[1][0];
	for (int i = 0; i < result_vv.size(); ++i)
	{
		for (int j = i + 1; j < result_vv.size(); ++j)
		{
			//std::cout << data_vector[i].GetName() << " �� " << data_vector[j].GetName() << " ����룺" << result_vv[i][j] << std::endl;
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
			result_vv[j][i] = 1.0 - (result_vv[j][i] - min) / (max - min);//ʹ��Min-Max ��һ��
			//std::cout << data_vector[i].GetName() << " �� " << data_vector[j].GetName() << " ���ƶȣ�" << result_vv[i][j] << std::endl;
			if (result_vv[j][i] > threshold_value);
				//std::cout << data_vector[i].GetName() << " �� " << data_vector[j].GetName() << " ���ƶȣ�" << result_vv[j][i] << std::endl;
		}
	}
	std::unique_lock<std::mutex> lock(tasks_mux_);
	//std::cout << "Stop" << std::endl;
	threads_.clear();
}

//�̳߳� �̵߳���ں����������߳̾�ִ���������ֱ�� is_exit_ = true
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
				cout << "�߳��������" << endl;
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
			std::cerr << "�����쳣: " << e.what() << std::endl;
		}
		//--task_run_count_;
	}
}

//�������,ʹ��ģ�棬bind ���Խ������������
//template<class F, class... Args>
//void XThreadPool::AddTask(F&& f, Args&&... args)
//{
//	{
//		unique_lock<mutex> lock(mux_);
//		std::function<void()> task =
//			std::bind(std::forward<F>(f), std::forward<Args>(args)...); //����������󶨣��������Դ��� void()
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

//��ȡ����
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