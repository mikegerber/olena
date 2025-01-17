## tests.mk					-*- Makefile-Automake -*-

## Shared configuration for tests.

## Copyright (C) 2009, 2010, 2013 EPITA Research and Development
## Laboratory (LRDE).
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/scribo/common.mk

# Find test/data.hh (generated header).
AM_CPPFLAGS += -I$(top_builddir)/scribo

AM_CXXFLAGS = $(SCRIBO_TESTS_CXXFLAGS)

# Inject rules to compile tests (recursively), without running them.
include $(top_srcdir)/build-aux/tests-recursive.mk
