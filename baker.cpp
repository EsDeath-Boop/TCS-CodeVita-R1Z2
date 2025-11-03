#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

vector<vector<string>> get_next_states(const vector<string>& current_list, int n) {
    vector<vector<string>> next_states;
    for (int i = 0; i < n; ++i) { 
        for (int j = i; j < n; ++j) { 
            vector<string> segment(current_list.begin() + i, current_list.begin() + j + 1);
            
            vector<string> remaining;
            remaining.insert(remaining.end(), current_list.begin(), current_list.begin() + i);
            remaining.insert(remaining.end(), current_list.begin() + j + 1, current_list.end());
            
            for (int k = 0; k <= (int)remaining.size(); ++k) {
                if (k == i) continue; 

                vector<string> new_list = remaining;
                new_list.insert(new_list.begin() + k, segment.begin(), segment.end());
                next_states.push_back(new_list);
            }
        }
    }
    return next_states;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    string temp;
    getline(cin, temp); 
    
    getline(cin, temp); 

    vector<string> shuffled_list(n);
    for (int i = 0; i < n; ++i) {
        getline(cin, shuffled_list[i]);
    }

    getline(cin, temp); 

    vector<string> original_list(n);
    for (int i = 0; i < n; ++i) {
        getline(cin, original_list[i]);
    }

    if (shuffled_list == original_list) {
        cout << 0 << endl;
        return 0;
    }

    queue<vector<string>> q_forward, q_backward;
    map<vector<string>, int> visited_forward, visited_backward;

    q_forward.push(shuffled_list);
    visited_forward[shuffled_list] = 0;

    q_backward.push(original_list);
    visited_backward[original_list] = 0;

    while (!q_forward.empty() && !q_backward.empty()) {
        
        if (q_forward.size() <= q_backward.size()) {
            int level_size = q_forward.size();
            for (int i = 0; i < level_size; ++i) {
                vector<string> current = q_forward.front();
                q_forward.pop();

                int cost = visited_forward[current];

                vector<vector<string>> next_states = get_next_states(current, n);
                for (const auto& next : next_states) {
                    if (visited_backward.count(next)) {
                        cout << cost + 1 + visited_backward[next] << endl;
                        return 0;
                    }
                    if (visited_forward.find(next) == visited_forward.end()) {
                        visited_forward[next] = cost + 1;
                        q_forward.push(next);
                    }
                }
            }
        } else { 
             int level_size = q_backward.size();
             for (int i = 0; i < level_size; ++i) {
                vector<string> current = q_backward.front();
                q_backward.pop();

                int cost = visited_backward[current];

                vector<vector<string>> next_states = get_next_states(current, n);
                for (const auto& next : next_states) {
                    if (visited_forward.count(next)) {
                        cout << cost + 1 + visited_forward[next] << endl;
                        return 0;
                    }
                    if (visited_backward.find(next) == visited_backward.end()) {
                        visited_backward[next] = cost + 1;
                        q_backward.push(next);
                    }
                }
            }
        }
    }

    return 0;
}