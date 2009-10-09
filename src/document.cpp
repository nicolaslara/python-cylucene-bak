
#include <boost/python.hpp>

#include <CLucene.h>

#include "typeconversion.h"


using namespace boost::python;
using namespace lucene::document;


/* Field. We expose this interface because you can iterate through a
   document's fields. However, CyLucene typically requires you to use
   the Pythonic Document.add function rather than Field instances. */
struct FieldWrap : Field, wrapper<Field>
{
  FieldWrap(const wstring w1, const wstring w2, int i) : 
    Field(wstring_to_tchar(w1), 
          wstring_to_tchar(w2), i){ };
  FieldWrap(const Field& f) : Field(f) { };
};

wstring field_name(Field &f) { return tchar_to_wstring(f.name()); }
wstring field_toString(Field &f) { return tchar_to_wstring(f.toString()); }
wstring field_stringValue(Field &f) { return tchar_to_wstring(f.stringValue()); }


/* Document */
struct DocumentWrap : Document, wrapper<Document>
{
  DocumentWrap() : Document() { }
  DocumentWrap(const Document &d)
    : Document(d) { }
};

wstring document_toString(Document& d)
{
  std::auto_ptr<TCHAR> ret(d.toString());
  return tchar_to_wstring(ret.get());
}

wstring document_get(Document& d, wstring field)
{
  std::auto_ptr<TCHAR> scoped_field(wstring_to_tchar(field));
  return wstring(d.get(scoped_field.get()));
}

void document_add(Document& d, const wstring name, const wstring value, int config)
{
  std::auto_ptr<TCHAR> scoped_name(wstring_to_tchar(name));
  std::auto_ptr<TCHAR> scoped_value(wstring_to_tchar(value));
  Field *f = new Field(scoped_name.get(), scoped_value.get(),
                       config);
  /* d owns f, despite the call-by-reference declaration. Oh CLucene,
     why do you throw sand in the eyes of object ownership sanity? */
  d.add(*f);
}

void document_add_default(Document& d, const wstring w1, const wstring w2)
{
  document_add(d, w1, w2, Field::STORE_YES | Field::INDEX_TOKENIZED);
}

void document_removeField(Document& d, wstring field)
{
  std::auto_ptr<TCHAR> scoped_field(wstring_to_tchar(field));
  d.removeField(scoped_field.get());
}

Field *document_getField(Document& d, const wstring field)
{
  std::auto_ptr<TCHAR> scoped_field(wstring_to_tchar(field));
  return d.getField(scoped_field.get());
}


void init_document()
{
  class_<DocumentWrap>("Document")
    .def("get", &document_get)
    .def("getBoost", &DocumentWrap::getBoost)
    .def("setBoost", &DocumentWrap::setBoost)
    .add_property("boost", &DocumentWrap::getBoost, &DocumentWrap::setBoost)
    .def("add", &document_add, with_custodian_and_ward<1,2>())
    .def("add", &document_add_default, with_custodian_and_ward<1,2>())
    .def("clear", &Document::clear)
    .def("removeField", &document_removeField)
    .def("getField", document_getField,
         return_internal_reference<>())

    /* The caller owns the DocumentFieldEnumeration that
       Document::fields() returns. The Fields that
       DocumentFieldEnumeration returns, however are not owned by the
       caller, rather they're still the wards of the Document
       object. */
    .def("fields", &Document::fields,
         return_value_policy<manage_new_object>())

    .def("toString", &document_toString)
    ;

  class_<FieldWrap>("Field", init<const wstring, const wstring, int>())
    //reader?
    .def("name", &field_name)
    .def("toString", &field_toString)
    .def("stringValue", &field_stringValue)
    .def("isStored", &Field::isStored)
    .def("isIndexed", &Field::isIndexed)
    .def("isTokenized", &Field::isTokenized)
    .def("isCompressed", &Field::isCompressed)
    .def("isTermVectorStored", &Field::isTermVectorStored)
    .def("isStoreOffsetWithTermVector", &Field::isStoreOffsetWithTermVector)
    .def("isStorePositionWithTermVector", &Field::isStorePositionWithTermVector)
    .def("isTermVectorStored", &Field::isTermVectorStored)
    .def("isTermVectorStored", &Field::isTermVectorStored)
    .def("isBinary", &Field::isBinary)
    .def("getBoost", &Field::getBoost)
    .def("setBoost", &Field::setBoost)
    .add_property("boost", &Field::getBoost, &Field::setBoost)
    .def("getOmitNorms", &Field::getOmitNorms)
    //.def("setOmitNorms", &Field::setOmitNorms) //Expert
    ;

  enum_<Field::Store>("Store")
    .value("STORE_YES", Field::STORE_YES)
    .value("STORE_NO", Field::STORE_NO)
    .value("STORE_COMPRESS", Field::STORE_COMPRESS)
    ;

  enum_<Field::Index>("Index")
    .value("INDEX_NO", Field::INDEX_NO)
    .value("INDEX_TOKENIZED", Field::INDEX_TOKENIZED)
    .value("INDEX_UNTOKENIZED", Field::INDEX_UNTOKENIZED)
    .value("INDEX_NONORMS", Field::INDEX_NONORMS)
    ;

  enum_<Field::TermVector>("TermVector")
    .value("TERMVECTOR_NO", Field::TERMVECTOR_NO)
    .value("TERMVECTOR_YES", Field::TERMVECTOR_YES)
    .value("TERMVECTOR_WITH_POSITIONS", Field::TERMVECTOR_WITH_POSITIONS)
    .value("TERMVECTOR_WITH_OFFSETS", Field::TERMVECTOR_WITH_OFFSETS)
    ;

  class_<DocumentFieldEnumeration>("DocumentFieldEnumeration", no_init)
    .def("hasMoreElements", &DocumentFieldEnumeration::hasMoreElements)
    .def("nextElement", &DocumentFieldEnumeration::nextElement,
         return_internal_reference<>())
    ;
  
}
