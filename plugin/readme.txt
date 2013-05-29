How to build the plugin

$ mkdir buildDir
$ cd buildDir
$ # <pluginTop> is e.g. .../pepperCrypto
$ cmake -DCMAKE_TOOLCHAIN_FILE=<pluginTop>/cmake/toolchains/[linux64|crosArm].cmake -DCMAKE_BUILD_TYPE=[Debug|Release] <pluginTop>
$ make -jN  # N=number active cores on build machine

The plugin .so and .info file will land in <pluginTop>/cadmium-plugin

For cros builds, these steps must be done inside the chroot.
