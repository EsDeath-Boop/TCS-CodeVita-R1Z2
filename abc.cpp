#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <climits>
#include <sstream>  
#include <limits>   

int calculate_lcs(const std::string& s1, const std::string& s2) {
    int n = s1.length();
    if (n == 0) return 0;
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[n][n];
}

int main() {
    // Fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::string initial_str = "";
    int counts[3] = {0, 0, 0}; 
    for (int i = 0; i < n; ++i) {
        char c;
        std::cin >> c; 
        initial_str += c;
        if (c == 'A') counts[0]++;
        else if (c == 'B') counts[1]++;
        else counts[2]++;
    }

    std::map<int, char> fixed_positions;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string line;
    if (std::getline(std::cin, line) && !line.empty()) {
        std::stringstream ss(line);
        int fixed_pos;
        while (ss >> fixed_pos) {
            fixed_positions[fixed_pos - 1] = initial_str[fixed_pos - 1];
        }
    }
    
    std::vector<char> order = {'A', 'B', 'C'};
    int min_shifts = INT_MAX;

    do {
        std::string target_str = "";
        target_str += std::string(counts[order[0] - 'A'], order[0]);
        target_str += std::string(counts[order[1] - 'A'], order[1]);
        target_str += std::string(counts[order[2] - 'A'], order[2]);
        bool possible = true;
        for (auto const& pair : fixed_positions) {
            int pos = pair.first;
            char owner = pair.second;
            if (target_str[pos] != owner) {
                possible = false;
                break;
            }
        }

        if (possible) {
            int lcs_length = calculate_lcs(initial_str, target_str);
            int shifts = n - lcs_length;
            min_shifts = std::min(min_shifts, shifts);
        }

    } while (std::next_permutation(order.begin(), order.end()));

    if (min_shifts == INT_MAX) {
        std::cout << "Impossible" << std::endl;
    } else {
        std::cout << min_shifts << std::endl;
    }

    return 0;
}   