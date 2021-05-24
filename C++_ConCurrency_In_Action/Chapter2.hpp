#include "../Common.hpp"

namespace Chapter2{
    template <typename Iterator, typename T>
    struct accumulate_block{
        void operator()(Iterator first, Iterator second, T& result){
            result = std::accumulate(first, second, result);
        }
    };

    // parallel accumulate
    template <typename Iterator, typename T>
    T parallel_accumulate(Iterator first, Iterator last, T init){
        auto len = std::distance(first, last);
        if(!len){
            return init;
        }
        uint64_t minPerThread = 25;
        uint64_t maxThreads = (len + minPerThread-1) / minPerThread;
        uint64_t hardwareThreads = std::thread::hardware_concurrency();
        uint64_t numThreads = std::min(hardwareThreads==0? 2: hardwareThreads, maxThreads);
        uint64_t blockSize = len / numThreads;
        std::vector<T> results(numThreads);
        std::vector<std::thread> threads(numThreads-1);

        Iterator blockStart = first;
        for(int i =0; i < (numThreads-1); ++i){
            auto blockEnd = blockStart;
            std::advance(blockEnd, blockSize);
            threads[i] = std::thread(accumulate_block<Iterator, T>(), blockStart, blockEnd, std::ref(results[i]));
            blockStart = blockEnd;
        }
        accumulate_block<Iterator, T>()(blockStart, last, results[numThreads-1]);

        for(auto &&entry : threads){
            if(entry.joinable()){
                entry.join();
            }
        }
        return std::accumulate(results.begin(), results.end(), init);
    }
}
