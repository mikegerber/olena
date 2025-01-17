#! /bin/sh

# Copyright (C) 2009, 2012 EPITA Research and Development Laboratory (LRDE).
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


# Generate a list of distributed files w.r.t. a list of file which must
# be excluded.
# --------------------
# List all the headers in order to make them part of distribution.

# Use the C locale to have a deterministic sort.
export LC_ALL=C

if [ $# -ne 2 ] && [ $# -ne 4 ]; then
  echo "Usage: $0 <scanned_dir> <output> [<module name> <inc_path>]"
  exit 1
fi

me=`basename $0`
scanned_dir=$1
output=$2

echo "Generating $output..." >&2
rm -f "$output"

if [ $# -eq 4 ]; then
    cat <<EOF > "$output"
## Generated by \`$me', do not edit by hand.
$3dir = \$(includedir)/$4
nobase_$3_HEADERS = \\
EOF
else
    cat <<EOF > "$output"
## Generated by \`$me', do not edit by hand.

nobase_include_HEADERS = \\
EOF
fi


find $scanned_dir -type f -a \( -name '*.hh' -o -name '*.hxx' \) \
  | sort						\
  | sed -e 's/$/ \\/g' >> $output


last_line=`tail -n 1 $output | sed -e 's/\\\//g'` # remove '\' in last line
sed '$d' < $output > $output.tmp # remove last line
mv $output.tmp $output
echo $last_line >> $output # put the cleaned last line back.
