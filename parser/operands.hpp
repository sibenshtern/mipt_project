#pragma once

#include <vector>
#include <cmath>

#include "FormulaParser.hpp"

void calculateDataAndVariable(Node &node, double firstVariable, double firstError, double secondVariable, double secondError, int op) {
    double error = 0, value = 0;
    switch (op) {
        // +
        case 1: 
            node.data.push_back(firstVariable + secondVariable);
            node.errors.push_back(std::pow(std::pow(firstError, 2) + std::pow(secondError, 2), 0.5));
            return;
        // -
        case 2:
            node.data.push_back(firstVariable - secondVariable);
            node.errors.push_back(std::pow(std::pow(firstError, 2) + std::pow(secondError, 2), 0.5));
            return;
        // *
        case 3:
            error = 0;
            value = firstVariable * secondVariable;
            node.data.push_back(value);

            if (firstVariable != 0) error += std::pow(firstError / firstVariable, 2);
            if (secondVariable != 0) error += std::pow(secondError / secondVariable, 2);

            node.errors.push_back(std::pow(error, 0.5) * abs(value));
            return;
        // /
        case 4:
            error = 0;
            value = 0;

            if (secondVariable != 0) {
                value = firstVariable / secondVariable;
                node.data.push_back(value);
            }
            else throw std::runtime_error("Divide by zero");

            if (firstVariable != 0) error += std::pow(firstError / firstVariable, 2);
            if (secondVariable != 0) error += std::pow(secondError / secondVariable, 2);

            node.errors.push_back(std::pow(error, 0.5) * abs(value));
            return;
        // ^
        case 5:
            error = 0;
            value = std::pow(firstVariable, secondVariable);

            node.data.push_back(value);

            if (firstVariable != 0) error += std::pow(firstError / firstVariable, 2) * std::pow(secondVariable, 2);
            node.errors.push_back(std::pow(error, 0.5) * abs(value));
            return;
        throw std::runtime_error("Unknown precedence ");
    }

}


Node operation(Node &first, Node &second, int op) {
    Node node = Node();
    if (second.data.size() == 1) {
        int size = first.data.size();
        for (int item = 0; item < size; item++) {
            calculateDataAndVariable(node, first.data[item], first.errors[item], second.data[0], second.errors[0], op);
        }
        return node;
    }
    else if (first.data.size() == 1) {
        int size = second.data.size();
        for (int item = 0; item < size; item++) {
            calculateDataAndVariable(node, first.data[0], first.errors[0], second.data[item], second.errors[item], op);
        }
        return node;
    }
    else if(second.data.size() == first.data.size()) {
        int size = first.data.size();
        for (int item = 0; item < size; item++) {
            calculateDataAndVariable(node,first.data[item], first.errors[item], second.data[item], second.errors[item], op);
        }
        return node;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

Node operator + (Node &first, Node &second) {   
    return operation(first, second, 1);
}


Node operator - (Node &first, Node &second) {   
   return operation(first, second, 2);
}

Node operator * (Node &first, Node &second) {   
    return operation(first, second, 3);
}

Node operator / (Node &first, Node &second) {   
    return operation(first, second, 4);
}

Node operator ^ (Node &first, Node &second) {   
    return operation(first, second, 5);
}
