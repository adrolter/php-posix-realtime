#!/bin/sh

[ ! -f "configure" ] && phpize

[ ! -f "Makefile" ] && ./configure

make