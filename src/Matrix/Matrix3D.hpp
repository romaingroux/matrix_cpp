#ifndef MATRIX3D_HPP
#define MATRIX3D_HPP

#include <Matrix.hpp>

#include <string>
#include <vector>
#include <utility>    // std::move()
#include <iostream>
#include <iomanip>      // setw(), setprecision(), fixed
#include <fstream>      // ifstream
#include <sstream>      // istringstream
#include <stdexcept>    // runtime_error, out_of_range
#include <algorithm>    // equal()

#define BUFFER_SIZE 4096

/*!
 * The Matrix3D class is a specialisation of the Matrix
 * class to make work with 3D matrices more easily.
 *
 * A format to save a Matrix3D objects in a binary file is defined. The following values
 * are written :
 * 1x size_t :     the number <N> of dimensions of the Matrix stored. This is the value
 *                 of _dim_size field. This value must be 3 otherwise this is not a 3D
 *                 matrix.
 * 3x size_t :     the width of the matrix in each dimension. These values correspond
 *                 to the content of the _dim vector and can be loaded inside this
 *                 vector as they are.
 * Dx <T> values : the <D> values contained in the matrix, in the _data vector.
 *                 These values can be loaded directly in this vector. <D> is equal to the
 *                 product of the 3 values stored right before. The type <T> depends on
 *                 the type of the data stored in the matrix. The 1st of the D values is
 *                 also the first value of the _data vector.
 *
 *
 * A text file format is defined to store Matrix3D objects. The specifications are as
 * follows :
 * Absolutely NO empty lines are allowed!
 * The following lines should contain :
 *
 * 1st line       : a slice header, ',,0' indicates that a slice of the 3rd dimension
 *                  is beginning (this is a z slice).
 * 2nd - Nth line : the firt slice, as a 2d matrix (the exemple below has dimensions 3x4).
 * N+1th line     : a slice header, ',,1' indicates that the 2nd slice is beginning.
 * N+1th - ...    : the second slice
 * and so on...
 *
 * Example of a 3x4x2 3D matrix
 * ---- start ----
 * ,,0
 *  1  2  3  4
 *  5  6  7  8
 *  8  9 10 11
 *,,1
 * 12 13 14 15
 * 16 17 18 19
 * 20 21 22 23
 * ----- end -----
 *
 * Constructing a matrix from an empty file (0 bytes or only an EOL char) returns a null
 * matrix (0x0x0 dimensions). Writting a null matrix (that is with at least one null
 * dimension creates an empty file.
 *
 */
template<class T>
class Matrix3D : public Matrix<T>
{
    public:
        // constructors
        /*!
         * Default constructor.
         */
        Matrix3D() = default ;

        /*!
         * \brief Constructs a matrix with the given dimensions,
         * filled with 0 values.
         * \param dim1 the first dimension.
         * \param dim2 the second dimension.
         * \param dim3 the third dimension.
         */
        Matrix3D(size_t dim1, size_t dim2, size_t dim3) ;

        /*!
         * \brief Constructs a matrix with the given dimensions and
         * initialize the values to the given value.
         * \param dim1 the first dimension.
         * \param dim2 the second dimension.
         * \param dim3 the third dimension.
         * \param value the value to initialize the matrix content
         * with.
         */
        Matrix3D(size_t dim1, size_t dim2, size_t dim3, T value) ;

        /*!
         * \brief Copy constructor
         * \param other the matrix to copy the values from.
         */
        Matrix3D(const Matrix3D& other) ;

        /*!
         * \brief Move constructor
         * \param other the matrix to use the values from.
         */
        Matrix3D(Matrix3D&& other) ;

        /*!
         * \brief Constructs a matrix from a text file. A matrix contructed
         * from an empty file (or a file containing only one EOL char) returns
         * an empty matrix (null dimensions).
         * \param file_address the address of the file containing the matrix.
         * \throw std::runtime_error if anything happen while reading the
         * file (format error, file not found, etc).
         */
        Matrix3D(const std::string& file_address) ;

        /*!
         * \brief Destructor.
         */
        virtual ~Matrix3D() ;

        // methods overloaded from Matrix
        using Matrix<T>::get ;
        using Matrix<T>::set ;

        // methods
        /*!
         * \brief loads a binary file containing
         * a matrix.
         * \param path the path to the file.
         */
        void load(const std::string& file_address) ;

        /*!
         * \brief Gets the element at the given coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         * \return the element.
         */
        T get(size_t dim1, size_t dim2, size_t dim3) const ;

