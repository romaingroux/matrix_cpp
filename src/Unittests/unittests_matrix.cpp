#include <UnitTest++/UnitTest++.h>
#include <numeric> // accumulate()


#include "Matrix/Matrix.hpp"
#include "Matrix/Matrix2D.hpp"
#include "Matrix/Matrix3D.hpp"
#include "Matrix/Matrix4D.hpp"

/*!
 * \brief Given a matrix and an offset, this methods converts
 * the offset into a coordinates vector (row, col, ...). It is
 * a simple copy/paste of Matrix<T>::convert_to_coord() which is
 * private.
 * \param m a matrix.
 * \param offset an offset
 * \return a vector of coordinates (row,col,...) corresponding to
 * the offset for the given matrix.
 */
std::vector<size_t> convert_to_coord(const Matrix<int>& m, size_t offset)
{
    std::vector<size_t> dim = m.get_dim() ; // (row, col, ...) format
    if(dim.size() > 1)
    {   std::swap(dim[0], dim[1]) ; } // (x,y,...) format

    std::vector<size_t> coord(dim.size(), 0) ;
    std::vector<size_t> dim_prod(dim.size(), 0) ;
    dim_prod[0] = 1 ;
    if(dim.size() > 1)
    {   dim_prod[1] = dim[0] ; }
    if(dim.size() > 2)
    {   for(size_t i=2; i<dim.size(); i++)
        {   dim_prod[i] = dim_prod[i-1]*dim[i-1] ; }
    }

    for(int i=dim.size()-1; i>=0; i--)
    {   size_t c = offset / dim_prod[i] ;
        coord[i] = c ;
        offset -= (dim_prod[i]*c) ;
    }

    if(dim.size() > 1)
    {   std::swap(coord[0], coord[1]) ; } // (row,col,...) format
    return coord ;
}

template<class T>
std::ostream& operator << (std::ostream& stream, const std::vector<T>& v)
{   for(const auto& i : v)
    {   stream << i << ' ' ; }
    return stream ;
}


