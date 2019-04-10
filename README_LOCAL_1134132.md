# ScummVM 1.8.1 AmigaOS 3 Port

I obtained this code from TuKo (Vampire Team). The original patches for AmigaOS 3.x support are from NovaCoder and updates have been made by relnev. My goal is to bring development of this fork to the public on GitHub and contribute some clean-ups and fixes and make it work with my graphics card MNT VA2000.

This port re-enables the original AGA port, but replaces the chunky-to-planar routines (no source was available) with WriteChunkyPixels. You NEED to install BlazeWCP to get any decent speed out of it.
Midi playback via CAMD was added, too - enjoy nice MT32 music on your Amiga! A nice side effect of Midi playback (in games that support it) is that it has super low tax on the CPU and it sounds superior.
The original Music/SoundFX output via AHI is still in place. But it is seriously slow, which is most likely a flaw in how ScummVM is managing
sound playback - lots of room for improvement.

## How to build

This source will build with bebbo's GCC 6.4.1 toolchain at: https://github.com/bebbo/amiga-gcc

The binary directory of the toolchain is expectyd to be in the search path.

1) Configure
./configure --host=m68k-amigaos --disable-all-engines --enable-engine=scumm,scumm-7-8 --disable-mt32emu --enable-release --disable-hq-scalers --with-amiga-prefix=/media/sf_Amiga/ScummVM

This only enables the Scumm based engines. Other engines like sci, sci32 and tinsel can be added but will increase the binary size.
'--with-amiga-prefix=/media/sf_Amiga/ScummVM'  points to a directory shared with the AMiga (or Amiga Emulator)

2) Run
make amigaos3dist -j8

Will build in 8 threads and put the resulting binaries where '--with-amiga-prefix' pointed to


## Readme from Aminet Binaries

The readme on aminet contains some hints about compilation:

These are binaries of ScummVM 1.8.1 with RTG AmigaOS 3 support, using
the work done by NovaCoder for ScummVM 1.5.0 and continued by relnev.

http://aminet.net/package/game/misc/ScummVM_RTG

These binaries were built using the m68k-amigaos netsurf toolchain.

The scummvm 1.8.1 configure flags used were:

--host=m68k-unknown-amigaos --disable-all-engines 
--enable-engine=sword1,tinsel,agos,kyra,dreamweb,sky,
scumm,queen,lure,agi,sci,scumm-7-8,sci32 --disable-mt32emu 
--enable-release --disable-hq-scalers

And the CXXFLAGS used were:
-Os -ffast-math -fsingle-precision-constant -fweb -frename-registers 
-finline-functions -funit-at-a-time -fomit-frame-pointer 
-fno-exceptions -fno-rtti -DNDEBUG
