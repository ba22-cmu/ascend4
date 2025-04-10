#! /bin/bash
# script to extract macros for command registration from tcltk/interface/$files
#  Benjamin Allan, 3/2025
files=$(grep HN ../interface/*.h -l)
for f in $files; do
	j=$(echo $f | sed -e 's/...interface.//' -e 's/.h$/.json.h/')
	h=$(echo $j | sed -e 's/\./_/g')
	echo creating $j from $f 
	echo "#ifndef $h" > $j
	echo "#define $h" >> $j
	echo >> $j
	grep -E '(#define|\\$)' -A 1 $f | \
		sed -e 's/^--$//g' -e 's/[A-Za-z0-9]*.h[:-]//' | \
		sed '/define Asc_[a-zA-Z]*CmdHN "[a-zA-Z_]*"$/{p;s/HN/HC/;s/"/ /g;}' | \
		grep -v 'H$' | \
		sed -e '/^$/N;/^\n$/D;' >> $j
	echo >> $j
	echo "#endif // $h" >> $j
done
