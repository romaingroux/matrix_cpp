#include <iostream>
#include <string>
#include <vector>

#include "Matrix/Matrix.hpp"
#include "Matrix/Matrix2D.hpp"
#include "Matrix/Matrix3D.hpp"


int main()
{	Matrix3D<double> m(2,3,4) ;
	for(size_t i=0; i<m.get_data_size(); i++)
	{	m.set(i,i) ; }
    std::cout << "--------------------" << std::endl ;
    std::cout << m << std::endl ;
	std::cout << "--------------------" << std::endl ;
	
    Matrix3D<double> m2 = m + 3. ;
    std::cout << m2 << std::endl ;

	return 0 ;
}
