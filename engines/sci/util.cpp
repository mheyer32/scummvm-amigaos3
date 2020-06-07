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

#include "sci/util.h"
#include "sci/sci.h"

namespace Sci {

uint16 REGPARM (*READ_SCIENDIAN_UINT16)(const void *ptr) = NULL;
void REGPARM (*WRITE_SCIENDIAN_UINT16)(void *ptr, uint16 val) = NULL;
uint16 REGPARM (*READ_SCI11ENDIAN_UINT16)(const void *ptr) = NULL;
uint32 REGPARM (*READ_SCI11ENDIAN_UINT32)(const void *ptr) = NULL;
void REGPARM (*WRITE_SCI11ENDIAN_UINT16)(void *ptr, uint16 val) = NULL;
uint16 REGPARM (*READ_SCI32ENDIAN_UINT16)(const void *ptr) = NULL;
void REGPARM (*WRITE_SCI11ENDIAN_UINT32)(void *ptr, uint32 val) = NULL;

namespace {
uint16 REGPARM SCI_READ_LE_UINT16(const void *ptr) {
	return READ_LE_UINT16(ptr);
}

uint16 REGPARM SCI_READ_BE_UINT16(const void *ptr) {
	return READ_BE_UINT16(ptr);
}

uint32 REGPARM SCI_READ_LE_UINT32(const void *ptr) {
	return READ_LE_UINT32(ptr);
}

uint32 REGPARM SCI_READ_BE_UINT32(const void *ptr) {
	return READ_BE_UINT32(ptr);
}

void REGPARM SCI_WRITE_LE_UINT16(void *ptr, uint16 value) {
	WRITE_LE_UINT16(ptr, value);
}

void REGPARM SCI_WRITE_BE_UINT16(void *ptr, uint16 value) {
	WRITE_BE_UINT16(ptr, value);
}

void REGPARM SCI_WRITE_LE_UINT32(void *ptr, uint32 value) {
	WRITE_LE_UINT32(ptr, value);
}

void REGPARM SCI_WRITE_BE_UINT32(void *ptr, uint32 value) {
	WRITE_BE_UINT32(ptr, value);
}

}  // namespace

void InitSciReadWriteFunctions() {
	if (g_sci->isBE())
		READ_SCIENDIAN_UINT16 = SCI_READ_BE_UINT16;
	else
		READ_SCIENDIAN_UINT16 = SCI_READ_LE_UINT16;

	if (g_sci->isBE())
		WRITE_SCIENDIAN_UINT16 = SCI_WRITE_BE_UINT16;
	else
		WRITE_SCIENDIAN_UINT16 = SCI_WRITE_LE_UINT16;

	if (g_sci->getPlatform() == Common::kPlatformMacintosh && getSciVersion() >= SCI_VERSION_1_1)
		READ_SCI11ENDIAN_UINT16 = SCI_READ_BE_UINT16;
	else
		READ_SCI11ENDIAN_UINT16 = SCI_READ_LE_UINT16;
#ifdef ENABLE_SCI32
	if (g_sci->getPlatform() == Common::kPlatformMacintosh && getSciVersion() >= SCI_VERSION_2_1_EARLY)
		READ_SCI32ENDIAN_UINT16 = SCI_READ_BE_UINT16;
	else
#endif
		READ_SCI32ENDIAN_UINT16 = SCI_READ_LE_UINT16;

	if (g_sci->getPlatform() == Common::kPlatformMacintosh && getSciVersion() >= SCI_VERSION_1_1)
		READ_SCI11ENDIAN_UINT32 = SCI_READ_BE_UINT32;
	else
		READ_SCI11ENDIAN_UINT32 = SCI_READ_LE_UINT32;

	if (g_sci->getPlatform() == Common::kPlatformMacintosh && getSciVersion() >= SCI_VERSION_1_1)
		WRITE_SCI11ENDIAN_UINT16 = SCI_WRITE_BE_UINT16;
	else
		WRITE_SCI11ENDIAN_UINT16 = SCI_WRITE_LE_UINT16;

	if (g_sci->getPlatform() == Common::kPlatformMacintosh && getSciVersion() >= SCI_VERSION_1_1)
		WRITE_SCI11ENDIAN_UINT32 = SCI_WRITE_BE_UINT32;
	else
		WRITE_SCI11ENDIAN_UINT32 = SCI_WRITE_LE_UINT32;
}
}  // End of namespace Sci
