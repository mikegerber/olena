// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_EDGE_IMAGE_HH
# define MLN_MAKE_EDGE_IMAGE_HH

/// \file mln/make/edge_image.hh
///
/// Routine to create a edge image.

# include <mln/core/image/edge_image.hh>
# include <mln/core/image/vertex_image.hh>
# include <mln/core/concept/function.hh>
# include <mln/util/internal/id2element.hh>


namespace mln
{

  namespace make
  {


    /// Construct an edge image.
    ///
    /// \param[in] g  A graph
    /// \param[in] fv A function mapping edge ids to values.
    ///
    /// \return an edge image.
    //
    template <typename V, typename G>
    mln::edge_image<void,V,G>
    edge_image(const Graph<G>& g, const fun::i2v::array<V>& fv);


    /// Construct an edge image.
    ///
    /// \param[in] g  A graph
    /// \param[in] fp A function mapping edge ids to sites.
    /// \param[in] fv A function mapping edge ids to values.
    ///
    /// \return an edge image.
    //
    template <typename FP, typename FV, typename G>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const Graph<G>& g_,
	       const Function_i2v<FP>& fp,
	       const Function_i2v<FV>& fv);


    /// Construct an edge image.
    ///
    /// \param[in] v_ima_ A vertex image.
    /// \param[in] fp	  A function mapping edge ids to sites.
    /// \param[in] fv	  A function mapping two vertex ids to a value.
    ///			  The result is associated to the corresponding edge.
    ///
    /// \return an edge image.
    //
    template <typename P, typename V, typename FP, typename FV, typename G>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const vertex_image<P,V,G>& v_ima_,
	       const Function_v2v<FP>& fp_,
	       const Function_vv2v<FV>& fv_);



# ifndef MLN_INCLUDE_ONLY


    template <typename V, typename G>
    mln::edge_image<void,V,G>
    edge_image(const Graph<G>& g, const fun::i2v::array<V>& fv)
    {
      trace::entering("make::edge_image");
      mln_precondition(exact(g).is_valid());

      p_vertices<G> pv(g);
      mln::edge_image<void,V,G> ima(pv, fv);

      trace::exiting("make::edge_image");
      return ima;
    }


    template <typename FP, typename FV, typename G>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const Graph<G>& g_,
	       const Function_i2v<FP>& fp,
	       const Function_i2v<FV>& fv)
    {
      trace::entering("make::edge_image");
      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      p_vertices<G,FP> pv(g,fp);
      mln::edge_image<mln_result(FP),mln_result(FV),G> ima(pv, fv);

      trace::exiting("make::edge_image");
      return ima;
    }


    template <typename P, typename V, typename FP, typename FV, typename G>
    mln::edge_image<mln_result(FP),mln_result(FV),G>
    edge_image(const vertex_image<P,V,G>& v_ima_,
	       const Function_v2v<FP>& fp_,
	       const Function_vv2v<FV>& fv_)
    {
      trace::entering("make::edge_image");

      const FP& fp = exact(fp_);
      const FV& fv = exact(fv_);
      const vertex_image<P,V,G>& v_ima = exact(v_ima_);
      mln_precondition(v_ima.is_valid());

      fun::i2v::array<mln_result(FV)> tmp_fv(v_ima.domain().graph().e_nmax());

      p_edges<G,FP> pe(v_ima.domain().graph(), fp);

      typedef mln::edge_image<mln_result(FP),mln_result(FV),G> edge_ima_t;
      edge_ima_t ima_e(pe, tmp_fv);

      mln_piter(edge_ima_t) e(ima_e.domain());
      for_all(e)
	ima_e(e) = fv(v_ima(e.element().v1()), v_ima(e.element().v2()));

      trace::exiting("make::edge_image");
      return ima_e;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::make

} // end of namespace mln

# endif // ! MLN_MAKE_EDGE_IMAGE_HH