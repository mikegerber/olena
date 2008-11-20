#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  image2d<int> ima(make::box2d(0,0, 10,12));

  std::cout << "nrows = " << ima.nrows()
	    << " - "
	    << "ncols = " << ima.ncols()
	    << std::endl;
}