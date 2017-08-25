// move example
#include <utility>      // std::move
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <string>       // std::string

class Bar
{
public:
    Bar(std::string&& bar):
    bar_(std::move(bar))
    {
    }

    std::string bar_;
};

class Foo
{
public:
    Foo():
        foo_("Test"),
        bar_(std::move(foo_))
    {

    }
    
    std::string foo_;
    Bar bar_;
};


int main () {
  std::string foo = "foo-string";
  std::string bar = "bar-string";
  std::vector<std::string> myvector;

  myvector.push_back (foo);                    // copies
  myvector.push_back (std::move(bar));         // moves

  std::cout << "myvector contains:";
  for (std::string& x:myvector) std::cout << ' ' << x;
  std::cout << '\n';

  std::cout << foo << std::endl;
  std::cout << bar << std::endl;

  Foo fooo;
  std::cout << fooo.foo_ << std::endl;
  std::cout << fooo.bar_.bar_ << std::endl;
  return 0;
}
