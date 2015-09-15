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

#ifndef BLADERUNNER_ITEMS_H
#define BLADERUNNER_ITEMS_H

#include "bladerunner/bladerunner.h"
#include "bladerunner/item.h"

#include "common/array.h"
#include "common/rect.h"

namespace BladeRunner {	

	class Items {
		BladeRunnerEngine *_vm;
	private:
		Common::Array<Item> _items;

	public:
		Items(BladeRunnerEngine *vm);
		~Items();
		
		void getXyz(int itemId, float *x, float *y, float *z);

	private:
		int findItem(int itemId);
	};
}

#endif
