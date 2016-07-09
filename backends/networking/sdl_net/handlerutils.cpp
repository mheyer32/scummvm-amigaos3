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

#include "backends/networking/sdl_net/handlerutils.h"
#include "backends/networking/sdl_net/localwebserver.h"
#include "common/archive.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/translation.h"
#include "common/unzip.h"

namespace Networking {

#define ARCHIVE_NAME "wwwroot.zip"

#define INDEX_PAGE_NAME ".index.html"

Common::Archive *HandlerUtils::getZipArchive() {
	// first search in themepath
	if (ConfMan.hasKey("themepath")) {
		const Common::FSNode &node = Common::FSNode(ConfMan.get("themepath"));
		if (!node.exists() || !node.isReadable() || !node.isDirectory())
			return nullptr;

		Common::FSNode fileNode = node.getChild(ARCHIVE_NAME);
		if (fileNode.exists() && fileNode.isReadable() && !fileNode.isDirectory()) {
			Common::SeekableReadStream *const stream = fileNode.createReadStream();
			Common::Archive *zipArchive = Common::makeZipArchive(stream);
			if (zipArchive) return zipArchive;
		}
	}

	// then use SearchMan to find it
	Common::ArchiveMemberList fileList;
	SearchMan.listMatchingMembers(fileList, ARCHIVE_NAME);
	for (Common::ArchiveMemberList::iterator it = fileList.begin(); it != fileList.end(); ++it) {
		Common::ArchiveMember       const &m = **it;
		Common::SeekableReadStream *const stream = m.createReadStream();
		Common::Archive *zipArchive = Common::makeZipArchive(stream);
		if (zipArchive) return zipArchive;
	}

	return nullptr;
}

Common::ArchiveMemberList HandlerUtils::listArchive() {
	Common::ArchiveMemberList resultList;
	Common::Archive *zipArchive = getZipArchive();
	if (zipArchive) {
		zipArchive->listMembers(resultList);
		delete zipArchive;
	}
	return resultList;
}

Common::SeekableReadStream *HandlerUtils::getArchiveFile(Common::String name) {
	Common::SeekableReadStream *result = nullptr;
	Common::Archive *zipArchive = getZipArchive();
	if (zipArchive) {
		const Common::ArchiveMemberPtr ptr = zipArchive->getMember(name);
		if (ptr.get() == nullptr) return nullptr;
		result = ptr->createReadStream();
		delete zipArchive;
	}
	return result;
}

Common::String HandlerUtils::readEverythingFromStream(Common::SeekableReadStream *const stream) {
	Common::String result;
	char buf[1024];
	uint32 readBytes;
	while (!stream->eos()) {
		readBytes = stream->read(buf, 1024);
		result += Common::String(buf, readBytes);
	}
	return result;
}

void HandlerUtils::setMessageHandler(Client &client, Common::String message, Common::String redirectTo) {
	Common::String response = "<html><head><title>ScummVM</title></head><body>{message}</body></html>";

	// load stylish response page from the archive
	Common::SeekableReadStream *const stream = getArchiveFile(INDEX_PAGE_NAME);
	if (stream) response = readEverythingFromStream(stream);

	replace(response, "{message}", message);
	if (redirectTo.empty()) LocalWebserver::setClientGetHandler(client, response);
	else LocalWebserver::setClientRedirectHandler(client, response, redirectTo);
}

void HandlerUtils::setFilesManagerErrorMessageHandler(Client &client, Common::String message, Common::String redirectTo) {
	setMessageHandler(client,
		Common::String::format(
			"%s<br/><a href=\"files?path=%s\">%s</a>",
			message.c_str(),
			"%2F", //that's encoded "/"
			_("Back to the files manager")
		), redirectTo
	);
}

} // End of namespace Networking
