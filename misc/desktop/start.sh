#!/bin/bash

PWD=`pwd`
PLUGINHOME="$PWD/cadmium-plugin"
INFO_FILE="$PLUGINHOME/nfwebcrypto.info"

export LD_LIBRARY_PATH="$PLUGINHOME"

# This snippet was mostly copied from /sbin/session_manager_setup.sh on an Alex
# It parses data from the .info file to put into the command line,
# to properly register the plugin, including making the version number and
# other info visible in about:plugins
FILE_NAME=
PLUGIN_NAME=
DESCRIPTION=
VERSION=
MIME_TYPES=
. $INFO_FILE
PLUGIN_STRING="${PLUGIN}${FILE_NAME}"
if [ -n "$PLUGIN_NAME" ]; then
    PLUGIN_STRING="${PLUGIN_STRING}#${PLUGIN_NAME}"
    PLUGIN_STRING="${PLUGIN_STRING}#"
    [ -n "$VERSION" ] && PLUGIN_STRING="${PLUGIN_STRING}#${VERSION}"
fi
PLUGIN_STRING="${PLUGIN_STRING};${MIME_TYPES}"
REGISTER_PLUGINS="${REGISTER_PLUGINS}${COMMA}${PLUGIN_STRING}"
COMMA=","
# end snippet

# Run chrome with chrome with all the right options, echoing the command line to
# the console for reference.
CHROME_PATH="./chrome-linux"
CHROME="$CHROME_PATH/chrome"
#CHROME="google-chrome"
USERAGENT="Mozilla/5.0 (X11; CrO armv7l 2876.0.0) AppleWebKit/537.10 (KHTML, like Gecko) Chrome/23.0.1262.2 Safari/537.10"
URL="http://localhost/nfwebcrypto/test_qa.html"
#URL="http://localhost/nfwebcrypto/test_qa.html?spec=SignVerifyRSA%20SignVerifyLargeData.#"
#URL="http://localhost/htmlplayer/unittests.html"

OPT=(
--user-agent="$USERAGENT"
--register-pepper-plugins=$REGISTER_PLUGINS
--profile-directory="nfwc"
--ppapi-out-of-process
--enable-dcheck
--enable-media-source
--enable-encrypted-media
--enable-accelerated-plugins
--enable-logging
)

# Options for in-process debug
#OPT=(--user-agent="$USERAGENT" --register-pepper-plugins=$REGISTER_PLUGINS --log-level=0
#--v=1 --enable-accelerated-plugins --allow-sandbox-debugging --disable-seccomp-sandbox
#--allow-running-insecure-content)

# Finally, echo and then run the command to launch the chrome. The env var
# PLAYREADY_CERT_PATH is specifically for the GTV chrome build
echo $CHROME "${OPT[@]}" "$URL"
$CHROME "${OPT[@]}" "$URL"
#$CHROME "${OPT[@]}" "$URL" 2>&1 | /home/padolph/tools/valgrind/asan/asan_symbolize.py | c++filt
#PLAYREADY_CERT_PATH=$CHROME_PATH/samples $CHROME "${OPT[@]}" "$URL"

# Misc notes here
#--ppapi-plugin-launcher='xterm -title plugin -e gdb --eval-command=run --args'
