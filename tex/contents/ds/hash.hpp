#include "../contest/template.hpp"
struct custom_hash {
    inline static const uint64_t ADD = CURRENT_TIMESTAMP;
    size_t operator()(uint64_t x) const {
    	x += ADD;
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
};
