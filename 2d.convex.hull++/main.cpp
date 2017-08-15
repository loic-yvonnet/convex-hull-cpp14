#include <iostream>
#include "point_concept.hpp"

struct p {
    int x;
    int y;
};

struct p2 {
    int x;
    int z;
};

struct p3 {

};

struct p4 {
    double X;
    double Y;
};

int main() {
    std::cout << hull::has_member_x<p>::value << std::endl;
    std::cout << hull::has_member_x<p3>::value << std::endl;
    
    std::cout << hull::is_point<p>::value << '\n';
    std::cout << hull::is_point<p2>::value << '\n';
    std::cout << hull::is_point<p3>::value << '\n';
    std::cout << hull::is_point<p4>::value << '\n';
    
    std::cout << std::endl;
}
