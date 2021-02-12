//
// Created by Zachary on 2021/2/10.
//

#pragma once

#include "Tags.h"
#include <vector>

namespace nbtpp {

    // It just extends the vector !!!!
    template<typename T>
    class BaseList : public List, public std::vector<T> {
    public:
        using std::vector<T>::vector;

        BaseList(const int& size) {
            this->reserve(size);
        }
    };
}