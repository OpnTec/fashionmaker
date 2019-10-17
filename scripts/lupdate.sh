#!/bin/bash
# Run this script if you want to find and update all strings in the code.
# lupdate doesn't work with recursive *.pro file and without direct pointing to correct *.pro file just update exists strings in code.
# Please, run this script from folder <root_folder>/scripts.

start=$(date +%s)

# Download all translations from transifex.com.
cd ../share/translations
RESOURCES=`find . -regextype sed -regex ".*/measurements_p[0-9]\{1,2\}\.ts"`
cd ../../scripts

# Empty means unstable branch
MEASUREMENTS_BRANCH='' # For example _05x
VALENTINA_BRANCH=''    # for example 05x

NUMBER=( $RESOURCES )
NUMBER=${#NUMBER[@]}

# Certant languages like he_IL and zh_CN are not supported by math parser
for ((i=0;i<NUMBER;i++)); do
  tx pull -r valentina-project.measurements_p${i}${MEASUREMENTS_BRANCH}ts --mode=developer -f --skip -l "ru_RU, uk, de_DE, cs, he_IL, fr_FR, it_IT, nl, id, es, fi, en_US, en_CA, en_IN, ro_RO, zh_CN, pt_BR, el_GR, pl_PL" &
  sleep 2
done

tx pull -r valentina-project.valentina_${VALENTINA_BRANCH}ts --mode=developer -f --skip &
sleep 2
tx pull -r valentina-project.measurements_p998${MEASUREMENTS_BRANCH}ts --mode=developer -f --skip -l "ru_RU, uk, de_DE, cs, he_IL, fr_FR, it_IT, nl, id, es, fi, en_US, en_CA, en_IN, ro_RO, zh_CN, pt_BR, el_GR, pl_PL" &

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

