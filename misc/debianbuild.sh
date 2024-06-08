#!/bin/bash
set -Eeuo pipefail

docker build -f "misc/Dockerfile" -t pandaresampler-dbuild .
