## HW1: Benchmarking the Memory Hierarchy 
##### Ariel Flaster 


makefile included, just type make to make

#### Results
| bytes |   minimum time (ns)| 
|------------|-------------|
|2048        |    0.557709 |
|4096        |   0.557709 |
|8192        |   0.559998 |
|16384       |   0.590849 |
|32768       |   0.650764 |
|65536       |   0.644863 |
|131072      |   0.664234 |
|262144      |   0.73806 |
|524288      |   0.754613 |
|1048576     |   0.835706 |
|2097152     |   1.1322 |
|4194304     |   1.31302 |
|8388608     |   1.80073 |
|16777216    |   2.81448 |
|33554432    |   4.47607 |
|67108864    |   6.47666 |
|134217728   |   7.60465 |
|268435456   |   8.50224 |
|536870912   |   10.0862 |


No matter how hard I tried, I could not foil the prefetcher. I saw an increase in time taken at 32768 
bytes, which I assume indicates the departure from L1 to L2 cache. This would mean that my computer's 
L1d cache size is 16 KB. This makes sense. My computer has 32 KB of L1 cache per core, so it makes 
sense that only half of that would be used for data. There is another jump at 262144 bytes, which 
shows that L3 cache is being used. This means that my L2 cache has 256 KB. I would expect this 
number to be around this size; I have 1 MB of L2 cache split up between 4 cores, so that number seems 
normal. After that, I think there is another jump at 2097152 bytes. That would correspond to the jump
from L3 cache to DRAM. I have 8 MB of L3 cache between all of my cores, so it makes sense that this 
core has 2 MB of its own. 

The fact that my numbers increases is pleasant, but they don't increase enough when I start accessing 
DRAM. This shows that I didn't entirely foil the prefetcher; I assume it figured out what indices I 
was using. 

| Cache |   time to access (ns)| 
|------|-----------|
| L1 |  .55 |
| L2  |  .65  |
|L3|.77|
|DRAM|4.99|

All of these numbers, except for L1, are too small. That damn prefetcher!

My graph is a little too slopey for my tastes. I think that since the prefetcher did its job as best
as it could, I didn't get as descrete steps as I'd like. The prefetcher made all of the times smaller,
so the steps are a lot less pronounced. 


##### Algorithm

My plan was to have an array of a lot of indexes that randomly point into an array of bytes.

I sequentially iterated through a list of randomly generated indices to get random indexes into my 
buffer. I believed that the prefetcher wouldn't see through that level of indirection, but I think I 
was wrong.


##### The reading method

To read, I checked to see if a set of 4 bytes (in the form of an uint32_t) was equal to a number that 
it really shouldn't be equal to. I figured that this wouldn't have any reads into cache/memory besides
the random index into the "bytes" vector. I hoped that the read from "indexes" (God forbid I can't spell!
I *am* a computer scientist, after all) wouldn't cause too much overhead. Since I read from "indexes" 
sequentially, I figured the prefetcher would make sure that the time to access it was negligible. 

