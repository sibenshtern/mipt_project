#include <vector>
#include <cmath>

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