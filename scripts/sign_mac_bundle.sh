#!/bin/sh

# Use the script to sign your app bundle on Mac OS X.
# Requirement Mac OS El Capitan or later.
# An app bundle must be in the same folder.

# Name of your certificate in look "Developer ID Application: <yourID>"
# Be sure that you use Developer ID certificate!
CERTIFICATE="Developer ID Application: <yourID>"
BUNDLE=Valentina.app

# all must be signed
#s ign all *.dylib
find $BUNDLE -name *.dylib | xargs -I $ codesign -f -v -v -vvvv --deep --strict -s "$CERTIFICATE" $

# sign Qt frameworks
find $BUNDLE -name Qt* -type f | xargs -I $ codesign -f -v -v -vvvv --deep --strict -s "$CERTIFICATE" $

# sign all binaries
find $BUNDLE -path */MacOS/* -type f | xargs -I $ codesign -f -v -v -vvvv --deep --strict -s "$CERTIFICATE" $

# sign the app bundle
codesign -f -v -v -vvvv --deep --strict -s "$CERTIFICATE" ./$BUNDLE

#verify in the end
codesign -v -vvvv ./$BUNDLE
spctl -a -t exec -vv ./$BUNDLE
