// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_COMPLEX_HH
# define MLN_CORE_COMPLEX_HH

/// \file mln/core/complex.hh
/// \brief Structures for general complexes.
///
/// A complexes defines a topological space which can be used as a
/// support for an image (i.e., as site sets).
///
/// FIXME: More.

# include <cstddef>

# include <limits>
# include <iosfwd>

# include <mln/metal/bool.hh>

# include <mln/core/face.hh>


namespace mln
{

  // Forward declaration.
  namespace internal
  {
    template <unsigned N, unsigned D>
    struct faces_set_mixin;
  }


  /*----------.
  | Complex.  |
  `----------*/

  /// \brief General complex of dimension \p D.
  template <unsigned D>
  class complex : private internal::faces_set_mixin<D, D>
  {
  public:
    /// Complex construction.
    /// \{
    /// \brief Add a 0-face to the complex.
    face_handle<0u, D> add_face();

    /// \brief Add a \p (N+1)-face to the complex (with \p N >= 0).
    ///
    /// \param adjacent_faces The (\p N-1)-faces adjacent to the new
    /// \p N-face.
    template <unsigned N>
    face_handle<N + 1, D> add_face(const faces_set<N, D>& adjacent_faces);
    /// \}

    /// \brief Static manipulators.
    ///
    /// These methods use statically-known input.
    /// \{
    /// \brief Return the total number of faces, whatever their
    /// dimension.
    std::size_t nfaces() const;

    /// \brief Return the number of \p N-faces.
    template <unsigned N>
    std::size_t nfaces() const;
    /// \}

    /// \brief Dynamic manipulators.
    ///
    /// These methods use input know as run time.
    /// \{
    /// \brief Return the number of \a n-faces.
    ///
    /// Warning, this function has a complexity linear in term of N,
    /// since each faces_set is checked (the present implementation
    /// does not provide a direct access to faces_set through a
    /// dynamic value of the dimension).
    std::size_t nfaces(unsigned n) const;
    /// \}

    /// Pretty-printing.
    /// \{
    /// Print the complex.
    void print(std::ostream& ostr) const;
    /// Print the faces of dimension \p N.
    template <unsigned N>
    void print_faces(std::ostream& ostr) const;
    /// \}

  private:
    /// Accessors.
    /// \{
    template <unsigned N, unsigned D_> friend class face_handle;

    template <unsigned N>
    face<N, D>& face_(unsigned face_id);

    template <unsigned N>
    const face<N, D>& face_(unsigned face_id) const;
    /// \}

    /// Functional meta-manipulators.
    /// \{
    /* FIXME: Use something more constraining than the STL's
       UnaryFunction/BinaryFunction.  Use Function or Function_v2v?
       Or a new subclass of Function?  */

    /* FIXME: Replace F and ACCU by a Milena accumulator?  */

    /** \brief Apply a kind of static fold left operator to the
        implicit list of faces_set using a functor \a f and a value \a
        accu.

	Argument \a is called an "accumulator", but with a slightly
        different meaning than mln:accu members.

	We might want to use TypeLists or something similar, is they
	provide an explicit static fold left operator.  */
    template <typename BinaryFunction, typename T>
    T fold_left_(const BinaryFunction& f, const T& accu) const;

    /// \brief Apply a functor \a f to this list of \a n-faces.
    template <typename UnaryFunction>
    typename UnaryFunction::result_type
    apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
    /// \}

    /// \brief connect two faces.
    ///
    /// \param f1 A face of dimension \p N
    /// \param f2 A face of dimension \p N + 1
    ///
    /// \pre \p N must be lower or equal to \p D.
    template <unsigned N>
    void connect_(const face_handle<N, D>& f1,
		  const face_handle<N + 1, D>& f2);
  };


  /// Pretty print a complex.
  template <unsigned D>
  std::ostream&
  operator<<(std::ostream& ostr, const complex<D>& c);


