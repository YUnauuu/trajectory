#include"euclidean.h"
#include<queue>
#include<chrono>

extern std::mutex mtx_data_queue;//互斥使用 data_queue
//extern std::queue<Data> data_queue;//存放所有读取的数据
extern std::vector<Data> data_vector;//存放所有读取的数据
extern std::condition_variable cv_data;//如果data为空则阻塞计算，当读取文件后唤醒计算
extern std::atomic<int> files_count;//记录文件数
extern std::atomic<bool> all_file_have_add;//标记是否所有文件读取已加入任务列表

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
	//std::cout << data.GetName() << " 与 " << data2.GetName() << " 间距离 = " << result << std::endl;
	/*auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "用时：" << duration.count() << "s" << std::endl;*/
	//this->SetValue(result);
}
//计算data_queue 中第rank位的数据
Euclidean::Euclidean(int ran)
{
	this->rank = ran;
	{	//阻塞等待 data_vector 中存储的数据的个数 > rank
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
		std::cout << data.GetName() << " 与 " << data_vector[rank + 1 + i].GetName() << " 间距离 = " << result_vector[i] << std::endl;
	}
}