#include "amigaos3-fs-file.h"

/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "backends/fs/amigaos3/amigaos3-fs-file.h"
#include <proto/dos.h>
#include <dos/stdio.h>

AmigaOS3File::AmigaOS3File(BPTR handle) : _handle(handle), _size(0), _error(0), _eof(false) {
	assert(_handle);

	// remember current position
	LONG oldpos = Seek(_handle, 0, OFFSET_END);

	//determine size
	LONG end = Seek(_handle, 0, OFFSET_CURRENT);
	assert(end != -1);
	_size = end;

	// rewind
	oldpos = Seek(_handle, oldpos, OFFSET_BEGINNING);
	assert(oldpos != -1);
}

AmigaOS3File::~AmigaOS3File() {
	Close(_handle);
}

bool AmigaOS3File::err() const {
	return _error;
}

void AmigaOS3File::clearErr() {
	_error = 0;
}

bool AmigaOS3File::eos() const {
	return _eof;
}

int32 AmigaOS3File::pos() const {
	LONG pos = Seek(_handle, 0, OFFSET_CURRENT);
	return pos;
}

int32 AmigaOS3File::size() const {
	return _size;
}

bool AmigaOS3File::seek(int32 offs, int whence) {
	LONG mode = whence == SEEK_SET ? OFFSET_BEGINNING
			  : whence == SEEK_CUR ? OFFSET_CURRENT
								   : OFFSET_END;
	LONG success = Seek(_handle, offs, mode);
	if (success == -1) {
		_error = IoErr();
	}
	_eof = false;
	return success != -1;
}

uint32 AmigaOS3File::read(void *ptr, uint32 len) {
	LONG bytesRead = FRead(_handle, ptr, 1, len);
	if (bytesRead == 0) {
		_error = IoErr();
		_eof = true;
		return 0;
	}
	return bytesRead;
}

uint32 AmigaOS3File::write(const void *ptr, uint32 len) {
	LONG blocksWritten = FWrite(_handle, const_cast<void*>(ptr), len, 1);
	if (blocksWritten == 0) {
		_error = IoErr();
		return 0;
	}
	assert(blocksWritten == 1);
	return len;
}

bool AmigaOS3File::flush() {
	LONG success =  Flush(_handle);
	return success;
}

AmigaOS3File *AmigaOS3File::makeFromPath(const Common::String &path, bool writeMode) {

	BPTR handle = Open(path.c_str(), writeMode ? MODE_NEWFILE : MODE_OLDFILE);

	if (handle && !writeMode) {
		SetVBuf(handle, NULL, BUF_FULL, 8192);
	}

	if (handle)
		return new AmigaOS3File(handle);
	return 0;
}

