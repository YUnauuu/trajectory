#include"euclidean.h"
#include<iostream>

int Euclidean::compute()
{
	double sum = 0.0;
	size_t numSize = vec_a.size();
	for (int i = 0; i < numSize; ++i)
	{
		sum += (vec_a[i][0] - vec_b[i][0]) * (vec_a[i][0] - vec_b[i][0]) + (vec_a[i][1] - vec_b[i][1]) * (vec_a[i][1] - vec_b[i][1]);
	}
	this->SetValue(sum);
	std::cout << sum << std::endl;
	return 0;
}