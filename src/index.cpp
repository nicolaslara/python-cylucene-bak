#include <CLucene.h>
#include <boost/python.hpp>
#include "typeconversion.h"
#include <iostream>

using namespace boost::python;
using namespace lucene::index;


struct IndexReaderWrap : IndexReader, wrapper<IndexReader>
{
  IndexReaderWrap(const IndexReader& c) : IndexReader(c){}
  void doCommit() { }
  void doClose() { }
  void doSetNorm(int32_t, const TCHAR*, uint8_t) { }
  void doUndeleteAll() { }
  void doDelete(int32_t) { }
  void getFieldNames(lucene::index::IndexReader::FieldOption, lucene::util::StringArrayWithDeletor&) { }
  uint8_t* norms(const TCHAR*) { }
  void norms(const TCHAR*, uint8_t*) { }
  bool getTermFreqVectors(int32_t, Array<lucene::index::TermFreqVector*>&) { }
  TermFreqVector* getTermFreqVector(int32_t, const TCHAR*) { }
  int32_t numDocs() { }
  int32_t maxDoc() const { }
  bool document(int32_t, lucene::document::Document*) { }
  bool isDeleted(int32_t) { }
  bool hasDeletions() const { }
  TermEnum* terms() const { }
  TermEnum* terms(const Term*) const { }
  int32_t docFreq(const Term*) const { }
  TermPositions* termPositions() const { }
  TermDocs* termDocs() const { }
};

// IndexReaderMethods
static IndexReader * openIR(const char * path)
{ return IndexReader::open(path); }

int32_t simpleDelete(IndexReader* i, wstring w1, wstring w2)
{ 
  Term * t = new Term(wstring_to_tchar(w1),
                      wstring_to_tchar(w2));
  return i->deleteDocuments(t);
}

void closeIR(IndexReader * i)
{ i->close(); }

// IndexWriter methods
void addDocument1(IndexWriter & i, lucene::document::Document * d) 
{ i.addDocument(d); }

void init_index()
{
  class_<IndexWriter>("IndexWriter", 
                      init<const char *, lucene::analysis::Analyzer*, const bool>())
    .def("addDocument", &IndexWriter::addDocument)
    .def("addDocument", &addDocument1)
    .def("getMaxFieldLength", &IndexWriter::getMaxFieldLength)
    .def("setMaxFieldLength", &IndexWriter::setMaxFieldLength)
    .add_property("max_field_length", &IndexWriter::getMaxFieldLength, 
                  &IndexWriter::setMaxFieldLength)
    .def("getMaxBufferedDocs", &IndexWriter::getMaxBufferedDocs)
    .def("setMaxBufferedDocs", &IndexWriter::setMaxBufferedDocs)
    .add_property("max_buffered_docs", &IndexWriter::getMaxBufferedDocs, 
                  &IndexWriter::setMaxBufferedDocs)
    .def("getWriteLockTimeout", &IndexWriter::getWriteLockTimeout)
    .def("setWriteLockTimeout", &IndexWriter::setWriteLockTimeout)
    .add_property("write_lock_timeout", &IndexWriter::getWriteLockTimeout, 
                  &IndexWriter::setWriteLockTimeout)
    .def("getMergeFactor", &IndexWriter::getMergeFactor)
    .def("setMergeFactor", &IndexWriter::setMergeFactor)
    .add_property("merge_factor", &IndexWriter::getMergeFactor, 
                  &IndexWriter::setMergeFactor)
    .def("getCommitLockTimeout", &IndexWriter::getCommitLockTimeout)
    .def("setCommitLockTimeout", &IndexWriter::setCommitLockTimeout)
    .add_property("commit_lock_timeout", &IndexWriter::getCommitLockTimeout, 
                  &IndexWriter::setCommitLockTimeout)
    .def("getMinMergeDocs", &IndexWriter::getMinMergeDocs)
    .def("setMinMergeDocs", &IndexWriter::setMinMergeDocs)
    .add_property("min_merge_docs", &IndexWriter::getMinMergeDocs, 
                  &IndexWriter::setMinMergeDocs)
    .def("getUseCompoundFile", &IndexWriter::getUseCompoundFile)
    .def("setUseCompoundFile", &IndexWriter::setUseCompoundFile)
    .add_property("use_compound_file", &IndexWriter::getUseCompoundFile, 
                  &IndexWriter::setUseCompoundFile)
    .def("docCount", &IndexWriter::docCount)
    .def("optimize", &IndexWriter::optimize)
    .def("close", &IndexWriter::close)
    ;

  class_<IndexReaderWrap>("IndexReader", no_init)
    .def("close", &closeIR)
    .def("deleteDocuments", &simpleDelete);
    ;

  def("open", openIR,
      return_value_policy<manage_new_object>());
}
