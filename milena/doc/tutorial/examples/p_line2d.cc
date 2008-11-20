# include <mln/core/image/image2d.hh>
# include <mln/pw/all.hh>

# include <mln/core/site_set/p_line2d.hh>
# include <mln/core/site_set/p_set.hh>

# include <mln/core/image/sub_image.hh>
# include <mln/level/fill.hh>
# include <mln/debug/println.hh>



int main()
{
  using namespace mln;

  image2d<char> ima(6, 11);
  level::fill(ima, '.');

  p_line2d line(point2d(4,10),
		point2d(1,1));
  std::cout << line << std::endl;

  level::fill((ima | line).rw(), 'x');
  debug::println(ima);

  p_set<point2d> s;
  s += line;
  mln_assertion(s.has(line.begin()) && s.has(line.end()));
  std::cout << s << std::endl;

  debug::println(pw::cst('o') | line);
  // ...
}