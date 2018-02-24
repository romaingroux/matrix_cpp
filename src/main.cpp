#include <iostream>
#include "Matrix/Matrix3D.hpp"

int main()
{
	Matrix3D<int> m("/home/romaingroux/dev/matrix/src/Unittests/data/matrix3d_int1.mat") ;
	std::cout << m << std::endl ;
	std::cout << "---------------------" << std::endl ;
	return 0 ;
}
