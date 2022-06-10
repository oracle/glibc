/* Change for backporting: The build system does not define _ISOMAC
   for us because the tests-internal changes have not been
   backported.  */
#define _ISOMAC 1

#include <iostream>

using namespace std;

int
main (int argc, char **argv)
{
  /* Complexity to keep gcc from optimizing this away.  */
  cout << (argc > 1 ? argv[1] : "null");
  return 0;
}
