#!/bin/bash

gen_deps() {
  local root="$1"
  local prefix="$2"
  local outputfile="$3"
  local command="$4"
  for file in $prefix*; do
    if [ -f $file ]; then
      local list=$(gcc -MM $file -I $root | tr ': \\' '\n')
      local target='$(OBJ_DIR)'$(echo $list | tr ' ' '\n' | awk '/\.o/{print $1}')
      local source='$(SRC_DIR)'$(echo $list | tr ' ' '\n' | awk '/\.c/{sub(/src\//, ""); print $1}')
      local headers=
      for header in $(echo $list | tr ': \\' '\n' | awk '/\.h/{sub(/include\//, ""); print $1}'); do
        headers="$headers "'$(INC_DIR)'$header
      done
      echo $target ':' $source $headers '|' create_dir >> $outputfile
      echo -e "\t$command\n" >> $outputfile
    else
      gen_deps $root $file/ $outputfile "$command"
    fi
  done
}

gen_deps_pic() {
  local root="$1"
  local prefix="$2"
  local outputfile="$3"
  local command="$4"
  for file in $prefix*; do
    if [ -f $file ]; then
      local list=$(gcc -MM $file -I $root | tr ': \\' '\n')
      local target='$(OBJ_DIR)'$(echo $list | tr ' ' '\n' | awk '/\.o/{sub(/\.o/, ".pic.o"); print $1}')
      local source='$(SRC_DIR)'$(echo $list | tr ' ' '\n' | awk '/.*\.c/{sub(/src\//, ""); print $1}')
      local headers=
      for header in $(echo $list | tr ': \\' '\n' | awk '/\.h/{sub(/include\//, ""); print $1}'); do
        headers="$headers "'$(INC_DIR)'$header
      done
      echo $target : $source $headers '|' create_dir >> $outputfile
      echo -e "\t$command\n" >> $outputfile
    else
      gen_deps_pic $root $file/ $outputfile "$command"
    fi
  done
}

cat Makefile.pre > Makefile

gen_deps . ./src/ Makefile '$(CC) $(CFLAGS) -c -o $@ $<'

echo "# ==========================SHARED OBJECT=============================" >> Makefile
gen_deps_pic . ./src/ Makefile '$(CC) -fPIC $(CFLAGS) -c -o $@ $<'

cat Makefile.post >> Makefile
