# include <mln/core/image2d.hh>
# include "fllt.hh"
# include <mln/debug/println.hh>
# include <mln/convert/to_w_window.hh>
# include <mln/core/w_window2d_int.hh>
# include <mln/convert/to_image.hh>

int main()
{

  using namespace mln;

  int ws[81] = {3,2,3,3,5,5,5,5,5,
		2,1,3,4,4,4,4,5,5,
		2,3,4,2,3,3,2,4,4,
		1,4,2,1,1,2,1,2,2,
		1,2,4,2,1,2,1,1,1,
		1,3,3,4,2,3,2,1,1,
		1,3,3,4,2,3,2,1,1,
		1,3,3,4,2,3,2,1,1,
		1,3,3,4,2,3,2,1,1};

  w_window2d_int w_win = make::w_window2d(ws);

  image2d<int> ima = convert::to_image(w_win);

  debug::println(ima);
  compute_level_set(ima);
}
