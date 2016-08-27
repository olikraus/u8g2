
mkdir u8g2
cp ../../../csrc/*.c ./u8g2/.
cp ../../../csrc/*.h ./u8g2/.


ver=`../print_release.sh`

rm u8g2_plain_c_${ver}.zip
zip -q -r --exclude="*.git*" u8g2_plain_c_${ver}.zip ./u8g2

rm -rf u8g2