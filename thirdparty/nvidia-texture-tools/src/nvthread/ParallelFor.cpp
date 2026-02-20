// This code is in the public domain -- Ignacio Casta�o <castano@gmail.com>

#include <atomic>

#include "ParallelFor.h"
#include "progschj/ThreadPool.h"

#include "nvcore/Utils.h" // toI32

using namespace nv;

static void worker(ParallelFor* owner, size_t tid) {
    while(true) {
        uint new_idx = owner->idx.fetch_add(owner->step);
        if (new_idx >= owner->count) {
            break;
        }

        const uint count = min(owner->count.load(), new_idx + owner->step.load());
        for (uint i = new_idx; i < count; i++) {
            owner->task(owner->context, /*tid, */i);
        }
    }
}


ParallelFor::ParallelFor(ForTask * task, void * context) : task(task), context(context) {}

void ParallelFor::run(uint count, uint step/*= 1*/)
{
    const size_t workerCount = processorCount();
    ThreadPool pool(workerCount);
    std::vector< std::future<void>> results;

    this->count.store(count);
    this->step.store(step);

    // Init atomic counter to zero.
    this->idx.store(0);

    // Start threads.
    for(size_t i = 0; i < workerCount; ++i) {
        results.emplace_back(
            pool.enqueue([owner = this, i] {
                worker(owner, i);
            })
        );
    }

    // wait for all threads to finish
    for(auto && result: results) {
        result.get();
    }

    nvDebugCheck(idx >= count);
}

