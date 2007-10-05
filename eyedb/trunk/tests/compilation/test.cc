#include <iostream>
#include "libtest.h"

using namespace std;

int main( int ac, char **av)
{
  Test t;

  std::cout << "f returns " << t.f( 0) << std::endl;
}
