#!/usr/bin/env zsh

replace() {
  for f in build/**/**.$1
  do
    # SPIFFS likes short filenames. MD5 it and shorten it to 7 chars!
    new=${"$(echo "$f" | md5)"[0,4]}
    mv $f "build/$new.$1"

    relOld=$f[7,${#f}]
    relNew="$new.$1"

    find build -maxdepth 1 -type f | xargs sed -i "s@$relOld@$relNew@g"
  done
}

replace "js"
replace "css"

rm -rf build/static

rm -rf ../esp32/sketch/data/*

cp build/* ../esp32/sketch/data
