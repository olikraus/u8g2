#!/bin/bash

SOURCE_DIR=~/git/u8g2.wiki/fntpic
DEST_DIR=~/git/u8g2/tools/font/build

# Create destination directory if it doesn't exist
# mkdir -p "$DEST_DIR"

# Loop through all .png files in the source directory
find $SOURCE_DIR -type f -name "*te*.png" | while read -r src_file; do
    # Get the relative path of the file from source directory
    rel_path="${src_file#$SOURCE_DIR/}"
    dest_file="$DEST_DIR/$rel_path"

    # If file doesn't exist in destination, or size has changed, copy it
    if [ ! -f "$dest_file" ] || [ "$(stat -c %s "$src_file")" -ne "$(stat -c %s "$dest_file" 2>/dev/null)" ]; then
        # mkdir -p "$(dirname "$dest_file")"   # Ensure destination subdirectory exists
        ls -al "$src_file" "$dest_file"
        cp "$src_file" "$dest_file"
        echo "Copied: $rel_path"
    fi
done
