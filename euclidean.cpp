#include"euclidean.h"
#include<queue>


//extern std::mutex mtx_data_vector;//����ʹ�� data_vector
extern std::vector<Data> data_vector;//������ж�ȡ������
extern std::mutex mtx_result_vv;//����ʹ�� result_vv
extern std::vector<std::vector<double>> result_vv;//������飬��ά����

//extern std::condition_variable cv_data;//���dataΪ�����������㣬����ȡ�ļ����Ѽ���
//extern std::atomic<int> files_count;//��¼�ļ���
//extern std::atomic<bool> all_file_have_add;//����Ƿ������ļ���ȡ�Ѽ��������б�

//void Euclidean::compute(int i, int j)
//{
	//auto start = std::chrono::steady_clock::now();
	//double result_double = 0.0;
	//double gap = 100000.0;
	//for (int i = 0; i < data.GetData().size(); ++i)
	//{
	//	gap = 100000.0;
	//	for (int j = 0; j < data2.GetData().size(); ++j)
	//	{
	//		double temp_gap = (std::get<0>(data.GetData()[i]) - std::get<0>(data2.GetData()[j])) * (std::get<0>(data.GetData()[i]) - std::get<0>(data2.GetData()[j])) +
	//			(std::get<1>(data.GetData()[i]) - std::get<1>(data2.GetData()[j])) * (std::get<1>(data.GetData()[i]) - std::get<1>(data2.GetData()[j]));
	//		if (temp_gap < gap)
	//		{
	//			gap = temp_gap;
	//		}
	//	}
	//	result_double += gap;
	//	//std::cout << result << std::endl;
	//}
	//
	//for (int i = 0; i < data2.GetData().size(); ++i)
	//{
	//	gap = 100000.0;
	//	for (int j = 0; j < data.GetData().size(); ++j)
	//	{
	//		double temp_gap = (std::get<0>(data.GetData()[j]) - std::get<0>(data2.GetData()[i])) * (std::get<0>(data.GetData()[j]) - std::get<0>(data2.GetData()[i])) +
	//			(std::get<1>(data.GetData()[j]) - std::get<1>(data2.GetData()[i])) * (std::get<1>(data.GetData()[j]) - std::get<1>(data2.GetData()[i]));
	//		if (temp_gap < gap)
	//		{
	//			gap = temp_gap;
	//		}
	//	}
	//	result_double += gap;
	//}
	//result_double = result_double / (data.GetData().size() + data2.GetData().size());
	//{
	//	std::unique_lock<std::mutex> lock(mtx_result_vv);
	//	result_vv[rank][i] = result_double;
	//	result_vv[i][rank] = result_double;
	//}
	//std::cout << data.GetName() << " �� " << data2.GetName() << " ����� = " << result << std::endl;
	/*auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "��ʱ��" << duration.count() << "s" << std::endl;*/
	//this->SetValue(result);
//}
//����data_queue �е�rankλ������
Euclidean::Euclidean(int i, int j)
{
	//this->rank = ran;
	//{	//�����ȴ� data_vector �д洢�����ݵĸ��� > rank
	//	std::unique_lock<std::mutex> lock(mtx_data_vector);
	//	cv_data.wait(lock, [&] {return data_vector.size() > rank; });
	//	data = data_vector[rank];
	//}
	//
	//Data data2;
	//for (int i = rank + 1; i < data_vector.size(); ++i)
	//{
	//	std::cout << ". ";
	//	compute(rank, i);
	//}
	/*for (int i = 0; i < result_vector.size(); ++i)
	{
		std::cout << data.GetName() << " �� " << data_vector[rank + 1 + i].GetName() << " ����� = " << result_vector[i] << std::endl;
	}*/
	Data data1 = data_vector[i];//���̶߳������̰߳�ȫ��
	Data data2 = data_vector[j];//���̶߳������̰߳�ȫ��
	if (data_vector[i].GetData().size() > data_vector[j].GetData().size())
	{
		data1 = data_vector[j];//���̶߳������̰߳�ȫ��
		data2 = data_vector[i];//���̶߳������̰߳�ȫ��
	}
	size_t n = data1.GetData().size();
	size_t m = data2.GetData().size();//n <= m


	std::function<double(int)> sum_function = [&](int j)->double
	{
		double sum_result = 0.0;
		for (int i = 0; i < n; ++i)
		{
			sum_result += (std::get<0>(data1.GetData()[i]) - std::get<0>(data2.GetData()[i + j])) * (std::get<0>(data1.GetData()[i]) - std::get<0>(data2.GetData()[i + j])) +
				(std::get<1>(data1.GetData()[i]) - std::get<1>(data2.GetData()[i + j])) * (std::get<1>(data1.GetData()[i]) - std::get<1>(data2.GetData()[i + j]));
		}
		return sum_result;
	};

	double min_result = sum_function(0);
	for (int j = 0; j < m - n; ++j)
	{
		double sum = sum_function(j);
		if (min_result > sum)
		{
			min_result = sum;
		}
	}
	min_result /= n;
	{
		std::unique_lock<std::mutex> lock(mtx_result_vv);
		result_vv[i][j] = min_result;
		result_vv[j][i] = min_result;
	}
}