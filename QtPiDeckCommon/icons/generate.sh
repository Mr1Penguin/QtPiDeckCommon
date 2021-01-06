#!/bin/bash

themename="maintheme" # switch to loop if more themes needed
svgdir="svg"
icons=`cd $themename/$svgdir && ls *.svg`
tab="    "
iconsres="../icons.qrc"


echo "<RCC>" > $iconsres
echo "${tab}<qresource prefix=\"/\">" >> $iconsres

themefile=${themename}/index.theme
echo "${tab}${tab}<file>icons/${themename}/index.theme</file>" >> $iconsres

echo "[Icon Theme]" > $themefile
echo "Name=${themename}" >> $themefile
echo "Comment=${themename}" >> $themefile

directories="Directories="

# 24 is a default value for RoundButton(everywhere?)
resolutions=(16 24 32 64)

for resolution in ${resolutions[@]}
do
    directories="${directories}${resolution}x${resolution},"
done
echo "${directories%?}" >> $themefile
for resolution in ${resolutions[@]}
do
    echo "" >> $themefile
    resdir="${resolution}x${resolution}"
    echo "[${resdir}]" >> $themefile
    echo "Size=${resolution}" >> $themefile
    echo "Type=Fixed" >> $themefile
    dirname="${themename}/${resdir}"
    echo $dirname
    rm -rf $dirname
    mkdir -p $dirname
    for icon in $icons
    do
        pngpath="$dirname/${icon/svg/png}"
        inkscape -w $resolution -h $resolution $themename/$svgdir/$icon --export-filename $pngpath 2>/dev/null
        echo "${tab}${tab}<file>icons/${pngpath}</file>" >> $iconsres
    done
done

echo "${tab}</qresource>" >> $iconsres
echo "</RCC>" >> $iconsres

