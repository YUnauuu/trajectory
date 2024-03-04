#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"
#include<filesystem>
#include<chrono>
#include<future>
#include<sstream>
#include<fstream>

enum compute_method //使用枚举类型
{
	EUCLIDEAN  //欧式距离
};

const double threshold_value = 0.5;//输出阈值
std::chrono::steady_clock::time_point start;//记录开始时间


std::vector<Data> data_vector;//存放所有读取的数据
std::mutex mtx_result_vv;//互斥使用 result_vv
std::vector<std::vector<double>> result_vv;//结果数组，二维数组，注意是左下三角 查找[i][j] i>j

std::atomic<bool> all_tasks_have_add(false);//标记全部任务已添加到任务队列

//读取文件
void data_function(std::vector<std::string>& str_vec, int& fileNum)
//void data_function(std::shared_ptr<std::vector<std::string>> strP, int& fileNum)
{
	/*std::promise<int> pro;
	pro.set_value(0);*/
	std::atomic<int> num(0);//已读数据个数
	std::condition_variable cv;
	std::mutex mtx_data_vector;//互斥使用 data_vector
	std::function<void(std::string)> data_read_function= [&](std::string path)
	{
		Data data;
		data.ReadData(path);
		{
			std::unique_lock<std::mutex> lock(mtx_data_vector);
			data_vector.emplace_back(data);
		}
		++num;
		cv.notify_one();
	};
	
	for (std::string& path : str_vec)
	{
		XThreadPool::GetInstance().AddTask(data_read_function, path);
	}
	{
		std::mutex m;
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, [&] {return fileNum == num; });
	}

}

//计算, m 枚举类型的计算方法，rank 在data_queue 中的排位
void compute_function(compute_method m, int i, int j)
{
	switch (m)
	{
	case EUCLIDEAN:
		Euclidean eu(i, j);
		
		break;

	}

}
//读取一个文件夹下的所有文件
void allFile(std::string path)
{
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
	int fileNum = 0;//记录文件数
	std::vector<std::string> strVec;//保存文件路径
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		// 跳过子文件夹  
		if (entry.is_directory()) {
			continue;
		}
		++fileNum;//文件数 +1
		strVec.emplace_back(entry.path().string());//文件路径添加至 strVec
		if (fileNum >= 1000)
			break;
		//400 2m57s
		//1000 19m29s
		//XThreadPool::GetInstance().AddTask(data_function, entry.path().string());
		//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, static_cast<int>(files_count));
		
		// 输出文件名
		//std::cout << entry.path().filename() << std::endl;
	}
	
	data_function(strVec, fileNum);
	/*strVec.clear();
	strVec.shrink_to_fit();*/
	//std::cout << data_vector.size() << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();//记录结束时间
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "用时：" << duration.count() / 60 << "分" << duration.count() % 60 << "秒" << std::endl;
	//设置 result_vv 结果二维数组大小
	result_vv.resize(fileNum);
	for (int i = 0; i < fileNum; ++i)
	{
		result_vv[i].resize(i + 1);
	}
	/*for (auto& vec : result_vv)
	{
		vec.resize(fileNum);
	}*/
	std::cout << fileNum << std::endl;
	for (int i = 0; i < fileNum; ++i)
	{
		for(int j = i+1; j< fileNum; ++j)
		{
			XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, j, i);
		}
	}
	all_tasks_have_add.store(true);//全部任务已添加到任务队列
}

//切割数据
void cuttingData(std::string sourcePath, std::string targetPath)
{
	int fileCount = 0;
	std::stringstream targetFileName;

	for (const auto& entry : std::filesystem::directory_iterator(sourcePath)) {
		// 跳过子文件夹  
		if (entry.is_directory()) {
			continue;
		}
		int pointCount = 0;
		std::ifstream infile(entry.path());
		targetFileName.str("");
		targetFileName << targetPath << fileCount<<".txt";
		std::ofstream outfile(targetFileName.str());
		std::string line_str;
		while (getline(infile, line_str))
		{
			if (pointCount < 100)
			{
				outfile << line_str<<std::endl;
				++pointCount;
			}
			else
			{
				outfile.close();
				++fileCount;
				targetFileName.str("");
				targetFileName << targetPath << fileCount<<".txt";
				outfile.open(targetFileName.str());
				outfile << line_str << std::endl;
				pointCount = 1;
			}
		}
		outfile.close();
		infile.close();
		//std::cout << entry.path().filename() << std::endl;
	}
}
int main()
{
	start = std::chrono::steady_clock::now();//记录开始时间
	XThreadPool::GetInstance().Init(16);//线程池初始化 16 线程
	allFile("./data/");

	//切割数据
	//////cuttingData("./dataDrive/", "./data/");

	//Data d;
	//d.ReadData("./data/17893.txt");
	/*std::cout << d.GetName() << std::endl;
	for (int i = 0; i < d.GetData().size(); ++i)
	{
		std::cout << std::get<0>(d.GetData()[i]) << " , " << std::get<1>(d.GetData()[i]) << std::endl;
	}*/
	return 0;
}
