#include <iostream>
#include <string>
#include <vector>

#include "Matrix/Matrix.hpp"
#include "Matrix/Matrix2D.hpp"


int main()
{	Matrix<double>* m = new Matrix2D<double>(2,3) ;
	for(size_t i=0; i<m->get_data_size(); i++)
	{	m->set(i,i) ; }
    std::cout << "--------------------" << std::endl ;
    std::cout << *m << std::endl ;
	std::cout << "--------------------" << std::endl ;
	delete m ;
	return 0 ;
}
