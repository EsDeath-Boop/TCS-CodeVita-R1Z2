#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>

struct Ladder {
    int r1, c1, r2, c2;

    bool is_horizontal() const {
        return r1 == r2;
    }

    int length() const {
        return is_horizontal() ? (abs(c1 - c2) + 1) : (abs(r1 - r2) + 1);
    }

    bool operator==(const Ladder& other) const {
        return r1 == other.r1 && c1 == other.c1 && r2 == other.r2 && c2 == other.c2;
    }
};

struct State {
    Ladder ladder;
    int steps;
};

Ladder get_canonical(const Ladder& lad) {
    if (lad.r1 < lad.r2 || (lad.r1 == lad.r2 && lad.c1 < lad.c2)) {
        return lad;
    }
    return {lad.r2, lad.c2, lad.r1, lad.c1};
}

struct LadderCompare {
    bool operator()(const Ladder& a, const Ladder& b) const {
        Ladder can_a = get_canonical(a);
        Ladder can_b = get_canonical(b);
        if (can_a.r1 != can_b.r1) return can_a.r1 < can_b.r1;
        if (can_a.c1 != can_b.c1) return can_a.c1 < can_b.c1;
        if (can_a.r2 != can_b.r2) return can_a.r2 < can_b.r2;
        return can_a.c2 < can_b.c2;
    }
};

bool is_valid(int r, int c, int M, int N, const std::vector<std::vector<char>>& grid) {
    return r >= 0 && r < M && c >= 0 && c < N && grid[r][c] != 'B';
}

void find_ladder_positions(int M, int N, const std::vector<std::vector<char>>& grid, char target_char, std::vector<std::pair<int, int>>& positions) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[i][j] == target_char) {
                positions.push_back({i, j});
            }
        }
    }
    std::sort(positions.begin(), positions.end());
}

bool is_ladder_valid(const Ladder& lad, int M, int N, const std::vector<std::vector<char>>& grid) {
    Ladder can_lad = get_canonical(lad);
    if (can_lad.is_horizontal()) {
        for (int c = can_lad.c1; c <= can_lad.c2; ++c) {
            if (!is_valid(can_lad.r1, c, M, N, grid)) return false;
        }
    } else {
        for (int r = can_lad.r1; r <= can_lad.r2; ++r) {
            if (!is_valid(r, can_lad.c1, M, N, grid)) return false;
        }
    }
    return true;
}

bool is_area_clear(int r_start, int c_start, int size, int M, int N, const std::vector<std::vector<char>>& grid) {
    if (r_start < 0 || c_start < 0 || r_start + size > M || c_start + size > N) {
        return false;
    }
    for (int r = r_start; r < r_start + size; ++r) {
        for (int c = c_start; c < c_start + size; ++c) {
            if (!is_valid(r, c, M, N, grid)) return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M, N;
    std::cin >> M >> N;

    std::vector<std::vector<char>> grid(M, std::vector<char>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> grid[i][j];
        }
    }

    std::vector<std::pair<int, int>> l_pos, L_pos;
    find_ladder_positions(M, N, grid, 'l', l_pos);
    find_ladder_positions(M, N, grid, 'L', L_pos);

    if (l_pos.empty() || L_pos.empty()) {
        std::cout << "Impossible" << std::endl;
        return 0;
    }

    Ladder initial_ladder = {l_pos[0].first, l_pos[0].second, l_pos.back().first, l_pos.back().second};
    Ladder target_ladder = {L_pos[0].first, L_pos[0].second, L_pos.back().first, L_pos.back().second};

    std::queue<State> q;
    q.push({initial_ladder, 0});

    std::set<Ladder, LadderCompare> visited;
    visited.insert(get_canonical(initial_ladder));

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        State current_state = q.front();
        q.pop();

        Ladder current_ladder = get_canonical(current_state.ladder);

        if (current_ladder == target_ladder) {
            std::cout << current_state.steps << std::endl;
            return 0;
        }

        for (int i = 0; i < 4; ++i) {
            Ladder next_lad = {current_ladder.r1 + dr[i], current_ladder.c1 + dc[i], current_ladder.r2 + dr[i], current_ladder.c2 + dc[i]};
            if (is_ladder_valid(next_lad, M, N, grid)) {
                if (visited.find(next_lad) == visited.end()) {
                    visited.insert(get_canonical(next_lad));
                    q.push({next_lad, current_state.steps + 1});
                }
            }
        }

        int len = current_ladder.length();
        if (current_ladder.is_horizontal()) {
            int r = current_ladder.r1;
            int c = current_ladder.c1;
            if (is_area_clear(r, c, len, M, N, grid)) {
                for (int i = 0; i < len; ++i) {
                    Ladder next_lad = {r, c + i, r + len - 1, c + i};
                    if (visited.find(next_lad) == visited.end()) {
                        visited.insert(get_canonical(next_lad));
                        q.push({next_lad, current_state.steps + 1});
                    }
                }
            }
            if (is_area_clear(r - len + 1, c, len, M, N, grid)) {
                for (int i = 0; i < len; ++i) {
                    Ladder next_lad = {r - len + 1, c + i, r, c + i};
                    if (visited.find(next_lad) == visited.end()) {
                        visited.insert(get_canonical(next_lad));
                        q.push({next_lad, current_state.steps + 1});
                    }
                }
            }
        } else { 
            int r = current_ladder.r1;
            int c = current_ladder.c1;
            if (is_area_clear(r, c, len, M, N, grid)) {
                for (int i = 0; i < len; ++i) {
                    Ladder next_lad = {r + i, c, r + i, c + len - 1};
                    if (visited.find(next_lad) == visited.end()) {
                        visited.insert(get_canonical(next_lad));
                        q.push({next_lad, current_state.steps + 1});
                    }
                }
            }
            if (is_area_clear(r, c - len + 1, len, M, N, grid)) {
                for (int i = 0; i < len; ++i) {
                    Ladder next_lad = {r + i, c - len + 1, r + i, c};
                     if (visited.find(next_lad) == visited.end()) {
                        visited.insert(get_canonical(next_lad));
                        q.push({next_lad, current_state.steps + 1});
                    }
                }
            }
        }
    }

    std::cout << "Impossible" << std::endl;
    return 0;
}