/*
// Matrix test suite
SUITE(Matrix)
{   // displays message
    TEST(message)
    {   std::cout << "Starting Matrix tests..." << std::endl ; }

    // tests normal constructor
    TEST(constructor)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_1.size(), m1.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_1, m1.get_dim(), dim_1.size()) ;
            CHECK_EQUAL(data_size_1, m1.get_data_size()) ;

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_2.size(), m2.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_2, m2.get_dim(), dim_2.size()) ;
            CHECK_EQUAL(data_size_2, m2.get_data_size()) ;
            CHECK_EQUAL(data_size_2, m2.get_data().size()) ;

            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_3.size(), m3.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_3, m3.get_dim(), dim_3.size()) ;
            CHECK_EQUAL(data_size_3, m3.get_data_size()) ;
            CHECK_EQUAL(data_size_3, m3.get_data().size()) ;
        }

    }

    // tests contructor with value
    TEST(constructor_value)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_1.size(), m1.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_1, m1.get_dim(), dim_1.size()) ;
            CHECK_EQUAL(data_size_1, m1.get_data_size()) ;
            for(const auto x : m1.get_data())
            {   CHECK_EQUAL(i, x) ; }

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_2.size(), m2.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_2, m2.get_dim(), dim_2.size()) ;
            CHECK_EQUAL(data_size_2, m2.get_data_size()) ;
            CHECK_EQUAL(data_size_2, m2.get_data().size()) ;
            for(const auto x : m2.get_data())
            {   CHECK_EQUAL(i, x) ; }

            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            CHECK_EQUAL(dim_3.size(), m3.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim_3, m3.get_dim(), dim_3.size()) ;
            CHECK_EQUAL(data_size_3, m3.get_data_size()) ;
            CHECK_EQUAL(data_size_3, m3.get_data().size()) ;
            for(const auto x : m3.get_data())
            {   CHECK_EQUAL(i, x) ; }
        }
    }

    // tests the get() method, compare a value get with offset with the value get with coordinates
    // (computed from offset)
    TEST(get)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   CHECK_EQUAL(m1.get(j), m1.get(convert_to_coord(m1, j))) ; }

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   CHECK_EQUAL(m2.get(j), m2.get(convert_to_coord(m2, j))) ; }

            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   CHECK_EQUAL(m3.get(j), m3.get(convert_to_coord(m3, j))) ; }
        }
    }


    // test the set() method, set a value and then check it using get()
    TEST(set)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   m1.set(j,j) ; }
            for(size_t j=0; j<data_size_1; j++)
            {   CHECK_EQUAL(m1.get(j), j) ; }

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   m2.set(j,j) ; }
            for(size_t j=0; j<data_size_2; j++)
            {   CHECK_EQUAL(m2.get(j), j) ; }

            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   m3.set(j,j) ; }
            for(size_t j=0; j<data_size_3; j++)
            {   CHECK_EQUAL(m3.get(j), j) ; }
        }
    }

    // tests the == and =! operators
    TEST(comparison_operators)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            Matrix<int> m1_2(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   m1.set(j,j) ; m1_2.set(j,j) ; }
            // equal content
            CHECK_EQUAL(true, m1 == m1) ;
            CHECK_EQUAL(true, m1 == m1_2) ;
            // different content
            m1.set(data_size_1-1, -100) ;
            CHECK_EQUAL(false, m1 == m1_2) ;
            CHECK_EQUAL(true,  m1 != m1_2) ;

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            Matrix<int> m2_2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   m2.set(j,j) ; m2_2.set(j,j) ; }
            // equal content
            CHECK_EQUAL(true, m2 == m2) ;
            CHECK_EQUAL(true, m2 == m2_2) ;
            // different content
            m2.set(data_size_2 == 0 ? 0 : data_size_2-1, -100) ; // sometimes contains 0 elements
            CHECK_EQUAL(false, m2 == m2_2) ;
            CHECK_EQUAL(true,  m2 != m2_2) ;

            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            Matrix<int> m3_2(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   m3.set(j,j) ; m3_2.set(j,j) ; }
            // equal content
            CHECK_EQUAL(true, m3 == m3) ;
            CHECK_EQUAL(true, m3 == m3_2) ;

            // different dimensions
            CHECK_EQUAL(false, m1 == m3) ;
            CHECK_EQUAL(true,  m1 != m3) ;
        }
    }

    // tests the = operator
    TEST(assignment_operator)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            Matrix<int> m1_2(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   m1.set(j,j) ; }
            m1_2 = m1 ;
            CHECK_EQUAL(true, m1 == m1_2) ;

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            Matrix<int> m2_2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   m2.set(j,j) ; }
            m2_2 = m2 ;
            CHECK_EQUAL(true, m2 == m2_2) ;


            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            Matrix<int> m3_2(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   m3.set(j,j) ; }
            m3_2 = m3 ;
            CHECK_EQUAL(true, m3 == m3_2) ;
        }
    }

    // tests the copy constuctor, not before because it uses the == operator to
    // check that the content of two matrices are equal.
    TEST(constructor_copy)
    {   std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   m1.set(j,j) ; }
            Matrix<int> m1_2(m1) ;
            CHECK_EQUAL(true, m1 == m1_2) ;

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   m2.set(j,j) ; }
            Matrix<int> m2_2(m2) ;
            CHECK_EQUAL(true, m2 == m2_2) ;


            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   m3.set(j,j) ; }
            Matrix<int> m3_2(m3) ;
            CHECK_EQUAL(true, m3 == m3_2) ;
        }
    }

    // tests the () operator
    TEST(parenthesis_operator)
    {
        std::vector<size_t> dim_1, dim_2, dim_3 ;
        size_t data_size_1, data_size_2, data_size_3 ;

        // from 0D to 10D
        for(size_t i=1; i<11; i++)
        {
            dim_1.push_back(i+1) ;
            dim_2.push_back(i) ;
            dim_3.push_back(0) ;

            // has non-0 dimensions : 1 /1x2 / 1x2x3 / ... / 1x2x...x11
            Matrix<int> m1(dim_1, i) ;
            data_size_1 = std::accumulate(dim_1.begin(), dim_1.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_1; j++)
            {   m1.set(j,j) ; }
            for(size_t j=0; j<data_size_1; j++)
            {   CHECK_EQUAL(m1.get(j), m1(convert_to_coord(m1, j))) ; }

            // always has a zero dimension : 0 / 0x1 / 0x1x2/ ... / 0x1x...x10
            Matrix<int> m2(dim_2, i) ;
            Matrix<int> m2_2(dim_2, i) ;
            data_size_2 = std::accumulate(dim_2.begin(), dim_2.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_2; j++)
            {   m2.set(j,j) ; }
            for(size_t j=0; j<data_size_2; j++)
            {   CHECK_EQUAL(m2.get(j), m2(convert_to_coord(m2, j))) ; }


            // is a 0 dimension matrix : 0 / 0x0 / 0x0x...x0
            Matrix<int> m3(dim_3, i) ;
            Matrix<int> m3_2(dim_3, i) ;
            data_size_3 = std::accumulate(dim_3.begin(), dim_3.end(), 1, std::multiplies<int>()) ;
            for(size_t j=0; j<data_size_3; j++)
            {   m3.set(j,j) ; }
            for(size_t j=0; j<data_size_3; j++)
            {   CHECK_EQUAL(m3.get(j), m3(convert_to_coord(m3, j))) ; }
        }
    }
}

*/


