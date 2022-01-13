#pragma once

#include "eviction_policy.h"

class ClockPolicy : EvictionPolicy {
private:
    int clock_head;

public:
    ClockPolicy() {}
    ~ClockPolicy() {}

    void init() override;
    void find(int thread_n, offset_t *offset_buf, index_t buf_sz) override;
    index_t get_result() override;

    index_t evict(offset_t *offset_buf, index_t buf_sz) override;


}