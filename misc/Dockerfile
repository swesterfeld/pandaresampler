FROM gcc:latest

RUN apt-get update && apt-get install -y libfftw3-dev autoconf-archive libzita-resampler-dev clang meson

ADD . /pandaresampler
WORKDIR /pandaresampler

RUN misc/build.sh
