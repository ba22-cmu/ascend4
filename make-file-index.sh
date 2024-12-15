#! /bin/bash
# make a tree of html files indexing a parallel tree of files and directories
# Don't want to pollute the source tree
# The source tree and parallel tree can be recursively copied to a web server.
indir=models
outdir=models.index
if test "x$indir" = "x$outdir"; then
	echo cowardly refusing to pollute the source dir $indir
	exit 1
fi
if ! test -d $indir; then
	echo no directory $indir present in pwd $(pwd)
	exit 1
fi
if test -d $outdir; then
	od=$(mktemp -d $outdir.XXXXXXX)
	echo directory $outdir will be replaced. old content is in $od
	repldir=$od
	mv $outdir $od
	mkdir -p $outdir
fi

make_dir_header () {
	echo "<html><pre>"
}

make_dir_footer () {
	echo "</pre></html>"
}

for i in $(find $indir -type d ); do
	od=$(echo $i |sed -e "s/^$indir/$outdir/")
	orel=$(echo $i |sed -e "s/^$indir/./")
	irel=$(echo $i |sed -e "s%[^/]*%..%g")
	# echo $i $od
	mkdir -p $od
	of=$od/index.html
	# for each line in ls  make link or ignore
	make_dir_header > $of
	for f in $(ls $i --file-type -X -1 ); do
		case $f in
		Make*|SCon*|PACK*) : ignore build
			;;
		*@) : ignore symlinks
			;;
		*.in) : ignore .in files
			;;
		*/)
			echo "<a href=\"${f}index.html\">$f</a>"
			;;
		*)
			echo "<a href=\"$irel/$i/${f}\">$f</a>"
			;;
		esac
	done >> $of
	make_dir_footer >> $of
done
if test -n $repldir; then
	echo old content is in $repldir
fi
echo new content is in $outdir
