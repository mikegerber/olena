# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/core/image/sub_image.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>

# include <mln/morpho/elementary/gradient.hh>
# include <mln/level/fill_with_value.hh>


int main()
{
  using namespace mln;

  typedef image2d<unsigned> I;
  I ima(3, 5);
  debug::iota(ima);
  debug::println(ima);

  sub_image<I, box2d> sub = ima | box2d(2,3);
  debug::println(sub);
  debug::println(morpho::elementary::gradient(sub, c4()));

  level::fill_with_value((ima | box2d(2,3)).rw(), 0);
  debug::println(ima);

  trait::image::print(sub);
}