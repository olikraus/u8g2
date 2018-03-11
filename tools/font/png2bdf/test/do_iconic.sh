#!/bin/bash

# https://github.com/iconic/open-iconic
# SIL OPEN FONT LICENSE

function bdf {
f=${2//path/\/home\/kraus\/git\/open-iconic\/png}
ff=${f//-1x/}
../png2bdf -c "https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE"  -f open_iconic_${1}_1x -o open_iconic_${1}_1x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '64-511' -v open_iconic_${1}_1x.bdf -n open_iconic_${1}_1x -o open_iconic_${1}_1x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_${1}_1x.jpg

ff=${f//-1x/-2x}
echo $ff
../png2bdf -c "https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE"  -f open_iconic_${1}_2x -o open_iconic_${1}_2x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '64-511' -v open_iconic_${1}_2x.bdf -n open_iconic_${1}_2x -o open_iconic_${1}_2x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_${1}_2x.jpg
 
ff=${f//-1x/-4x}
echo $ff
#echo  ../png2bdf  -f open_iconic_${1}_4x -o open_iconic_${1}_4x.bdf -e 64 ${ff}
#sleep 10

../png2bdf -c "https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE" -f open_iconic_${1}_4x -o open_iconic_${1}_4x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '64-511' -v open_iconic_${1}_4x.bdf -n open_iconic_${1}_4x -o open_iconic_${1}_4x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_${1}_4x.jpg

ff=${f//-1x/-6x}
echo $ff
../png2bdf -c "https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE"  -f open_iconic_${1}_6x -o open_iconic_${1}_6x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '64-511' -v open_iconic_${1}_6x.bdf -n open_iconic_${1}_6x -o open_iconic_${1}_6x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_${1}_6x.jpg

ff=${f//-1x/-8x}
echo $ff
../png2bdf -c "https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE"  -f open_iconic_${1}_8x -o open_iconic_${1}_8x.bdf -e 64 ${ff}
../../bdfconv/bdfconv -f 1 -b 0 -m '64-511' -v open_iconic_${1}_8x.bdf -n open_iconic_${1}_8x -o open_iconic_${1}_8x.c -d ../../bdf/helvB12.bdf -g 8
convert bdf.tga open_iconic_${1}_8x.jpg
}


rm open_iconic*.c
rm open_iconic*.bdf
rm open_iconic*.jpg





app="path/aperture-1x.png  path/calculator-1x.png  path/calendar-1x.png  path/camera-slr-1x.png  path/chat-1x.png  path/clock-1x.png  path/compass-1x.png  path/shield-1x.png  path/timer-1x.png"

arrow="path/arrow-bottom-1x.png \
path/arrow-left-1x.png \
path/arrow-right-1x.png \
path/arrow-top-1x.png \
path/arrow-circle-bottom-1x.png \
path/arrow-circle-left-1x.png \
path/arrow-circle-right-1x.png \
path/arrow-circle-top-1x.png \
path/arrow-thick-bottom-1x.png \
path/arrow-thick-left-1x.png \
path/arrow-thick-right-1x.png \
path/arrow-thick-top-1x.png \
path/caret-bottom-1x.png \
path/caret-left-1x.png \
path/caret-right-1x.png \
path/caret-top-1x.png \
path/chevron-bottom-1x.png \
path/chevron-left-1x.png \
path/chevron-right-1x.png \
path/chevron-top-1x.png \
path/data-transfer-download-1x.png \
path/data-transfer-upload-1x.png \
path/loop-1x.png \
path/loop-circular-1x.png \
path/loop-square-1x.png \
path/random-1x.png \
path/share-1x.png \
path/share-boxed-1x.png"


check="path/check-1x.png  path/circle-check-1x.png  path/circle-x-1x.png  path/task-1x.png  path/x-1x.png"

email="path/envelope-closed-1x.png  path/envelope-open-1x.png  path/inbox-1x.png  path/tag-1x.png  path/tags-1x.png"

embedded="path/battery-empty-1x.png  path/bell-1x.png       path/cog-1x.png       path/flash-1x.png       path/home-1x.png       path/pencil-1x.png         path/pulse-1x.png   path/warning-1x.png  path/wrench-1x.png \
path/battery-full-1x.png   path/bluetooth-1x.png  path/contrast-1x.png  path/hard-drive-1x.png  path/lightbulb-1x.png  path/power-standby-1x.png  path/reload-1x.png  path/wifi-1x.png"

gui="path/account-login-1x.png \
path/account-logout-1x.png \
path/action-redo-1x.png \
path/action-undo-1x.png \
path/collapse-down-1x.png \
path/collapse-left-1x.png \
path/collapse-right-1x.png \
path/collapse-up-1x.png \
path/crop-1x.png \
path/dashboard-1x.png \
path/elevator-1x.png \
path/expand-down-1x.png \
path/expand-left-1x.png \
path/expand-right-1x.png \
path/expand-up-1x.png \
path/fullscreen-enter-1x.png \
path/fullscreen-exit-1x.png \
path/layers-1x.png \
path/list-1x.png \
path/list-rich-1x.png \
path/menu-1x.png \
path/move-1x.png \
path/print-1x.png \
path/resize-both-1x.png \
path/resize-height-1x.png \
path/resize-width-1x.png \
path/sort-ascending-1x.png \
path/sort-descending-1x.png \
path/zoom-in-1x.png \
path/zoom-out-1x.png"

human="path/eye-1x.png  path/eyedropper-1x.png  path/heart-1x.png  path/people-1x.png  path/person-1x.png"

mime="path/audio-1x.png      path/book-1x.png       path/document-1x.png  path/folder-1x.png  path/image-1x.png      path/project-1x.png  path/spreadsheet-1x.png  path/video-1x.png \
bar-chart-1x.png  path/clipboard-1x.png  path/file-1x.png      path/graph-1x.png   path/pie-chart-1x.png  path/script-1x.png   path/trash-1x.png"

other="path/bolt-1x.png  path/dial-1x.png  path/fork-1x.png  path/grid-four-up-1x.png  path/grid-three-up-1x.png  path/grid-two-up-1x.png  path/signal-1x.png  path/target-1x.png"

play="path/audio-spectrum-1x.png \
path/bullhorn-1x.png \
path/eject-1x.png \
path/headphones-1x.png \
path/media-pause-1x.png \
path/media-play-1x.png \
path/media-record-1x.png \
path/media-skip-backward-1x.png \
path/media-skip-forward-1x.png \
path/media-step-backward-1x.png \
path/media-step-forward-1x.png \
path/media-stop-1x.png \
path/microphone-1x.png \
path/musical-note-1x.png \
path/play-circle-1x.png \
path/volume-high-1x.png \
path/volume-low-1x.png \
path/volume-off-1x.png"

text="path/align-center-1x.png \
path/align-left-1x.png \
path/align-right-1x.png \
path/bold-1x.png \
path/code-1x.png \
path/command-1x.png \
path/comment-square-1x.png \
path/copywriting-1x.png \
path/delete-1x.png \
path/double-quote-sans-left-1x.png \
path/double-quote-sans-right-1x.png \
path/double-quote-serif-left-1x.png \
path/double-quote-serif-right-1x.png \
path/ellipses-1x.png \
path/excerpt-1x.png \
path/header-1x.png \
path/infinity-1x.png \
path/info-1x.png \
path/italic-1x.png \
path/justify-center-1x.png \
path/justify-left-1x.png \
path/justify-right-1x.png \
path/medical-cross-1x.png \
path/minus-1x.png \
path/plus-1x.png \
path/question-mark-1x.png \
path/terminal-1x.png \
path/text-1x.png \
path/underline-1x.png \
path/vertical-align-bottom-1x.png \
path/vertical-align-center-1x.png \
path/vertical-align-top-1x.png"

thing="path/basket-1x.png  path/briefcase-1x.png  path/credit-card-1x.png  path/key-1x.png          path/lock-unlocked-1x.png     path/paperclip-1x.png  path/puzzle-piece-1x.png \
beaker-1x.png  path/brush-1x.png      path/droplet-1x.png      path/laptop-1x.png       path/magnifying-glass-1x.png  path/phone-1x.png      path/signpost-1x.png \
box-1x.png     path/bug-1x.png        path/fire-1x.png         path/lock-locked-1x.png  path/monitor-1x.png           path/pin-1x.png        path/tablet-1x.png"

weather="path/cloud-1x.png  path/cloudy-1x.png  path/moon-1x.png  path/rain-1x.png  path/star-1x.png  path/sun-1x.png"

www="path/badge-1x.png  path/bookmark-1x.png  path/cart-1x.png            path/cloud-upload-1x.png   path/flag-1x.png   path/link-broken-1x.png  path/location-1x.png  path/map-marker-1x.png  path/rss-alt-1x.png     path/thumb-up-1x.png \
path/ban-1x.png    path/browser-1x.png   path/cloud-download-1x.png  path/external-link-1x.png  path/globe-1x.png  path/link-intact-1x.png  path/map-1x.png       path/rss-1x.png         path/thumb-down-1x.png  path/transfer-1x.png"

all="path/account-login-1x.png \
path/account-logout-1x.png \
path/action-redo-1x.png \
path/action-undo-1x.png \
path/align-center-1x.png \
path/align-left-1x.png \
path/align-right-1x.png \
path/aperture-1x.png \
path/arrow-bottom-1x.png \
path/arrow-circle-bottom-1x.png \
path/arrow-circle-left-1x.png \
path/arrow-circle-right-1x.png \
path/arrow-circle-top-1x.png \
path/arrow-left-1x.png \
path/arrow-right-1x.png \
path/arrow-thick-bottom-1x.png \
path/arrow-thick-left-1x.png \
path/arrow-thick-right-1x.png \
path/arrow-thick-top-1x.png \
path/arrow-top-1x.png \
path/audio-1x.png \
path/audio-spectrum-1x.png \
path/badge-1x.png \
path/ban-1x.png \
path/bar-chart-1x.png \
path/basket-1x.png \
path/battery-empty-1x.png \
path/battery-full-1x.png \
path/beaker-1x.png \
path/bell-1x.png \
path/bluetooth-1x.png \
path/bold-1x.png \
path/bolt-1x.png \
path/book-1x.png \
path/bookmark-1x.png \
path/box-1x.png \
path/briefcase-1x.png \
path/british-pound-1x.png \
path/browser-1x.png \
path/brush-1x.png \
path/bug-1x.png \
path/bullhorn-1x.png \
path/calculator-1x.png \
path/calendar-1x.png \
path/camera-slr-1x.png \
path/caret-bottom-1x.png \
path/caret-left-1x.png \
path/caret-right-1x.png \
path/caret-top-1x.png \
path/cart-1x.png \
path/chat-1x.png \
path/check-1x.png \
path/chevron-bottom-1x.png \
path/chevron-left-1x.png \
path/chevron-right-1x.png \
path/chevron-top-1x.png \
path/circle-check-1x.png \
path/circle-x-1x.png \
path/clipboard-1x.png \
path/clock-1x.png \
path/cloud-1x.png \
path/cloud-download-1x.png \
path/cloud-upload-1x.png \
path/cloudy-1x.png \
path/code-1x.png \
path/cog-1x.png \
path/collapse-down-1x.png \
path/collapse-left-1x.png \
path/collapse-right-1x.png \
path/collapse-up-1x.png \
path/command-1x.png \
path/comment-square-1x.png \
path/compass-1x.png \
path/contrast-1x.png \
path/copywriting-1x.png \
path/credit-card-1x.png \
path/crop-1x.png \
path/dashboard-1x.png \
path/data-transfer-download-1x.png \
path/data-transfer-upload-1x.png \
path/delete-1x.png \
path/dial-1x.png \
path/document-1x.png \
path/dollar-1x.png \
path/double-quote-sans-left-1x.png \
path/double-quote-sans-right-1x.png \
path/double-quote-serif-left-1x.png \
path/double-quote-serif-right-1x.png \
path/droplet-1x.png \
path/eject-1x.png \
path/elevator-1x.png \
path/ellipses-1x.png \
path/envelope-closed-1x.png \
path/envelope-open-1x.png \
path/euro-1x.png \
path/excerpt-1x.png \
path/expand-down-1x.png \
path/expand-left-1x.png \
path/expand-right-1x.png \
path/expand-up-1x.png \
path/external-link-1x.png \
path/eye-1x.png \
path/eyedropper-1x.png \
path/file-1x.png \
path/fire-1x.png \
path/flag-1x.png \
path/flash-1x.png \
path/folder-1x.png \
path/fork-1x.png \
path/fullscreen-enter-1x.png \
path/fullscreen-exit-1x.png \
path/globe-1x.png \
path/graph-1x.png \
path/grid-four-up-1x.png \
path/grid-three-up-1x.png \
path/grid-two-up-1x.png \
path/hard-drive-1x.png \
path/header-1x.png \
path/headphones-1x.png \
path/heart-1x.png \
path/home-1x.png \
path/image-1x.png \
path/inbox-1x.png \
path/infinity-1x.png \
path/info-1x.png \
path/italic-1x.png \
path/justify-center-1x.png \
path/justify-left-1x.png \
path/justify-right-1x.png \
path/key-1x.png \
path/laptop-1x.png \
path/layers-1x.png \
path/lightbulb-1x.png \
path/link-broken-1x.png \
path/link-intact-1x.png \
path/list-1x.png \
path/list-rich-1x.png \
path/location-1x.png \
path/lock-locked-1x.png \
path/lock-unlocked-1x.png \
path/loop-1x.png \
path/loop-circular-1x.png \
path/loop-square-1x.png \
path/magnifying-glass-1x.png \
path/map-1x.png \
path/map-marker-1x.png \
path/media-pause-1x.png \
path/media-play-1x.png \
path/media-record-1x.png \
path/media-skip-backward-1x.png \
path/media-skip-forward-1x.png \
path/media-step-backward-1x.png \
path/media-step-forward-1x.png \
path/media-stop-1x.png \
path/medical-cross-1x.png \
path/menu-1x.png \
path/microphone-1x.png \
path/minus-1x.png \
path/monitor-1x.png \
path/moon-1x.png \
path/move-1x.png \
path/musical-note-1x.png \
path/paperclip-1x.png \
path/pencil-1x.png \
path/people-1x.png \
path/person-1x.png \
path/phone-1x.png \
path/pie-chart-1x.png \
path/pin-1x.png \
path/play-circle-1x.png \
path/plus-1x.png \
path/power-standby-1x.png \
path/print-1x.png \
path/project-1x.png \
path/pulse-1x.png \
path/puzzle-piece-1x.png \
path/question-mark-1x.png \
path/rain-1x.png \
path/random-1x.png \
path/reload-1x.png \
path/resize-both-1x.png \
path/resize-height-1x.png \
path/resize-width-1x.png \
path/rss-1x.png \
path/rss-alt-1x.png \
path/script-1x.png \
path/share-1x.png \
path/share-boxed-1x.png \
path/shield-1x.png \
path/signal-1x.png \
path/signpost-1x.png \
path/sort-ascending-1x.png \
path/sort-descending-1x.png \
path/spreadsheet-1x.png \
path/star-1x.png \
path/sun-1x.png \
path/tablet-1x.png \
path/tag-1x.png \
path/tags-1x.png \
path/target-1x.png \
path/task-1x.png \
path/terminal-1x.png \
path/text-1x.png \
path/thumb-down-1x.png \
path/thumb-up-1x.png \
path/timer-1x.png \
path/transfer-1x.png \
path/trash-1x.png \
path/underline-1x.png \
path/vertical-align-bottom-1x.png \
path/vertical-align-center-1x.png \
path/vertical-align-top-1x.png \
path/video-1x.png \
path/volume-high-1x.png \
path/volume-low-1x.png \
path/volume-off-1x.png \
path/warning-1x.png \
path/wifi-1x.png \
path/wrench-1x.png \
path/x-1x.png \
path/yen-1x.png \
path/zoom-in-1x.png \
path/zoom-out-1x.png"


bdf app "${app}"
bdf arrow "${arrow}"
bdf check "${check}"
bdf email "${email}"
bdf embedded "${embedded}"
bdf gui "${gui}"
bdf human "${human}"
bdf mime "${mime}"
bdf other "${other}"
bdf play "${play}"
bdf text "${text}"
bdf thing "${thing}"
bdf weather "${weather}"
bdf www "${www}"
bdf all "${all}"
