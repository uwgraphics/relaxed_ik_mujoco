#include "relaxedIK.h"
#include <iterator>
#include <sstream>

std::vector<double> solveIK(std::vector<double> pos, std::vector<double> quat) {
    Opt x = solve(pos.data(), (int) pos.size(), quat.data(), (int) quat.size());
    std::vector<double> sol(x.length, 0.0);
    for (int i = 0; i < x.length; i++) {
        sol[i] = x.data[i];
    }

    std::ostringstream sol_str; 
    if (!sol.empty()) 
    { 
        sol_str << "[";
        // Convert all but the last element to avoid a trailing "," 
        std::copy(sol.begin(), sol.end() - 1, 
            std::ostream_iterator<double>(sol_str, ", ")); 
        // Now add the last element with no delimiter 
        sol_str << sol.back() << "]";
    } 

    // log a message;
    printf("%s\n",sol_str.str().c_str());

    return sol;
}
