#include <CLucene.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace lucene::analysis;


struct AnalyzerWrap : Analyzer, wrapper<Analyzer>
{
  AnalyzerWrap(const Analyzer&) { };
  TokenStream* tokenStream(const TCHAR* c, lucene::util::Reader* r)
  {
    // fixme: look at this in depth later -- how do we represent tokenstreams?
    override tokenStream = this->get_override("tokenStream");
    return tokenStream(c,r);
  };

};


void init_analysis(){
  class_<AnalyzerWrap>("Analyzer", no_init);
  class_<standard::StandardAnalyzer, bases<Analyzer> >("StandardAnalyzer");
}
