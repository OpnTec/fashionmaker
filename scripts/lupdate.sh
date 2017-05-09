#!/bin/bash
# Run this script if you want to find and update all strings in the code.
# lupdate doesn't work with recursive *.pro file and without direct pointing to correct *.pro file just update exists strings in code.
# Please, run this script from folder <root_folder>/scripts.

start=$(date +%s)

# Download all translations from transifex.com.
cd ../share/translations
RESOURCES=`find . -regextype sed -regex ".*/measurements_p[0-9]\{1,2\}\.ts"`
cd ../../scripts

NUMBER=( $RESOURCES )
NUMBER=${#NUMBER[@]}

for ((i=0;i<NUMBER;i++)); do
  tx pull -r valentina-project.measurements_p${i}ts --mode=developer -f --skip &
  sleep 5
done

tx pull -r valentina-project.valentina_ts --mode=developer -f --skip &
sleep 5
tx pull -r valentina-project.measurements_p998ts --mode=developer -f --skip &

wait

# Update local strings
lupdate -recursive ../share/translations/translations.pro
lupdate -recursive ../share/translations/measurements.pro
#clean stale QM files
rm -f -v ../share/translations/*.qm
# force to run qmake
MAKEFILES=`find ../../ -name Makefile`

for var in $MAKEFILES
do
    rm -f -v $var
done

end=$(date +%s)
runtime=$(python -c "print 'Time passed %u:%02u seconds' % ((${end} - ${start})/60, (${end} - ${start})%60)")
echo $runtime
echo For updating files run: build all.

