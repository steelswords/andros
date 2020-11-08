#!/bin/bash

# Requirements: ImageMagick for the screenshot.

# Description: Runs the simulation and takes a screenshot, saving
# it as the current git commit's hash.

projectRoot="$(git rev-parse --show-toplevel)"
currentDir="$(pwd)"
cd "$projectRoot"

currentRevision="$(git rev-parse HEAD)"
mkdir -p "${projectRoot}/screenshots/automatic"
screenshotFile="${projectRoot}/screenshots/automatic/${currentRevision}.png"
echo "$screenshotFile"
echo "Compiling code...."

make
make test &
sleep 4

echo "Taking screenshot; Say cheese!"
import -window QEMU "$screenshotFile"

cd "$currentDir"
killall qemu-system-x86_64
