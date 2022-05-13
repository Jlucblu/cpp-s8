#include "log_duration.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <future>


using namespace std;


template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

// �������� � ������� ��������������
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end, int level = 0) {
    // 1. ���� �������� �������� ������ 2 ���������, ������� �� �������
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    ++level;
    // 2. ������ ������, ���������� ��� �������� �������� ���������
    vector elements(range_begin, range_end);
    // ��� ��������� � typename iterator_traits<RandomIt>::value_type

    // 3. ��������� ������ �� ��� ������ �����
    auto mid = elements.begin() + range_length / 2;
    // 4. �������� ������� MergeSort �� ������ �������� �������
    if (level <= 2) {
        auto left_future = async([&]
            {
                MergeSort(elements.begin(), mid, level);
            });

        auto rigth_future = async([&]
            {
                MergeSort(mid, elements.end(), level);
            });
        left_future.get();
        rigth_future.get();
    }
    else {
        MergeSort(elements.begin(), mid, level);
        MergeSort(mid, elements.end(), level);
    }
    // 5. � ������� ��������� merge ������� ��������������� ��������
    // � �������� ��������
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

template <typename RandomIt>
void MergeSort2(RandomIt range_begin, RandomIt range_end) {
    // 1. ���� �������� �������� ������ 2 ���������, ������� �� �������
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. ������ ������, ���������� ��� �������� �������� ���������
    vector elements(range_begin, range_end);
    // ��� ��������� � typename iterator_traits<RandomIt>::value_type

    // 3. ��������� ������ �� ��� ������ �����
    auto mid = elements.begin() + range_length / 2;

    // 4. �������� ������� MergeSort �� ������ �������� �������
    MergeSort2(elements.begin(), mid);
    MergeSort2(mid, elements.end());

    // 5. � ������� ��������� merge ������� ��������������� ��������
    // � �������� ��������
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

void TestMergeSort() {
    {
        mt19937 generator;

        vector<int> test_vector(100);

        // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
        // ��������� �������� ��������������� ������������� ����������
        iota(test_vector.begin(), test_vector.end(), 1);

        // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
        // ������������ �������� � ��������� �������
        shuffle(test_vector.begin(), test_vector.end(), generator);

        // ������� ������ �� ����������
        //PrintRange(test_vector.begin(), test_vector.end());

        LOG_DURATION("Test1");
        // ��������� ������ � ������� ���������� ��������
        MergeSort(test_vector.begin(), test_vector.end());

        // ������� ���������
        //PrintRange(test_vector.begin(), test_vector.end());

        // ���������, ����� �� �������� ���������
        MergeSort(test_vector.data(), test_vector.data() + test_vector.size());
    }
    {
        mt19937 generator;

        vector<int> test_vector(100);

        // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
        // ��������� �������� ��������������� ������������� ����������
        iota(test_vector.begin(), test_vector.end(), 1);

        // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
        // ������������ �������� � ��������� �������
        shuffle(test_vector.begin(), test_vector.end(), generator);

        // ������� ������ �� ����������
        //PrintRange(test_vector.begin(), test_vector.end());

        LOG_DURATION("Test2");
        // ��������� ������ � ������� ���������� ��������
        MergeSort2(test_vector.begin(), test_vector.end());

        // ������� ���������
        //PrintRange(test_vector.begin(), test_vector.end());

        // ���������, ����� �� �������� ���������
        MergeSort2(test_vector.data(), test_vector.data() + test_vector.size());
    }
}