#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

struct Rectangle {
    int x1, y1, x2, y2;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;

    std::vector<Rectangle> rectangles(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> rectangles[i].x1 >> rectangles[i].y1 >> rectangles[i].x2 >> rectangles[i].y2;
    }

    Rectangle paper;
    std::cin >> paper.x1 >> paper.y1 >> paper.x2 >> paper.y2;
    std::vector<int> vertical_dividers;
    vertical_dividers.push_back(paper.x1);
    vertical_dividers.push_back(paper.x2);

    for (int k = paper.x1 + 1; k < paper.x2; ++k) {
        bool is_valid_fold = true;
        for (const auto& rect : rectangles) {
            if (rect.x1 < k && k < rect.x2) {
                is_valid_fold = false;
                break;
            }
        }
        if (is_valid_fold) {
            vertical_dividers.push_back(k);
        }
    }

    std::sort(vertical_dividers.begin(), vertical_dividers.end());
    
    int min_width = INT_MAX;
    for (size_t i = 1; i < vertical_dividers.size(); ++i) {
        min_width = std::min(min_width, vertical_dividers[i] - vertical_dividers[i - 1]);
    }

    std::vector<int> horizontal_dividers;
    horizontal_dividers.push_back(paper.y1);
    horizontal_dividers.push_back(paper.y2);
    
    for (int k = paper.y1 + 1; k < paper.y2; ++k) {
        bool is_valid_fold = true;
        for (const auto& rect : rectangles) {
            if (rect.y1 < k && k < rect.y2) {
                is_valid_fold = false;
                break;
            }
        }
        if (is_valid_fold) {
            horizontal_dividers.push_back(k);
        }
    }
    
    std::sort(horizontal_dividers.begin(), horizontal_dividers.end());

    int min_height = INT_MAX;
    for (size_t i = 1; i < horizontal_dividers.size(); ++i) {
        min_height = std::min(min_height, horizontal_dividers[i] - horizontal_dividers[i - 1]);
    }
    
    long long result = (long long)min_width * min_height;
    std::cout << result << std::endl;

    return 0;
}