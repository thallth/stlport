# Time-stamp: <04/03/03 15:49:24 ptr>
# $Id$

SO := sl

ARCH := a
AR := ar
AR_INS_R := -rs
AR_EXTR := -x
AR_OUT = $@

INSTALL := $$HOME/bin/install-sh

INSTALL_SO := ${INSTALL} -c -m 0755
INSTALL_A := ${INSTALL} -c -m 0644
INSTALL_EXE := ${INSTALL} -c -m 0755
