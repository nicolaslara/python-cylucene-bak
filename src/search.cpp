
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include <CLucene.h>


using namespace boost::python;
using namespace lucene::search;



struct QueryWrap : Query, wrapper<Query>
{
  QueryWrap(const Query&q)
    : Query(q) { }

  virtual Query* clone() const { return this->get_override("clone")(); }
  virtual const TCHAR* getQueryName() const { return this->get_override("getQueryName")(); }
  virtual TCHAR* toString(const TCHAR *field) const { return this->get_override("toString")(field); }
  virtual bool equals(Query *other) const { return this->get_override("equals")(other); }
  virtual size_t hashCode() const { return this->get_override("hashCode")(); };
};


/* Exclusively using CyHits and CySearcher for getting hit results
   ensures that we never destroy a Searcher that has gone out of
   scope, even though we still hold a reference to a Hit object.

   For simplicity's sake, we want to return Searcher-like and
   Hits-like objects to Python that are safe for it to memory
   manage. We manually expose the Hits methods we care about. Since
   these aren't bridges to the actual CLucene classes, we don't need
   to inherit off them as would normally be done with Boost.Python. */

class CyHits {
public:
  CyHits() { }
  CyHits(boost::shared_ptr<lucene::search::Searcher> _s,
	 lucene::search::Hits *_h)
    : searcher(_s), hits(_h) { }

  CyHits(boost::shared_ptr<lucene::search::Searcher> _s,
	 boost::shared_ptr<lucene::search::Hits> _h)
    : searcher(_s), hits(_h) { }

  // No error checking here, as CyHits should never be directly
  // instantiated from Python and is guaranteed to always have a
  // non-NULL Hits pointer.
  int32_t length() const { return hits->length(); }
  lucene::document::Document &doc(const int32_t n) { return hits->doc(n); }
  int32_t id(const int32_t n) { return hits->id(n); }
  float_t score(const int32_t n) { return hits->score(n); }

protected:
  boost::shared_ptr<lucene::search::Searcher> searcher; // or perhaps can make this a weak_ptr
  boost::shared_ptr<lucene::search::Hits> hits;
};


class CySearcher {
public:
  CySearcher(lucene::search::Searcher *_s)
    : searcher(_s) { }

  CyHits *search(Query *q) {
    return new CyHits(searcher, searcher->search(q));
  }

protected:
  boost::shared_ptr<lucene::search::Searcher> searcher;
};


class CyIndexSearcher : public CySearcher {
public:
  CyIndexSearcher(char *path)
    : CySearcher(new lucene::search::IndexSearcher(path)) { }
};



void init_search()
{
  class_<QueryWrap>("Query", no_init);

  class_<CyHits>("Hits")
    .def("length", &CyHits::length)
    .def("doc", &CyHits::doc,
    	 return_internal_reference<>())
    .def("id", &CyHits::id)
    .def("score", &CyHits::score)
    ;

  class_<CyIndexSearcher>("IndexSearcher", init<char *>())
    .def("search", &CySearcher::search,
	 return_value_policy<manage_new_object>())
    ;
}
