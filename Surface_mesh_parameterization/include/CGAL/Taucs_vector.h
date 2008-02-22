// Copyright (c) 2005  INRIA (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Laurent Saboret, Pierre Alliez, Bruno Levy


#ifndef CGAL_TAUCS_VECTOR
#define CGAL_TAUCS_VECTOR

#include <CGAL/basic.h>

CGAL_BEGIN_NAMESPACE


/// The class Taucs_vector
/// is a C++ wrapper around TAUCS' vector type, which is a simple array.
///
/// @heading Is Model for the Concepts: Model of the SparseLinearAlgebraTraits_d::Vector concept.

template<class T>       // Tested with T = taucs_single or taucs_double
                        // May also work with T = taucs_dcomplex and taucs_scomplex
class Taucs_vector
{
// Public types
public:

    typedef T NT;

// Public operations
public:

    /// Create a vector initialized with zeros.
    Taucs_vector(int dimension)
    {
        m_dimension = dimension;
        m_element = new T[dimension];

        // init with zeros
        for (int i=0; i < m_dimension; i++)
            m_element[i] = 0;
    }

    /// Copy constructor.
    Taucs_vector(const Taucs_vector& toCopy)
    {
        m_dimension = toCopy.m_dimension;
        m_element = new T[m_dimension];
        for (int i=0; i < m_dimension; i++)
            m_element[i] = toCopy.m_element[i];
    }

    /// operator =()
    Taucs_vector& operator =(const Taucs_vector& toCopy)
    {
        delete[] m_element;

        m_dimension = toCopy.m_dimension;
        m_element = new T[m_dimension];
        for (int i=0; i < m_dimension; i++)
            m_element[i] = toCopy.m_element[i];

        return *this;
    }

    ~Taucs_vector()
    {
        delete[] m_element;
        m_element = NULL;
    }

    /// Return the vector's number of coefficients.
    int dimension() const {
        return m_dimension;
    }

    /// Read/write access to a vector coefficient.
    ///
    /// Preconditions:
    /// 0 <= i < dimension().
    T operator[](int i) const {
        CGAL_precondition(i < m_dimension);
        return m_element[i];
    }
    T& operator[](int i) {
        CGAL_precondition(i < m_dimension);
        return m_element[i];
    }

    /// Get TAUCS vector wrapped by this object.
    const T* get_taucs_vector() const {
        return m_element;
    }
    T* get_taucs_vector() {
        return m_element;
    }

// Fields
private:

    int m_dimension;    ///< Vector size.
    T* m_element;       ///< Array of m_dimension T elements.
};


CGAL_END_NAMESPACE

#endif // CGAL_TAUCS_VECTOR
