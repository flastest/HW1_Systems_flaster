#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdlib>


using data_t = uint32_t;

/*
 * measures the latency of reading a byte from DRAM 
 *
 *
 *
 */


// creates a random array of bytes given a size
std::vector<data_t> random_array(int size)
{
	std::mt19937 mt_rand(size);
	std::vector<data_t> byte_vector(size);

	for (int i = 0; i < size; i++)
	{
		byte_vector[i] = mt_rand();
	}

	return byte_vector;
}


float time_to_get_from_DRAM(int buffer_size)
{

	std::mt19937 mt_rand(buffer_size);


	std::vector<data_t> bytes = random_array(buffer_size);



	std::vector<int> indexes;

	//make random permutation of indexes
	for (int i = 0; i < buffer_size; i ++)
	{
		indexes.push_back(i);
	}
	random_shuffle(indexes.begin(),indexes.end());

	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < buffer_size; i++)
	{

		bytes[indexes[i]];

	}
	
	auto finTime = std::chrono::high_resolution_clock::now();
	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( finTime - startTime ).count();


	return t/buffer_size;
	
}



// as a control, i wrote something that just fetches from the cache.
//it sequentially reads things from an array. Each item in the array
//is prefetched just in case. This should guarantee that the memory 
//reads are read from L1. 

float time_to_get_from_cache(int buffer_size)
{
	//adding a one to the seed so that I don't use the same seed twice.
	std::vector<data_t> bytes = random_array(buffer_size+1);
	

	
	//take minimum!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! not average!!!!
	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < buffer_size; ++i)
	{
		

		bytes[i];

		

		
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( endTime - startTime ).count();


	return t/buffer_size;


}






// takes an input of some number of trials and a buffer size and runs
// that number of trials with the given buffer size and returns the smallest
// time it took to do the thing. This returns the minimum
float get_min_from_n_trials(int num_trials, int buffer_size)
{
	
	

	float min = time_to_get_from_cache(buffer_size);


	for (int i = 0; i < num_trials; i++)
	{
		float new_time = time_to_get_from_cache(buffer_size);
		min = new_time < min ? new_time : min;
	}

	return min;


}







int main()
{

	std::cout<<"# Bytes\ttime"<<std::endl;

	int smallest_trial = pow(2,11);
	int biggest_trial = pow(2,26);
	for (int i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = time_to_get_from_cache(i);
		std::cout<<i<<"\t"<<time<<std::endl;

	}

	std::cout<<"here is with the DRAM"<<std::endl;
	std::cout<<"# Bytes\ttime"<<std::endl;

	for (int i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = time_to_get_from_DRAM(i);
		std::cout<<i<<"\t"<<time<<std::endl;

	}

	std::mt19937 mt_rand(std::time(0));

	//time it takes for ++ to happen:
	auto startTime = std::chrono::high_resolution_clock::now();
		
	mt_rand();
		
	auto endTime = std::chrono::high_resolution_clock::now();
	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( endTime - startTime ).count();
 
	std::cout<<"this is how long it takes to gen a rando num to happen:"<<t<<std::endl;



	return 0;
}

