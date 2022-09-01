#ifndef HELPERS_H
#define HELPERS_H

#include <bits/stdc++.h>

/**
 * @brief Some useful functions
 * 
 */
class Helpers
{
public:
    /**
     * @brief Clamp value between some interval
     * 
     * @param min 
     * @param value 
     * @param max 
     * @return int 
     */
    static inline int Clamp(int min, int value, const int max)
    {
        return std::max(min, value) == value ? std::min(max, value) : min;
    }

    /**
     * @brief Return true if every element of the list is true
     * 
     * @param list 
     * @return true 
     * @return false 
     */
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

    /**
     * @brief Return 1 if value is positive else -1
     * 
     * @param value 
     * @return int 
     */
    static inline int Sign(int value)
    {
        return value >= 0 ? 1 : -1;
    }
};
#endif