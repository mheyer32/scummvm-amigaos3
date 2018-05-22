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

#include "common/debug.h"

#include "pink/objects/actors/actor.h"
#include "pink/objects/pages/game_page.h"
#include "pink/objects/sequences/sequence.h"
#include "pink/objects/sequences/sequence_context.h"
#include "pink/objects/sequences/sequence_item.h"
#include "pink/objects/sequences/sequencer.h"

namespace Pink {

SequenceActorState::SequenceActorState(const Common::String &name)
		:_actorName(name), _index(0)
{}

const Common::String &SequenceActorState::getActor() const {
	return _actorName;
}

void SequenceActorState::check(int index, Sequence *sequence, bool unk) {
	Actor *actor = sequence->_sequencer->_page->findActor(_actorName);
	debug("%s %s", _actorName.c_str(), _actionName.c_str());
	if (_index != index && !_actionName.empty()){
		Action *action = actor->findAction(_actionName);
		if (actor->getAction() != action)
			actor->setAction(action, unk);
	}
}

SequenceContext::SequenceContext(Sequence *sequence, Sequencer *sequencer)
		: _sequence(sequence), _sequencer(sequencer),
		  _nextItemIndex(0), _index(1), _actor(nullptr)
{
	sequence->setContext(this);
	Common::Array<SequenceItem*> &items = sequence->getItems();
	debug("SequenceContext for %s", _sequence->getName().c_str());

	for (uint i = 0; i < items.size(); ++i) {
		bool found = 0;
		for (uint j = 0; j < _states.size(); ++j) {
			if (items[i]->getActor() == _states[j].getActor()){
				found = 1;
				break;
			}
		}
		if (!found) {
			debug(items[i]->getActor().c_str());
			_states.push_back(SequenceActorState(items[i]->getActor()));
		}
	}
}

uint SequenceContext::getNextItemIndex() const {
	return _nextItemIndex;
}

Sequence *SequenceContext::getSequence() const {
	return _sequence;
}

void SequenceContext::clearActionsFromActorStates() {
	for (uint i = 0; i < _states.size(); ++i) {
		_states[i]._actionName.clear();
	}
}

void SequenceContext::setNextItemIndex(int index) {
	_nextItemIndex = index;
}

} // End of namespace Pink
