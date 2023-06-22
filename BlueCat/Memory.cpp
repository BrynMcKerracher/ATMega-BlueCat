#include "Common.h"
#include "Memory.h"

namespace BlueCat::Board {
#if defined(AVR_UNO)
    constexpr size_t RAM = 2048;
    const char* Name = "AVR UNO";
#elif defined(AVR_DUE)
    constexpr size_t RAM = 98304; 
    const char* Name = "AVR DUE";
#elif defined(AVR_NANO)
    constexpr size_t RAM = 2048;
    const char* Name = "AVR NANO";
#elif defined(AVR_MEGA2560)
    constexpr size_t RAM = 8192; 
    const char* Name = "AVR MEGA2560";
#elif defined(AVR_MEGA)
    constexpr size_t RAM = 8192; 
    const char* Name = "AVR MEGA";
#elif defined(AVR_MICRO)
    constexpr size_t RAM = 2560;  
    const char* Name = "AVR MICRO";
#else
    constexpr size_t RAM = 0;
    const char* Name = "UNKNOWN BOARD";
#endif
}