        /*!
         * \brief Sets the element at the given coordinates
         * to the given value.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \param value the new value.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         */
        void set(size_t dim1, size_t dim2, size_t dim3, T value) ;

        /*!
         * \brief Produces a nice representation of the matrix on the given
         * stream.
         * \param stream the stream.
         * \param precision the rounding precision.
         * \param width the column width in number of characters.
         * \param sep the character separator.
         */
        virtual void print(std::ostream& stream, size_t precision=4 ,size_t width=8, char sep=' ') const override ;

        // operators
        /*!
         * Assignment operator.
         * \param other an other matrix to copy the values from.
         * \return a reference to the current the instance.
         */
        Matrix3D& operator = (const Matrix3D& other) ;

        /*!
         * Move Assignment operator.
         * \param other an other matrix to use the values from.
         * \return a reference to the instance.
         */
        Matrix3D& operator = (Matrix3D&& other) ;

        /*!
         * \brief Returns a reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \return a reference to this element.
         */
        T& operator () (size_t dim1, size_t dim2, size_t dim3) ;

        /*!
         * \brief Returns a constant reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \return a constant reference to this element.
         */
        const T& operator () (size_t dim1, size_t dim2, size_t dim3) const ;

    private:
        // methods
        /*!
         * \brief Checks whether a given string is a slice header
         * (such as ",,0"), as found in files storing Matrix3D.
         * \param str the string to check.
         * \return whether the string is a slice header.
         */
        bool is_header(const std::string& str) const ;

        /*!
         * \brief Converts a triplet of VALID (dim1, dim2, dim3) coordinates
         * to a the corresponding offset allowing to get an element in the
         * data vector.
         * \param dim1 the index of the 1st dimension slice (row).
         * \param dim2 the index of the 2nd dimension slice (column).
         * \param dim3 the index of the 3rd dimension slice.
         * \return the corresponding offset.
         */
        size_t convert_to_offset(size_t dim1, size_t dim2, size_t dim3) const ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 1st dimension (row) starts.
         */
        void compute_dim1_offsets() ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 2nd dimension (column) starts.
         */
        void compute_dim2_offsets() ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 3rd dimension (3rd dimension
         * slice) starts.
         */
        void compute_dim3_offsets() ;

        /*!
         * \brief Contains the offsets at which each x slice
         * starts. Each element corresponds to the corresponding
         * x slice (1st element -> 1st x slice (row)).
         */
        std::vector<size_t> _dim1_offsets ;
        /*!
         * \brief Contains the offsets at which each y slice
         * starts. Each element corresponds to the corresponding
         * y slice (1st element -> 1st y slice (column)).
         */
        std::vector<size_t> _dim2_offsets ;
        /*!
         * \brief Contains the offsets at which each x slice
         * starts. Each element corresponds to the corresponding
         * x slice (1st element -> 1st z slice).
         */
        std::vector<size_t> _dim3_offsets ;
} ;

// operators
/*!
 * \brief Addition operator.
 * \param m the matrix of interest
 * \param value the value to add to each element.
 * \return the resulting matrix.
 */
template<class T>
const Matrix3D<T> operator + (Matrix3D<T> m, T value)
{   Matrix3D<T> other(m) ;
    m += value ;
    return m ;
}

/*!
 * \brief Substraction operator
 * \param m the matrix of interest.
 * \param value the value to substract to each element.
 * \return the resulting matrix.
 */
template<class T>
const Matrix3D<T> operator - (Matrix3D<T> m, T value)
{    Matrix3D<T> other(m) ;
     m -= value ;
     return m ;
}

/*!
 * \brief Multiplication operator.
 * \param m the matrix of interest.
 * \param value the value to multiply each elements by.
 * \return the resulting matrix.
 */
template<class T>
const Matrix3D<T> operator * (Matrix3D<T> m, T value)
{    Matrix3D<T> other(m) ;
     m *= value ;
     return m ;
}

/*!
 * \brief Division operator.
 * \param m the matrix of interest.
 * \param value the value to divide each elements by.
 * \throw std::invalid_argument if value is 0.
 * \return the resulting matrix.
 */
template<class T>
const Matrix3D<T> operator / (Matrix3D<T> m, T value)
{   if(value == static_cast<T>(0))
    {   throw std::invalid_argument("division by 0!") ; }
    Matrix3D<T> other(m) ;
    other /= value ;
    return other ;
}

/*!
 * \brief Sends a representation of the matrix to the stream.
 * \param stream the stream of interest.
 * \param m the matrix of interest.
 * \return a reference to the stream.
 */
template<class T>
std::ostream& operator << (std::ostream& stream, const Matrix3D<T>& m)
{   m.print(stream) ;
    return stream ;
}



