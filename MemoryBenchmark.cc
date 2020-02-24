#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <cassert>


using data_t = uint32_t;

/*
 * measures the latency of reading a byte from DRAM 
 * runs num_trials number of trials. each trial takes a random list
 * and makes a random permutation of every index in the list. Then, 
 * the list of indexes is used to call random indexes in the random
 * list of numbers. 
 * The reads are timed. Each read is an if statement that always evals
 * to false. 
 *
 */


// creates a random array of bytes given a size
std::vector<data_t> random_array(int size)
{
	std::mt19937 mt_rand(time(0));
	std::vector<data_t> byte_vector(size);

	for (int i = 0; i < size; i++)
	{
//		byte_vector[i] = mt_rand()%size;
		byte_vector[i] = (mt_rand()%255);
	}

	return byte_vector;
}





/*
 * This takes a size of a buffer and takes buffer_size number random
 * reads from a random array.
 *
 * Reads are a comparison to an immediate that always evals to false.
 *
 * Output is the average time it takes to do a read
 */
float time_to_get_from_DRAM(uint64_t buffer_size)
{
	//I want an immediate in my if statement in the for loop
	// that evaluates quickly and always fails.
	const auto BIGGEST_INDEX_PLUS_ONE  = buffer_size + 1;

  //buffer size is number of ints, but I want number of bytes
	buffer_size /= sizeof(data_t);

	std::vector<data_t> bytes = random_array(buffer_size);


	//determine how many iters to do
	int iters = buffer_size < 100000 ? buffer_size * 2048 : buffer_size;



	std::vector<int> indexes;

	//make random permutation of indexes
	for (int i = 0; i < iters; i ++)
	{
		indexes.push_back((i)%(buffer_size));
	}
	random_shuffle(indexes.begin(),indexes.end());



	//time "iters" amount of reads from bytes vector 
	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iters; i++)
	{

		auto index = indexes[i];


		assert((bytes[index] != BIGGEST_INDEX_PLUS_ONE));

	}
	auto finTime = std::chrono::high_resolution_clock::now();
	

	float t = std::chrono::duration_cast<std::chrono::nanoseconds>( finTime - startTime ).count();
	
	return t/iters;
	
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

	uint64_t smallest_trial = pow(2,9);
	uint64_t biggest_trial = pow(2,40);
	int num_trials = 16;

	std::cout<<"# Bytes\ttime"<<std::endl;

	for (uint64_t i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = get_min_from_n_trials(num_trials,i);
		std::cout<<i*sizeof(data_t)<<"\t"<<time<<std::endl;

	}

	return 0;
}

