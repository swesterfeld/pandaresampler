# PandaResampler

## Description

PandaResampler is a fast C++ (SSEified) factor 2 resampler for audio signals.
It is easy to integrate into projects since it supports header only mode.

PandaResampler is currently *experimental* which means that right now there
still can be API changes that break backward compatibility. API documentation
is available here:  https://space.twc.de/~stefan/pandaresampler/api-0.2.0

## Header only C++ library

To integrate PandaResampler into your project, copy all sources from the
`lib` and `lib/hiir` directories into your project. For true header only mode,
enable the
```
#define PANDA_RESAMPLER_HEADER_ONLY
```
line at the beginning of `pandaresampler.hh`. Or build the `pandaresampler.cc`
file as part of your project.

## License

PandaResampler is released under
[MPL-2.0](https://github.com/swesterfeld/pandaresampler/blob/master/MPL-2.0.txt),
however it also contains code from hiir which is licensed under
[WTFPL2](https://github.com/swesterfeld/pandaresampler/blob/master/lib/hiir/license.txt).
