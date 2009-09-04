// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_TILED2D_HH
# define MLN_CORE_IMAGE_TILED2D_HH

/// \file
/// Definition of the basic mln::magick_tiled2d class.

# include <Magick++.h>

# include <mln/core/internal/image_primary.hh>
# include <mln/core/internal/fixme.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/routine/init.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all_to.hh>
# include <mln/value/proxy.hh>



namespace mln
{

  // Forward declaration.
  template <typename T> struct magick_tiled2d;


  namespace internal
  {

    /// Data structure for \c mln::magick_tiled2d<T>.
    template <typename T>
    struct data< magick_tiled2d<T> >
    {
      data(const std::string& filename);
      ~data();

      Magick::Image buffer_;
      Magick::PixelPacket* pixel_cache;
      T value_;

      box2d b_;  // theoretical box
      unsigned bdr_;
      box2d vb_; // virtual box, i.e., box including the virtual border

      void update_vb_();
      void allocate_();
      void deallocate_();
      void swap_(data< magick_tiled2d<T> >& other_);
      void reallocate_(unsigned new_border);
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename T>
    struct image_< magick_tiled2d<T> > : default_image_< T, magick_tiled2d<T> >
    {
      // misc
      typedef trait::image::category::primary category;
      typedef trait::image::speed::slow	      speed;
      typedef trait::image::size::regular     size;

      // value
      typedef trait::image::vw_io::none                    vw_io;
      typedef trait::image::vw_set::none                   vw_set;
      typedef trait::image::value_access::direct           value_access;
      typedef trait::image::value_storage::one_block       value_storage;
      typedef trait::image::value_browsing::site_wise_only value_browsing;
      typedef trait::image::value_alignment::with_grid     value_alignment;
      typedef trait::image::value_io::read_only           value_io;

      // site / domain
      typedef trait::image::pw_io::read_write        pw_io;
      typedef trait::image::localization::basic_grid localization;
      typedef trait::image::dimension::two_d         dimension;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_only     ext_io;
    };

  } // end of namespace mln::trait



  /// Basic 2D image class.
  ///
  /// The parameter \c T is the type of pixel values.  This image class
  /// stores data in memory and has a virtual border with constant
  /// thickness around data.
  ///
  /// \ingroup modimageconcrete
  //
  template <typename T>
  class magick_tiled2d : public internal::image_primary< T, mln::box2d, magick_tiled2d<T> >
  {
    typedef internal::image_primary< T, mln::box2d, magick_tiled2d<T> > super_;
  public:

    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef mln::value::proxy< magick_tiled2d<T> >       lvalue;


    /// Skeleton.
    typedef magick_tiled2d< tag::value_<T> > skeleton;


    /// Constructor without argument.
    magick_tiled2d();

    /// Constructor with a filename.
    magick_tiled2d(const std::string& filename);


    /// Initialize an empty image.
    void init_(const std::string& filename);


    /// Test if \p p is valid.
    bool has(const point2d& p) const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Give the bounding box domain.
    const box2d& bbox() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at point \p p.
    mln::value::proxy< magick_tiled2d<T> > operator()(const point2d& p);

    // Read access to the image value located at point \p p.
    const T& read_(const point2d& p) const;

    // Write access to the image value located at point \p p.
    void write_(const point2d& p, const T& value);


    template <typename P>
    T& alt(const P& p)
    {
      typedef def::coord coord_t;
      mln_precondition(this->has(p));

      return this->data_->array_ [*(coord_t*)(&p.p_hook_())] [*((coord_t*)(&p.p_hook_()) + 1)];
    }


    // Specific methods:
    // -----------------

    /// Read-only access to the image value located at (\p row, \p col).
    const T& at_(unsigned row, unsigned col) const;

    /// Read-write access to the image value located at (\p row, \p col).
    T& at_(unsigned row, unsigned col);

    /// Give the number of rows.
    unsigned nrows() const;

    /// Give the number of columns.
    unsigned ncols() const;


    // As a fastest image:
    // -------------------

    // Give the index of a point.
    //using super_::index_of_point;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of elements (points including border ones).
    unsigned nelements() const;

    /// Read-only access to the image value located at index \p i.
    const T& element(unsigned i) const;

    /// Read-write access to the image value located at index \p i.
    T& element(unsigned i);

    /// Give the delta-index corresponding to the delta-point \p dp.
    int delta_index(const dpoint2d& dp) const;

