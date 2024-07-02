# PandaResampler

## Description

PandaResampler is a fast C++ (SSEified) factor 2 resampler for audio signals.
It is easy to integrate into projects since it supports header only mode.

PandaResampler is currently *experimental* which means that right now there
still can be API changes that break backward compatibility. API documentation
is available here:  https://space.twc.de/~stefan/pandaresampler/api-0.2.1

## Using the Shared Library

To use PandaResampler as shared library, build it using

    meson setup build                      # configure the build
    meson compile -C build                 # compile PandaResampler
    sudo meson install -C build            # install PandaResampler

Then, you can use `pkg-config` to get the necessary compiler flags to use
the shared library:

 * `pkg-config --cflags pandaresampler` to get the flags for compiling
 * `pkg-config --libs pandaresampler` for linking

## Header only C++ Library

If you do not want to link against any library, you can use

 * `pkg-config --cflags pandaresampler` and the define `-DPANDA_RESAMPLER_HEADER_ONLY`

while compiling your code, this will not require any library to be present to
compile or run your compiled program.

If you prefer, you can simply copy everything from the `include/` directory of
the pandaresampler to your project to make it fully self-contained and avoid
using `pkg-config`.

## License

PandaResampler is released under
[MPL-2.0](https://github.com/swesterfeld/pandaresampler/blob/master/MPL-2.0.txt),
however it also contains code from hiir which is licensed under
[WTFPL2](https://github.com/swesterfeld/pandaresampler/blob/master/include/pandaresampler/hiir/license.txt).