  /*---------------------.
  | Faces of a complex.  |
  `---------------------*/

  /// The sets of n-faces of a complex are recursively aggregated as
  /// mixins.
  namespace internal
  {

    // Forward declarations.
    template <unsigned N, unsigned D> struct lower_dim_faces_set_mixin;
    template <unsigned N, unsigned D> struct higher_dim_faces_set_mixin;


    /*---------------------------------.
    | mln::internal::faces_set_mixin.  |
    `---------------------------------*/

    /// \brief Recursive mixins of set of faces.
    /// \{
    template <unsigned N, unsigned D> struct faces_set_mixin;

    /// Faces of highest dimension (\p D).
    template <unsigned D>
    struct faces_set_mixin<D, D> : public faces_set_mixin<D - 1, D>,
				   public lower_dim_faces_set_mixin<D, D>
    {
      std::vector< face<D, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p D.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}

      /// Functional meta-manipulators.
      /// \{
      /// \brief Fold left.
      /// \see mln::complex<D>::fold_left_.
      template <typename BinaryFunction, typename T>
      T fold_left_(const BinaryFunction& f, const T& accu) const;
      /// \brief Apply a functor \a f to the list of faces if \a n == \p D.
      /// \see mln::complex<D>::apply_if_dim_matches_.
      template <typename UnaryFunction>
      typename UnaryFunction::result_type
      apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
      /// \}
    };

    /// Faces of intermediate dimension (greater than 0, lower than \p D).
    template <unsigned N, unsigned D>
    struct faces_set_mixin : public faces_set_mixin<N - 1, D>,
                             public lower_dim_faces_set_mixin<N, D>,
                             public higher_dim_faces_set_mixin<N, D>
    {
      std::vector< face<N, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p N.
      void print(std::ostream& ostr) const;
      /// Recursively print the faces of dimensions 0 to \p N
      /// (in ascending dimension).
      void print_rec_asc(std::ostream& ostr) const;
      /// \}

      /// Functional meta-manipulators.
      /// \{
      /// \brief Fold left.
      /// \see mln::complex<D>::fold_left_.
      template <typename BinaryFunction, typename T>
      T fold_left_(const BinaryFunction& f, const T& accu) const;
      /// \brief Apply a functor \a f to the list of faces if \a n == \p N.
      /// \see mln::complex<D>::apply_if_dim_matches_.
      template <typename UnaryFunction>
      typename UnaryFunction::result_type
      apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
      /// \}
    };

    /// Faces of lowest dimension (0).
    template <unsigned D>
    struct faces_set_mixin<0u, D> : public higher_dim_faces_set_mixin<0u, D>
    {
      std::vector< face<0u, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension 0.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}

