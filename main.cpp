#include <iostream>
#include <vector>
#include <time.h>

#include "Matrix/Matrix2D.hpp"



int main()
{   
    clock_t t1,t2;
    float diff ;

    // construction time
    t1=clock();
    for(size_t i=0; i<1000; i++)
    {   Matrix2D<int> m(1000, 1000, -1) ; }
    t2=clock();
    diff = ((float)t2-(float)t1);
    std::cout << "construction time for Matrix " << diff / 1000 << std::endl ;

    t1=clock();
    for(size_t i=0; i<1000; i++)
    {   std::vector<std::vector<int>> m(1000, std::vector<int>(1000,-1)) ; }
    t2=clock();
    diff = ((float)t2-(float)t1);
    std::cout << "construction time for vector " << diff / 1000 << std::endl ;
    

    // access time
    t1=clock();
    {    
        Matrix2D<int> m(1000, 1000, -1) ;
        for(size_t i=0; i<1000; i++)
        {   m.get(999,999) ; }
    }
    t2=clock();
    diff = ((float)t2-(float)t1);
    std::cout << "access time for Matrix " << diff / 1000 << std::endl ;

    t1=clock();
    {
        Matrix2D<int> m(1000, 1000, -1) ;
        for(size_t i=0; i<1000; i++)
        {   m(999,999) ; }
    }
    t2=clock();
    diff = ((float)t2-(float)t1);
    std::cout << "access time for Matrix " << diff / 1000 << std::endl ;

    t1=clock();
    {
        std::vector<std::vector<int>> m(1000, std::vector<int>(1000,-1)) ;
        for(size_t i=0; i<1000; i++)
        {   m[999][999] ; }
    }
    t2=clock();
    diff = ((float)t2-(float)t1);
    std::cout << "construction time for vector " << diff / 1000 << std::endl ;

	return 0 ;
}
