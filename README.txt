About
----------

TacitPixel is a prototyping tool and framework for building scene graphs. It is not feature complete
but does demonstrate a number of things that are somewhat helpful.

TacitPixel started its life in 1999 when there were not many scene graph libraries available. 
I started to write it to support my research and to quickly prototype tools for architectural design. That's where the original "The Architectural Playground" libTAP name came from and also the tpXYZ naming scheme. You might also stumble upon the software as TwistedPair when I worked at the HIT Lab NZ. It was probably one of the first real OpenGL ES based scene graphs on Windows Mobile 5.


Install
-------

You need CMake 2.8 or higher to generate the build system.


Dependencies
------------

TacitPixel comes with batteries included. Hence there are a number of libraries that I have chosen
purely from the viewpoint of being small (work on embedded systems), fast and flexible.

* zlib 		1.2.6
* minizip 
* lib3ds	2.0.1
* glm		N/A	N/A, 1997, Nate Robins
* libpng 
* libjpeg
* libogg
* libvorbis 
* dlmalloc	2.8.5	public domain, 2011 Doug Lea
* stb_truetype	0.5 	public domain, 2009 Sean Barrett / RAD Game Tools
* rapidxml 	1.13 	copyright (C) 2006, 2009 Marcin Kalicinski (Boost/MIT)


ToDo
----

+ migrate to libarchive (New BSD) for handling compressed files
- remove zlib, minizip etc.

 
