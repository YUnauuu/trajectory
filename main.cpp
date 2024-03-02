#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"
#include<filesystem>
#include<chrono>


enum compute_method //ʹ��ö������
{
	EUCLIDEAN  //ŷʽ����
};

const double threshold_value = 0.5;//�����ֵ
std::chrono::steady_clock::time_point start;//��¼��ʼʱ��

std::mutex mtx_data_vector;//����ʹ�� data_queue
//std::queue<Data> data_queue;//������ж�ȡ������
std::vector<Data> data_vector;//������ж�ȡ������
std::mutex mtx_result_vv;//����ʹ�� result_vv
std::vector<std::vector<double>> result_vv;//������飬��ά����

std::condition_variable cv_data;//���dataΪ�����������㣬����ȡ�ļ����Ѽ���
std::atomic<int> files_count(0);//��¼���ļ���
std::atomic<int> read_data_count(0);//��¼�Ѷ�ȡ���ݸ���
//std::atomic<bool> all_file_have_add(false);//����Ƿ������ļ���ȡ�Ѽ��������б�
std::condition_variable cv_have_read_all_data;//�������� �Ѿ�ͨ�� data_function ������������
std::atomic<bool> files_count_get(false);//�Ƿ��ȡ�ļ����� files_count
std::atomic<bool> all_tasks_have_add(false);//���ȫ����������ӵ��������
//int file_count;//�ļ���

//��ȡ�ļ�
void data_function(std::string path)
{
	Data data;
	data.ReadData(path);
	//std::unique_lock<std::mutex> lock(mtx);
	{
		std::unique_lock<std::mutex> lock(mtx_data_vector);
		data_vector.emplace_back(data);
	}
	cv_data.notify_one();
	++read_data_count;
	if (files_count_get && files_count == read_data_count)
		cv_have_read_all_data.notify_one();
	
	/*Data data2 = data;

	auto d = data2.GetData();
	for (int i = 0; i < d.size(); ++i)
	{
		std::cout << std::get<0>(d[i]) << " , " << std::get<1>(d[i]) << std::endl;
	}
	std::cout << data2.GetName() << std::endl;*/
}

//����, m ö�����͵ļ��㷽����rank ��data_queue �е���λ
void compute_function(compute_method m, int rank)
{
	switch (m)
	{
	case EUCLIDEAN:
		Euclidean eu(rank);
		
		break;

	}

}
//��ȡһ���ļ����µ������ļ�
void allFile(std::string path)
{
	std::string folder_path = path;
	// ����ļ����Ƿ����  
	if (!std::filesystem::exists(folder_path)) {
		std::cerr << "Folder does not exist!" << std::endl;
		return;
	}
	// ����Ƿ���һ���ļ���  
	if (!std::filesystem::is_directory(folder_path)) {
		std::cerr << "Not a folder!" << std::endl;
		return;
	}
	// �����ļ����е������ļ�
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		// �������ļ���  
		if (entry.is_directory()) {
			continue;
		}
		//��Ӷ�ȡ�ļ����񣬲���Ӽ�������
		
		XThreadPool::GetInstance().AddTask(data_function, entry.path().string());
		//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, static_cast<int>(files_count));
		++files_count;//�ļ��� +1
		// ����ļ���
		//std::cout << entry.path().filename() << std::endl;
	}
	//���ý����ά�����С
	result_vv.resize(files_count);
	for (auto& vec : result_vv)
	{
		vec.resize(files_count);
	}
	files_count_get.store(true);
	std::mutex temp_mtx;
	std::unique_lock<std::mutex> lock(temp_mtx);
	cv_have_read_all_data.wait(lock);
	for (int i = 0; i < files_count; ++i)
	{
		XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, i);
	}
	all_tasks_have_add.store(true);//ȫ����������ӵ��������
}
int main()
{
	start = std::chrono::steady_clock::now();//��¼��ʼʱ��
	
	XThreadPool::GetInstance().Init(16);//�̳߳س�ʼ�� 16 �߳�
	//������� ��ȡ�ļ�	
	//XThreadPool::GetInstance().AddTask(data_function, "./data/11296094.gpx");
	//XThreadPool::GetInstance().AddTask(data_function, "./data/11296434.gpx");
	//system("pause");
	
	
	allFile("./data/");
	

	return 0;
}
