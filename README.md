# CSE306PRE

We've been compiling the code with:

gcc -o csv csv.c

and running it with:

./csv [-frh] [-max field] [-min field] [-mean field] [-records field value] file

for example:

./csv -f 05020004-eng.csv

or

./csv -h -min "Ref_Date" -max "Ref_Date" -mean "Value" 05020004-eng.csv
