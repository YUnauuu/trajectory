#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"
#include<filesystem>


enum compute_method //ʹ��ö������
{
	EUCLIDEAN  //ŷʽ����
};
double result = 0.0;//�������

std::mutex mtx_data_queue;//����ʹ�� data_queue
//std::queue<Data> data_queue;//������ж�ȡ������
std::vector<Data> data_vector;//������ж�ȡ������
std::condition_variable cv_data;//���dataΪ�����������㣬����ȡ�ļ����Ѽ���
//std::atomic<int> files_count(0);//��¼�ļ���
std::atomic<int> read_data_count(0);//��¼��ȡ���ݸ���
//std::atomic<bool> all_file_have_add(false);//����Ƿ������ļ���ȡ�Ѽ��������б�
std::condition_variable cv_have_read_all_data;
int file_count;//�ļ���

//��ȡ�ļ�
void data_function(std::string path)
{
	Data data;
	data.ReadData(path);
	//std::unique_lock<std::mutex> lock(mtx);
	{
		std::unique_lock<std::mutex> lock(mtx_data_queue);
		data_vector.emplace_back(data);
	}
	cv_data.notify_one();
	++read_data_count;
	if (file_count == read_data_count)
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
void allFile(std::string path, int count)
{
	file_count = count;
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
		//++files_count;//�ļ��� +1
		// ����ļ���
		//std::cout << entry.path().filename() << std::endl;
	}

}
int main()
{
	XThreadPool::GetInstance().Init(16);//�̳߳س�ʼ�� 16 �߳�
	//������� ��ȡ�ļ�	
	//XThreadPool::GetInstance().AddTask(data_function, "./data/11296094.gpx");
	//XThreadPool::GetInstance().AddTask(data_function, "./data/11296434.gpx");
	//system("pause");
	
	std::mutex temp_mtx;
	std::unique_lock<std::mutex> lock(temp_mtx);
	allFile("./data/", 17);
	cv_have_read_all_data.wait(lock);
	for (int i = 0; i < file_count; ++i)
	{
		XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, i);
	}
	//cv_have_read_all_data.wait(mtx_data_queue);
	//std::this_thread::sleep_for(std::chrono::seconds(10));
	
	//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, 0);
	//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, 1);
	//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, 2);

	return 0;
}
