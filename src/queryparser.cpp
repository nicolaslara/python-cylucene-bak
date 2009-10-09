
#include <boost/python.hpp>

#include <CLucene.h>

#include "typeconversion.h"


using namespace boost::python;
using namespace lucene::queryParser;


/* CLucene provides a way to directly instantiate a QueryParser (and
   MultiQueryParser), but we only expose the static parse method
   provided by QueryParser. */

lucene::search::Query* queryparser_parse(wstring query, wstring field,
					 lucene::analysis::Analyzer *analyzer)
{
  std::auto_ptr<TCHAR> scoped_query(wstring_to_tchar(query));
  std::auto_ptr<TCHAR> scoped_field(wstring_to_tchar(field));

  return QueryParser::parse(scoped_query.get(),
			    scoped_field.get(),
			    analyzer);
}


void init_queryparser(){
  class_<QueryParser>("QueryParser",
		      no_init);
  def("parse", queryparser_parse,
      return_value_policy<manage_new_object>());
}
