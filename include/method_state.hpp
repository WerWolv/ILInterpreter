#pragma once

#include "types.hpp"

struct MethodState {
    u8 *ILPtr;
    u32 programCounter;
};