f="\
 path/action-redo-1x.png \
 path/action-undo-1x.png \
 path/aperture-1x.png \
 path/ban-1x.png \
 path/bar-chart-1x.png \
 path/basket-1x.png \
 path/beaker-1x.png \
 path/bell-1x.png \
 path/bluetooth-1x.png \
 path/bolt-1x.png \
 path/book-1x.png \
 path/box-1x.png \
 path/briefcase-1x.png \
 path/browser-1x.png \
 path/brush-1x.png \
 path/bug-1x.png \
 path/bullhorn-1x.png \
 path/calculator-1x.png \
 path/calendar-1x.png \
 path/camera-slr-1x.png \
 path/cart-1x.png \
 path/chat-1x.png \
 path/check-1x.png \
 path/circle-check-1x.png \
 path/circle-x-1x.png \
 path/clipboard-1x.png \
 path/clock-1x.png \
 path/cloud-1x.png \
 path/cloud-download-1x.png \
 path/cloud-upload-1x.png \
 path/cloudy-1x.png \
 path/cog-1x.png \
 path/comment-square-1x.png \
 path/compass-1x.png \
 path/contrast-1x.png \
 path/credit-card-1x.png \
 path/dashboard-1x.png \
 path/dial-1x.png \
 path/document-1x.png \
 path/droplet-1x.png \
 path/envelope-closed-1x.png \
 path/envelope-open-1x.png \
 path/eye-1x.png \
 path/eyedropper-1x.png \
 path/file-1x.png \
 path/fire-1x.png \
 path/flash-1x.png \
 path/folder-1x.png \
 path/fork-1x.png \
 path/globe-1x.png \
 path/graph-1x.png \
 path/hard-drive-1x.png \
 path/headphones-1x.png \
 path/heart-1x.png \
 path/home-1x.png \
 path/image-1x.png \
 path/inbox-1x.png \
 path/key-1x.png \
 path/laptop-1x.png \
 path/lightbulb-1x.png \
 path/list \
 path/list-1x.png \
 path/lock-locked-1x.png \
 path/lock-unlocked-1x.png \
 path/loop-1x.png \
 path/loop-circular-1x.png \
 path/loop-square-1x.png \
 path/magnifying-glass-1x.png \
 path/map-1x.png \
 path/map-marker-1x.png \
 path/microphone-1x.png \
 path/monitor-1x.png \
 path/moon-1x.png \
 path/musical-note-1x.png \
 path/paperclip-1x.png \
 path/pencil-1x.png \
 path/people-1x.png \
 path/person-1x.png \
 path/phone-1x.png \
 path/pie-chart-1x.png \
 path/pin-1x.png \
 path/power-standby-1x.png \
 path/print-1x.png \
 path/pulse-1x.png \
 path/puzzle-piece-1x.png \
 path/rain-1x.png \
 path/reload-1x.png \
 path/rss-1x.png \
 path/rss-alt-1x.png \
 path/script-1x.png \
 path/share-1x.png \
 path/share-boxed-1x.png \
 path/shield-1x.png \
 path/signpost-1x.png \
 path/spreadsheet-1x.png \
 path/star-1x.png \
 path/sun-1x.png \
 path/tablet-1x.png \
 path/tag-1x.png \
 path/tags-1x.png \
 path/target-1x.png \
 path/task-1x.png \
 path/thumb-down-1x.png \
 path/thumb-up-1x.png \
 path/timer-1x.png \
 path/trash-1x.png \
 path/video-1x.png \
 path/volume-high-1x.png \
 path/volume-low-1x.png \
 path/volume-off-1x.png \
 path/warning-1x.png \
 path/wifi-1x.png \
 path/wrench-1x.png \
 path/x-1x.png \
 path/zoom-in-1x.png \
 path/zoom-out-1x.png"
 
f=${f//path/\/home\/kraus\/git\/open-iconic\/png}
ff=${f//-1x/}
echo $ff
../png2bdf -f open_iconic_1x -o open_iconic_1x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '65-255' -v open_iconic_1x.bdf -n open_iconic_1x -o open_iconic_1x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_1x.jpg

ff=${f//-1x/-2x}
echo $ff
../png2bdf -f open_iconic_2x -o open_iconic_2x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '65-255' -v open_iconic_2x.bdf -n open_iconic_2x -o open_iconic_2x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_2x.jpg
 
ff=${f//-1x/-4x}
echo $ff
../png2bdf -f open_iconic_4x -o open_iconic_4x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '65-255' -v open_iconic_4x.bdf -n open_iconic_4x -o open_iconic_4x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_4x.jpg

ff=${f//-1x/-6x}
echo $ff
../png2bdf -f open_iconic_6x -o open_iconic_6x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '65-255' -v open_iconic_6x.bdf -n open_iconic_6x -o open_iconic_6x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_6x.jpg

ff=${f//-1x/-8x}
echo $ff
../png2bdf -f open_iconic_8x -o open_iconic_8x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '65-255' -v open_iconic_8x.bdf -n open_iconic_8x -o open_iconic_8x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_8x.jpg
