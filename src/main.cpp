#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Matrix/Matrix2D.hpp"
#include "Matrix/Matrix3D.hpp"
#include "Matrix/Matrix4D.hpp"

template<class T>
std::ostream& operator << (std::ostream& stream, const std::vector<T>& v)
{   for(const auto& i : v)
    {   stream << i << ' ' ; }
    return stream ;
}

int main()
{	Matrix2D<int> m(1,1) ;
    std::cout << "--------------------" << std::endl ;
    std::cout << m << std::endl ;
	std::cout << "--------------------" << std::endl ;

	return 0 ;
}
