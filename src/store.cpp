#include <CLucene.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace lucene::store;

/* CyLucene does not yet expose the ability to specify different
   Directory objects. */
struct DirectoryWrap : Directory, wrapper<Directory>
{ };

void init_store()
{ }
