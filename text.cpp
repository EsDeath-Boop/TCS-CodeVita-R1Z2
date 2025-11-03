#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <deque>

struct FormatSettings {
    int width_even = 75;
    int width_odd = 75;
    bool hyphenate = false;
};

bool is_special_word(const std::string& word) {
    return word.find('@') != std::string::npos || 
           word.rfind("http://", 0) == 0 || 
           word.rfind("https://", 0) == 0;
}

bool is_bullet(const std::string& word) {
    if (word.empty()) return false;
    char last_char = word.back();
    if (word == "*" || word == "-") return true;
    if (last_char == '.' || last_char == ')') {
        if (word.length() == 1) return false;
        for (size_t i = 0; i < word.length() - 1; ++i) {
            if (!isdigit(word[i])) return false;
        }
        return true;
    }
    return false;
}

void add_word_to_line(std::string& line, const std::string& word) {
    if (!line.empty()) {
        line += " ";
    }
    line += word;
}

int string_to_int(const std::string& s) {
    std::stringstream ss(s);
    int result = 0;
    ss >> result;
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    std::cin >> n;
    std::string line_buffer;
    std::getline(std::cin, line_buffer); 

    std::deque<std::string> tokens;
    bool in_paragraph = false;
    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, line_buffer);
        if (line_buffer.find_first_not_of(" \t\r\n") == std::string::npos) { 
            if (in_paragraph) {
                tokens.push_back("%%PARAGRAPH_BREAK%%");
                in_paragraph = false;
            }
        } else {
            in_paragraph = true;
            std::stringstream ss(line_buffer);
            std::string word;
            while (ss >> word) {
                tokens.push_back(word);
            }
        }
    }
    std::getline(std::cin, line_buffer);
    std::stringstream cmd_ss(line_buffer);
    std::string token;
    FormatSettings settings;
    while (cmd_ss >> token) {
        if (token == "-w") {
            char next_char = cmd_ss.peek();
            while (isspace(next_char)) {
                cmd_ss.get(); 
                next_char = cmd_ss.peek();
            }
            if (isdigit(next_char)) {
                cmd_ss >> token;
                int width = string_to_int(token);
                settings.width_even = width;
                settings.width_odd = width;
            }
        } else if (token == "-w-e") {
            cmd_ss >> token; settings.width_even = string_to_int(token);
        } else if (token == "-w-o") {
            cmd_ss >> token; settings.width_odd = string_to_int(token);
        } else if (token == "h") {
            settings.hyphenate = true;
        }
    }

    if (tokens.empty()) return 0;

    std::vector<std::string> formatted_lines;
    std::string current_line = "";
    int line_index = 0;

    while (!tokens.empty()) {
        std::string word = tokens.front();
        tokens.pop_front();

        if (word == "%%PARAGRAPH_BREAK%%") {
            if (!current_line.empty()) {
                formatted_lines.push_back(current_line);
                line_index++;
            }
            current_line = "";
            continue;
        }

        if (is_bullet(word)) {
            if (!current_line.empty()) {
                formatted_lines.push_back(current_line);
                line_index++;
            }
            current_line = word;
            continue;
        }

        int max_width = (line_index % 2 == 0) ? settings.width_even : settings.width_odd;
        int space_needed = current_line.empty() ? 0 : 1;

        if (current_line.length() + space_needed + word.length() <= (size_t)max_width) {
            add_word_to_line(current_line, word);
        } else { 
            if (is_special_word(word)) {
                if (!current_line.empty()) {
                    formatted_lines.push_back(current_line);
                    line_index++;
                }
                formatted_lines.push_back(word);
                line_index++;
                current_line = "";
            } else if (settings.hyphenate) {
                int space_left = max_width - (current_line.length() + space_needed);
                int chars_to_take = space_left - 1;

                if (chars_to_take > 0 && word.length() > (size_t)chars_to_take) {
                    add_word_to_line(current_line, word.substr(0, chars_to_take) + "-");
                    tokens.push_front(word.substr(chars_to_take));
                    
                    formatted_lines.push_back(current_line);
                    line_index++;
                    current_line = "";
                } else {
                    if (!current_line.empty()) {
                       formatted_lines.push_back(current_line);
                       line_index++;
                    }
                    current_line = word;
                }
            } else {
                if (!current_line.empty()) {
                    formatted_lines.push_back(current_line);
                    line_index++;
                }
                current_line = word;
            }
        }
    }

    if (!current_line.empty()) {
        formatted_lines.push_back(current_line);
    }

    for (size_t i = 0; i < formatted_lines.size(); ++i) {
        std::cout << formatted_lines[i] << (i == formatted_lines.size() - 1 ? "" : "\n");
    }
    
    return 0;
}