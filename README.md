Netflix WebCrypto (nfwebcrypto)
================================

Netflix WebCrypto is a partial implementation of the [W3C Web Cryptography API](http://www.w3.org/TR/WebCryptoAPI/),
22 April 2013 Editor's Draft, as a native Chrome Pepper (PPAPI) plugin. The
goal is to make the Web Crypto Javascript API freely available to web developers for
experimentation prior to its implementation by browser vendors. Currently only
Google Chrome / Chromium on linux amd64 and Chrome OS amd64 is supported.

The published Web Crypto API is not currently implemented in its entirety, due to
limitations of browser plugin technology and initial focus on operations and
algorithms most useful to Netflix. Specifically,

Not Supported

* The streaming/progressive processing model in not supported
* Synchronous API's like getRandomValues() are not supported
* Algorithm normalizing rules are not fully implemented
* keyUsage is not currently enforced

Supported

* Interfaces supported:
  + Key, KeyPair
  + KeyOperation
  + CryptoOperation
* SubtleCrypto interface methods supported
  + encrypt, decrypt
  + sign, verify
  + generateKey
  + exportKey, importKey
  + wrapKey, unwrapKey
* Key formats supported
  + symmetric keys: raw and jwk (raw)
  + asymmetric keys: pkcs#8 (public), spki (private), and jwk (public only)
* Algorithms supported
  + HMAC SHA-256: sign, verify, importKey, exportKey, generateKey
  + AES-128 CBC w/ PKCS#5 padding: encrypt, decrypt, importKey, exportKey, generateKey
  + RSASSA-PKCS1-v1_5: sign, verify, importKey, generateKey
  + RSAES-PKCS1-v1_5: encrypt, decrypt, importKey, exportKey, generateKey
  + RSA-OAEP: wrapKey*, unwrapKey*
  + AES-KW: wrapKey*, unwrapKey*

*Wrap/Unwrap operations follow the Netflix [KeyWrap Proposal](http://www.w3.org/2012/webcrypto/wiki/KeyWrap_Proposal)
and support protection of the JWE payload with AES128-GCM.
It is be possible to wrap/unwrap the following key types: HMAC SHA-256 and AES-128 CBC.

Moving forward, Netflix will try to keep this implementation as much in sync with
the latest draft Web Crypto API spec as possible.

Requirements
------------

Linux

* Ubuntu 12.04 64-bit with build-essentials, openssl-dev, and cmake 2.8, or equivalent
* 64-bit Google Chrome / Chromium R22 or later

Chrome OS

* Chromium OS cross-build environment R22 or later
* Chrome OS R22 or later (tested on R27)

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
    $ cmake -DCMAKE_TOOLCHAIN_FILE=(repo)/cmake/toolchains/linux64.cmake -DCMAKE_BUILD_TYPE=[Debug|Release] (repo)
    $ make -j<N>

Build Results
-------------

Browser plugin - This is registered and run within the Chrome browser.

    (repo)/cadmium-plugin/libnfwebcrypto.so
    (repo)/cadmium-plugin/nfwebcrypto.info
    
Native gtest unit test executable (if built). This is run from the command
line.

    (repo)/crypto/test/test
    
Native CadmiumCrypto archive. Native apps will link to this archive.

    (repo)/crypto/libcadcrypto.a


How to run Unit Tests
---------------------

1. Run Chrome with the plugin installed using the --register-pepper-plugins command line. See
(repo)/misc/start.sh for an example. Once chrome is running, make sure the
plugin is loaded in chrome://plugins.

2. Point Chrome at http://netflix.github.io/nfwebcrypto/web/test_qa.html. The tests will run
automatically.

Sample Code
-----------

Please see the javascript unit tests for examples of how to operate the
Web Crypto API.

TODO - add some simple examples here


