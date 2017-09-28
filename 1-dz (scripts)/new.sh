cat /dev/urandom | tr -dc "a-z" | fold -w 4 | head -n 100 | \
while read x
do
   let rand=$RANDOM%3
   echo $rand | tr 012 abc | xargs echo -n
   echo "$x"
done | xargs touch

exit 0
