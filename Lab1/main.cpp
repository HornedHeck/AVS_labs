#include <immintrin.h>
#include <iostream>
#include <vector>
#include <chrono>
using std::vector;

vector<int32_t> F(vector<int8_t> &A, vector<int8_t> &B, vector<int8_t> &C, vector<int16_t> &D);

vector<int32_t> FPP(vector<int8_t> &A, vector<int8_t> &B, vector<int8_t> &C, vector<int16_t> &D);

int main() {

    vector<int8_t> A{-128,      -128,   100, 0, 1, 127,     127, 0};
    vector<int8_t> B{-128,      -128,   100, 1, 5, 127,     7,   8};
    vector<int8_t> C{-128,      127,    0,   2, 5, 127,     7,   8};
    vector<int16_t> D{32767,    -32768, -1,  3, 5, 32767,   7,   8};

    auto res = F(A, B, C, D);
    auto done = std::chrono::high_resolution_clock::now();
    auto res_pp = FPP(A , B , C , D);
    std::cout << "AVX\t\tC++" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << res[i] << "\t\t" << res_pp[i]  << std::endl;
    }



    return 0;
}

vector<int32_t> FPP(vector<int8_t> &A, vector<int8_t> &B, vector<int8_t> &C, vector<int16_t> &D){
    vector<int32_t> res(8);
    for (int i = 0; i < 8; ++i) {
        res[i] = (A[i] + B[i]) * C[i] + D[i];
    }
    return res;
}

vector<int32_t> F(vector<int8_t> &A, vector<int8_t> &B, vector<int8_t> &C, vector<int16_t> &D) {
/// (A + B) * C + D
    __m128i a = _mm_setr_epi16(A[0], A[1], A[2], A[3], A[4], A[5], A[6], A[7]);
    auto b = _mm_setr_epi16(B[0], B[1], B[2], B[3], B[4], B[5], B[6], B[7]);
    a = _mm_add_epi16(a, b);
    __m256i c = _mm256_setr_epi32(C[0], C[1], C[2], C[3], C[4], C[5], C[6], C[7]);
    __m256i a_ext = _mm256_cvtepi16_epi32(a);
    __m256i res = _mm256_mullo_epi32(a_ext, c);
    __m256i d = _mm256_cvtepi16_epi32(_mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&(D[0]))));
    res = _mm256_add_epi32(res, d);
    vector<int32_t> ans(8);
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(&(ans[0])), res);
    return ans;
}
