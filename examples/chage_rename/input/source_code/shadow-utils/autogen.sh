#! /bin/sh

autoreconf -v -f --install || exit 1

./configure \
	CFLAGS="-O0 -Wall -g -m32 " \
	--enable-man \
	--enable-maintainer-mode \
	--disable-shared \
	--without-libpam \
	--with-selinux \
	"$@"
