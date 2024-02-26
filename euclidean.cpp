#include"euclidean.h"
#include<iostream>

using namespace std;

int Euclidean::compute()
{
	double sum = 0.0;
	size_t numSize = vec_a.size();
	for (int i = 0; i < numSize; ++i)
	{
		sum += (get<0>(vec_a[i]) - get<0>(vec_b[i])) * (get<0>(vec_a[i]) - get<0>(vec_b[i])) + (get<1>(vec_a[i]) - get<1>(vec_b[i])) * (get<1>(vec_a[i]) - get<1>(vec_b[i]));
	}
	this->SetValue(sum);
	std::cout << sum << std::endl;

	return 0;
}