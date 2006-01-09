//Small header to get STLport numerous defines:
#include <utility>

#if defined (STLPORT) && !defined (_STLP_NO_EXTENSIONS) && !defined (__DMC__)
#  include <rope>
#  if !defined (_STLP_USE_NO_IOSTREAMS)
#    include <sstream>

#    include "cppunit/cppunit_proxy.h"

#    if defined (_STLP_USE_NAMESPACES)
using namespace std;
#    endif

//
// TestCase class
//
class RopeTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(RopeTest);
#    if defined (__BORLANDC__)
  CPPUNIT_IGNORE;
#    endif
  CPPUNIT_TEST(io);
  CPPUNIT_TEST_SUITE_END();

protected:
  void io();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RopeTest);

//
// tests implementation
//
void RopeTest::io()
{
#    if !defined (__BORLANDC__)
  char const* cstr = "rope test string";
  crope rstr(cstr);

  {
    ostringstream ostr;
    ostr << rstr;

    CPPUNIT_ASSERT( ostr );
    CPPUNIT_ASSERT( ostr.str() == cstr );
  }
#    endif
}
#  endif
#endif