// method implementation
template<class T>
Matrix3D<T>::Matrix3D(size_t dim1, size_t dim2, size_t dim3)
    : Matrix3D<T>(dim1, dim2, dim3, 0)
{}

template<class T>
Matrix3D<T>::Matrix3D(size_t dim1, size_t dim2, size_t dim3, T value)
    : Matrix<T>({dim1, dim2, dim3}, value),
      _dim1_offsets(dim1),
      _dim2_offsets(dim2),
      _dim3_offsets(dim3)
{   this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
}

template<class T>
Matrix3D<T>::Matrix3D(const Matrix3D& other)
    : Matrix<T>(other)
{   this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
}

template<class T>
Matrix3D<T>::Matrix3D(Matrix3D<T>&& other)
    : Matrix<T>(std::move(other))
{   this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
}

template<class T>
Matrix3D<T>::Matrix3D(const std::string &file_address)
{
    this->_dim       = {0,0,0} ;
    this->_data      = new std::vector<T>() ;
    this->_dim_size  = this->_dim.size() ;
    this->_data_size = this->_data->size() ;
    this->_dim_prod  = std::vector<size_t>(this->_dim_size, 0) ;

    std::ifstream file(file_address, std::ifstream::in) ;
    if(file.fail())
    {   char msg[BUFFER_SIZE] ;
        sprintf(msg, "error! cannot open %s", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    std::string         buffer_str ;
    std::vector<T>      buffer_vec ;
    T                   buffer_T ;

    // read file
    size_t n_line      = 0, n_line_data = 0 ; // number of line and of data line read
    size_t row_len     = 0, col_len     = 0 ; // length of row and column in nber of values
    size_t row_len_cur = 0, col_len_cur = 0 ; // current number of values read in row and col

    while(getline(file, buffer_str))
    {   if(file.fail())
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        // check empty line
        if(buffer_str.size() == 0)
        {   // this file only contains one eol char and should be considered as empty,
            // -> returns empty matrix not an error
            if(n_line == 0 and file.peek() == EOF and file.eof())
            {  break ; }

            file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! while reading %s (empty line)", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }


        // check whether it is the beginning of a slice
        // 1st line in file should be one like this
        if(this->is_header(buffer_str))
        {   // check that slice have a constant number of rows
            if(this->_dim[2] == 1)
            {   col_len = col_len_cur ;
                // this->_dim[0] = row_len ;
                // this->_dim[1]  = col_len ;
            }
            else if(col_len_cur != col_len)
            {   file.close() ;
                char msg[BUFFER_SIZE] ;
                sprintf(msg, "format error! slice have variable dimensions 1 in %s", file_address.c_str()) ;
                throw std::runtime_error(msg) ;
            }
            this->_dim[2]++ ;
            col_len_cur = 0 ;
            n_line++ ;
            continue ;
        }
        // 1st line in file should be a header and entering
        // this block is forbidden
        if(n_line == 0)
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! first line is not a slice header in %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // parse line
        row_len_cur = 0 ;
        buffer_vec.clear() ;
        std::istringstream buffer_ss(buffer_str) ;
        while(buffer_ss >> buffer_T)
        {   buffer_vec.push_back(buffer_T) ;
            row_len_cur++ ;
        }
        // check for an error which likely indicates that a value could not be
        // casted into a type T (mixed data types in the file)
        if(buffer_ss.fail() and not buffer_ss.eof())
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! could not read a line in %s (incompatible data types)", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // check that number of column is constant
        if(n_line_data == 0)
        {  row_len = row_len_cur ; }
        else if(row_len_cur != row_len)
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! slice have variable dimensions 2 in %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // update matrix content
        for(auto i : buffer_vec)
        {   this->_data->push_back(i) ;
            this->_data_size++ ;
        }
        col_len_cur++ ;
        n_line_data++ ;
        n_line++ ;
        // update matrix dimensions
        this->_dim[0] = row_len_cur ;
        this->_dim[1] = col_len_cur ;
    }
    // check dimensions of last slice
    if(col_len_cur != this->_dim[1])
    {   file.close() ;
        char msg[BUFFER_SIZE] ;
        sprintf(msg, "format error! slice have variable dimensions in %s", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    file.close() ;
    this->compute_dim_product() ;

    this->_dim1_offsets = std::vector<size_t>(this->_dim[1]) ;
    this->_dim2_offsets = std::vector<size_t>(this->_dim[0]) ;
    this->_dim3_offsets = std::vector<size_t>(this->_dim[2]) ;
    this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
}

template<class T>
Matrix3D<T>::~Matrix3D()
{   if(this->_data != nullptr)
    {   delete this->_data ;
        this->_data = nullptr ;
    }
}

template<class T>
void Matrix3D<T>::load(const std::string& file_address)
{
    Matrix<T>::load(file_address, 3) ;

    this->_dim1_offsets = std::vector<size_t>(this->_dim[1]) ;
    this->_dim2_offsets = std::vector<size_t>(this->_dim[0]) ;
    this->_dim3_offsets = std::vector<size_t>(this->_dim[2]) ;
    this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
}

template<class T>
T Matrix3D<T>::get(size_t dim1, size_t dim2, size_t dim3) const
{   try
    {  return this->get({dim1, dim2, dim3}) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}

template<class T>
void Matrix3D<T>::set(size_t dim1, size_t dim2, size_t dim3, T value)
{   try
    {  return this->set({dim1, dim2, dim3}, value) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}

template<class T>
Matrix3D<T>& Matrix3D<T>::operator = (const Matrix3D<T>& other)
{   /*
    this->_dim           = other._dim ;
    this->_dim_size      = other._dim_size ;
    this->_data          = new std::vector<T>(*(other._data)) ;
    this->_data_size     = other._data_size ;
    this->_dim_prod      = other._dim_prod ;
    */
    Matrix<T>::operator=(other) ;
    this->_dim1_offsets  = other._dim1_offsets ;
    this->_dim2_offsets  = other._dim2_offsets ;
    this->_dim3_offsets  = other._dim3_offsets ;
    return *this ;
}

template<class T>
Matrix3D<T>& Matrix3D<T>::operator = (Matrix3D<T>&& other)
{   Matrix<T>::operator=(std::move(other)) ;
    this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
    return *this ;
}

template<class T>
T& Matrix3D<T>::operator () (size_t dim1, size_t dim2, size_t dim3)
{   return (*this->_data)[this->convert_to_offset(dim1, dim2, dim3)] ; }

template<class T>
const T& Matrix3D<T>::operator () (size_t dim1, size_t dim2, size_t dim3) const
{   return (*this->_data)[this->convert_to_offset(dim1, dim2, dim3)] ; }

template<class T>
void Matrix3D<T>::print(std::ostream& stream, size_t precision, size_t width, char sep) const
{   // if the matrix has at least one 0 dimension (no data), don't do anything
    if(this->_dim[0]==0 or this->_dim[1]==0 or this->_dim[2]==0)
    {   return ; }
    stream.setf(std::ios::left) ;
    stream << std::setprecision(precision) << std::fixed ;
    std::vector<size_t> dim = this->get_dim() ;

    size_t    n  = 0 ;
    size_t n_tot = std::accumulate(dim.begin(), dim.end(), (size_t)1, std::multiplies<int>()) ;

    for(size_t z=0; z<dim[2]; z++)
    {   stream << ",," << z << std::endl ;
        for(size_t x=0; x<dim[0]; x++)
        {   for(size_t y=0; y<dim[1]; y++, n++)
            {   stream << std::setw(width) << (*this)(x,y,z) << sep ; }
            if(n<n_tot)
            {   stream << std::endl ; }
        }
    }
}

template<class T>
bool Matrix3D<T>::is_header(const std::string& str) const
{   if(str[0] == ',' and
            str[1] == ',' and
            str.find(',', 2) == std::string::npos)
   {   return true ; }
   return false ;
}

template<class T>
void Matrix3D<T>::compute_dim1_offsets()
{   for(size_t i=0; i<this->_dim[1]; i++)
    {   this->_dim1_offsets[i] = i * this->_dim_prod[1] ; }
}

template<class T>
void Matrix3D<T>::compute_dim2_offsets()
{   for(size_t i=0; i<this->_dim[0]; i++)
    {   this->_dim2_offsets[i] = i * this->_dim_prod[0] ; }
}

template<class T>
void Matrix3D<T>::compute_dim3_offsets()
{   for(size_t i=0; i<this->_dim[2]; i++)
    {   this->_dim3_offsets[i] = i * this->_dim_prod[2] ; }
}

template<class T>
size_t Matrix3D<T>::convert_to_offset(size_t dim1, size_t dim2, size_t dim3) const
{   /*
    size_t offset = 0 ;

    for(size_t i=0; i<this->_dim_size; i++)
    {   offset += coord[i] * this->_dim_prod[i] ; }

    return offset ;
    */
    size_t offset = this->_dim1_offsets[dim1] +
                    this->_dim2_offsets[dim2] +
                    this->_dim3_offsets[dim3] ;
    return offset ;
}

#endif // MATRIX3D_HPP
