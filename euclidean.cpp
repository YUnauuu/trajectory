#include"euclidean.h"
#include<queue>
#include<chrono>

extern std::mutex mtx_data_queue;//����ʹ�� data_queue
//extern std::queue<Data> data_queue;//������ж�ȡ������
extern std::vector<Data> data_vector;//������ж�ȡ������
extern std::condition_variable cv_data;//���dataΪ�����������㣬����ȡ�ļ����Ѽ���
extern std::atomic<int> files_count;//��¼�ļ���
extern std::atomic<bool> all_file_have_add;//����Ƿ������ļ���ȡ�Ѽ��������б�

void Euclidean::compute(Data& data2)
{
	//auto start = std::chrono::steady_clock::now();
	
	double result1 = 0.0;
	double gap = 100000.0;
	for (int i = 0; i < data.GetData().size(); ++i)
	{
		gap = 100000.0;
		for (int j = 0; j < data2.GetData().size(); ++j)
		{
			double temp_gap = (std::get<0>(data.GetData()[i]) - std::get<0>(data2.GetData()[j])) * (std::get<0>(data.GetData()[i]) - std::get<0>(data2.GetData()[j])) +
				(std::get<1>(data.GetData()[i]) - std::get<1>(data2.GetData()[j])) * (std::get<1>(data.GetData()[i]) - std::get<1>(data2.GetData()[j]));
			if (temp_gap < gap)
			{
				gap = temp_gap;
			}
		}
		result += gap;
		//std::cout << result << std::endl;
	}
	
	for (int i = 0; i < data2.GetData().size(); ++i)
	{
		gap = 100000.0;
		for (int j = 0; j < data.GetData().size(); ++j)
		{
			double temp_gap = (std::get<0>(data.GetData()[j]) - std::get<0>(data2.GetData()[i])) * (std::get<0>(data.GetData()[j]) - std::get<0>(data2.GetData()[i])) +
				(std::get<1>(data.GetData()[j]) - std::get<1>(data2.GetData()[i])) * (std::get<1>(data.GetData()[j]) - std::get<1>(data2.GetData()[i]));
			if (temp_gap < gap)
			{
				gap = temp_gap;
			}
		}
		result += gap;
	}

	result /= 2.0;
	result_vector.emplace_back(result);
	//std::cout << data.GetName() << " �� " << data2.GetName() << " ����� = " << result << std::endl;
	/*auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "��ʱ��" << duration.count() << "s" << std::endl;*/
	//this->SetValue(result);
}
//����data_queue �е�rankλ������
Euclidean::Euclidean(int ran)
{
	this->rank = ran;
	{	//�����ȴ� data_vector �д洢�����ݵĸ��� > rank
		std::unique_lock<std::mutex> lock(mtx_data_queue);
		cv_data.wait(lock, [&] {return data_vector.size() > rank; });
		data = data_vector[rank];
	}
	
	Data data2;
	for (int i = rank + 1; i < data_vector.size(); ++i)
	{
		compute(data_vector[i]);
	}
	for (int i = 0; i < result_vector.size(); ++i)
	{
		std::cout << data.GetName() << " �� " << data_vector[rank + 1 + i].GetName() << " ����� = " << result_vector[i] << std::endl;
	}
}