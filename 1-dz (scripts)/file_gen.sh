IFS="\n"
cat /dev/urandom | tr -dc "a-z" | fold -w 100 | head -c 1000  >> test.txt
