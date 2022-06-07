#!/bin/csh

set files = ` awk '{print $1}' < ${1}.list `
set i = 1
set m = 1
while ( $i <= $#files )
  set k = ` expr $i - 1 `
  echo $k

  echo $files[$i]
  if ( $i < 2 ) then
    hadd $1.$i.root $files[$i]
  else
    hadd -f $1.$i.root $1.$k.root $files[$i]
    rm -f $1.$k.root
  endif
  @ i++
end
