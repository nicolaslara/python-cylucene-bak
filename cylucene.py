
__all__ = ('document', 'analysis', 'index', 'search', 'queryparser')

import _cylucene


# We take the Boost.Python wrapped classes in _cylucene and generate
# more Pythonic interfaces here.


# Document

def document_str(self):
    return self.toString()

def document_getitem(self, field):
    try:
        return self.getField(field)
    except ArgumentError, e:
        raise TypeError("expected a string for field name")

def document_gen(self):
    df = self.fields()
    while df.hasMoreElements():
        yield df.nextElement()

def document_add(self, name, value, index='tokenized',
                 store='yes', termvector='undefined'):
    i = {
        'no': document.Field.Index.INDEX_NO,
        'tokenized': document.Field.Index.INDEX_TOKENIZED,
        'untokenized': document.Field.Index.INDEX_UNTOKENIZED,
        'nonorms': document.Field.Index.INDEX_NONORMS
        }
    s = {
        'yes': document.Field.Store.STORE_YES,
        'no': document.Field.Store.STORE_NO,
        'compress' : document.Field.Store.STORE_COMPRESS
        }
    t = {
        'with_offsets': document.Field.TermVector.TERMVECTOR_WITH_OFFSETS,
        'with_possitions': document.Field.TermVector.TERMVECTOR_WITH_POSITIONS,
        'no': document.Field.TermVector.TERMVECTOR_NO,
        'yes': document.Field.TermVector.TERMVECTOR_YES,
        'undefined' : 0
        }
    config = i[index] | s[store] | t[termvector]
    self._add(name, value, config)


_cylucene.Document.__str__ = document_str
_cylucene.Document.__getitem__ = document_getitem
_cylucene.Document.__iter__ = document_gen

# We're going to replace the _cylucene.Document.add function with our
# own. But first, rename the old function since our new one expects it
# at _add.
_cylucene.Document._add = _cylucene.Document.add
_cylucene.Document.add = document_add

# Don't need these references anymore.
del document_str
del document_getitem
del document_gen
del document_add




def field_str(self):
    return self.stringValue()

_cylucene.Field.__str__ = field_str

# _cylucene rearranges the namespace a little. Here, we fix it back up
# so that it more closely resembles CLucene's namespace.
_cylucene.Field.Store = _cylucene.Store
_cylucene.Field.Index = _cylucene.Index
_cylucene.Field.TermVector = _cylucene.TermVector



# some functions for CyHits objects

def hits_length(self):
    return self.length()

def hits_gen(self):
    for i in xrange(len(self)):
        yield self.doc(i)

def hits_gen_with_score(self):
    for i in xrange(len(self)):
        yield (self.doc(i), self.score(i))

def hits_getitem(self, item):
    if isinstance(item, slice):
        return [self.doc(i) for i in range(*item.indices(self.length()))]
    else:
        if item < 0 or item >= len(self):
            raise IndexError("Index %d out of range: [0,%s)" % (item, len(self)))

        return self.doc(item)

_cylucene.Hits.__len__ = hits_length
_cylucene.Hits.__iter__ = hits_gen
_cylucene.Hits.__getitem__ = hits_getitem
_cylucene.Hits.with_score = hits_gen_with_score

del hits_length
del hits_gen
del hits_getitem
del hits_gen_with_score



def index_writer_length(self):
    return self.docCount()

_cylucene.IndexWriter.__len__ = index_writer_length
del index_writer_length




# set up some namespaces for cylucene module, ie cylucene.document,
# cylucene.index, etc...

class Module():
    def __init__(self, doc=""):
        """Create an object to represent a module."""
        self.__doc__ = doc

    def _add(self, *objs):
        """Add an object to a Module. For internal use only."""
        for obj in objs:
            setattr(self, obj.__name__, obj)

document    = Module("CyLucene's document namespace")
analysis    = Module("CyLucene's analysis namespace")
index       = Module("CyLucene's index namespace")
queryparser = Module("CyLucene's queryParser namespace")
search      = Module("CyLucene's search namespace")
store       = Module("CyLucene's store namespace")
util        = Module("CyLucene's util namespace")

del Module

document._add(_cylucene.DocumentFieldEnumeration,
              _cylucene.Document,
              _cylucene.Field)

analysis._add(_cylucene.Analyzer,
              _cylucene.StandardAnalyzer)

index._add(_cylucene.IndexWriter,
           _cylucene.IndexReader,
           _cylucene.open)

search._add(_cylucene.IndexSearcher,
            _cylucene.Hits)

queryparser._add(_cylucene.QueryParser,
                 _cylucene.Query,
                 _cylucene.parse)

