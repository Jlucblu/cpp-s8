#pragma once
#include "log_duration.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <execution>
#include <iterator>

using namespace std;


string GenerateQuery(mt19937& generator, int max_length, int space_rate) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string query(length, ' ');
    for (char& c : query) {
        const int rnd = uniform_int_distribution(0, space_rate - 1)(generator);
        if (rnd > 0) {
            c = 'a' + (rnd - 1);
        }
    }
    return query;
}

template <typename Solver>
void Test(string_view mark, string_view s, Solver solver) {
    int result;
    {
        LOG_DURATION(mark);
        result = solver(s);
    }
    cout << result << endl;
}

#define TEST(solver) Test(#solver, s, solver)

/*int CountWords(string_view str) {
    return count(execution::par, str.begin(), str.end(), ' ') + 1;
}*/

int CountWords(string_view str) {
    /* int count = 0;

    for (auto it = 0; it < str.size() - 1; ++it) {
        if (str.begin() == str.end()) {
            break;
        }

        if (it + 1 == str.size()) {
            ++count;
            break;
        }

        if (it == 0 && str[0] != ' ') {
            ++count;
        }

        if (str[it] != ' ' && str[it + 1] == ' ') {
            ++count;
        }
    }
    return count; */
    // подсчитайте количество слов,
    // игнорируя начальные, конечные
    // и подряд идущие пробелы

    // некорректное решение, с которым сравнивается производительность
    //return count(str.begin(), str.end(), ' ') + 1;
    if (str.empty()) {
        return 0;
    }

    int space_count = transform_reduce(execution::par,
        next(str.begin()), str.end(), // диапазон
        str.begin(), 0, // начальное значение
        plus<>{}, // группирующая функция
        [](char rh, char lh) {
            return (rh != ' ') && (lh == ' ');
        });

    if (str[0] != ' ') {
        ++space_count;
    }

    return space_count;
}

void Test1() {
    // должно вывести 3
    cout << CountWords("  pretty  little octopus "sv) << endl;

    mt19937 generator;

    const string s = GenerateQuery(generator, 100'000'000, 4);

    TEST(CountWords);
}