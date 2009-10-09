
from __future__ import with_statement

import sys
import os

import cylucene


path = raw_input("Enter the path to index (default: .): ") or '.'
index_p = (raw_input("Enter the path to store the index (default: /tmp/index/):")
         or '/tmp/index/')

analyzer = cylucene.analysis.StandardAnalyzer()
index = cylucene.index.IndexWriter(index_p, analyzer, True)

for root, dirs, filenames in os.walk(path):
    for filename in filenames:
        print "indexing %s/%s" % (root, filename)

        with open("%s/%s" % (root, filename)) as finp:
            doc = cylucene.document.Document()
            doc.add('contents', unicode(finp.read(), errors='ignore'),
                    store='yes', index='tokenized')
            doc.add("path", "%s/%s" % (root, filename),
                    store='yes', index='no')
            index.addDocument(doc, analyzer)

index.optimize()
index.close()

s = cylucene.search.IndexSearcher(index_p)

while True:
    query = raw_input("\nEnter the search term (blank to exit): ")
    if query:
        q = cylucene.queryparser.parse(query, "contents", analyzer)
        if q:
            h = s.search(q)
            print "\n%s results found:" % h.length()
            print

            i = 1
            for doc, score in h.with_score():
                print "%d (%f): %s" % (i, score, doc["path"])
                i+=1
    else:
        break

print 'Enjoy CyLucene!'
