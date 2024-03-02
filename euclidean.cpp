#include"euclidean.h"
#include<queue>


extern std::mutex mtx_data_vector;//����ʹ�� data_queue
//extern std::queue<Data> data_queue;//������ж�ȡ������
extern std::vector<Data> data_vector;//������ж�ȡ������
extern std::mutex mtx_result_vv;//����ʹ�� result_vv
extern std::vector<std::vector<double>> result_vv;//������飬��ά����

extern std::condition_variable cv_data;//���dataΪ�����������㣬����ȡ�ļ����Ѽ���
extern std::atomic<int> files_count;//��¼�ļ���
extern std::atomic<bool> all_file_have_add;//����Ƿ������ļ���ȡ�Ѽ��������б�

void Euclidean::compute(int i)
{
	//auto start = std::chrono::steady_clock::now();
	Data data2 = data_vector[i];//���̶߳������̰߳�ȫ��
	double result_double = 0.0;
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
		result_double += gap;
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
		result_double += gap;
	}

	result_double /= 2.0;
	//result_vector.emplace_back(result_double);
	{
		std::unique_lock<std::mutex> lock(mtx_result_vv);
		result_vv[rank][i] = result_double;
		result_vv[i][rank] = result_double;
	}
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
		std::unique_lock<std::mutex> lock(mtx_data_vector);
		cv_data.wait(lock, [&] {return data_vector.size() > rank; });
		data = data_vector[rank];
	}
	
	Data data2;
	for (int i = rank + 1; i < data_vector.size(); ++i)
	{
		std::cout << ". ";
		compute(i);
	}
	/*for (int i = 0; i < result_vector.size(); ++i)
	{
		std::cout << data.GetName() << " �� " << data_vector[rank + 1 + i].GetName() << " ����� = " << result_vector[i] << std::endl;
	}*/
}