    /// Give the point corresponding to the index \p i.
    point2d point_at_index(unsigned i) const;

    /// Give a hook to the value buffer.
    const Magick::Image buffer() const;

    /// Give a hook to the value buffer.
    Magick::Image buffer();


    /// Resize image border with new_border.
    void resize_(unsigned new_border);
  };



  // Forward declaration

  template <typename T>
  void init_(tag::border_t, unsigned& bdr, const magick_tiled2d<T>& model);

  template <typename T, typename J>
  void init_(tag::image_t, mln::magick_tiled2d<T>& target, const J& model);



# ifndef MLN_INCLUDE_ONLY

  // init_

  template <typename T>
  inline
  void init_(tag::border_t, unsigned& bdr, const magick_tiled2d<T>& model)
  {
    bdr = model.border();
  }

  template <typename T, typename J>
  inline
  void init_(tag::image_t, magick_tiled2d<T>& target, const J& model)
  {
    box2d b;
    init_(tag::bbox, b, model);
    unsigned bdr;
    init_(tag::border, bdr, model);
    target.init_(b, bdr);
  }


  // internal::data< magick_tiled2d<T> >

  namespace internal
  {

    template <typename T>
    inline
    data< magick_tiled2d<T> >::data(const std::string& filename)
    {
      buffer_.read(filename);
      b_ = make::box2d(buffer_.rows(), buffer_.columns());
    }

    template <typename T>
    inline
    data< magick_tiled2d<T> >::~data()
    {
      deallocate_();
    }

    template <typename T>
    inline
    void
    data< magick_tiled2d<T> >::update_vb_()
    {
      vb_.pmin() = b_.pmin() - dpoint2d(all_to(bdr_));
      vb_.pmax() = b_.pmax() + dpoint2d(all_to(bdr_));
    }

    template <typename T>
    inline
    void
    data< magick_tiled2d<T> >::allocate_()
    {
      update_vb_();
      mln_postcondition(vb_.len(0) == b_.len(0) + 2 * bdr_);
      mln_postcondition(vb_.len(1) == b_.len(1) + 2 * bdr_);
    }

    template <typename T>
    inline
    void
    data< magick_tiled2d<T> >::deallocate_()
    {
    }

    template <typename T>
    inline
    void
    data< magick_tiled2d<T> >::swap_(data< magick_tiled2d<T> >& other_)
    {
      data< magick_tiled2d<T> > self_ = *this;
      *this = other_;
      other_ = self_;
    }

    template <typename T>
    inline
    void
    data< magick_tiled2d<T> >::reallocate_(unsigned new_border)
    {
      data< magick_tiled2d<T> >& tmp = *(new data< magick_tiled2d<T> >(this->b_, new_border));
      this->swap_(tmp);
    }


  } // end of namespace mln::internal


  // magick_tiled2d<T>

  template <typename T>
  inline
  magick_tiled2d<T>::magick_tiled2d()
  {
  }

  template <typename T>
  inline
  magick_tiled2d<T>::magick_tiled2d(const std::string& filename)
  {
    init_(filename);
  }

  template <typename T>
  inline
  void
  magick_tiled2d<T>::init_(const std::string& filename)
  {
    mln_precondition(! this->is_valid());
    this->data_ = new internal::data< magick_tiled2d<T> >(filename);
  }

  template <typename T>
  inline
  const box2d&
  magick_tiled2d<T>::domain() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  const box2d&
  magick_tiled2d<T>::bbox() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_;
  }

  template <typename T>
  inline
  bool
  magick_tiled2d<T>::has(const point2d& p) const
  {
    mln_precondition(this->is_valid());
    return this->data_->vb_.has(p);
  }

  template <typename T>
  inline
  const T&
  magick_tiled2d<T>::operator()(const point2d& p) const
  {
    return read_(p);
  }

  template <typename T>
  inline
  mln::value::proxy< magick_tiled2d<T> >
  magick_tiled2d<T>::operator()(const point2d& p)
  {
    mln::value::proxy<magick_tiled2d> prx(*this, p);
    return prx;
  }

