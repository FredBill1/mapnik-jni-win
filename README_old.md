mapnik-jni
==========

Bindings to use Mapnik from Java.  Supports programatically building/introspecting a map, enumerating features and rendering with [AGG](https://github.com/tomhughes/agg).

This repository was forked from [jlnr/mapnik-jni](https://github.com/jlnr/mapnik-jni), which seems to be defunct, and the original [SpatialInteractive/mapnik-jni](https://github.com/SpatialInteractive/mapnik-jni) was not useful for mapnik 3.x.

**I will do some changes and update this repository continuously.
I also accept pull requests to make mapnik jni better.**

Prerequisites
-------------

* Mapnik 3.x (aka mapnik trunk)
* JDK 6 ≤ x ≤ 8
* Ant ≥ 1.7
* On Windows, a Mapnik 3 SDK built using [these tools](https://github.com/mapbox/windows-builds)

Links
-----

* [JavaDocs](http://spatialinteractive.github.com/mapnik-jni/javadocs/index.html)
* [NanoMaps Server](https://github.com/SpatialInteractive/nanomaps-server) A tile server.  One of the first things written with the Java bindings and Mapnik 2. But it could still be used as a reference.
* [Mapnik Distiller](https://github.com/SpatialInteractive/mapnik-distiller) Map data optimizer.  Another bit written with the Java bindings.

Building
--------

* Make sure that the mapnik-config program is on your path and is the one you want to be linking against
* In the mapnik-jni directory, run "ant test"
* Your jar and shared library are in the build/dist directory.  Copy them to your project.  If the shared library is found in the same directory as the jar, then that is used with no further configuration.  Otherwise, the java.library.path and system default locations are searched depending on how you have your JVM setup.
* On Windows: Be sure to adjust the header/library paths in the Visual C++ project to match your current JDK.

Quick Start
-----------

If you have coded with the C++ API or the Python bindings, the Java API should feel familiar to you.  There are a few deviations:

* In Java, the Map class is called MapDefinition to avoid conflicts with java.util.Map
* Random global methods in Mapnik have generally been attached to the classes they operate on (ie. Map save/load, discovering style attributes, etc).  If you poke around with your favorite IDE, you should find things easily enough.
* Rendering methods are in the Renderer class

Here is some sample code:

```java
	import mapnik.*;
	
	// You must call initialize once.
	// Calling it repeatedly is fine and has no effect.
	Mapnik.initialize();
	
	Box2d bounds = new Box2d(-180, -90, 180, 90);
	try (
		MapDefinition m = new MapDefinition();
		Image image = new Image(512, 512)
	) {
		m.loadMap("mymapfile.xml", false);
		m.setSrs(Projection.LATLNG_PARAMS);
		m.resize(512, 512);
		m.zoomToBox(bounds);
		
		Renderer.renderAgg(m, image);
		
		byte[] contents = image.saveToMemory("png");
	}
```

Memory Management
-----------------

There are a few things to know about memory management:

* Most of the things you attach to a MapDefinition use pass-by-value semantics even though everything is obviously an object in Java.  For example, adding a Layer or Style copies the Layer or Style into the map.  The object you are left hanging on to is not "live."  A notable exception to this is the Datasource object.  These are pass by reference, just as they are in C++.
* Some very light weight Mapnik objects such as Box2d, Color, LayerDescriptor and Coord are implemented in pure Java.  Under the covers these will be converted to and from the given native representation.  These are all more or less value types in C++ so this made sense to do it this way and saves wrapping an endless number of small objects around native pointers.
* Anything that descends from NativeObject is backed by at least one pointer to a C++ mapnik object.  This is all transparent to you unless if you want to be aware of it.  NativeObject has a "close()" method that you can use to prematurely deallocate the backing native memory.  Any attempt to access methods on the object after that point will throw an exception.  If you don't call close() everything will be cleaned up at finalization.  However, remember that most of these objects are like icebergs: from the JVM's perspective they occupy 8 bytes of memory (a pointer) but reference potentially very large blocks of non Java heap memory.  For long running systems I have found that just letting the garbage collector take care of things is a good way to run your system out of memory.  If you know you are done with something, particular a "big" object like a Map, FeatureSet, Datasource, Image, etc, just close() it right then and there and save yourself the hassle.
* If you are going to be enumerating features off of a datasource from the Java side, be aware that when you seek to the next Feature, any objects you got from the previous feature (ie. the feature itself, geometries, etc) are effectively closed for you.  Don't try to access them.

Format strings
--------------

When invoking Image.saveToMemory/saveToFile, along with the format name, you can pass several format-specific options. To make the construction of the format string less error-prone, a builder class for each format supported was provided in the mapnik.outputformat package.

```java
//this returns "png8:c=16:z=9"
String format = new PngFormatStringBuilder()
    .withPalette()
    .withColors(16)
    .withCompression(9)
    .toString();

image.saveToFile(path, format);
```

What's New
-----------------
2020-03-24
### **FreetypeEngine API**
* Get All faceNames From fontsDir

2020-08-26
### **Logger**
* Set Logger Path And Level (debug = 0 warn = 1 error = 2 none = 3)

Build On Visual Studio
-----------------
* Visual Studio ≥ 2015
* CD folder msvc And Open [mapnik-jni.sln](msvc/mapnik-jni.sln) With VS
* Open Property Pages
* Select Configuration Properties -> VC++ Directories
* Change Include Directories to your jni files and your mapnik3.x include folder that you builded
* Change Library Directories to your mapnik3.x library floder that you builded