SUITE(Matrix2D)
{   // displays message
    TEST(message)
    {   std::cout << "Starting Matrix2D tests..." << std::endl ; }

    // tests contructor
    TEST(constructor)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   std::vector<size_t> dim = {i,j} ;
                Matrix2D<int> m(i,j) ;
                CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                            m.get_data_size()) ;
            }
        }
    }

    // tests contructor with value
    TEST(constructor_value)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   std::vector<size_t> dim = {i,j} ;
                Matrix2D<int> m(i,j,n) ;
                CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                            m.get_data_size()) ;
                for(const auto& i : m.get_data())
                {   CHECK_EQUAL(n, i) ; }
            }
        }
    }

    // tests the copy constructor
    TEST(constructor_copy)
    {
        for(size_t i=1; i<11; i++)
        {   std::vector<size_t> dim ;

            // has non-0 dimensions : 1x2 / 2x3 / ...
            dim = {i, i+1} ;
            Matrix2D<int> m1(i,i+1) ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   m1.set(j,j) ; }
            Matrix2D<int> m1_2(m1) ;
            CHECK_EQUAL(true, m1 == m1_2) ;

            // always has a zero dimension : // has a zero dimension : 0x1 / 0x2 / ...
            dim = {0, i} ;
            Matrix2D<int> m2(0,i) ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   m2.set(j,j) ; }
            Matrix2D<int> m2_2(m2) ;
            CHECK_EQUAL(true, m2 == m2_2) ;

            // is a 0 dimension matrix : 0x0
            dim = {0, 0} ;
            Matrix2D<int> m3(0,0) ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   m3.set(j,j) ; }
            Matrix2D<int> m3_2(m3) ;
            CHECK_EQUAL(true, m3 == m3_2) ;
        }
    }

    // tests the get() method, compare a value get with offset with the value get with coordinates
    // (computed from offset)
    TEST(get)
    {
        for(size_t i=1; i<11; i++)
        {   std::vector<size_t> dim ;

            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1, i) ;
            dim = {i,i+1} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m1, j) ;
                CHECK_EQUAL(m1.get(j), m1.get(coord[0], coord[1])) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m2, j) ;
                CHECK_EQUAL(m2.get(j), m2.get(coord[0], coord[1])) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m3, j) ;
                CHECK_EQUAL(m3.get(j), m3.get(coord[0], coord[1])) ;
            }
        }
    }


    // test the set() method, set a value and then check it using get()
    TEST(set)
    {
        for(size_t i=1; i<11; i++)
        {   std::vector<size_t> dim ;

            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1, i) ;
            dim = {i,i+1} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m1, j) ;
                m1.set(coord[0], coord[1], j) ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m1.get(j)) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m2, j) ;
                m2.set(coord[0], coord[1], j) ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m2.get(j)) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m3, j) ;
                m3.set(coord[0], coord[1], j) ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m3.get(j)) ; }
        }
    }

    // tests get_nrow()
    TEST(get_nrow)
    {   for(size_t i=1; i<11; i++)
        {
            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1) ;
            CHECK_EQUAL(i, m1.get_nrow()) ;

            // always has a zero dimension : // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i) ;
            CHECK_EQUAL(0, m2.get_nrow()) ;

            // is a 0 dimension matrix : 0x0
            Matrix2D<int> m3(0,0) ;
            CHECK_EQUAL(0, m3.get_nrow()) ;
        }
    }

    // tests get_ncol()
    TEST(get_ncol)
    {   for(size_t i=1; i<11; i++)
        {
            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1) ;
            CHECK_EQUAL(i+1, m1.get_ncol()) ;

            // always has a zero dimension : // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i) ;
            CHECK_EQUAL(i, m2.get_ncol()) ;

            // is a 0 dimension matrix : 0x0
            Matrix2D<int> m3(0,0) ;
            CHECK_EQUAL(0, m3.get_ncol()) ;
        }
    }

    // tests get_row()
    TEST(get_row)
    {   for(size_t i=0; i<11; i++)
        {
            Matrix2D<int> m(5,i) ;
            for(size_t j=0; j<m.get_nrow()*m.get_ncol(); j++)
            {   m.set(j, j) ; }

            for(size_t j=0; j<m.get_nrow(); j++)
            {
                std::vector<int> row(m.get_ncol()) ;
                for(size_t n=0, k=j*m.get_ncol(); n<m.get_ncol(); n++, k++)
                {   row[n] = k ; }

                CHECK_EQUAL(i, m.get_row(j).size()) ;
                CHECK_ARRAY_EQUAL(row, m.get_row(j), row.size()) ;
            }
            CHECK_THROW(m.get_row(9999), std::out_of_range) ;
        }
    }

    // tests get_col()
    TEST(get_col)
    {   for(size_t i=4; i<5; i++)
        {
            Matrix2D<int> m(i,5) ;
            for(size_t j=0; j<m.get_nrow()*m.get_ncol(); j++)
            {   m.set(j, j) ; }

            for(size_t j=0; j<m.get_ncol(); j++)
            {
                std::vector<int> col(m.get_nrow()) ;
                for(size_t n=0, k=j; n<m.get_nrow(); n++, k+=m.get_ncol())
                {   col[n] = k ; }

                CHECK_EQUAL(i, m.get_col(j).size()) ;
                CHECK_ARRAY_EQUAL(col, m.get_col(j), col.size()) ;
            }
            CHECK_THROW(m.get_col(9999), std::out_of_range) ;
        }
    }

    // tests set_row()
    TEST(set_row)
    {   for(size_t i=0; i<11; i++)
        {
            Matrix2D<int> m(5,i) ;
            for(size_t j=0; j<m.get_nrow()*m.get_ncol(); j++)
            {   m.set(j, j) ; }

            for(size_t j=0; j<m.get_nrow(); j++)
            {   std::vector<int> new_row(i, 999) ;
                m.set_row(j, new_row) ;
                CHECK_EQUAL(i, m.get_row(j).size()) ;
                CHECK_ARRAY_EQUAL(new_row, m.get_row(j), new_row.size()) ;
            }

            CHECK_THROW(m.set_row(9999, std::vector<int>(i,0)),   std::out_of_range) ;
            CHECK_THROW(m.set_row(0,    std::vector<int>(i+1,0)), std::invalid_argument) ;
        }
    }

    // tests set_col()
    TEST(set_col)
    {   for(size_t i=0; i<11; i++)
        {
            Matrix2D<int> m(i,5) ;
            for(size_t j=0; j<m.get_nrow()*m.get_ncol(); j++)
            {   m.set(j, j) ; }

            for(size_t j=0; j<m.get_ncol(); j++)
            {   std::vector<int> new_col(i, 999) ;
                m.set_col(j, new_col) ;
                CHECK_EQUAL(i, m.get_col(j).size()) ;
                CHECK_ARRAY_EQUAL(new_col, m.get_col(j), new_col.size()) ;
            }
            CHECK_THROW(m.set_col(9999, std::vector<int>(i,0)),   std::out_of_range) ;
            CHECK_THROW(m.set_col(0,    std::vector<int>(i+1,0)), std::invalid_argument) ;
        }
    }

    TEST(parenthesis_operator)
    {   for(size_t i=1; i<11; i++)
        {   std::vector<size_t> dim ;

            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1, i) ;
            dim = {i,i+1} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m1, j) ;
                m1(coord[0], coord[1]) = j ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m1.get(j)) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m2, j) ;
                m2(coord[0], coord[1]) = j ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m2.get(j)) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   std::vector<size_t> coord = convert_to_coord(m3, j) ;
                m3(coord[0], coord[1]) = j ;
            }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m3.get(j)) ; }
        }

    }

    // tests contructor from file
    // now because it uses previously tested methods
    TEST(constructor_file)
    {   std::string file_int1("./src/Unittests/data/matrix2d_int1.mat") ;
        std::string file_int2("./src/Unittests/data/matrix2d_int2.mat") ;
        std::string file_int3("./src/Unittests/data/matrix2d_int3.mat") ;
        std::string file_int4("./src/Unittests/data/matrix2d_int4.mat") ;
        std::string file_int5("./src/Unittests/data/matrix2d_int5.mat") ;
        std::string file_int6("./src/Unittests/data/matrix2d_int6.mat") ;
        std::string file_int7("./src/Unittests/data/matrix2d_int7.mat") ;
        std::string file_int8("./src/Unittests/data/matrix2d_int8.mat") ;
        std::string file_int9("./src/Unittests/data/matrix2d_int9.mat") ;

        std::string file_char1("./src/Unittests/data/matrix2d_char1.mat") ;
        std::string file_double1("./src/Unittests/data/matrix2d_double1.mat") ;
        std::string file_ghost("./src/Unittests/data/foo.mat") ;


        std::vector<std::vector<int>>     v_int({{0,1,2,3},{4,5,6,7}}) ;
        std::vector<std::vector<char>>    v_char({{'A','A','A'},{'C','C','C'},
                                                  {'G','G','G'},{'T','T','T'}}) ;
        std::vector<std::vector<double>>  v_double({{0.,1.,2.,3.},{4.,5.,6.,7.}}) ;

        Matrix2D<int>  m_int(2,4)  ;  m_int.set_row(0, {0,1,2,3}) ; m_int.set_row(1, {4,5,6,7}) ;
        Matrix2D<char> m_char(4,3) ;  m_char.set_row(0, {'A','A','A'}) ; m_char.set_row(1, {'C','C','C'}) ;
                                      m_char.set_row(2, {'G','G','G'}) ; m_char.set_row(3, {'T','T','T'}) ;
        Matrix2D<double> m_dbl(2,4) ; m_dbl.set_row(0, {0.,1.,2.,3.}) ; m_dbl.set_row(1, {4.,5.,6.,7.}) ;

        // matrix of int
        Matrix2D<int> m_int1(file_int1) ; // this one is perfect
        Matrix2D<int> m_int2(file_int2) ; // this one has inhomogeneous spaceers but is OK

        CHECK_EQUAL(m_int, m_int1) ;
        CHECK_EQUAL(m_int, m_int2) ;

        // matrix with only 1 int
        Matrix2D<int> m_int3(file_int7) ;
        CHECK_EQUAL( Matrix2D<int>(1,1,1), m_int3) ;

        // empty matrix (empty file)
        Matrix2D<int> m_int4(file_int8) ;
        CHECK_EQUAL(Matrix2D<int>(0,0), m_int4) ;

        // empty matrix (only eol in file)
        Matrix2D<int> m_int5(file_int9) ;
        CHECK_EQUAL(Matrix2D<int>(0,0), m_int5) ;

        // these files are not well formatted
        CHECK_THROW(m_int2 = Matrix2D<int>(file_int3), std::runtime_error) ; // data are inhomogeneous
        CHECK_THROW(m_int2 = Matrix2D<int>(file_int4), std::runtime_error) ; // empty line
        CHECK_THROW(m_int2 = Matrix2D<int>(file_int5), std::runtime_error) ; // empty line
        CHECK_THROW(m_int2 = Matrix2D<int>(file_int6), std::runtime_error) ; // empty line

        // matrix of char
        Matrix2D<char> m_char1(file_char1) ;
        CHECK_EQUAL(m_char, m_char1) ;

        // matrix of double
        Matrix2D<double> m_dbl1(file_double1) ;
        CHECK_EQUAL(m_dbl, m_dbl1) ;

        // file does not exist
        CHECK_THROW(Matrix2D<int> m_int2(file_ghost), std::runtime_error) ;
    }

    // tests file format, writting a matrix and reading it should return the
    // same matrix, uses set() and the == operator
    // loading an empty file is not allowed (has no meaning, the file is empty)
    TEST(file_format)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   Matrix2D<int> m(i,j) ;
                for(size_t a=0; a<m.get_data_size(); a++)
                {   m.set(a, a) ; }
                std::ofstream file("./src/Unittests/data/matrix2d_out.mat") ;
                // write to file
                file << m << std::endl ;
                file.close() ;
                // read from file
                Matrix2D<int> m2("./src/Unittests/data/matrix2d_out.mat") ;
                // any matrix with at least one zero dimension is a null
                // matrix
                if(i==0 or j==0)
                {   CHECK_EQUAL(Matrix2D<int>(0,0), m2) ; }
                else
                {    CHECK_EQUAL(m, m2) ; }
            }
        }
    }
}




