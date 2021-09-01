#pragma once

#include "Device.hpp"
#include <vector>

class Input {
    private:
        Input() {}
        ~Input() {}
    
    public:
        template <typename T>
        static std::vector<T> list();
};

template <>
std::vector<Device> Input::list();