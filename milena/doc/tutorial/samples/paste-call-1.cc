#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  image2d<char> imgb(make::box2d(5,5, 7,8));
  // Initialize imga with the same domain as imgb.
  image2d<char> imga(imgb.domain());

  // Initialize the image values.
  level::fill(imgb, 'b');

  // Paste the content of imgb in imga.
  level::paste(imgb, imga);

  debug::println(imga);
}