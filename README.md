# ScummVM 2.0.0 AmigaOS 3 Port

The basis for this ScummVM port for classic Amigas is a ScummVM 1.8.1 port obtained form:
https://github.com/mntmn/scummvm-amigaos3

I imported this Amiga port and replanted it ontop of the official 1.8.1 ScummVM branch.
Later, ScummVM 2.0 was merged into it.

This port re-enables the original AGA port, but replaces the chunky-to-planar routines (no source was available) with WriteChunkyPixels. You NEED to install BlazeWCP to get any decent speed out of it.
Midi playback via CAMD was added, too - enjoy nice MT32 music on your Amiga! A nice side effect of Midi playback (in games that support it) is that it has super low tax on the CPU and it sounds superior.
The original Music/SoundFX output via AHI is still in place. But it is seriously slow, which is most likely a flaw in how ScummVM is managing
sound playback - lots of room for improvement.

## How to build

This source will build with Stefan 'Bebbo' Franke's GCC 6.4.1 toolchain at: https://github.com/bebbo/amiga-gcc

The binary directory of the toolchain is expected to be in the search path.

1) Configure
./configure --host=m68k-amigaos --disable-all-engines --enable-engine=scumm,scumm-7-8 --disable-mt32emu --enable-release --disable-hq-scalers --with-amiga-prefix=/media/sf_Amiga/ScummVM

This only enables the SCUMM based engines. Other engines like SCI and Tinsel can be added but will increase the binary size.
'--with-amiga-prefix=/media/sf_Amiga/ScummVM'  points to a directory shared with the Amiga (or Amiga Emulator)
All required binaries will be installed there along with a stripped version of the executable

Some engine code makes use of dynamic_cast<>, which requires RTTI to be available. Edit the 'configure' script and find the
amigaos3 relevant section and disable the line
	append_var CXXFLAGS "-fno-rtti"
there.

2) Run
make amigaos3dist -j8

Will build in 8 threads and put the resulting binaries where '--with-amiga-prefix' pointed to.

