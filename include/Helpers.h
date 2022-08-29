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

    static inline bool All(std::vector<bool> list)
    {
        for(bool v : list)
        {
            if(not v)
            {
                return false;
            }
        }

        return true;
    }

    static inline int Sign(int value)
    {
        return value >= 0 ? 1 : -1;
    }
};
#endif