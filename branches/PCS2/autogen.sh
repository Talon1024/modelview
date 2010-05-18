#!/bin/sh

export WANT_AUTOCONF_2_5="1"
export WANT_AUTOMAKE_1_9="1"

aclocal
#libtoolize --automake --force
autoconf
#autoheader
automake -a
