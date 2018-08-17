# ios-minicap

iOS Minicap provides a socket interface for streaming realtime screen capture data out of iOS devices. It is built with AVFoundation and iOS Screen mirroring feature.


## Requirements

* brew install ffmpeg
* brew install mpv or vlc or the player you prefer for live viewing
* Xcode (for the Frameworks)
* [cmake](https://cmake.org/)
* OS X Yosemite (10.9) or higher
* iOS 8 or higher
* [Lightning cable](https://en.wikipedia.org/wiki/Lightning_(connector)). See the list of devices.

## Building

```
./build.sh
```

You should now have the binaries available in `./build`.

## Usage

Before running, please check that:

* the computer is trusted by the phone
* the phone screen is not turned off
* ffmpeg is installed

You can try it using:

```
$ ./run.sh
```

And in another window:

### To record the screen to a file:

```
$ ffmpeg -f rawvideo -pix_fmt bgra -s:v 1080x1920 -r 60 -i tcp://127.0.0.1:12345 -c:v libx264 -preset ultrafast -crf 30 -f matroska output.mkv
```

### To view the screen in real time (with some delay):

```
$ ffmpeg -f rawvideo -pix_fmt bgra -s:v 1080x1920 -r 60 -i tcp://127.0.0.1:12345 -c:v libx264 -preset ultrafast -crf 30 -f matroska pipe:1 | mpv -
```

Other players can be used as well, and the output resolution and frame rate can be lowered by adding for example `-r 30 -s 720x1280` to the output flags.

## Generation for Xcode

Optionally you may want to use Xcode for developing or building. But still, you should use cmake as a build system.

```
./open_xcode.sh
```
