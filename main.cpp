#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"
#include<filesystem>


enum compute_method //使用枚举类型
{
	EUCLIDEAN  //欧式距离
};
double result = 0.0;//结果数组

std::mutex mtx_data_queue;//互斥使用 data_queue
//std::queue<Data> data_queue;//存放所有读取的数据
std::vector<Data> data_vector;//存放所有读取的数据
std::condition_variable cv_data;//如果data为空则阻塞计算，当读取文件后唤醒计算
//std::atomic<int> files_count(0);//记录文件数
std::atomic<int> read_data_count(0);//记录读取数据个数
//std::atomic<bool> all_file_have_add(false);//标记是否所有文件读取已加入任务列表
std::condition_variable cv_have_read_all_data;
int file_count;//文件数

//读取文件
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

//计算, m 枚举类型的计算方法，rank 在data_queue 中的排位
void compute_function(compute_method m, int rank)
{
	switch (m)
	{
	case EUCLIDEAN:
		Euclidean eu(rank);
		
		break;

	}

}
//读取一个文件夹下的所有文件
void allFile(std::string path, int count)
{
	file_count = count;
	std::string folder_path = path;
	// 检查文件夹是否存在  
	if (!std::filesystem::exists(folder_path)) {
		std::cerr << "Folder does not exist!" << std::endl;
		return;
	}
	// 检查是否是一个文件夹  
	if (!std::filesystem::is_directory(folder_path)) {
		std::cerr << "Not a folder!" << std::endl;
		return;
	}
	// 遍历文件夹中的所有文件
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		// 跳过子文件夹  
		if (entry.is_directory()) {
			continue;
		}
		//添加读取文件任务，并添加计算任务
		
		XThreadPool::GetInstance().AddTask(data_function, entry.path().string());
		//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, static_cast<int>(files_count));
		//++files_count;//文件数 +1
		// 输出文件名
		//std::cout << entry.path().filename() << std::endl;
	}

}
int main()
{
	XThreadPool::GetInstance().Init(16);//线程池初始化 16 线程
	//添加任务 读取文件	
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
