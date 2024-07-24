#!/bin/bash

function download_devkitpro() {
    echo "Downloading devkitpro packages..."

    devkitpro_archives=(
        "https://wii.leseratte10.de/devkitPro/file.php/devkitPPC-r41-2-linux_x86_64.pkg.tar.xz"
        "https://wii.leseratte10.de/devkitPro/file.php/libogc-2.3.1-1-any.pkg.tar.xz"
        "https://wii.leseratte10.de/devkitPro/file.php/devkitppc-rules-1.1.1-1-any.pkg.tar.xz"
        "https://wii.leseratte10.de/devkitPro/file.php/general-tools-1.2.0-2-linux_x86_64.pkg.tar.xz"
        "https://wii.leseratte10.de/devkitPro/file.php/gamecube-tools-1.0.3-1-linux_x86_64.pkg.tar.xz"
    )

    # Create a directory for the downloads
    mkdir -p devkitPro_assets

    any_downloads=false

    # Download the files
    for url in "${devkitpro_archives[@]}"; do
        # download if not already downloaded
        [ ! -f ".devkitpro_cache/$(basename "$url")" ] \
            && echo "Downloading $(basename "$url") from $url" \
            && wget -q --show-progress -P .devkitpro_cache "$url" \
            && any_downloads=true
    done

    if [ "$any_downloads" = false ]; then
        echo "All devkitpro packages are already downloaded."
    fi

    echo ""
}

download_devkitpro

echo "Building the project in docker..."
DOCKER_BUILDKIT=1 docker build -o output .

echo ""
echo "Done! Check output/ for the built files."
