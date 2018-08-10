#ifndef _INLINE_CAMD_H
#define _INLINE_CAMD_H

#ifndef CLIB_CAMD_PROTOS_H
#define CLIB_CAMD_PROTOS_H
#endif

#ifndef __INLINE_MACROS_H
#include <inline/macros.h>
#endif

#ifndef  EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef  MIDI_CAMD_H
#include <midi/camd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CAMD_BASE_NAME
#define CAMD_BASE_NAME CamdBase
#endif

#define LockCAMD(locktype) \
	LP1(0x1e, APTR, LockCAMD, ULONG, locktype, d0, \
	, CAMD_BASE_NAME)

#define UnlockCAMD(lock) \
	LP1NR(0x24, UnlockCAMD, APTR, lock, a0, \
	, CAMD_BASE_NAME)

#define CreateMidiA(tags) \
	LP1(0x2a, struct MidiNode *, CreateMidiA, struct TagItem *, tags, a0, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define CreateMidi(tags...) \
	({ULONG _tags[] = {tags}; CreateMidiA((struct TagItem *) _tags);})
#endif

#define DeleteMidi(mi) \
	LP1NR(0x30, DeleteMidi, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define SetMidiAttrsA(mi, tags) \
	LP2(0x36, BOOL, SetMidiAttrsA, struct MidiNode *, mi, a0, struct TagItem *, tags, a1, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define SetMidiAttrs(mi, tags...) \
	({ULONG _tags[] = {tags}; SetMidiAttrsA((mi), (struct TagItem *) _tags);})
#endif

#define GetMidiAttrsA(mi, tags) \
	LP2(0x3c, ULONG, GetMidiAttrsA, struct MidiNode *, mi, a0, struct TagItem *, tags, a1, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define GetMidiAttrs(mi, tags...) \
	({ULONG _tags[] = {tags}; GetMidiAttrsA((mi), (struct TagItem *) _tags);})
#endif

#define NextMidi(mi) \
	LP1(0x42, struct MidiNode *, NextMidi, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define FindMidi(name) \
	LP1(0x48, struct MidiNode *, FindMidi, STRPTR, name, a1, \
	, CAMD_BASE_NAME)

#define FlushMidi(mi) \
	LP1NR(0x4e, FlushMidi, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define AddMidiLinkA(mi, type, tags) \
	LP3(0x54, struct MidiLink *, AddMidiLinkA, struct MidiNode *, mi, a0, LONG, type, d0, struct TagItem *, tags, a1, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define AddMidiLink(mi, type, tags...) \
	({ULONG _tags[] = {tags}; AddMidiLinkA((mi), (type), (struct TagItem *) _tags);})
#endif

#define RemoveMidiLink(ml) \
	LP1NR(0x5a, RemoveMidiLink, struct MidiLink *, ml, a0, \
	, CAMD_BASE_NAME)

#define SetMidiLinkAttrsA(ml, tags) \
	LP2(0x60, BOOL, SetMidiLinkAttrsA, struct MidiLink *, ml, a0, struct TagItem *, tags, a1, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define SetMidiLinkAttrs(ml, tags...) \
	({ULONG _tags[] = {tags}; SetMidiLinkAttrsA((ml), (struct TagItem *) _tags);})
#endif

#define GetMidiLinkAttrsA(ml, tags) \
	LP2(0x66, ULONG, GetMidiLinkAttrsA, struct MidiLink *, ml, a0, struct TagItem *, tags, a1, \
	, CAMD_BASE_NAME)

#ifndef NO_INLINE_STDARG
#define GetMidiLinkAttrs(ml, tags...) \
	({ULONG _tags[] = {tags}; GetMidiLinkAttrsA((ml), (struct TagItem *) _tags);})
#endif

#define NextClusterLink(mc, ml, type) \
	LP3(0x6c, struct MidiLink *, NextClusterLink, struct MidiCluster *, mc, a0, struct MidiLink *, ml, a1, LONG, type, d0, \
	, CAMD_BASE_NAME)

#define NextMidiLink(mi, ml, type) \
	LP3(0x72, struct MidiLink *, NextMidiLink, struct MidiNode *, mi, a0, struct MidiLink *, ml, a1, LONG, type, d0, \
	, CAMD_BASE_NAME)

#define MidiLinkConnected(ml) \
	LP1(0x78, BOOL, MidiLinkConnected, struct MidiLink *, ml, a0, \
	, CAMD_BASE_NAME)

#define NextCluster(mc) \
	LP1(0x7e, struct MidiCluster *, NextCluster, struct MidiCluster *, mc, a0, \
	, CAMD_BASE_NAME)

#define FindCluster(name) \
	LP1(0x84, struct MidiCluster *, FindCluster, STRPTR, name, a0, \
	, CAMD_BASE_NAME)

#define PutMidi(ml, msgdata) \
	LP2NR(0x8a, PutMidi, struct MidiLink *, ml, a0, LONG, msgdata, d0, \
	, CAMD_BASE_NAME)

#define GetMidi(mi, msg) \
	LP2(0x90, BOOL, GetMidi, struct MidiNode *, mi, a0, MidiMsg *, msg, a1, \
	, CAMD_BASE_NAME)

#define WaitMidi(mi, msg) \
	LP2(0x96, BOOL, WaitMidi, struct MidiNode *, mi, a0, MidiMsg *, msg, a1, \
	, CAMD_BASE_NAME)

#define PutSysEx(ml, buffer) \
	LP2NR(0x9c, PutSysEx, struct MidiLink *, ml, a0, UBYTE *, buffer, a1, \
	, CAMD_BASE_NAME)

#define GetSysEx(mi, buffer, len) \
	LP3(0xa2, ULONG, GetSysEx, struct MidiNode *, mi, a0, UBYTE *, buffer, a1, ULONG, len, d0, \
	, CAMD_BASE_NAME)

#define QuerySysEx(mi) \
	LP1(0xa8, ULONG, QuerySysEx, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define SkipSysEx(mi) \
	LP1NR(0xae, SkipSysEx, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define GetMidiErr(mi) \
	LP1(0xb4, UBYTE, GetMidiErr, struct MidiNode *, mi, a0, \
	, CAMD_BASE_NAME)

#define MidiMsgType(msg) \
	LP1(0xba, WORD, MidiMsgType, MidiMsg *, msg, a0, \
	, CAMD_BASE_NAME)

#define MidiMsgLen(status) \
	LP1(0xc0, WORD, MidiMsgLen, ULONG, status, d0, \
	, CAMD_BASE_NAME)

#define ParseMidi(ml, buffer, length) \
	LP3NR(0xc6, ParseMidi, struct MidiLink *, ml, a0, UBYTE *, buffer, a1, ULONG, length, d0, \
	, CAMD_BASE_NAME)

#define OpenMidiDevice(name) \
	LP1(0xcc, struct MidiDeviceData *, OpenMidiDevice, UBYTE *, name, a0, \
	, CAMD_BASE_NAME)

#define CloseMidiDevice(mdd) \
	LP1NR(0xd2, CloseMidiDevice, struct MidiDeviceData *, mdd, a0, \
	, CAMD_BASE_NAME)

#define RethinkCAMD() \
	LP0(0xd8, int, RethinkCAMD, \
	, CAMD_BASE_NAME)

#define StartClusterNotify(node) \
	LP1NR(0xde, StartClusterNotify, struct ClusterNotifyNode *, node, a0, \
	, CAMD_BASE_NAME)

#define EndClusterNotify(node) \
	LP1NR(0xe4, EndClusterNotify, struct ClusterNotifyNode *, node, a0, \
	, CAMD_BASE_NAME)


#ifdef __cplusplus
}
#endif
#endif /*  _INLINE_CAMD_H  */