      /// Functional meta-manipulators.
      /// \{
      /// \brief Fold left.
      /// \see mln::complex<D>::fold_left_.
      template <typename BinaryFunction, typename T>
      T fold_left_(const BinaryFunction& f, const T& accu) const;
      /// \brief Apply a functor \a f to the list of faces if \a n == 0.
      /// \see mln::complex<D>::apply_if_dim_matches_.
      template <typename UnaryFunction>
      typename UnaryFunction::result_type
      apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
      /// \}
    };

    /// Faces of a 0-complex.
    template <>
    struct faces_set_mixin<0u, 0u>
    {
      std::vector< face<0u, 0u> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p D.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}

      /// Functional meta-manipulators.
      /// \{
      /// \brief Fold left.
      /// \see mln::complex<D>::fold_left_.
      template <typename BinaryFunction, typename T>
      T fold_left_(const BinaryFunction& f, const T& accu) const;
      /// \brief Apply a functor \a f to the list of faces if \a n == 0.
      /// \see mln::complex<D>::apply_if_dim_matches_.
      template <typename UnaryFunction>
      typename UnaryFunction::result_type
      apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
      /// \}
    };
    /// \}


    /*--------------------------------------------.
    | mln::internal::lower_dim_faces_set_mixin.   |
    | mln::internal::higher_dim_faces_set_mixin.  |
    `--------------------------------------------*/

    /// Mixins of mixin mln::faces_set_mixin.
    /// \{
    template <unsigned N, unsigned D>
    struct lower_dim_faces_set_mixin
    {
      void print(std::ostream& ostr, const face<N, D>& f) const;
    };

    template <unsigned N, unsigned D>
    struct higher_dim_faces_set_mixin
    {
      void print(std::ostream& ostr, const face<N, D>& f) const;
    };
    /// \}

  } // end of namespace mln::internal



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------.
  | Complex construction.  |
  `-----------------------*/

  template <unsigned D>
  face_handle<0u, D>
  complex<D>::add_face()
  {
    /* FIXME: This is not thread-proof (these two lines should
       form an atomic section).  */
    internal::faces_set_mixin<0u, D>::faces_.push_back(face<0u, D>());
    unsigned id = nfaces<0u>() - 1;

    return face_handle<0u, D>(*this, id);
  }

  template <unsigned D>
  template <unsigned N>
  face_handle<N + 1, D>
  complex<D>::add_face(const faces_set<N, D>& adjacent_faces)
  {
    typedef typename std::vector< face_handle<N, D> >::const_iterator iter_t;

    // Ensure ADJACENT_FACES are already part of the complex.
    /* FIXME: We need additional macros in mln/core/contract.hh for
       big blocks of preconditions like this one.  */
# ifndef NDEBUG
    for (iter_t a = adjacent_faces.faces().begin();
	 a != adjacent_faces.faces().end(); ++a)
      {
	mln_precondition(&a->cplx() == this);
	mln_precondition(a->is_valid());
      }
# endif // !NDEBUG

    face<N + 1, D> f;
    /* FIXME: This is not thread-proof (these two lines should
       form an atomic section).  */
    internal::faces_set_mixin<N + 1, D>::faces_.push_back(f);
    unsigned id = nfaces<N + 1>() - 1;

    face_handle<N + 1, D> fh(*this, id);
    // Connect F and its ADJACENT_FACES.
    for (iter_t a = adjacent_faces.faces().begin();
	 a != adjacent_faces.faces().end(); ++a)
      connect_(*a, fh);
    return fh;
  }


  /*-------.
  | Misc.  |
  `-------*/

  namespace internal
  {

    /// A binary meta-functor defined by:
    ///
    /// \code
    /// add_size : x, c -> x + c.size()
    /// \endcode
    ///
    /// \see mln::complex<D>::nfaces (static version).
    /// \see mln::complex<D>::fold_left_.
    struct add_size
    {
      template <typename T, typename Container>
      T operator()(const T& x, const Container& c) const
      {
	return x + c.size();
      }
    };

    /// An unary meta-functor defined by:
    ///
    /// \code
    /// add_size : c -> c.size()
    /// \endcode
    ///
    /// \see mln::complex<D>::nfaces (dynamic version).
    /// \see mln::complex<D>::apply_if_dim_matches_.
    struct get_size
    {
      typedef std::size_t result_type;

      template <typename Container>
      typename Container::size_type operator()(const Container& c) const
      {
	return c.size();
      }
    };

  } // end of namespace mln::internal


  /*----------------------.
  | Static manipulators.  |
  `----------------------*/

  template <unsigned D>
  std::size_t
  complex<D>::nfaces() const
  {
    return fold_left_(internal::add_size(), 0);
  }

  template <unsigned D>
  template <unsigned N>
  std::size_t
  complex<D>::nfaces() const
  {
    return internal::faces_set_mixin<N, D>::faces_.size();
  }


  /*-----------------------.
  | Dynamic manipulators.  |
  `-----------------------*/

  template <unsigned D>
  std::size_t
  complex<D>::nfaces(unsigned n) const
  {
    // Ensure N is compatible with D.
    mln_precondition(n <= D);
    return apply_if_dim_matches_(n, internal::get_size());
  }


  /*-------------------.
  | Internal methods.  |
  `-------------------*/

  template <unsigned D>
  template <unsigned N>
  face<N, D>&
  complex<D>::face_(unsigned face_id)
  {
    return internal::faces_set_mixin<N, D>::faces_[face_id];
  }

  template <unsigned D>
  template <unsigned N>
  const face<N, D>&
  complex<D>::face_(unsigned face_id) const
  {
    return internal::faces_set_mixin<N, D>::faces_[face_id];
  }

  template <unsigned D>
  template <unsigned N>
  void
  complex<D>::connect_(const face_handle<N, D>& f1,
		       const face_handle<N + 1, D>& f2)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    f1.to_face().connect_higher_dim_face(f2);
    f2.to_face().connect_lower_dim_face(f1);
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <unsigned D>
  std::ostream&
  operator<<(std::ostream& ostr, const complex<D>& c)
  {
    c.print(ostr);
    return ostr;
  }

  template <unsigned D>
  void
  complex<D>::print(std::ostream& ostr) const
  {
    internal::faces_set_mixin<D, D>::print_rec_asc(ostr);
  }

  template <unsigned D>
  template <unsigned N>
  void
  complex<D>::print_faces(std::ostream& ostr) const
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    internal::faces_set_mixin<N, D>::print(ostr);
  }


  namespace internal
  {

    template <unsigned N, unsigned D>
    void
    faces_set_mixin<N, D>::print_rec_asc(std::ostream& ostr) const
    {
      faces_set_mixin<N - 1, D>::print_rec_asc(ostr);
      print(ostr);
    }

    template <unsigned D>
    void
    faces_set_mixin<0u, D>::print_rec_asc(std::ostream& ostr) const
    {
      print(ostr);
    }

    template <unsigned D>
    void
    faces_set_mixin<D, D>::print_rec_asc(std::ostream& ostr) const
    {
      faces_set_mixin<D - 1, D>::print_rec_asc(ostr);
      print(ostr);
    }

    void
    faces_set_mixin<0u, 0u>::print_rec_asc(std::ostream& ostr) const
    {
      print(ostr);
    }


    template <unsigned N, unsigned D>
    void
    faces_set_mixin<N, D>::print(std::ostream& ostr) const
    {
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N - 1 << " and "
	   << N + 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	  lower_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	  ostr << "},  dim " << N + 1 << ": { ";
	  higher_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	  ostr << "}" << std::endl;
	}
    }

    template <unsigned D>
    void
    faces_set_mixin<0u, D>::print(std::ostream& ostr) const
    {
      const unsigned N = 0u;
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N + 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N + 1 << ": { ";
	  higher_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	  ostr << "}" << std::endl;
	}
    }

    template <unsigned D>
    void
    faces_set_mixin<D, D>::print(std::ostream& ostr) const
    {
      const unsigned N = D;
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N - 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	  lower_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	  ostr << "}" << std::endl;
	}
    }

    void
    faces_set_mixin<0u, 0u>::print(std::ostream& ostr) const
    {
      const unsigned N = 0u;
      ostr << "Faces of dimension " << N << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	ostr << "  " << f << std::endl;
    }


    template <unsigned N, unsigned D>
    void
    lower_dim_faces_set_mixin<N, D>::print(std::ostream& ostr,
					   const face<N, D>& f) const
    {
      for (typename std::vector< face_handle<N - 1, D> >::const_iterator l =
	     f.lower_dim_faces_.begin(); l != f.lower_dim_faces_.end(); ++l)
	ostr << l->face_id() << " ";
    }

    template <unsigned N, unsigned D>
    void
    higher_dim_faces_set_mixin<N, D>::print(std::ostream& ostr,
					    const face<N, D>& f) const
    {
      for (typename std::vector< face_handle<N + 1, D> >::const_iterator h =
	     f.higher_dim_faces_.begin(); h != f.higher_dim_faces_.end(); ++h)
	ostr << h->face_id() << " ";
    }

  } // end of namespace mln::internal


  /*-------------------------------.
  | Functional meta-manipulators.  |
  `-------------------------------*/

  /* ------------------------------- */
  /* ``Static Fold Left'' Operator.  */
  /* ------------------------------- */

  template <unsigned D>
  template <typename BinaryFunction, typename T>
  T
  complex<D>::fold_left_(const BinaryFunction& f, const T& accu) const
  {
    return internal::faces_set_mixin<D, D>::fold_left_(f, accu);
  }

  namespace internal
  {

    // FIXME: Try to factor.

    template <unsigned D>
    template <typename BinaryFunction, typename T>
    T
    faces_set_mixin<D, D>::fold_left_(const BinaryFunction& f,
				      const T& accu) const
    {
      return faces_set_mixin<D - 1, D>::fold_left_(f, f(accu, faces_));
    }

    template <unsigned N, unsigned D>
    template <typename BinaryFunction, typename T>
    T
    faces_set_mixin<N, D>::fold_left_(const BinaryFunction& f,
				      const T& accu) const
    {
      return faces_set_mixin<N - 1, D>::fold_left_(f, f(accu, faces_));
    }

    template <unsigned D>
    template <typename BinaryFunction, typename T>
    T
    faces_set_mixin<0u, D>::fold_left_(const BinaryFunction& f,
				       const T& accu) const
    {
      return f(accu, faces_);
    }

    template <typename BinaryFunction, typename T>
    T
    faces_set_mixin<0u, 0u>::fold_left_(const BinaryFunction& f,
					const T& accu) const
    {
      return f(accu, faces_);
    }

  } // end of namespace mln::internal


  /* ------------------------------------------------ */
  /* ``Static Apply-If-Dimension-Matches'' Operator.  */
  /* ------------------------------------------------ */

  template <unsigned D>
  template <typename UnaryFunction>
  typename UnaryFunction::result_type
  complex<D>::apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const
  {
    // Ensure N is compatible with D.
    mln_precondition(n <= D);
    return internal::faces_set_mixin<D, D>::apply_if_dim_matches_(n, f);
  }

  namespace internal
  {

    // FIXME: Try to factor.

    template <unsigned D>
    template <typename UnaryFunction>
    typename UnaryFunction::result_type
    faces_set_mixin<D, D>::apply_if_dim_matches_(unsigned n,
						 const UnaryFunction& f) const
    {
      // Ensure N and D are compatible.
      mln_precondition(n <= D);
      return n == D ?
	f(faces_) :
	faces_set_mixin<D - 1, D>::apply_if_dim_matches_(n, f);
    }

    template <unsigned N, unsigned D>
    template <typename UnaryFunction>
    typename UnaryFunction::result_type
    faces_set_mixin<N, D>::apply_if_dim_matches_(unsigned n,
						 const UnaryFunction& f) const
    {
      // Ensure N and D are compatible.
      mln_precondition(n <= D);
      return n == N ?
	f(faces_) :
	faces_set_mixin<N - 1, D>::apply_if_dim_matches_(n, f);
    }

    template <unsigned D>
    template <typename UnaryFunction>
    typename UnaryFunction::result_type
    faces_set_mixin<0u, D>::apply_if_dim_matches_(unsigned n,
						  const UnaryFunction& f) const
    {
      // If we reached this method, then N should be 0.
      mln_precondition(n == 0);
      return f(faces_);
    }

    template <typename UnaryFunction>
    typename UnaryFunction::result_type
    faces_set_mixin<0u, 0u>::apply_if_dim_matches_(unsigned n,
						  const UnaryFunction& f) const
    {
      // If we reached this method, then N should be 0.
      mln_precondition(n == 0);
      return f(faces_);
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_COMPLEX_HH