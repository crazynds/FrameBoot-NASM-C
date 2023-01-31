HEADER=bin/all_src.cpp

rm $HEADER

for file in $(find ./lib -type f -print)
do
    echo "#include \"../$file\"" >> $HEADER
done
