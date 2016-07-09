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

#include "backends/networking/sdl_net/uploadfileclienthandler.h"
#include "common/textconsole.h"
#include <common/memstream.h>
#include <common/translation.h>
#include <common/system.h>
#include <backends/fs/fs-factory.h>
#include <common/debug.h>
#include "handlerutils.h"
#include "localwebserver.h"

namespace Networking {

UploadFileClientHandler::UploadFileClientHandler(Common::String parentDirectoryPath):
	_state(UFH_READING_CONTENT), _headersStream(nullptr), _contentStream(nullptr),
	_parentDirectoryPath(parentDirectoryPath) {}

UploadFileClientHandler::~UploadFileClientHandler() {
	delete _headersStream;
	delete _contentStream;
}

namespace {
void readFromThatUntilLineEnd(const char *cstr, Common::String needle, Common::String &result) {
	const char *position = strstr(cstr, needle.c_str());

	if (position) {
		char c;
		for (const char *i = position + needle.size(); c = *i, c != 0; ++i) {
			if (c == '\n' || c == '\r') break;
			result += c;
		}
	}
}

void readFromThatUntilDoubleQuote(const char *cstr, Common::String needle, Common::String &result) {
	const char *position = strstr(cstr, needle.c_str());

	if (position) {
		char c;
		for (const char *i = position + needle.size(); c = *i, c != 0; ++i) {
			if (c == '"') break;
			result += c;
		}
	}
}

Common::String readEverythingFromMemoryStream(Common::MemoryReadWriteStream *stream) {
	Common::String result;
	char buf[1024];
	uint32 readBytes;
	while (true) {
		readBytes = stream->read(buf, 1024);
		if (readBytes == 0) break;
		result += Common::String(buf, readBytes);
	}
	return result;
}
}

void UploadFileClientHandler::handleBlockHeaders(Client *client) {
	// search for "upload_file" field
	Common::String headers = readEverythingFromMemoryStream(_headersStream);
	Common::String fieldName = "";
	readFromThatUntilDoubleQuote(headers.c_str(), "name=\"", fieldName);
	if (fieldName != "upload_file") return;

	Common::String filename = "";
	readFromThatUntilDoubleQuote(headers.c_str(), "filename=\"", filename);

	// check that <filename> is not empty
	if (filename.empty()) {
		setErrorMessageHandler(*client, _("Invalid filename!"));
		return;
	}

	// check that <path>/<filename> doesn't exist
	Common::String path = _parentDirectoryPath;
	if (path.lastChar() != '/' && path.lastChar() != '\\') path += '/';
	AbstractFSNode *originalNode = g_system->getFilesystemFactory()->makeFileNodePath(path + filename);
	if (originalNode->exists()) {
		setErrorMessageHandler(*client, _("There is a file with that name in the parent directory!"));
		return;
	}

	// create file stream
	_contentStream = originalNode->createWriteStream();
	if (_contentStream == nullptr) {
		setErrorMessageHandler(*client, _("Failed to upload the file!"));
		return;
	}
}

void UploadFileClientHandler::handle(Client *client) {
	if (client == nullptr) {
		warning("UploadFileClientHandler::handle(): empty client pointer");
		return;
	}

	while (true) {
		switch (_state) {
		case UFH_READING_CONTENT:
			if (client->readContent(nullptr)) {
				_state = UFH_READING_BLOCK_HEADERS;
				continue;
			}
			break;

		case UFH_READING_BLOCK_HEADERS:
			if (_headersStream == nullptr)
				_headersStream = new Common::MemoryReadWriteStream(DisposeAfterUse::YES);

			if (client->readBlockHeaders(_headersStream)) {
				handleBlockHeaders(client);
				_state = UFH_READING_BLOCK_CONTENT;
				continue;
			}
			break;

		case UFH_READING_BLOCK_CONTENT:
			if (client->readBlockContent(_contentStream)) {
				if (_contentStream) {
					_contentStream->flush();
					// success - redirect back to directory listing
					HandlerUtils::setMessageHandler(*client,
						Common::String::format(
							"%s<br/><a href=\"files?path=%s\">%s</a>",
							_("Uploaded successfully!"),
							client->queryParameter("path").c_str(),
							_("Back to parent directory")
						),
						"/files?path=" + LocalWebserver::urlEncodeQueryParameterValue(client->queryParameter("path"))
					);
					return;
				}
				_state = UFH_READING_BLOCK_HEADERS;
				continue;
			}
			break;

		case UFH_ERROR:
			return;
		}

		break;
	}
}


void UploadFileClientHandler::setErrorMessageHandler(Client &client, Common::String message) {
	HandlerUtils::setFilesManagerErrorMessageHandler(client, message);
	_state = UFH_ERROR;
}

} // End of namespace Networking
