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


//�̳߳�
class XThreadPool
{
public:
	//��ʼ���̳߳�
	//@para num �߳�����
	void Init(int num);

	//���������̣߳����ȵ���Init
	//void Start();

	//�̳߳��˳�
	void Stop();

	//�������
	//void AddTask(XTask* task);
	void AddTask(std::shared_ptr<XTask> task);


	//��ȡ����
	//XTask* GetTask();
	std::shared_ptr<XTask> GetTask();

	//�����̳߳��˳����
	bool is_exit() { return is_exit_; }

	//�����������е���������
	int task_run_count() { return task_run_count_; }
	~XThreadPool() { Stop(); }
private:
	int thread_num_ = 0;//�߳�����
	//std::vector<std::thread*> threads_;//�߳�
	std::vector<std::shared_ptr<std::thread>> threads_;//�߳�����ָ��
	std::mutex mux_;//������
	//std::list<XTask*> tasks_;//�����б�
	std::list<std::shared_ptr<XTask>> tasks_;//�����б�
	std::condition_variable cv_;//�ź���-���һ������ʱ����һ���߳�
	std::condition_variable cv_task_maxNum_;//�ź���-�ﵽ��������������ȴ�һ���������
	bool is_exit_ = false;//�̳߳��˳����

	//�������е���������,�̰߳�ȫ
	std::atomic<int> task_run_count_ = { 0 };

	//�̳߳� �̵߳���ں���
	void Run();

};

