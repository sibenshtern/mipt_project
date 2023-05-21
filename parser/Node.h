#ifndef NODE_H
#define NODE_H

#include <vector>

struct Node{
    std::vector<double> data{};
    std::vector<double> errors{};

    Node() = default;
    Node(std::vector<double> data) : data(data) {};
    Node(std::vector<double> data, std::vector<double> errors) : data(data), errors(errors) {};
};

#endif //NODE_H