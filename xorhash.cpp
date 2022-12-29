#include <iostream>
#include <stdio.h>
#include <cstring>
#include <iomanip>

template <typename T>
std::string hexify(T i)
{
    std::stringbuf buf;
    std::ostream os(&buf);

    os << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;

    return buf.str().c_str();
}

int main(int argc, char *argv[])
{
    uint64_t state_1 = 0;
    uint64_t state_2 = 0;
    uint64_t state_3 = 0;
    uint64_t state_4 = 0;
    FILE *file = freopen(NULL, "rb", stdin);
    size_t bytesRead = 0;
    char buffer[32];
    if (file != NULL)
    {
        while ((bytesRead = fread(buffer, 1, 32, file)) > 0)
        {
            if (bytesRead == 32)
            {
                int j = 0;
                for (uint8_t i = 24; i < 32; i++, j++)
                {
                    state_4 ^= ((uint64_t)(buffer[i])) << (j * 8);
                }
            }
            if (bytesRead >= 24)
            {
                int j = 0;
                for (uint8_t i = 16; i < 24; i++, j++)
                {
                    state_3 ^= ((uint64_t)(buffer[i])) << (j * 8);
                }
            }
            if (bytesRead >= 16)
            {
                int j = 0;
                for (uint8_t i = 8; i < 16; i++, j++)
                {
                    state_2 ^= ((uint64_t)(buffer[i])) << (j * 8);
                }
            }
            if (bytesRead >= 8)
            {
                int j = 0;
                for (uint8_t i = 0; i < 8; i++, j++)
                {
                    state_1 ^= ((uint64_t)(buffer[i])) << (j * 8);
                }
            }
        }
    }
    fclose(file);
    std::cout << hexify(state_1);
    std::cout << hexify(state_2);
    std::cout << hexify(state_3);
    std::cout << hexify(state_4);
    return 0;
}