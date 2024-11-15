#pragma once

namespace ili::util {

    template<typename T>
    T alignUp(T value, T alignment) {
        return (value + alignment - 1) & ~(alignment - 1);
    }

}