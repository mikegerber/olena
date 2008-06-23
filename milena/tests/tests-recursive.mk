# tests-recursive.mk					-*- Automake -*-
# Compile tests recursively, but don't them.

# Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
# 2003, 2004, 2005, 2006, 2008  Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


# Sometimes, we just want to check whether tests compile, without
# actually running them.  Hence this `tests' target, which does half
# the job of the `check' target.

.PHONY: tests-am tests-recursive tests
# ``Halved'' version of check-am.
tests-am: $(check_PROGRAMS)
	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
# FIXME: Copied from a generated Makefile.in (rule `$(RECURSIVE_TARGETS)').
# (Automake does not allow us to interact with the code it generates in
# Makefile.in.)  We might want to propose this as a patch to the Automake
# project.
tests-recursive:
	@failcom='exit 1'; \
	for f in x $$MAKEFLAGS; do \
	  case $$f in \
	    *=* | --[!k]*);; \
	    *k*) failcom='fail=yes';; \
	  esac; \
	done; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"
tests: tests-recursive
