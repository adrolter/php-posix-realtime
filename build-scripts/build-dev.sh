#!/usr/bin/env sh

[ ! -f "configure" ] && phpize

[ ! -f "Makefile" ] && ./configure

make
