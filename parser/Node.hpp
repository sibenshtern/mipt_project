#pragma once

#include <vector>

struct Node{
    std::vector<double> data{};
    std::vector<double> errors{};

    Node() = default;
    Node(std::vector<double> data) : data(data){};
    Node(std::vector<double> data, std::vector<double> errors) : data(data), errors(errors){};
};
