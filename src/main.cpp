#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <functional>

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
{	Matrix<int>* m = new Matrix2D<int>(2,3) ;
	for(size_t i=0; i<m->get_data_size(); i++)
	{	m->set(i,i) ; }
    std::cout << "--------------------" << std::endl ;
    std::cout << *m << std::endl ;
	std::cout << "--------------------" << std::endl ;
	return 0 ;
}