  template <typename T>
  inline
  const T&
  magick_tiled2d<T>::read_(const point2d& p) const
  {
    mln::magick_tiled2d<T>* this_ = const_cast<mln::magick_tiled2d<T>* >(this); // Trust me, I have to do this(_).
    this_->data_->pixel_cache = this_->data_->buffer_.getPixels(p.col(), p.row(), p.col(), p.row());
    this->data_->value_.red() = this->data_->pixel_cache->red % 256;
    this->data_->value_.green() = this->data_->pixel_cache->green % 256;
    this->data_->value_.blue() = this->data_->pixel_cache->blue % 256;
    return this->data_->value_;
  }

  template <typename T>
  inline
  void
  magick_tiled2d<T>::write_(const point2d& p, const T& value)
  {
    std::cout << "setting value " << value << " at point " << p << std::endl; // DELETEME
    /*this->data_->pixel_cache = this->data_->buffer_.getPixels(p.col(), p.row(), p.col(), p.row());
    *(this->data_->pixel_cache) =  Magick::ColorRGB(256 - value.red(),
						    256 - value.green(),
						    256 - value.blue());*/
  }


  // Specific methods:

  template <typename T>
  inline
  const T&
  magick_tiled2d<T>::at_(unsigned row, unsigned col) const
  {
    mln_precondition(this->has(point2d(row, col)));
    //FIXME: use the cache Luke.
    this->data_->pixel_cache = this->data_->buffer_.getPixels(col, row, col, row);
    this->data_->value_.red() = this->data_->pixel_cache->red % 256;
    this->data_->value_.green() = this->data_->pixel_cache->green % 256;
    this->data_->value_.blue() = this->data_->pixel_cache->blue % 256;
    return this->data_->value_;
  }

  template <typename T>
  inline
  T&
  magick_tiled2d<T>::at_(unsigned row, unsigned col)
  {
    mln_precondition(this->has(point2d(row, col)));
    //FIXME: use the cache Luke.
    this->data_->pixel_cache = this->data_->buffer_.getPixels(col, row, col, row);
    this->data_->value_.red() = this->data_->pixel_cache->red % 256;
    this->data_->value_.green() = this->data_->pixel_cache->green % 256;
    this->data_->value_.blue() = this->data_->pixel_cache->blue % 256;
    return this->data_->value_;
  }

  template <typename T>
  inline
  unsigned
  magick_tiled2d<T>::nrows() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(0);
  }

  template <typename T>
  inline
  unsigned
  magick_tiled2d<T>::ncols() const
  {
    mln_precondition(this->is_valid());
    return this->data_->b_.len(1);
  }


  // Hooks.

  template <typename T>
  inline
  const Magick::Image
  magick_tiled2d<T>::buffer() const
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }

  template <typename T>
  inline
  Magick::Image
  magick_tiled2d<T>::buffer()
  {
    mln_precondition(this->is_valid());
    return this->data_->buffer_;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter2d.hh>
// # include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< magick_tiled2d<T> >
    {
      typedef fwd_pixter2d< magick_tiled2d<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const magick_tiled2d<T> >
    {
      typedef fwd_pixter2d< const magick_tiled2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< magick_tiled2d<T> >
    {
      typedef bkd_pixter2d< magick_tiled2d<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< const magick_tiled2d<T> >
    {
      typedef bkd_pixter2d< const magick_tiled2d<T> > ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< magick_tiled2d<T>, W >
    {
      typedef dpoints_fwd_pixter< magick_tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const magick_tiled2d<T>, W >
    {
      typedef dpoints_fwd_pixter< const magick_tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< magick_tiled2d<T>, W >
    {
      typedef dpoints_bkd_pixter< magick_tiled2d<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< const magick_tiled2d<T>, W >
    {
      typedef dpoints_bkd_pixter< const magick_tiled2d<T> > ret;
    };

    // nixter

    template <typename T, typename N>
    struct fwd_nixter< magick_tiled2d<T>, N >
    {
      typedef dpoints_fwd_pixter< magick_tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct fwd_nixter< const magick_tiled2d<T>, N >
    {
      typedef dpoints_fwd_pixter< const magick_tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< magick_tiled2d<T>, N >
    {
      typedef dpoints_bkd_pixter< magick_tiled2d<T> > ret;
    };

    template <typename T, typename N>
    struct bkd_nixter< const magick_tiled2d<T>, N >
    {
      typedef dpoints_bkd_pixter< const magick_tiled2d<T> > ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


# include <mln/make/image.hh>


#endif // ! MLN_CORE_IMAGE_TILED2D_HH
