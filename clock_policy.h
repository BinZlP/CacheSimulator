#pragma once

#include "common_include.h"
#include "eviction_policy.h"

typedef struct {
    offset_t offset;
    bool chance;
} ClockDataEntry;

class ClockPolicy : EvictionPolicy {
private:
    index_t clock_head;
    ClockDataEntry *clock_data;

    void init() override;
    void find(int thread_n, offset_t *offset_buf) override;
    index_t get_result() override;

public:
    ClockPolicy(index_t _buf_sz) : EvictionPolicy(_buf_sz), clock_head(0) {
        clock_data = new ClockDataEntry[_buf_sz];
        for(index_t i=0; i<_buf_sz; i++) {
            clock_data[i].chance = false;
            clock_data[i].offset = -1;
        }
    }
    ~ClockPolicy() { delete clock_data; }

    void notify_replace(index_t slot, offset_t new_off);
    void update_score(index_t slot);
    int update_score_by_offset(offset_t offset);

    index_t evict(offset_t *offset_buf);
};