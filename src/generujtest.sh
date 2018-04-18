#!/bin/bash
hourminute=$(date --date="+1 minute" +%H:%M:) 
second=$(date +%S)
colon=:

set -e

make -s
rm -f crontaskfile.test

declare -a arr=("date +%y/%m"
		"date +%y/%m/%d | echo pipe!"
		"uptime --pretty | uname -r | echo more pipes")

if [ "$second" -gt "57" ]; then
	printf 'Sleeping 4 seconds...\\n'
	sleep 4
fi

for i in "${arr[@]}"
do
	myrand=$(($RANDOM % 3))
	echo "$hourminute$i$colon$myrand$newline" >> crontaskfile.test
done


hourminute=$(date +%H:%M:) 


for i in "${arr[@]}"
do
	myrand=$(($RANDOM % 3))
	echo "$hourminute$i$colon$myrand$newline" >> crontaskfile.test
done

./minicron crontaskfile.test outfile.txt
printf "\n"
cat outfile.txt
printf "\n"
echo ---------
tail -n 5 /var/log/syslog
exit
