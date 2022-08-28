#ifndef HELPERS_H
#define HELPERS_H

#include <bits/stdc++.h>

class Helpers
{
public:
    static inline int Clamp(int min, int value, const int max)
    {
        return std::max(min, value) == value ? std::min(max, value) : min;
    }
};
#endif