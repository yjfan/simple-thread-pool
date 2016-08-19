#include "../src/threadpool.h"

#include <cstdio>
#include <cassert>
#include <mutex>
#include <unistd.h>

#define MAX_THREADS 8
#define MAX_TEST_COUNT 10000

static std::mutex mutex;

static void increment(void *user_data)
{
    std::unique_lock<std::mutex> lock(mutex);

    unsigned int *number = reinterpret_cast<unsigned int *>(user_data);

    ++(*number);
}

int main(int argc, char *argv[])
{
    ThreadPool *pool = new ThreadPool(MAX_THREADS);

    unsigned int count = 0;
    unsigned int index;

    for (index = 0; index < MAX_TEST_COUNT; ++index) {
        assert(pool->push(increment, &count) == true);
    }

    while (count != MAX_TEST_COUNT) {
        usleep(200 * 1000);
    }

    delete pool;

    printf("[OK]\n");

    return 0;
}