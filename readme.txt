
CyLucene was written by Nicolas Lara (nicolaslara@gmail.com) with
contributions from Wes Chow (wes.chow@s7labs.com). The work was
sponsored by S7 Labs (www.s7labs.com). If you have any questions, take
a look at the Google Code page to see if it's been answered
already. If not, questions can be directed to Wes.

CyLucene exposes a minimally useful set of CLucene's
functionality. Feedback on what else is needed for the module to be
useful would be greatly appreciated.

If you are unfamiliar with Lucene concepts, here is a decent tutorial:
http://www.darksleep.com/lucene/

CyLucene depends on CLucene (obviously) and Boost.Python. It has only
been tested with Python 2.5.2, CLucene 0.9.21, and Boost.Python
1.36.0.



TODO

- expose Directory classes and make ram based directory an option
  (useful for testing)

- create real test suite

- need examples, document or tutorial video

- add --with-clucene and --with-boost options to setup.py
