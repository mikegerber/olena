// Copyright (C) 2008, 2009, 2010, 2014 EPITA Research and Development
// Laboratory (LRDE).
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_IMAGE_DMORPH_MUTABLE_EXTENSION_IMA_HH
# define MLN_CORE_IMAGE_DMORPH_MUTABLE_EXTENSION_IMA_HH

/// \file
///
/// Definition of a morpher that extends the domain of an image
/// with a readable and writable (rw) image.

# include <mln/core/internal/image_identity.hh>



namespace mln
{

  // Forward declaration.
  template <typename I, typename J> class mutable_extension_ima;


  namespace internal
  {

    /// Data structure for \c mln::mutable_extension_ima<I, J>.
    template <typename I, typename J>
    struct data< mutable_extension_ima<I, J> >
    {
      data(I& ima, J& ext);

      I ima_;
      J ext_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename J>
    struct image_< mutable_extension_ima<I, J> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::indirect value_access;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_only      ext_io;
    };

    template <typename I, typename J, typename V>
    struct ch_value< mutable_extension_ima<I, J>, V >
    {
      typedef mlc_converts_to(mln_value(J), V) keep_ext;
      typedef mln_ch_value(I, V)   Iv;
      typedef mutable_extension_ima<Iv, J> Iv_ext;
      typedef mlc_if(keep_ext, Iv_ext, Iv) ret;
    };

  } // end of namespace mln::trait



  /// Extends the domain of an image with an image.
  ///
  /// \ingroup modimagedomainmorpher
  //
  template <typename I, typename J>
  class mutable_extension_ima

    : public internal::image_identity< I, mln_domain(I), mutable_extension_ima<I, J> >,
      private mlc_converts_to(mln_value(J), mln_value(I))::check_t
  {
  public:

    /// Skeleton.
    typedef mutable_extension_ima< tag::image_<I>, tag::ext_<J> > skeleton;

    /// Image value type.
    typedef mln_value(I) value;

    /// Return type of read-only access.
    typedef mln_value(I) rvalue;

    /// Constructor without argument.
    mutable_extension_ima();

    /// Constructor from an image \p ima and a function \p ext.
    mutable_extension_ima(I& ima, J& ext);

    /// Deferred initialization from an image \p ima and a function \p
    /// ext.
    void init_(I& ima, J& ext);


    /// Test if \p p is valid.
    // Tech note: the 'template' allows for multiple input.
    template <typename P>
    bool has(const P& p) const;


    /// Read-only access to the image value located at site \p p;
    mln_value(I) operator()(const mln_psite(I)& p) const;

    /// Read-write access to the image value located at site \p p.
    mln_lvalue(I) operator()(const mln_psite(I)& p);


    /// Access to the extension domain (image).
    J& extension() ;
  };


  // init_

  template <typename I, typename J, typename M>
  void init_(tag::image_t, mutable_extension_ima<I,J>& target, const M& model);

  template <typename J, typename I>
  void init_(tag::extension_t, J& target, const mutable_extension_ima<I,J>& model);



# ifndef MLN_INCLUDE_ONLY

  // internal::data< mutable_extension_ima<I,S> >

  namespace internal
  {

    template <typename I, typename J>
    inline
    data< mutable_extension_ima<I, J> >::data(I& ima, J& ext)
      : ima_(ima),
	ext_(ext)
    {
    }

  } // end of namespace mln::internal

  // mutable_extension_ima<I, J>

  template <typename I, typename J>
  inline
  mutable_extension_ima<I, J>::mutable_extension_ima()
  {
  }

  template <typename I, typename J>
  inline
  mutable_extension_ima<I, J>::mutable_extension_ima(I& ima, J& ext)
  {
    init_(ima, ext);
  }

  template <typename I, typename J>
  inline
  void
  mutable_extension_ima<I, J>::init_(I& ima, J& ext)
  {
    this->data_ = new internal::data< mutable_extension_ima<I, J> >(ima, ext);
  }

  template <typename I, typename J>
  template <typename P>
  inline
  bool
  mutable_extension_ima<I, J>::has(const P& p) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(this->data_->ext_.is_valid());
    return
      this->data_->ima_.domain().has(p)
      || this->data_->ext_.has(p);
  }

  template <typename I, typename J>
  inline
  mln_value(I)
  mutable_extension_ima<I, J>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->is_valid());
    mln_precondition(has(p));
    // if-else is preferred to the ternary op to allow for the
    // function result to convert towards the expected return type.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      return this->data_->ext_(p);
  }

  template <typename I, typename J>
  inline
  mln_lvalue(I)
  mutable_extension_ima<I, J>::operator()(const mln_psite(I)& p)
  {
    mln_precondition(this->is_valid());
    mln_precondition(has(p));
    // if-else is preferred to the ternary op to allow for the
    // function result to convert towards the expected return type.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      return this->data_->ext_(p);
  }

  template <typename I, typename J>
  inline
  J&
  mutable_extension_ima<I, J>::extension()
  {
    mln_precondition(this->is_valid());
    return this->data_->ext_;
  }

  // init_

  template <typename I, typename J, typename M>
  inline
  void init_(tag::image_t, mutable_extension_ima<I,J>& target, const M& model)
  {
    I ima;
    init_(tag::image, ima, model);
    J ext;
    init_(tag::extension, ext, model);
    target.init_(ima, ext);
  }

  template <typename J, typename I>
  inline
  void init_(tag::extension_t, J& target, const mutable_extension_ima<I,J>& model)
  {
    typedef mlc_unconst(J) J_;
    J_& ext_ = const_cast< mutable_extension_ima<I,J>& >(model).extension();
    J_& target_ = const_cast<J_&>(target);
    target_ = ext_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_DMORPH_MUTABLE_EXTENSION_IMA_HH
