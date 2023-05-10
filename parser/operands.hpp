#pragma once

#include <vector>
#include <cmath>

#include "FormulaParser.hpp"

Node operator + (Node &first, Node &second) 
{   
    Node node = Node();
    if (second.data.size() == 1){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[item] + second.data[0]);
            node.errors.push_back(std::pow(std::pow(first.errors[item], 2) + std::pow(second.errors[0], 2), 0.5));
        }
        return node;
    }
    else if (first.data.size() == 1){
        int size = second.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[0] + second.data[item]);
            node.errors.push_back(std::pow(std::pow(first.errors[0], 2) + std::pow(second.errors[item], 2), 0.5));
        }
        return node;
    }
    else if(second.data.size() == first.data.size()){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[item] + second.data[item]);
            node.errors.push_back(std::pow(std::pow(first.errors[item], 2) + std::pow(second.errors[item], 2), 0.5));
        }
        return node;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

Node operator - (Node &first, Node &second) 
{   
    Node node = Node();
    if (second.data.size() == 1){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[item] - second.data[0]);
            node.errors.push_back(std::pow(std::pow(first.errors[item], 2) + std::pow(second.errors[0], 2), 0.5));
        }
        return node;
    }
    else if (first.data.size() == 1){
        int size = second.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[0] - second.data[item]);
            node.errors.push_back(std::pow(std::pow(first.errors[0], 2) + std::pow(second.errors[item], 2), 0.5));
        }
        return node;
    }
    else if(second.data.size() == first.data.size()){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[item] - second.data[item]);
            node.errors.push_back(std::pow(std::pow(first.errors[item], 2) + std::pow(second.errors[item], 2), 0.5));
        }
        return node;
    }
    throw std::runtime_error("Impossible to convert sizes");
}


// TODO OPTIMIZE
Node operator * (Node first, Node second) 
{   
    Node node = Node();
    if (second.data.size() == 1){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[item] * second.data[0]);
            double value = first.data[item] * second.data[0];
            double error = 0;
            if (first.data[item] != 0) error += std::pow(first.errors[item] / first.data[item], 2);
            if (second.data[0] != 0) error += std::pow(second.errors[0] / second.data[0], 2);
            node.errors.push_back(std::pow(error, 0.5) * abs(value));
        }
        return node;
    }
    else if (first.data.size() == 1){
        int size = second.data.size();
        for (int item = 0; item < size; item++){
            node.data.push_back(first.data[0] * second.data[item]);
            double value = first.data[0] * second.data[item];
            double error = 0;
            if (first.data[0] != 0) error += std::pow(first.errors[0] / first.data[0], 2);
            if (second.data[item] != 0) error += std::pow(second.errors[item] / second.data[item], 2);
            node.errors.push_back(std::pow(error, 0.5) * abs(value));
        }
        return node;
    }
    else if(second.data.size() == first.data.size()){
        int size = first.data.size();
        for (int item = 0; item < size; item++){
            double value = first.data[item] * second.data[item];
            node.data.push_back(first.data[item] * second.data[item]);
            double error = 0;
            if (first.data[item] != 0) error += std::pow(first.errors[item] / first.data[item], 2);
            if (second.data[item] != 0) error += std::pow(second.errors[item] / second.data[item], 2);
            node.errors.push_back(std::pow(error, 0.5) * abs(value));
        }
        return node;
    }
    throw std::runtime_error("Impossible to convert sizes");
}
































std::vector<double> operator + (std::vector<double> &first, std::vector<double> &second) 
{   
    std::vector <double> data;
    if (second.size() == 1){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] + second[0]);
        }
        return data;
    }
    else if (first.size() == 1){
        int size = second.size();
        for (int item = 0; item < size; item++){
            data.push_back(second[item] + first[0]);
        }
        return data;
    }
    else if(second.size() == first.size()){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] + second[item]);
        }
        return data;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

std::vector<double> operator - (std::vector<double> &first, std::vector<double> &second) 
{   
     
    std::vector <double> data;
    if (second.size() == 1){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] - second[0]);
        }
        return data;
    }
    else if (first.size() == 1){
        int size = second.size();
        for (int item = 0; item < size; item++){
            data.push_back(second[item] - first[0]);
        }
        return data;
    }
    else if(second.size() == first.size()){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] - second[item]);
        }
        return data;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

std::vector<double> operator * (std::vector<double> &first, std::vector<double> &second) 
{   

    std::vector <double> data;
    if (second.size() == 1){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] * second[0]);
        }
        return data;
    }
    else if (first.size() == 1){
        int size = second.size();
        for (int item = 0; item < size; item++){
            data.push_back(second[item] * first[0]);
        }
        return data;
    }
    else if(second.size() == first.size()){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(first[item] * second[item]);
        }
        return data;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

std::vector<double> operator / (std::vector<double> &first, std::vector<double> &second) 
{   
    std::vector <double> data;
    if (second.size() == 1){
        int size = first.size();
        for (int item = 0; item < size; item++){
            if (second[0] != 0) data.push_back(first[item] / second[0]);
            else throw std::runtime_error("Divide by zero");
        }
        return data;
    }
    else if (first.size() == 1){
        int size = second.size();
        for (int item = 0; item < size; item++){
            if (first[0] != 0) data.push_back(second[item] * first[0]);
            else throw std::runtime_error("Divide by zero");
        }
        return data;
    }
    else if(second.size() == first.size()){
        int size = first.size();
        for (int item = 0; item < size; item++){
            if (second[item]) data.push_back(first[item] * second[item]);
            else throw std::runtime_error("Divide to zero");
        }
        return data;
    }
    throw std::runtime_error("Impossible to convert sizes");
}

std::vector<double> operator ^ (std::vector<double> &first, std::vector<double> &second) 
{   
    std::vector <double> data;
    if (second.size() == 1){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(std::pow(first[item], second[0]));
        }
        return data;
    }
    else if (first.size() == 1){
        int size = second.size();
        for (int item = 0; item < size; item++){
            data.push_back(std::pow(second[item], first[0]));
        }
        return data;
    }
    else if(second.size() == first.size()){
        int size = first.size();
        for (int item = 0; item < size; item++){
            data.push_back(std::pow(first[item], second[item]));
        }
        return data;
    }
    throw std::runtime_error("Impossible to convert sizes");
}