#ifndef AMIGAOS3FSFILE_H
#define AMIGAOS3FSFILE_H

#include "common/scummsys.h"
#include "common/noncopyable.h"
#include "common/stream.h"
#include "common/str.h"

#include <dos/dos.h>

class AmigaOS3File : public Common::SeekableReadStream, public Common::WriteStream, public Common::NonCopyable
{
public:
	/**
	 * Given a path, invokes fopen on that path and wrap the result in a
	 * StdioStream instance.
	 */
	static AmigaOS3File *makeFromPath(const Common::String &path, bool writeMode);

	AmigaOS3File(BPTR handle);
	virtual ~AmigaOS3File();

	virtual bool err() const;
	virtual void clearErr();
	virtual bool eos() const;

	virtual uint32 write(const void *dataPtr, uint32 dataSize);
	virtual bool flush();

	virtual int32 pos() const;
	virtual int32 size() const;
	virtual bool seek(int32 offs, int whence = SEEK_SET);
	virtual uint32 read(void *dataPtr, uint32 dataSize);

protected:
	/** File handle to the actual file. */
	BPTR _handle;
	LONG _error;
	LONG _size;
};

#endif // AMIGAOS3FSFILE_H
