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

/* takes a number of iterations and a buffer_size and makes an 
 * array of iteration size that contains random indices from 0 to
 * buffer_size-1. The returned array should have no cycles.
 *
 */
std::vector<int> index_vector(int iters, int buffer_size)
{
	std::vector<int> indices;
	//to make sure there are no cycles, this array will contain
	// some fibonacci numbers
	int prior = 1;
	for (int i = 0; i < iters; i++)
	{
		indices.push_back((i+prior)%(buffer_size-1));
		prior = i+prior;
	}
	//random_shuffle(indices.begin(),indices.end());
	


	return indices;
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
	// that evaluates quickly and always fails, so I think that 
	// this will sort of act as an immediate. Looking up a variable
	// would be too slow
	const auto BIGGEST_INDEX_PLUS_ONE  = buffer_size + 1;
	buffer_size /= sizeof(data_t);

	std::vector<data_t> bytes = random_array(buffer_size);


	//determine how many iters to do
	int iters = buffer_size < 100000 ? buffer_size * 1024 : buffer_size;



	std::vector<int> indexes;

	//make random permutation of indexes
	for (int i = 0; i < iters; i ++)
	{
		indexes.push_back((i)%(buffer_size));
	}
	random_shuffle(indexes.begin(),indexes.end());

	//different algorithm to create a random array of indices
	//indexes = index_vector(iters, buffer_size);

//	uint64_t index = 0;

	//time "iters" amount of reads from bytes vector 
	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < iters; i++)
	{

//		index = bytes[index];
		auto index = indexes[i];


		//is it prefectching because it knows ahead what index it'll go to
		// or is it prefetching because it happens to read from the same place every time?
		assert((bytes[index] != BIGGEST_INDEX_PLUS_ONE));
		// {
		// 	std::cout<<"something wrong is happening"<<std::endl;
		// 	std::cout<<"buffer_size is " <<buffer_size<<std::endl;
		// }

//		if (bytes[indexes[i]] == 256) std::cout<<"something wrong is happening"<<std::endl;

		
		//bytes[indexes[i]]++;


	}
	auto finTime = std::chrono::high_resolution_clock::now();
	
//	if (index > BIGGEST_INDEX_PLUS_ONE) std::cout<<"something is going wrong" <<std::endl;

	double t = std::chrono::duration_cast<std::chrono::nanoseconds>( finTime - startTime ).count();
	

//	std::cout<<"t is "<<t<<std::endl;

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
	uint64_t biggest_trial = pow(2,26);
	int num_trials = 16;

	std::cout<<"# Bytes\ttime"<<std::endl;

	for (uint64_t i = smallest_trial; i <= biggest_trial; i *= 2)
	{
		float time = get_min_from_n_trials(num_trials,i);
		std::cout<<i*sizeof(data_t)<<"\t"<<time<<std::endl;

	}

	return 0;
}

