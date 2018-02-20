#include <UnitTest++/UnitTest++.h>
#include <numeric> // accumulate()


#include "Matrix/Matrix.hpp"

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

SUITE(Matrix2D)
{   // displays message
    TEST(message)
    {   std::cout << "Starting Matrix2D tests..." << std::endl ; }

}
