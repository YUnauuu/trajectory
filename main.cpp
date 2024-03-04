#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"
#include<filesystem>
#include<chrono>
#include<future>
#include<sstream>
#include<fstream>

enum compute_method //ʹ��ö������
{
	EUCLIDEAN  //ŷʽ����
};

const double threshold_value = 0.5;//�����ֵ
std::chrono::steady_clock::time_point start;//��¼��ʼʱ��


std::vector<Data> data_vector;//������ж�ȡ������
std::mutex mtx_result_vv;//����ʹ�� result_vv
std::vector<std::vector<double>> result_vv;//������飬��ά���飬ע������������ ����[i][j] i>j

std::atomic<bool> all_tasks_have_add(false);//���ȫ����������ӵ��������

//��ȡ�ļ�
void data_function(std::vector<std::string>& str_vec, int& fileNum)
//void data_function(std::shared_ptr<std::vector<std::string>> strP, int& fileNum)
{
	/*std::promise<int> pro;
	pro.set_value(0);*/
	std::atomic<int> num(0);//�Ѷ����ݸ���
	std::condition_variable cv;
	std::mutex mtx_data_vector;//����ʹ�� data_vector
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

//����, m ö�����͵ļ��㷽����rank ��data_queue �е���λ
void compute_function(compute_method m, int i, int j)
{
	switch (m)
	{
	case EUCLIDEAN:
		Euclidean eu(i, j);
		
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
	int fileNum = 0;//��¼�ļ���
	std::vector<std::string> strVec;//�����ļ�·��
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		// �������ļ���  
		if (entry.is_directory()) {
			continue;
		}
		++fileNum;//�ļ��� +1
		strVec.emplace_back(entry.path().string());//�ļ�·������� strVec
		if (fileNum >= 1000)
			break;
		//400 2m57s
		//1000 19m29s
		//XThreadPool::GetInstance().AddTask(data_function, entry.path().string());
		//XThreadPool::GetInstance().AddTask(compute_function, EUCLIDEAN, static_cast<int>(files_count));
		
		// ����ļ���
		//std::cout << entry.path().filename() << std::endl;
	}
	
	data_function(strVec, fileNum);
	/*strVec.clear();
	strVec.shrink_to_fit();*/
	//std::cout << data_vector.size() << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();//��¼����ʱ��
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
	std::cout << "��ʱ��" << duration.count() / 60 << "��" << duration.count() % 60 << "��" << std::endl;
	//���� result_vv �����ά�����С
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
	all_tasks_have_add.store(true);//ȫ����������ӵ��������
}

//�и�����
void cuttingData(std::string sourcePath, std::string targetPath)
{
	int fileCount = 0;
	std::stringstream targetFileName;

	for (const auto& entry : std::filesystem::directory_iterator(sourcePath)) {
		// �������ļ���  
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
	start = std::chrono::steady_clock::now();//��¼��ʼʱ��
	XThreadPool::GetInstance().Init(16);//�̳߳س�ʼ�� 16 �߳�
	allFile("./data/");

	//�и�����
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
