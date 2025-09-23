#pragma once

#include <cstdint>
#include <filesystem>
#include <sys/types.h>
#include <array>

using namespace std;

namespace fs = filesystem;

namespace Dualys
{
    constexpr array<uint8_t, 6> MAGIC = {0x4A, 0x61, 0x6A, 0x61, 0x37, 0x37};
    constexpr  uint32_t VERSION  = 0x00000001;

    const fs::path JAH_DIRECTORY_NAME =  ".jah";
    const fs::path JAH_DATABASE =  ".jah/ji.jah"; 
    enum class ObjectType: uint8_t {
        Commit      = 0x01,
        Tree        = 0x02,
        Blob        = 0x03,
        Runner      = 0x04,
        TimEntry    = 0x01,
    };

    class Jah 
    {
        public:
            int init();
            bool isJahRepositoryValid();
    };
}
