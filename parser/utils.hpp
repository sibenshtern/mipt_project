#include <iostream>
#include <vector>

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> v) {
    os << "{ ";
    for (int i = 0; i < v.size(); ++i)
        os << v[i] << " ";
    os << "}";
    return os;
}