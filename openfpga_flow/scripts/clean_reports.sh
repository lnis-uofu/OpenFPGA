#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

file="$1"
echo "Cleaning tool_comment from rr_graph tag in $file if it does not have .xml extension..."

# Check if file has .xml extension
if [[ "$file" == *.xml ]]; then
    # Remove tool_comment attribute from rr_graph tag
    sed -i 's/<rr_graph.*>/<rr_graph schema_file_id="" tool_comment="" tool_name="vpr" tool_version="">/g' "$file"
    echo "Cleaned tool_comment from rr_graph tag in $file"
else
    echo "File $file does not have .xml extension, skipping cleaning."
fi
