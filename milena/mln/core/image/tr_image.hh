// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_IMAGE_TR_IMAGE_HH
# define MLN_CORE_IMAGE_TR_IMAGE_HH

/*! \file mln/core/image/tr_image.hh
 *
 *  \brief Definition of the morpher mln::tr_image presenting an image
 *  through a (bijective) transformation.
 *
 */

# include <cmath>

# include <mln/core/internal/image_identity.hh>
# include <mln/algebra/vec.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Fwd decl.
  template <typename S, typename I, typename T> struct tr_image;

  namespace internal
  {

    /// Data structure for \c mln::tr_image<S,I,T>.
    template <typename S, typename I, typename T>
    struct data< tr_image<S,I,T> >
    {
      data(const S& s, I& ima, const T& tr);

      I ima_;
      T tr_;
      S s_;
    };

  } // end of namespace mln::internal

  namespace trait
  {

    template <typename S, typename I, typename T>
    struct image_< tr_image<S,I,T> >
      : public image_<I> // Same as I except...
    {
      // ...these changes.
      typedef trait::image::value_io::read_only value_io;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
    };

  } // end of namespace mln::trait

  /*! \brief Morpher that makes an image become transformed by a given
   *  transformation.
   *
   */
  template <typename S, typename I, typename T>
  struct tr_image :
    public mln::internal::image_identity< I, mln_pset(I), tr_image<S,I,T> >
  {

    /// Super type.
    typedef
    mln::internal::image_identity< I, mln_pset(I), tr_image<S,I,T> > super_;

    /// Point_Site associated type.
    typedef mln_psite(I) psite;

    /// Value associated type.
    typedef mln_value(I) value;

    /// Return type of read-write access.
    typedef mln_value(I) lvalue; // FIXME: Depends on lvalue presence in I.

    /// Return type of read-only access.
    typedef mln_value(I) rvalue;

    /// Skeleton.
    typedef tr_image< S, tag::image_<I>, T> skeleton;


    /// Constructors.
    tr_image(const S& s, I& ima, const T& tr);
    /* FIXME: What's the purpose of this ctor?  AFAIK, morphers
       objects (and images in general) cannot have their structure /
       core data altered after they're built.  Here, there's a
       (partial) exception: this morpher provides set_tr(), but has no
       set_ima().  I (Roland) don't see the point in keeping this ctor
       if we do not provide set_ima().  */
    tr_image();

    /// Initialize an empty image.
    void init_(const S& s, I& ima, const T& tr);

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Test if a pixel value is accessible at \p p.
    using super_::has;

    /// Test if a pixel value is accessible at \p v.
    bool has(const mln::algebra::vec<I::psite::dim, float>& v) const;

    /// Read-only access of pixel value at point site \p p.
    /// Mutable access is only OK for reading (not writing).
    //using super_::operator();

    mln_value(I) operator()(const psite& p) const;
    mln_value(I) operator()(const psite& p);

    void set_tr(T& tr);
  };



# ifndef MLN_INCLUDE_ONLY

  namespace internal
  {

    // internal::data< tr_image<I,S> >

    template <typename S, typename I, typename T>
    inline
    data< tr_image<S,I,T> >::data(const S& s, I& ima, const T& tr)
      : ima_(ima),
	tr_(tr),
        s_(s)
    {
    }

  } // end of namespace mln::internal

  template <typename S, typename I, typename T>
  inline
  tr_image<S,I,T>::tr_image(const S& s, I& ima, const T& tr)
  {
    init_(s, ima, tr);
  }

  template <typename S, typename I, typename T>
  inline
  void
  tr_image<S,I,T>::init_(const S& s, I& ima, const T& tr)
  {
    mln_precondition(ima.has_data());
    this->data_ = new internal::data< tr_image<S,I,T> >(s, ima, tr);
  }

  template <typename S, typename I, typename T>
  inline
  bool
  tr_image<S,I,T>::has_data() const
  {
    mln_invariant(this->delegatee_()->has_data());
    return true;
  }

  template <typename S, typename I, typename T>
  inline
  bool
  tr_image<S,I,T>::has(const algebra::vec<I::psite::dim, float>& v) const
  {
    mln_psite(I) p;
    algebra::vec<I::point::dim, float> v2 = this->data_->tr_.inv()(v);
    for (unsigned i = 0; i < I::point::dim; ++i)
      p[i] = static_cast<int>(round(v2[i]));
    return this->delegatee_().has(p);
  }


  template <typename S, typename I, typename T>
  inline
  mln_value(I)
  tr_image<S,I,T>::operator()(const psite& p) const
  {
    algebra::vec<I::psite::dim, float> v = p.to_vec();
    return this->data_->ima_(this->data_->tr_.inv()(v));
  }

  template <typename S, typename I, typename T>
  inline
  mln_value(I)
  tr_image<S,I,T>::operator()(const psite& p)
  {
    algebra::vec<I::psite::dim, float> v = p.to_vec();
    return this->data_->ima_(this->data_->tr_.inv()(v));
  }

  template <typename S, typename I, typename T>
  inline
  void
  tr_image<S,I,T>::set_tr(T& tr)
  {
    this->data_->tr_ = tr;
  }

  template <typename S, typename I, typename T>
  inline
  tr_image<S,I,T>
  transposed_image(const Site_Set<S>& s, Image<I>& ima, const Function_x2x<T>& t)
  {
    tr_image<S,I,T> tmp(exact(s), exact(ima), exact(t));
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_TR_IMAGE_HH