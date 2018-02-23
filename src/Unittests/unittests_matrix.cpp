#include <UnitTest++/UnitTest++.h>
#include <numeric> // accumulate()


#include "Matrix/Matrix.hpp"
#include "Matrix/Matrix2D.hpp"

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

// Matrix test suite
/*
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
} */



SUITE(Matrix2D)
{   // displays message
    TEST(message)
    {   std::cout << "Starting Matrix2D tests..." << std::endl ; }

    // tests contructor
    TEST(constructor)
    {
        std::vector<size_t> dim ;

        // has non-0 dimensions : 1x2
        Matrix2D<int> m1(1,2) ;
        dim = {1,2} ;
        CHECK_EQUAL(2, m1.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m1.get_dim(), 2) ;
        CHECK_EQUAL(1*2, m1.get_data_size()) ;

        // has a zero dimension : 0x1
        Matrix2D<int> m2(0,1) ;
        dim = {0,1} ;
        CHECK_EQUAL(2, m2.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m2.get_dim(), 2) ;
        CHECK_EQUAL(0*1, m2.get_data_size()) ;

        // has zero dimensions :  0x0
        Matrix2D<int> m3(0,0) ;
        dim = {0,0} ;
        CHECK_EQUAL(2, m3.get_dim_size()) ;
        CHECK_ARRAY_EQUAL(dim, m3.get_dim(), 2) ;
        CHECK_EQUAL(0*0, m3.get_data_size()) ;
    }

    // tests contructor with value
    TEST(constructor_value)
    {
        for(size_t i=1; i<11; i++)
        {   std::vector<size_t> dim ;

            // has non-0 dimensions : 1x2 / 2x3 / ...
            Matrix2D<int> m1(i,i+1, i) ;
            dim = {i,i+1} ;
            CHECK_EQUAL(2, m1.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim, m1.get_dim(), 2) ;
            CHECK_EQUAL(i*(i+1), m1.get_data_size()) ;
            for(const auto x : m1.get_data())
            {   CHECK_EQUAL(i, x) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            CHECK_EQUAL(2, m2.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim, m2.get_dim(), 2) ;
            CHECK_EQUAL(0*i, m2.get_data_size()) ;
            for(const auto x : m2.get_data())
            {   CHECK_EQUAL(i, x) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            CHECK_EQUAL(2, m3.get_dim_size()) ;
            CHECK_ARRAY_EQUAL(dim, m3.get_dim(), 2) ;
            CHECK_EQUAL(0*0, m3.get_data_size()) ;
            for(const auto x : m3.get_data())
            {   CHECK_EQUAL(i, x) ; }
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
            {   CHECK_EQUAL(m1.get(j), m1.get(convert_to_coord(m1, j))) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(m2.get(j), m2.get(convert_to_coord(m2, j))) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(m3.get(j), m3.get(convert_to_coord(m3, j))) ; }
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
            {   m1.set(j,j) ; }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m1.get(j)) ; }

            // has a zero dimension : 0x1 / 0x2 / ...
            Matrix2D<int> m2(0,i,i) ;
            dim = {0,i} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   m2.set(j,j) ; }
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   CHECK_EQUAL(j, m2.get(j)) ; }

            // has zero dimensions :  0x0
            Matrix2D<int> m3(0,0,i) ;
            dim = {0,0} ;
            for(size_t j=0; j<dim[0]*dim[1]; j++)
            {   m3.set(j,j) ; }
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

    // tests contructor from file
    // now because it uses previously tested methods
    TEST(constructor_file)
    {   std::string file_int1("./src/Unittests/data/matrix2d_int1.mat") ;
        std::string file_int2("./src/Unittests/data/matrix2d_int2.mat") ;
        std::string file_int3("./src/Unittests/data/matrix2d_int3.mat") ;
        std::string file_int4("./src/Unittests/data/matrix2d_int4.mat") ;
        std::string file_int5("./src/Unittests/data/matrix2d_int5.mat") ;
        std::string file_int6("./src/Unittests/data/matrix2d_int6.mat") ;

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
}
