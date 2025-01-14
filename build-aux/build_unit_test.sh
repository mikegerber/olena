#! /bin/sh

# Copyright (C) 2009, 2010, 2013 EPITA Research and Development Laboratory
# (LRDE)
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

## Build a list of unit tests.
##
## Conditional tests (external dependencies) can be handled
## automaticaly.  For each dependency, a file named
## "cond_tests_{DEPS}" must contain the list of files which need the
## dependency.  If a test needs more than one dependency, several
## dependency names can be used in the file name.
##
## Ex: cond_tests_qt_tesseract
##
## All the tests listed in this file will need both qt and tesseract
## to be compiled.
##
## "cond_tests_*" files must be created in the directory where
## unit-test.mk is generated (usually the unit-tests directory).
##
## NOTES
## -----
## * Don't forget to add "cond_tests_*" files in the COND_TESTS
## variable in the right Makefile.am.
##
## * ALWAYS end "cond_tests_*" files with a new line!
##
## * If the dependency has a composed name use '-' to concatenate the words.
##   (e.g., boost_tuple -> cond_tests_boost-tuple)
##

## FIXME: Don't use `echo -n', as echo's options are not portable.
##
## http://www.gnu.org/software/autoconf/manual/html_node/Limitations-of-Builtins.html#Limitations-of-Builtins
##
## As an ugly workaround, use `printf' instead.  The right approach
## would be to only emit lines ended with newlines.
ECHO_N=printf


add_targets()
{
    HEADERS_="$1"
    output_dir="$2"
    output="$3"
    IFDEF="$4"

    if ! (test -z "$IFDEF"); then
	echo "# Starting a conditional unit test list." >> "$output"
    else
	echo "# Starting non-conditional unit test list." >> "$output"
    fi

    # Start conditions if necessary

    # Construct a list of conditions in reverse order to write valid
    # endif further.
    IFDEF_TMP=""
    for i in $IFDEF; do
	echo "if HAVE_$i" >>"$output"
	IFDEF_TMP="$i $IFDEF_TMP"
    done
    IFDEF="$IFDEF_TMP"

    $ECHO_N "check_PROGRAMS += " >>"$output"

    for i in $HEADERS_; do
	FILE_CC=`echo $i | sed 's/[/.]/_/g' | sed 's/_hh/\.cc/g'`

    # Build .cc.
	cat > $output_dir/$FILE_CC << EOF
// Unit test for $i.
// Generated by $0, do not modify.

// Include the file twice, so we detect missing inclusion guards.
#include <$i>
#include <$i>

int main()
{
  // Nothing.
}
EOF

    # Build unit-tests.mk.
	TARGET=`echo "${FILE_CC}" | sed 's/\.cc//'`
	echo " \\" >>"$output"
	$ECHO_N "${TARGET}" >>"$output"
    done

    # Build "$output".
    echo "" >>"$output"
    echo "" >>"$output"
    for i in $HEADERS_; do
	FILE_CC=`echo $i | sed 's/[/.]/_/g' | sed 's/_hh/\.cc/g'`
	NAME=`echo $FILE_CC | sed 's/\.cc//g'`

	# Write specific flags if there are dependencies.
	if ! (test -z "$IFDEF"); then
	    cppflags="${NAME}_CPPFLAGS ="
	    ldflags="${NAME}_LDFLAGS ="
	    for i in $IFDEF; do
		cppflags="${cppflags} \$(${i}_CPPFLAGS) -DHAVE_${i}"
		ldflags="${ldflags} \$(${i}_LDFLAGS)"
	    done
	    echo "$cppflags \$(AM_CPPFLAGS)" >>"$output"
	    echo "$ldflags \$(AM_LDFLAGS)" >>"$output"

	    # Local hack: Handle the special case of Qt, which defines
	    # QT_LIBS, to be use for with ${NAME}_LDADD variables.
	    for i in $IFDEF; do
		if test "x$i" = xQT; then
		    echo "${NAME}_LDADD = \$(QT_LIBS) \$(LDADD)" >>"$output"
		fi
	    done
	fi
    done

    # End conditions if necessary
    for i in $IFDEF; do
	echo "endif HAVE_$i" >>"$output"
    done

    echo "" >>"$output"
}



# Use the C locale to have a deterministic sort.
export LC_ALL=C

test $# -eq 4 || { echo "Usage: $0 <scanned dir> <output_dir> <disabled_tests> <base include dir>" && exit 1; }

scanned_dir=$1
output_dir=$2
disabled_tests=$3
base_inc_dir=$4

# Create a list of tests to be disabled or considered as conditional.
ignored_list="$output_dir/ignored_list.tmp"
cat "$disabled_tests" > $ignored_list
COND_TESTS=$(find $output_dir -type f -name "cond_tests_*" | sort)
for f in $COND_TESTS; do
    cat "$f" >> "$ignored_list"
done
cat "$ignored_list" | sort > "$ignored_list.tmp"
mv -f "$ignored_list.tmp" "$ignored_list"

HEADERS=$(find $scanned_dir -type f -name "*.hh" \
    | sort \
    | sed -e "s/.*\/${base_inc_dir}\/\(.*\)/${base_inc_dir}\/\1/g" \
    | sed 's/\.\.\/\.\.\///g' \
    | comm -23 - "$ignored_list")

output="$output_dir/unit-tests.mk"

# Cleanup previously generated data.
rm -f "$output"
rm -f ${base_inc_dir}_*.cc

# Build unit-tests.mk.
echo "## Generated by $0, do not modify." > "$output"
echo >>"$output"

echo "check_PROGRAMS =" >>"$output"
echo "" >>"$output"


#-----------------------------------
# Insert lists of conditional tests.
#-----------------------------------

for f in $COND_TESTS; do
    # Generate the list of conditions.
    fpostfix="`echo \"$f\" | sed -e 's/.*\/cond_tests_//g'`_"
    i=1
    COND_LIST=""
    while true; do
	name=`echo $fpostfix | cut -s -d '_' -f $i \
	    | tr "[:lower:]" "[:upper:]" \
	    | sed -e 's/-/_/g'`
	i=$(($i+1))
	if (test -z "$name"); then
	    break;
	fi
	COND_LIST="${COND_LIST} $name"
    done

    COND_HEADERS=$(cat $f)

    # Writing conditional targets
    add_targets "$COND_HEADERS" "$output_dir" "$output" "$COND_LIST"
done


#-------------------------------------
# Insert list of non-conditional tests
#-------------------------------------

add_targets "$HEADERS" "$output_dir" "$output" ""


# Remove temporary files
rm -f "$ignored_list"
