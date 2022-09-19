HEADER=bin/all_src.c
for file in *.c
do
    echo "  #include \"../lib/$file\"" >> $HEADER
done
