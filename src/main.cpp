#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

using std::size_t;

namespace {
bool has_arg(const char * source, const char * arg)
{
    return std::strncmp(source, arg, std::strlen(arg)) == 0;
}

bool get_arg(const char * source, const char * arg, const char *& value)
{
    if (has_arg(source, arg)) {
        value = source + std::strlen(arg) + 1;
        return true;
    }
    return false;
}

std::optional<size_t> get_head_count(const char * head_count)
{
    return head_count == nullptr ? std::nullopt : std::optional<size_t>(std::strtoul(head_count, nullptr, 10));
}

bool greater_or_none(const std::optional<size_t> & opt, size_t value)
{
    return !opt || opt > value;
}

std::pair<size_t, size_t> get_input_range(const char * input_range)
{
    std::pair<size_t, size_t> res;
    char * end;
    res.first = std::strtoul(input_range, &end, 10);
    res.second = std::strtoul(end + 1, nullptr, 10);
    return res;
}

bool read_or_skip_line(std::ifstream & source, std::string * dest)
{
    if (dest == nullptr) {
        source.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else {
        std::getline(source, *dest);
    }
    return source.good();
}

} // anonymous namespace

int main(int argc, char * argv[])
{
    bool repeat = false;
    const char * head_count = nullptr;
    const char * input_range = nullptr;
    const char * input_file_name = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] != '-') {
                switch (argv[i][1]) {
                case 'i':
                    input_range = argv[++i];
                    break;
                case 'n':
                    head_count = argv[++i];
                    break;
                case 'r':
                    repeat = true;
                    break;
                }
            }
            else {
                if (get_arg(argv[i], "--input-range", input_range)) {
                }
                else if (get_arg(argv[i], "--head-count", head_count)) {
                }
                else if (has_arg(argv[i], "--repeat")) {
                    repeat = true;
                }
            }
        }
        else {
            input_file_name = argv[i];
        }
    }

    auto count = get_head_count(head_count);
    std::mt19937 engine(std::random_device{}());
    if (input_range != nullptr) {
        auto [lo, hi] = get_input_range(input_range);
        if (repeat) {
            for (size_t i = 0; greater_or_none(count, i); ++i) {
                size_t j = std::uniform_int_distribution<size_t>(lo, hi)(engine);
                std::cout << j << '\n';
            }
        }
        else {
            size_t size = greater_or_none(count, hi - lo + 1) ? hi - lo + 1 : count.value();

            /* Use Fisher-Yates shuffle, but stop after 'size' iterations
             * (so that if "head-count" is given we won't do unnecessary work) */
            std::unordered_map<size_t, size_t> permutation;
            permutation.reserve(size);
            for (size_t i = lo; i < size + lo; ++i) {
                size_t j = std::uniform_int_distribution<size_t>(i, hi)(engine);
                size_t & j_value = permutation.emplace(j, j).first->second;
                std::cout << j_value << '\n';
                if (i != j) {
                    auto it = permutation.find(i);
                    j_value = it != permutation.end() ? it->second : i;
                }
            }
        }
    }
    else {
        std::ifstream file(input_file_name);
        if (repeat) {
            std::vector<std::string> lines;
            std::string line;
            while (std::getline(file, line)) {
                lines.push_back(std::move(line));
            }
            if (lines.empty()) {
                return 0;
            }
            for (size_t i = 0; greater_or_none(count, i); ++i) {
                size_t j = std::uniform_int_distribution<size_t>(0, lines.size() - 1)(engine);
                std::cout << lines[j] << '\n';
            }
        }
        else {
            /* Use Reservoir sampling on input, so that if "head-count" is given we won't store all the strings
             * but only a random sample of them */
            std::vector<std::string> lines_sample;
            std::string line;
            while (greater_or_none(count, lines_sample.size()) && std::getline(file, line)) {
                lines_sample.push_back(std::move(line));
            }
            if (file.good()) {
                std::string * dest;
                size_t lines_count = lines_sample.size();
                do {
                    size_t j = std::uniform_int_distribution<size_t>(0, lines_count)(engine);
                    dest = j < lines_sample.size() ? &lines_sample[j] : nullptr;
                    ++lines_count;
                } while (read_or_skip_line(file, dest));
            }

            // Use Fisher-Yates shuffle on obtained sample
            for (size_t i = 0; i < lines_sample.size(); ++i) {
                size_t j = std::uniform_int_distribution<size_t>(i, lines_sample.size() - 1)(engine);
                std::cout << std::exchange(lines_sample[j], std::move(lines_sample[i])) << '\n';
            }
        }
    }
}
