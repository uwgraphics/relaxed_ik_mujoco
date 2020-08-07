#ifndef relaxedIK_h__
#define relaxedIK_h__

#include <vector>

std::vector<double> solveIK(std::vector<double> pos, std::vector<double> quat);

#ifdef __cplusplus
extern "C" {
#endif
    class Opt {
        public:
            double* data;
            int length;
    };

    Opt solve(double* pos_arr, int pos_length, double* quat_arr, int quat_length);
#ifdef __cplusplus
}
#endif

#endif // relaxedIK.h