SUITE(Matrix3D)
{   // displays message
    TEST(message)
    {   std::cout << "Starting Matrix3D tests..." << std::endl ; }


    // tests constructor
    TEST(constructor)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k) ;
                    CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                    CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                    CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                                m.get_data_size()) ;
                }
            }
        }
    }

    // test constructor value
    TEST(constructor_value)
    {   int  n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k,n) ;
                    CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                    CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                    CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                                m.get_data_size()) ;
                    for(const auto& i : m.get_data())
                    {   CHECK_EQUAL(n, i) ; }
                }
            }
        }
    }

    // tests copy constructor
    TEST(constructor_copy)
    {   int  n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k,n) ;
                    Matrix3D<int> m2(m) ;
                    CHECK_EQUAL(m, m2) ;
                }
            }
        }
    }

    // tests contructor from file, uses the == operator
    TEST(constructor_file)
    {   std::string file_int1("./src/Unittests/data/matrix3d_int1.mat") ;
        std::string file_int2("./src/Unittests/data/matrix3d_int2.mat") ;
        std::string file_int3("./src/Unittests/data/matrix3d_int3.mat") ;
        std::string file_int4("./src/Unittests/data/matrix3d_int4.mat") ;
        std::string file_int5("./src/Unittests/data/matrix3d_int5.mat") ;
        std::string file_int6("./src/Unittests/data/matrix3d_int6.mat") ;
        std::string file_int7("./src/Unittests/data/matrix3d_int7.mat") ;
        std::string file_int8("./src/Unittests/data/matrix3d_int8.mat") ;
        std::string file_int9("./src/Unittests/data/matrix3d_int9.mat") ;
        std::string file_int10("./src/Unittests/data/matrix3d_int10.mat") ;
        std::string file_int11("./src/Unittests/data/matrix3d_int11.mat") ;
        std::string file_int12("./src/Unittests/data/matrix3d_int12.mat") ;
        std::string file_int13("./src/Unittests/data/matrix3d_int13.mat") ;
        std::string file_int14("./src/Unittests/data/matrix3d_int14.mat") ;
        std::string file_double("./src/Unittests/data/matrix3d_double.mat") ;
        std::string file_ghost("./src/Unittests/data/foo.mat") ;


        std::vector<int> v_int = {-1,0,2,0,
                                   0,3,0,4,
                                   0,0,0,0,
                                   0,0,0,0,
                                   0,5,-6,0,
                                   0,7,0,0} ;

        std::vector<int> v_int2 = {1} ;

        std::vector<double> v_dbl = {-1.,0., 2.,0.,
                                      0.,3., 0.,4.,
                                      0.,0., 0.,0.,
                                      0.,0., 0.,0.,
                                      0.,5.,-6.,0.,
                                      0.,7., 0.,0.} ;

        std::vector<size_t> dim = {2,4,3} ;
        std::vector<size_t> dim2 = {1,1,1} ;

        // matrix of int
        Matrix3D<int> m_int(file_int1) ;
        CHECK_EQUAL(dim.size(), m_int.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m_int.get_dim(), dim.size()) ;
        CHECK_EQUAL(v_int.size(), m_int.get_data_size()) ;
        CHECK_ARRAY_EQUAL(v_int, m_int.get_data(), v_int.size()) ;

        // matrix with only 1 int
        Matrix3D<int> m_int2(file_int12) ;
        CHECK_EQUAL(Matrix3D<int>(1,1,1,1), m_int2) ;

        // empty matrix (empty file)
        Matrix3D<int> m_int3(file_int13) ;
        CHECK_EQUAL(Matrix3D<int>(0,0,0), m_int3) ;

        // empty matrix (only eol in file)
        Matrix3D<int> m_int4(file_int13) ;
        CHECK_EQUAL(Matrix3D<int>(0,0,0), m_int4) ;

        // these files are not well formatted
        CHECK_THROW(Matrix3D<int> m_int3(file_int2),  std::runtime_error) ; // mixed data types
        CHECK_THROW(Matrix3D<int> m_int3(file_int3),  std::runtime_error) ; // slice of variable dim
        CHECK_THROW(Matrix3D<int> m_int3(file_int4),  std::runtime_error) ; // slice of variable dim
        CHECK_THROW(Matrix3D<int> m_int3(file_int5),  std::runtime_error) ; // slice of variable dim
        CHECK_THROW(Matrix3D<int> m_int3(file_int6),  std::runtime_error) ;  // empty line
        CHECK_THROW(Matrix3D<int> m_int3(file_int7),  std::runtime_error) ;  // empty line
        CHECK_THROW(Matrix3D<int> m_int3(file_int8),  std::runtime_error) ;  // empty line
        CHECK_THROW(Matrix3D<int> m_int3(file_int9),  std::runtime_error) ;  // empty line
        CHECK_THROW(Matrix3D<int> m_int3(file_int10), std::runtime_error) ;  // empty line
        CHECK_THROW(Matrix3D<int> m_int3(file_int11), std::runtime_error) ;  // empty line

        // this file does not exist
        CHECK_THROW(Matrix3D<int> m_int3(file_ghost), std::runtime_error) ;

        // matrix of double
        Matrix3D<double> m_double(file_double) ;
        CHECK_EQUAL(dim.size(), m_double.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m_double.get_dim(), dim.size()) ;
        CHECK_EQUAL(v_int.size(), m_double.get_data_size()) ;
        CHECK_ARRAY_EQUAL(v_int, m_double.get_data(), v_int.size()) ;
    }

    // tests get()
    TEST(get)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k,n) ;
                    for(size_t l=0; l<m.get_data_size(); l++)
                    {   std::vector<size_t> coord = convert_to_coord(m, l) ;
                        CHECK_EQUAL(m.get(l), m.get(coord[0], coord[1], coord[2])) ;
                    }
                }
            }
        }
    }

    // tests set()
    TEST(set)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k,n) ;
                    for(size_t l=0; l<m.get_data_size(); l++)
                    {   std::vector<size_t> coord = convert_to_coord(m, l) ;
                        m.set(coord[0], coord[1], coord[2], l) ;
                    }
                    for(size_t l=0; l<m.get_data_size(); l++)
                    {   CHECK_EQUAL(l, m.get(l)) ; }
                }
            }
        }
    }

    // tests the parenthesis operator
    TEST(parenthesis_operator)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   std::vector<size_t> dim = {i,j,k} ;
                    Matrix3D<int> m(i,j,k,n) ;
                    for(size_t l=0; l<m.get_data_size(); l++)
                    {   std::vector<size_t> coord = convert_to_coord(m, l) ;
                        m(coord[0], coord[1], coord[2]) = l ;
                    }
                    for(size_t l=0; l<m.get_data_size(); l++)
                    {   CHECK_EQUAL(l, m.get(l)) ; }
                }
            }
        }
    }

    // tests file format, writting a matrix and reading it should return the
    // same matrix, uses set() and the == operator
    TEST(file_format)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   Matrix3D<int> m(i,j,k) ;
                    for(size_t a=0; a<m.get_data_size(); a++)
                    {   m.set(a, a) ; }
                    std::ofstream file("./src/Unittests/data/matrix3d_out.mat") ;
                    // write to file
                    file << m << std::endl ;
                    file.close() ;
                    // read from file
                    Matrix3D<int> m2("./src/Unittests/data/matrix3d_out.mat") ;
                    // any matrix with at least one zero dimension is a null
                    // matrix
                    if(i==0 or j==0 or k==0)
                    {   CHECK_EQUAL(Matrix3D<int>(0,0,0), m2) ; }
                    else
                    {    CHECK_EQUAL(m, m2) ; }
                }
            }
        }
    }
}


