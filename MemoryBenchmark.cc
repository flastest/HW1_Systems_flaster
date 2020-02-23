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
		byte_vector[i] = mt_rand()%255;
	}

	return byte_vector;
}


float time_to_get_from_DRAM(int buffer_size)
{


	std::vector<data_t> bytes = random_array(buffer_size);


	//determine how many iters to do
	int iters = buffer_size < 100000 ? buffer_size * 1024 : buffer_size;

	std::vector<int> indexes;

	//make random permutation of indexes
	for (int i = 0; i < iters; i ++)
	{
		indexes.push_back(i%buffer_size);
	}
	random_shuffle(indexes.begin(),indexes.end());


	//time iters amount of reads from bytes vector 
	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iters; i++)
	{
		//when I just had bytes[indexes[i]]; as an attempt to "read"
		// i got reallllly small times. I think -O3 didn't do the read
		// since I wasn't doing anything with the value. Doing ++
		// forces the compiler to read and write, but idk how long that 
		// takes
		
		if (bytes[indexes[i]] == 2556) std::cout<<"hah time is slow, lmao";
		//bytes[indexes[i]]++;

		//or I could do something that only reads. maybe a parallel lambda?
		//if statement?

	}
	
	auto finTime = std::chrono::high_resolution_clock::now();
	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( finTime - startTime ).count();
	

//	std::cout<<"t is "<<t<<std::endl;

	return t/(iters) ;
	
}








// takes an input of some number of trials and a buffer size and runs
// that number of trials with the given buffer size and returns the smallest
// time it took to do the thing. This returns the minimum
float get_min_from_n_trials(int num_trials, int buffer_size)
{
	
	float min = time_to_get_from_DRAM(buffer_size);


	for (int i = 0; i < num_trials; i++)
	{
		float new_time = time_to_get_from_DRAM(buffer_size);
		min = new_time < min ? new_time : min;
	}

	return min;


}







int main()
{

	int smallest_trial = pow(2,10);
	int biggest_trial = pow(2,27);
	int num_trials = 16;

	std::cout<<"# Bytes\ttime"<<std::endl;

	for (int i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = get_min_from_n_trials(num_trials,i);
		std::cout<<i<<"\t"<<time<<std::endl;

	}

	return 0;
}

