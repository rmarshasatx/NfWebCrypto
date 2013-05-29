Netflix WebCrypto (nfwebcrypto)
================================

Directory Tour
--------------

    base/
        Common C++ source for both the plugin and crypto component.
    cmake/
        Cmake toolchain files supporting linux desktop and Chrome OS builds.
        Linux desktop builds use the linux system build tools and libs.
        Only 64-bit builds are supported for now.
    crypto/
        C++ source for the crypto component. The contents of this directory is
        of primary interest to native devs; the entry point is the CadmiumCrypto
        class. This directory currently builds to an archive file.
    crypto/test/
        Contains C++ gtest unit tests that exercise the CadmiumCrypto class
        interface. Not fleshed out yet and currently not built.
    misc/
        Miscellaneous code to support development. Currently has code to run and
        debug the Chrome browser with the plugin properly registered.
    plugin/
        C++ source of the PPAPI plugin. This code builds to shared library that
        is dl-loaded by the browser when the plugin is registered. It handles
        interfacing with the browser, bridging to the crypto thread, and decode/
        dispatch of JSON messages to and from the browser. Native devs will
        probably only be interested in the NativeBridge class here.
    web/nfcrypt.js
        The javascript front-end for the plugin. The bottom level of this code
        handles the transport of JSON-serialized messages to and from the
        plugin, while the top level implements the W3C WebCrypto interface.
        Native devs will need to change the bottom level to match their bridge
        API. This source file borrows heavily from polycrypt (polycrypt.net)
    web/test_qa.html
        The Jasmine HTML unit tests that exercise the javascript WebCrypto
        API exposed to the javascript client by nfcrypt.js.
        

How to Build
------------
The following has been verified on Ubunutu 12.04. cmake 2.8 or later is required.

    $ mkdir buildDir
    $ cd buildDir
    $ cmake -DCMAKE_TOOLCHAIN_FILE=<repo>/cmake/toolchains/linux64.cmake -DCMAKE_BUILD_TYPE=[Debug|Release] <repo>
    $ make -j<N>

Build Results
-------------

Browser plugin - This is registered and run within the Chrome browser.

    <repo>/cadmium-plugin/libnfwebcrypto.so
    <repo>/cadmium-plugin/nfwebcrypto.info
    
Native gtest unit test executable (if built). This is run from the command
line.

    <repo>/crypto/test/test
    
Native CadmiumCrypto archive. Native apps will link to this archive.

    <repo>/crypto/libcadcrypto.a


How to run Unit Tests
---------------------

1. Host <repo>/web on your web server.

2. Run chrome wit the plugin installed using the command line. See
<repo>/misc/start.sh for an example. Once chrome is running, make sure the
plugin is loaded in chrome://plugins.

3. Point chrome to test_qa.html on your web server. The tests will run
automatically. 

