
echo "== copy files to work dir =="

# copy C++ files and update the C include path
cp ../../../cppsrc/*.cpp ./../../../../U8g2_Arduino/src/.
cp ../../../cppsrc/*.h ./../../../../U8g2_Arduino/src/.
sed -i 's|u8g2.h|clib/u8g2.h|g' ./../../../../U8g2_Arduino/src/U8g2lib.h
sed -i 's|u8x8.h|clib/u8x8.h|g' ./../../../../U8g2_Arduino/src/U8x8lib.h
# copy C files, exclude u8x8_d_stdio.c
cp ../../../csrc/*.c ./../../../../U8g2_Arduino/src/clib/.
cp ../../../csrc/*.h ./../../../../U8g2_Arduino/src/clib/.
rm ./../../../../U8g2_Arduino/src/clib/u8x8_d_stdio.c

# copy examples

# page buffer

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Devanagari
cp ../../../sys/arduino/u8g2_page_buffer/Devanagari/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Devanagari/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/UpdatePartly
cp ../../../sys/arduino/u8g2_page_buffer/UpdatePartly/*.ino ../../../../U8g2_Arduino/examples/page_buffer/UpdatePartly/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/ClipWindow
cp ../../../sys/arduino/u8g2_page_buffer/ClipWindow/*.ino ../../../../U8g2_Arduino/examples/page_buffer/ClipWindow/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/StateBufferLoop
cp ../../../sys/arduino/u8g2_page_buffer/StateBufferLoop/*.ino ../../../../U8g2_Arduino/examples/page_buffer/StateBufferLoop/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Serial
cp ../../../sys/arduino/u8g2_page_buffer/Serial/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Serial/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/DrawLog
cp ../../../sys/arduino/u8g2_page_buffer/DrawLog/*.ino ../../../../U8g2_Arduino/examples/page_buffer/DrawLog/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Terminal
cp ../../../sys/arduino/u8g2_page_buffer/Terminal/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Terminal/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/IconMenu
cp ../../../sys/arduino/u8g2_page_buffer/IconMenu/*.ino ../../../../U8g2_Arduino/examples/page_buffer/IconMenu/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Weather
cp ../../../sys/arduino/u8g2_page_buffer/Weather/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Weather/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Chinese
cp ../../../sys/arduino/u8g2_page_buffer/Chinese/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Chinese/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Shennong
cp ../../../sys/arduino/u8g2_page_buffer/Shennong/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Shennong/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Shennong
cp ../../../sys/arduino/u8g2_full_buffer/Shennong/*.ino ../../../../U8g2_Arduino/examples/full_buffer/Shennong/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Japanese
cp ../../../sys/arduino/u8g2_page_buffer/Japanese/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Japanese/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Korean
cp ../../../sys/arduino/u8g2_page_buffer/Korean/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Korean/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/ScrollingText
cp ../../../sys/arduino/u8g2_page_buffer/ScrollingText/*.ino ../../../../U8g2_Arduino/examples/page_buffer/ScrollingText/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/ContrastTest
cp ../../../sys/arduino/u8g2_page_buffer/ContrastTest/*.ino ../../../../U8g2_Arduino/examples/page_buffer/ContrastTest/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/DirectAccess
cp ../../../sys/arduino/u8g2_page_buffer/DirectAccess/*.ino ../../../../U8g2_Arduino/examples/page_buffer/DirectAccess/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/A2Printer
cp ../../../sys/arduino/u8g2_page_buffer/A2Printer/*.ino ../../../../U8g2_Arduino/examples/page_buffer/A2Printer/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/ExtUTF8
cp ../../../sys/arduino/u8g2_page_buffer/ExtUTF8/*.ino ../../../../U8g2_Arduino/examples/page_buffer/ExtUTF8/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/FPS
cp ../../../sys/arduino/u8g2_page_buffer/FPS/*.ino ../../../../U8g2_Arduino/examples/page_buffer/FPS/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/HelloWorld
cp ../../../sys/arduino/u8g2_page_buffer/HelloWorld/*.ino ../../../../U8g2_Arduino/examples/page_buffer/HelloWorld/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/PrintHelloWorld
cp ../../../sys/arduino/u8g2_page_buffer/PrintHelloWorld/*.ino ../../../../U8g2_Arduino/examples/page_buffer/PrintHelloWorld/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/PrintProgmem
cp ../../../sys/arduino/u8g2_page_buffer/PrintProgmem/*.ino ../../../../U8g2_Arduino/examples/page_buffer/PrintProgmem/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/PrintUTF8
cp ../../../sys/arduino/u8g2_page_buffer/PrintUTF8/*.ino ../../../../U8g2_Arduino/examples/page_buffer/PrintUTF8/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/U8g2Logo
cp ../../../sys/arduino/u8g2_page_buffer/U8g2Logo/*.ino ../../../../U8g2_Arduino/examples/page_buffer/U8g2Logo/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/FlipMode
cp ../../../sys/arduino/u8g2_page_buffer/FlipMode/*.ino ../../../../U8g2_Arduino/examples/page_buffer/FlipMode/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/SelectionList
cp ../../../sys/arduino/u8g2_page_buffer/SelectionList/*.ino ../../../../U8g2_Arduino/examples/page_buffer/SelectionList/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/GraphicsTest
cp ../../../sys/arduino/u8g2_page_buffer/GraphicsTest/*.ino ../../../../U8g2_Arduino/examples/page_buffer/GraphicsTest/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/Clock
cp ../../../sys/arduino/u8g2_page_buffer/Clock/*.ino ../../../../U8g2_Arduino/examples/page_buffer/Clock/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/XORTest
cp ../../../sys/arduino/u8g2_page_buffer/XORTest/*.ino ../../../../U8g2_Arduino/examples/page_buffer/XORTest/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/XBM
cp ../../../sys/arduino/u8g2_page_buffer/XBM/*.ino ../../../../U8g2_Arduino/examples/page_buffer/XBM/.

mkdir ../../../../U8g2_Arduino/examples/page_buffer/PowerSaveTest
cp ../../../sys/arduino/u8g2_page_buffer/PowerSaveTest/*.ino ../../../../U8g2_Arduino/examples/page_buffer/PowerSaveTest/.



# games
mkdir ../../../../U8g2_Arduino/examples/games/LittleRookChess
cp ../../../sys/arduino/u8g2_page_buffer/LittleRookChess/*.ino ../../../../U8g2_Arduino/examples/games/LittleRookChess/.

mkdir ../../../../U8g2_Arduino/examples/games/SpaceTrash
cp ../../../sys/arduino/u8g2_page_buffer/SpaceTrash/*.ino ../../../../U8g2_Arduino/examples/games/SpaceTrash/.


# full buffer

mkdir ../../../../U8g2_Arduino/examples/full_buffer/UpdateArea
cp ../../../sys/arduino/u8g2_full_buffer/UpdateArea/*.ino ../../../../U8g2_Arduino/examples/full_buffer/UpdateArea/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/IconMenu
cp ../../../sys/arduino/u8g2_full_buffer/IconMenu/*.ino ../../../../U8g2_Arduino/examples/full_buffer/IconMenu/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/Weather
cp ../../../sys/arduino/u8g2_full_buffer/Weather/*.ino ../../../../U8g2_Arduino/examples/full_buffer/Weather/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/HelloWorld
cp ../../../sys/arduino/u8g2_full_buffer/HelloWorld/*.ino ../../../../U8g2_Arduino/examples/full_buffer/HelloWorld/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/FontUsage
cp ../../../sys/arduino/u8g2_full_buffer/FontUsage/*.ino ../../../../U8g2_Arduino/examples/full_buffer/FontUsage/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/U8g2Logo
cp ../../../sys/arduino/u8g2_full_buffer/U8g2Logo/*.ino ../../../../U8g2_Arduino/examples/full_buffer/U8g2Logo/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/PrintUTF8
cp ../../../sys/arduino/u8g2_full_buffer/PrintUTF8/*.ino ../../../../U8g2_Arduino/examples/full_buffer/PrintUTF8/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/SelectionList
cp ../../../sys/arduino/u8g2_full_buffer/SelectionList/*.ino ../../../../U8g2_Arduino/examples/full_buffer/SelectionList/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/GraphicsTest
cp ../../../sys/arduino/u8g2_full_buffer/GraphicsTest/*.ino ../../../../U8g2_Arduino/examples/full_buffer/GraphicsTest/.

mkdir ../../../../U8g2_Arduino/examples/full_buffer/FPS
cp ../../../sys/arduino/u8g2_full_buffer/FPS/*.ino ../../../../U8g2_Arduino/examples/full_buffer/FPS/.

# u8x8

mkdir ../../../../U8g2_Arduino/examples/u8x8/16x16Font
cp ../../../sys/arduino/u8x8/16x16Font/*.ino ../../../../U8g2_Arduino/examples/u8x8/16x16Font/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/Terminal
cp ../../../sys/arduino/u8x8/Terminal/*.ino ../../../../U8g2_Arduino/examples/u8x8/Terminal/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/HelloWorld
cp ../../../sys/arduino/u8x8/HelloWorld/*.ino ../../../../U8g2_Arduino/examples/u8x8/HelloWorld/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/GraphicsTest
cp ../../../sys/arduino/u8x8/GraphicsTest/*.ino ../../../../U8g2_Arduino/examples/u8x8/GraphicsTest/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/FlipMode
cp ../../../sys/arduino/u8x8/FlipMode/*.ino ../../../../U8g2_Arduino/examples/u8x8/FlipMode/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/MessageBox
cp ../../../sys/arduino/u8x8/MessageBox/*.ino ../../../../U8g2_Arduino/examples/u8x8/MessageBox/.

mkdir ../../../../U8g2_Arduino/examples/u8x8/ArduboyTest
cp ../../../sys/arduino/u8x8/ArduboyTest/*.ino ../../../../U8g2_Arduino/examples/u8x8/ArduboyTest/.

# copy other files
cp ../../../ChangeLog ./../../../../U8g2_Arduino/extras/.

cp ../../font/build/keywords.txt ./../../../../U8g2_Arduino/.

pushd .
# goto the release project
cd ../../../../U8g2_Arduino

echo "== adding files to repo =="

# ensure that all example files are added to the repo
find . -name "*.ino" -exec git add {} \;

# ensure that all C/C++ files are added
cd src
git add *
cd clib
git add *.[hc]
cd ..
cd ..


ver=`../u8g2/tools/release/print_release.sh`

sed -i -e "s/version=.*/version=${ver}/" library.properties

sed -i -e "s/Download (.*)/Download (${ver})/" README.md

# git commit -a -m `../u8g2/tools/release/print_release.sh`
# git push

cd ..

echo "== create local zip file =="
pwd
rm u8g2_arduino_${ver}.zip
zip -q -r --exclude="*.git*" u8g2_arduino_${ver}.zip ./U8g2_Arduino
cp u8g2_arduino_${ver}.zip ~/Arduino/libraries/.
cd ~/Arduino/libraries
rm -rf U8g2_Arduino
unzip -o u8g2_arduino_${ver}.zip

popd

echo now create a release in gitub for U8glib_Arduino, tag/name = ${ver}
echo no prefix required, release name can be empty
# Relases in github:
# Tag: 1.02pre3
# Release  name: 1.02pre3


