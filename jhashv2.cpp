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
void inline permute_box1(uint64_t *x1, uint64_t *x2, uint64_t *x3, uint64_t *x4)
{
    *x4 *= (((*x1) << 32) ^ ((*x4) >> 32)) * (*x1) + 1;
    *x3 *= (((*x2) << 48) ^ ((*x3) >> 16)) * (*x2) + 1;
    *x2 *= (((*x3) << 32) ^ ((*x2) >> 32)) * (*x3) + 1;
    *x1 *= (((*x4) << 16) ^ ((*x1) >> 48)) * (*x4) + 1;
}
void inline permute_box3(uint64_t *x1, uint64_t *x2, uint64_t *x3, uint64_t *x4)
{
    *x1 ^= ((*x1) * (*x2) * (*x3) * (*x4)) + 1;
    *x2 ^= ((*x1) * (*x2) * (*x3) * (*x4)) + 1;
    *x3 ^= ((*x1) * (*x2) * (*x3) * (*x4)) + 1;
    *x4 ^= ((*x1) * (*x2) * (*x3) * (*x4)) + 1;
}
void inline permute_box2(uint64_t *x1, uint64_t *x2, uint64_t *x3, uint64_t *x4)
{
    *x4 *= (((*x1) << 32) ^ ((*x4) >> 32)) + 1;
    *x3 *= (((*x2) << 32) ^ ((*x3) >> 32)) + 1;
    *x2 *= (((*x3) << 32) ^ ((*x2) >> 32)) + 1;
    *x1 *= (((*x4) << 32) ^ ((*x1) >> 32)) + 1;
}

void compute_jhash(uint8_t *input, uint64_t inplen, uint8_t *output)
{
    uint64_t state_1 = 0xa7af31539e9190e7;
    uint64_t state_2 = 0xa293a5cc7f4400c8;
    uint64_t state_3 = 0x26458d663397c103;
    uint64_t state_4 = 0x99745a5c13c1216d;
    size_t bytesRead = 0;
    output = (uint8_t *)malloc(32);
    memset(output, 0, 32);
    uint64_t index = 0;
    if (input != NULL)
    {
        uint8_t j = 0;
        for (uint8_t i = 0; i < 32; i += 4, j++)
        {
            state_1 ^= ((uint64_t)(output[31 - i - 3])) << (j * 8);
            state_2 ^= ((uint64_t)(output[31 - i - 2])) << (j * 8);
            state_3 ^= ((uint64_t)(output[31 - i - 1])) << (j * 8);
            state_4 ^= ((uint64_t)(output[31 - i - 0])) << (j * 8);
            permute_box1(&state_1, &state_2, &state_3, &state_4);
            permute_box2(&state_1, &state_2, &state_3, &state_4);
            permute_box3(&state_1, &state_2, &state_3, &state_4);
        }
    }
}
int main(int argc, char *argv[])
{
    uint64_t state_1 = 0xa7af31539e9190e7;
    uint64_t state_2 = 0xa293a5cc7f4400c8;
    uint64_t state_3 = 0x26458d663397c103;
    uint64_t state_4 = 0x99745a5c13c1216d;
    FILE *file = freopen(NULL, "rb", stdin);
    size_t bytesRead = 0;
    char buffer[32];
    memset(buffer, 0, 32);
    if (file != NULL)
    {
        while ((bytesRead = fread(buffer, 1, 32, file)) > 0)
        {
            uint8_t j = 0;
            for (uint8_t i = 0; i < 32; i += 4, j++)
            {
                state_1 ^= ((uint64_t)(buffer[31 - i - 3])) << (j * 8);
                state_2 ^= ((uint64_t)(buffer[31 - i - 2])) << (j * 8);
                state_3 ^= ((uint64_t)(buffer[31 - i - 1])) << (j * 8);
                state_4 ^= ((uint64_t)(buffer[31 - i - 0])) << (j * 8);
                permute_box1(&state_1, &state_2, &state_3, &state_4);
                permute_box2(&state_1, &state_2, &state_3, &state_4);
                permute_box3(&state_1, &state_2, &state_3, &state_4);
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