SUITE(Matrix4D)
{
    // displays message
    TEST(message)
    {   std::cout << "Starting Matrix4D tests..." << std::endl ; }

    // constructor
    TEST(constructor)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,l) ;
                        CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                        CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                        CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                                    m.get_data_size()) ;
                    }
                }
            }
        }
    }

    // test constructor value
    TEST(constructor_value)
    {   int  n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,l,n) ;
                        CHECK_EQUAL(dim.size(), m.get_dim_size()) ;
                        CHECK_ARRAY_EQUAL(dim, m.get_dim(), dim.size()) ;
                        CHECK_EQUAL(std::accumulate(begin(dim), end(dim), 1, std::multiplies<int>()),
                                    m.get_data_size()) ;
                        for(const auto& i : m.get_data())
                        {   CHECK_EQUAL(n, i) ; }
                    }
                }
            }
        }
    }

    // tests copy constructor
    TEST(constructor_copy)
    {   int  n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,l,n) ;
                        Matrix4D<int> m2(m) ;
                        CHECK_EQUAL(m, m2) ;
                    }
                }
            }
        }
    }

    // tests contructor from file, uses the == operator
    TEST(constructor_file)
    {
        std::string file_int1("./src/Unittests/data/matrix4d_int1.mat") ;
        std::string file_int2("./src/Unittests/data/matrix4d_int2.mat") ;
        std::string file_int3("./src/Unittests/data/matrix4d_int3.mat") ;
        std::string file_int4("./src/Unittests/data/matrix4d_int4.mat") ;
        std::string file_int5("./src/Unittests/data/matrix4d_int5.mat") ;
        std::string file_int6("./src/Unittests/data/matrix4d_int6.mat") ;
        std::string file_int7("./src/Unittests/data/matrix4d_int7.mat") ;
        std::string file_int8("./src/Unittests/data/matrix4d_int8.mat") ;
        std::string file_int9("./src/Unittests/data/matrix4d_int9.mat") ;
        std::string file_int10("./src/Unittests/data/matrix4d_int10.mat") ;
        std::string file_int11("./src/Unittests/data/matrix4d_int11.mat") ;
        std::string file_int12("./src/Unittests/data/matrix4d_int12.mat") ;
        std::string file_int13("./src/Unittests/data/matrix4d_int13.mat") ;
        std::string file_int14("./src/Unittests/data/matrix4d_int14.mat") ;
        std::string file_int15("./src/Unittests/data/matrix4d_int15.mat") ;
        std::string file_int16("./src/Unittests/data/matrix4d_int16.mat") ;
        std::string file_int17("./src/Unittests/data/matrix4d_int17.mat") ;
        std::string file_int18("./src/Unittests/data/matrix4d_int18.mat") ;
        std::string file_int19("./src/Unittests/data/matrix4d_int19.mat") ;
        std::string file_int20("./src/Unittests/data/matrix4d_int20.mat") ;
        std::string file_dbl1("./src/Unittests/data/matrix4d_double1.mat") ;
        std::string file_ghost("./src/Unittests/data/foo.mat") ;


        std::vector<int> v_int = { 1, 2, 3,
                                   4, 5, 6,
                                   7, 8, 9,
                                  10,11,12,
                                  13,14,15,
                                  16,17,18,
                                  19,20,21,
                                  22,23,24,
                                   1, 2, 3,
                                   4, 5, 6,
                                   7, 8, 9,
                                  10,11,12,
                                  13,14,15,
                                  16,17,18,
                                  19,20,21,
                                  22,23,24} ;

        std::vector<double> v_dbl = { 1, 2, 3,
                                      4, 5, 6,
                                      7, 8, 9,
                                      10,11,12,
                                      13,14,15,
                                      16,17,18,
                                      19,20,21,
                                      22,23,24,
                                       1, 2, 3,
                                       4, 5, 6,
                                       7, 8, 9,
                                      10,11,12,
                                      13,14,15,
                                      16,17,18,
                                      19,20,21,
                                      22,23,24} ;

        std::vector<size_t> dim = {2,3,2,4} ;

        // matrix of int
        Matrix4D<int> m_int(file_int1) ;
        CHECK_EQUAL(dim.size(), m_int.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m_int.get_dim(), dim.size()) ;
        CHECK_EQUAL(v_int.size(), m_int.get_data_size()) ;
        CHECK_ARRAY_EQUAL(v_int, m_int.get_data(), v_int.size()) ;

        // matrix with only 1 int
        Matrix4D<int> m_int2(file_int18) ;
        CHECK_EQUAL(Matrix4D<int>(1,1,1,1,1), m_int2) ;

        // empty matrix (empty file)
        Matrix4D<int> m_int3(file_int19) ;
        CHECK_EQUAL(Matrix4D<int>(0,0,0,0), m_int3) ;

        // empty matrix (only eol in file)
        Matrix4D<int> m_int4(file_int20) ;
        CHECK_EQUAL(Matrix4D<int>(0,0,0,0), m_int3) ;


        // these files are not well formatted
        CHECK_THROW(Matrix4D<int> m_int5(file_int2), std::runtime_error) ;  // empty lines
        CHECK_THROW(Matrix4D<int> m_int5(file_int3), std::runtime_error) ;  // empty lines
        CHECK_THROW(Matrix4D<int> m_int5(file_int4), std::runtime_error) ;  // empty lines
        CHECK_THROW(Matrix4D<int> m_int5(file_int5), std::runtime_error) ;  // empty lines
        CHECK_THROW(Matrix4D<int> m_int5(file_int6), std::runtime_error) ;  // empty lines
        CHECK_THROW(Matrix4D<int> m_int5(file_int7), std::runtime_error) ;  // first line problem
        CHECK_THROW(Matrix4D<int> m_int5(file_int8), std::runtime_error) ;  // first line problem
        CHECK_THROW(Matrix4D<int> m_int5(file_int9), std::runtime_error) ;  // first line problem
        CHECK_THROW(Matrix4D<int> m_int5(file_int10), std::runtime_error) ; // second line problem
        CHECK_THROW(Matrix4D<int> m_int5(file_int11), std::runtime_error) ; // extra column
        CHECK_THROW(Matrix4D<int> m_int5(file_int12), std::runtime_error) ; // missing column
        CHECK_THROW(Matrix4D<int> m_int5(file_int13), std::runtime_error) ; // extra row
        CHECK_THROW(Matrix4D<int> m_int5(file_int14), std::runtime_error) ; // extra 2d slice
        CHECK_THROW(Matrix4D<int> m_int5(file_int15), std::runtime_error) ; // extra 2d slice
        CHECK_THROW(Matrix4D<int> m_int5(file_int16), std::runtime_error) ; // last line problem
        CHECK_THROW(Matrix4D<int> m_int5(file_int17), std::runtime_error) ; // mixded data types

        // this file does not exist
        CHECK_THROW(Matrix4D<int> m_int3(file_ghost), std::runtime_error) ;

        // matrix of double
        Matrix4D<double> m_dbl(file_dbl1) ;
        CHECK_EQUAL(dim.size(), m_dbl.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m_dbl.get_dim(), dim.size()) ;
        CHECK_EQUAL(v_dbl.size(), m_dbl.get_data_size()) ;
        CHECK_ARRAY_EQUAL(v_dbl, m_dbl.get_data(), v_dbl.size()) ;
    }

    // tests get()
    TEST(get)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,l,n) ;
                        for(size_t a=0; a<m.get_data_size(); a++)
                        {   std::vector<size_t> coord = convert_to_coord(m, a) ;
                            CHECK_EQUAL(m.get(a), m.get(coord[0], coord[1], coord[2], coord[3])) ;
                        }
                    }
                }
            }
        }
    }

    // tests set()
    TEST(set)
    {   int n = 999 ;
        for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,n) ;
                        for(size_t a=0; a<m.get_data_size(); a++)
                        {   std::vector<size_t> coord = convert_to_coord(m, a) ;
                            m.set(coord[0], coord[1], coord[2], coord[3], a) ;
                        }
                        for(size_t a=0; a<m.get_data_size(); a++)
                        {   CHECK_EQUAL(a, m.get(a)) ; }
                    }
                }
            }
        }
    }

    // tests file format, writting a matrix and reading it should return the
    // same matrix, uses set() and the == operator
    TEST(file_format)
    {   for(size_t i=0; i<10; i++)
        {   for(size_t j=0; j<10; j++)
            {   for(size_t k=0; k<10; k++)
                {   for(size_t l=0; l<10; l++)
                    {   std::vector<size_t> dim = {i,j,k,l} ;
                        Matrix4D<int> m(i,j,k,l) ;
                        for(size_t a=0; a<m.get_data_size(); a++)
                        {   m.set(a, a) ; }
                        std::ofstream file("./src/Unittests/data/matrix4d_out.mat") ;
                        // write to file
                        file << m << std::endl ;
                        file.close() ;
                        // read from file
                        Matrix4D<int> m2("./src/Unittests/data/matrix4d_out.mat") ;
                        // any matrix with at least one zero dimension is a null
                        // matrix
                        if(i==0 or j==0 or k==0 or l==0)
                        {   CHECK_EQUAL(Matrix4D<int>(0,0,0,0), m2) ; }
                        else
                        {    CHECK_EQUAL(m, m2) ; }
                    }
                }
            }
        }
    }
}
