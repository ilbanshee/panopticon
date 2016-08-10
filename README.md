Panopticon
=============================

A probe software used to monitor and debug remote systems, specifically
designed for linux embedded software development (but works on other
unices too).

Building
------------

Execute the following commands:

    $ ./autogen.sh && ./configure
    $ make

Dependencies
------------

In order to work Panopticon requires:
- [libuv](http://libuv.org/)
- [duktape](http://duktape.org/) (included under `/lib/duktape`)
- [apache avro](http://avro.apache.org/)
