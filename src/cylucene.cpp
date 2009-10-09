#include <boost/python.hpp>

void init_document();
void init_search();
void init_analysis();
void init_queryparser();
void init_index();
void init_store();
void init_util();

BOOST_PYTHON_MODULE(_cylucene)
{
  init_document();
  init_index();
  init_search();
  init_analysis();
  init_queryparser();
  init_store();
  init_util();
}
