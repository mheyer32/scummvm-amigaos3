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

#include "common/rect.h"
#include "common/util.h"
#include "scumm/he/intern_he.h"
#include "scumm/he/moonbase/ai_defenseunit.h"
#include "scumm/he/moonbase/ai_main.h"

namespace Scumm {

DefenseUnit::DefenseUnit() {
	m_state = DUS_ON;
}

DefenseUnit::DefenseUnit(DefenseUnit *inUnit) {
	m_ID = inUnit->getID();
	m_pos.x = inUnit->getPosX();
	m_pos.y = inUnit->getPosY();
	m_distanceTo = inUnit->getDistanceTo();
	m_state = inUnit->getState();
	m_radius = inUnit->getRadius();
	m_armor = inUnit->getArmor();
}

DefenseUnit::~DefenseUnit() {
}

Common::Point *AntiAirUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	float ratio;
	int radius;
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CRAWLER:
		radius = getRadius();

		if ((distance < radius) || (getState() == DUS_OFF)) {
			targetPos->x = getPosX();
			targetPos->y = getPosY();
		} else {
			ratio = MAX(0, (getRadius() / distance));
			targetPos->x = getPosX() - ratio * (getPosX() - sourceX);
			targetPos->y = getPosY() - ratio * (getPosY() - sourceY);
		}

		break;

	case ITEM_EMP:
		if (getRadius() + 215 > distance) { //emp radius
			double x1 = static_cast<double>(sourceX);
			double y1 = static_cast<double>(sourceY);
			double x2 = static_cast<double>(getPosX());
			double y2 = static_cast<double>(getPosY());
			double r1 = static_cast<double>(215);
			double r2 = static_cast<double>(getRadius() + 3);
			double d = static_cast<double>(distance);

			//formulae for calculating one point of intersection of two circles
			float root = sqrt((((r1 + r2) * (r1 + r2)) - (d * d)) * ((d * d) - ((r2 - r1) * (r2 - r1))));
			int x = ((x1 + x2) / 2) + ((x2 - x1) * (r1 * r1 - r2 * r2)) / (2 * d * d) + ((y2 - y1) / (2 * d * d)) * root;
			int y = ((y1 + y2) / 2) + ((y2 - y1) * (r1 * r1 - r2 * r2)) / (2 * d * d) - ((x2 - x1) / (2 * d * d)) * root;

			targetPos->x = x;
			targetPos->y = y;
		} else {
			ratio = 1 - (getRadius() / static_cast<float>(distance - 20));
			targetPos->x = sourceX + ratio * (getPosX() - sourceX);
			targetPos->y = sourceY + ratio * (getPosY() - sourceY);
		}

		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int AntiAirUnit::selectWeapon(int index) {
	switch (index) {
	case 0:
		return ITEM_CLUSTER;
		break;

	case 1:
		return ITEM_EMP;
		break;

	case 2:
		if (getState() == DUS_OFF) {
			if (getPlayerEnergy() > 6) {
				if (!_vm->_rnd.getRandomNumber(3)) {
					return ITEM_VIRUS;
				}
			}

			if (getPlayerEnergy() > 2) {
				if (!_vm->_rnd.getRandomNumber(1)) {
					return ITEM_SPIKE;
				}
			}

			return ITEM_BOMB;
		}

		return ITEM_CLUSTER;
		break;

	default:
		return ITEM_CLUSTER;
		break;
	}
}

Common::Point *ShieldUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	float ratio;
	Common::Point *targetPos = new Common::Point;

	if (getState() == DUS_OFF) {
		targetPos->x = getPosX();
		targetPos->y = getPosY();
	} else {
		switch (weaponType) {
		case ITEM_BOMB:
			targetPos->x = getPosX();
			targetPos->y = getPosY();
			break;

		case ITEM_CLUSTER:
			targetPos->x = getPosX();
			targetPos->y = getPosY();
			break;

		case ITEM_CRAWLER:
			ratio = MAX(0.0, 1.0 - (static_cast<float>(getRadius()) / static_cast<float>(distance - 20)));
			{
				int maxX = getMaxX();
				int maxY = getMaxY();
				int thisX = (static_cast<int>(sourceX + ratio * (getPosX() - sourceX)) + maxX) % maxX;
				int thisY = (static_cast<int>(sourceY + ratio * (getPosY() - sourceY)) + maxY) % maxY;
				targetPos->x = thisX;
				targetPos->y = thisY;
			}
			break;

		case ITEM_EMP:
			if (getRadius() + 215 > distance) { //emp radius
				double x1 = static_cast<double>(sourceX);
				double y1 = static_cast<double>(sourceY);
				double x2 = static_cast<double>(getPosX());
				double y2 = static_cast<double>(getPosY());
				double r1 = static_cast<double>(215);
				double r2 = static_cast<double>(getRadius() + 10);
				double d = static_cast<double>(distance);

				//formulae for calculating one point of intersection of two circles
				float root = sqrt((((r1 + r2) * (r1 + r2)) - (d * d)) * ((d * d) - ((r2 - r1) * (r2 - r1))));
				int x = ((x1 + x2) / 2) + ((x2 - x1) * (r1 * r1 - r2 * r2)) / (2 * d * d) + ((y2 - y1) / (2 * d * d)) * root;
				int y = ((y1 + y2) / 2) + ((y2 - y1) * (r1 * r1 - r2 * r2)) / (2 * d * d) - ((x2 - x1) / (2 * d * d)) * root;

				targetPos->x = x;
				targetPos->y = y;
			} else {
				ratio = 1 - (getRadius() / static_cast<float>(distance - 20));
				targetPos->x = sourceX + ratio * (getPosX() - sourceX);
				targetPos->y = sourceY + ratio * (getPosY() - sourceY);
			}

			if (distance < getRadius()) {
				targetPos->x = getPosX();
				targetPos->y = getPosY();
			}

			break;

		default:
			targetPos->x = getPosX();
			targetPos->y = getPosY();
			break;
		}
	}

	return targetPos;
}

int ShieldUnit::selectWeapon(int index) {
	warning("Shield weapon select");

	int myUnit = getClosestUnit(getPosX(), getPosY(), getMaxX(), getCurrentPlayer(), 1, BUILDING_MAIN_BASE, 1, 0);
	int dist = getDistance(getPosX(), getPosY(), getHubX(myUnit), getHubY(myUnit));

	if ((dist < (getRadius() - 20)) && (dist > 90)) {
		return ITEM_SPIKE;
	}

	switch (index) {
	case 0:
		if (getState() == DUS_OFF)  {
			if (getPlayerEnergy() < 3) {
				return ITEM_BOMB;
			} else {
				return ITEM_SPIKE;
			}
		}

		return ITEM_EMP;
		break;

	case 1:
		if (dist < getRadius() + 150) {
			return ITEM_EMP;
		} else {
			return ITEM_CRAWLER;
		}

		break;

	default:
		return ITEM_EMP;
		break;
	}
}

Common::Point *MineUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	float ratio;
	Common::Point *targetPos = new Common::Point;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_EMP:
		ratio = 1 - (getRadius() / static_cast<float>(distance - 20));
		targetPos->x = sourceX + ratio * (getPosX() - sourceX);
		targetPos->y = sourceY + ratio * (getPosY() - sourceY);
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int MineUnit::selectWeapon(int index) {
	int myUnit = getClosestUnit(getPosX(), getPosY(), getMaxX(), getCurrentPlayer(), 1, 0, 0, 0);
	int x = getPosX();
	int y = getPosY();

	int dist = getDistance(x, y, getHubX(myUnit), getHubY(myUnit));

	if ((getState() == DUS_ON) && (dist < 110)) {
		return ITEM_EMP;
	} else {
		return ITEM_BOMB;
	}
}


Common::Point *HubUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CRAWLER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int HubUnit::selectWeapon(int index) {
	warning("Hub weapon select");

	int energy = getPlayerEnergy();

	if (energy > 6) {
		//possibly choose crawler
		if (getBuildingWorth(getID()) > 21) {
			return ITEM_CRAWLER;
		}
	}

	//choose betw/ bomb and cluster
	if (getBuildingArmor(getID()) < 1.5) {
		return ITEM_CLUSTER;
	}

	if (energy > 2) {
		if (!_vm->_rnd.getRandomNumber(3)) {
			return ITEM_SPIKE;
		}

		if (!_vm->_rnd.getRandomNumber(4)) {
			return ITEM_GUIDED;
		}

		if (!_vm->_rnd.getRandomNumber(4)) {
			return ITEM_MINE;
		}

		if (!_vm->_rnd.getRandomNumber(9)) {
			return ITEM_EMP;
		}
	}

	return ITEM_BOMB;
}


Common::Point *TowerUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_SPIKE:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int TowerUnit::selectWeapon(int index) {
	switch (index) {
	case 0:
		return ITEM_SPIKE;
		break;

	default:
		return ITEM_SPIKE;
		break;
	}
}


Common::Point *BridgeUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int BridgeUnit::selectWeapon(int index) {
	switch (index) {
	case 0:
		return ITEM_BOMB;
		break;

	case 1:
		return ITEM_CLUSTER;
		break;

	default:
		return ITEM_BOMB;
		break;
	}
}


Common::Point *EnergyUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CRAWLER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int EnergyUnit::selectWeapon(int index) {
	warning("Energy weapon select");

	int energy = getPlayerEnergy();

	if (energy > 6) {
		//possibly choose crawler
		if (getBuildingWorth(getID()) > 21) {
			return ITEM_CRAWLER;
		}
	}

	//choose betw/ bomb and cluster
	if (getBuildingArmor(getID()) < 1.5) {
		return ITEM_CLUSTER;
	}

	if (energy > 2) {
		if (!_vm->_rnd.getRandomNumber(3)) {
			return ITEM_EMP;
		}
	}

	return ITEM_BOMB;
}

Common::Point *OffenseUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance) distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CRAWLER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int OffenseUnit::selectWeapon(int index) {
	warning("Offense weapon select");

	int energy = getPlayerEnergy();

	if (energy > 6) {
		//possibly choose crawler
		if (getBuildingWorth(getID()) > 21) {
			return ITEM_CRAWLER;
		}
	}

	//choose betw/ bomb and cluster
	if (getBuildingArmor(getID()) < 1.5) {
		return ITEM_CLUSTER;
	}

	return ITEM_BOMB;
}

Common::Point *CrawlerUnit::createTargetPos(int index, int distance, int weaponType, int sourceX, int sourceY) {
	Common::Point *targetPos = new Common::Point;

	if (!distance)
		distance = 1;

	switch (weaponType) {
	case ITEM_BOMB:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CLUSTER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	case ITEM_CRAWLER:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;

	default:
		targetPos->x = getPosX();
		targetPos->y = getPosY();
		break;
	}

	return targetPos;
}

int CrawlerUnit::selectWeapon(int index) {
	warning("Crawler weapon select");
	int myUnit = getClosestUnit(getPosX(), getPosY(), getMaxX(), getCurrentPlayer(), 1, 0, 0, 0);
	int dist = getDistance(getHubX(myUnit), getHubY(myUnit), getPosX(), getPosY());

	int x = getPosX();
	int y = getPosY();
	int energy = getPlayerEnergy();
	int terrain = getTerrain(x, y);

	if (terrain != TERRAIN_TYPE_WATER) {
		if ((energy > 2) && (dist < 220)) {
			return ITEM_RECLAIMER;
		} else {
			return ITEM_BOMB;
		}
	} else {
		if (energy > 6) {
			return ITEM_CRAWLER;
		}

		if (energy > 2) {
			if (_vm->_rnd.getRandomNumber(1)) {
				return ITEM_MINE;
			} else {
				return ITEM_TIME_EXPIRED;
			}
		}
	}

	return SKIP_TURN;
}

AntiAirUnit::AntiAirUnit() {
	setRadius(190);
	setArmor(3);
	setCost(1);
}

ShieldUnit::ShieldUnit() {
	setRadius(170);
	setArmor(3);
	setCost(7);
}

MineUnit::MineUnit() {
	setRadius(80);
	setArmor(1);
	setCost(3);
}

HubUnit::HubUnit() {
	setRadius(1);
	setArmor(5);
	setCost(7);
}

TowerUnit::TowerUnit() {
	setRadius(1);
	setArmor(3);
	setCost(1);
}

BridgeUnit::BridgeUnit() {
	setRadius(1);
	setArmor(3);
	setCost(1);
}

EnergyUnit::EnergyUnit() {
	setRadius(1);
	setArmor(5);
	setCost(7);
}

OffenseUnit::OffenseUnit() {
	setRadius(1);
	setArmor(3);
	setCost(7);
}

CrawlerUnit::CrawlerUnit() {
	setRadius(1);
	setArmor(3);
	setCost(7);
}

AntiAirUnit::AntiAirUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());

}

ShieldUnit::ShieldUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

MineUnit::MineUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

HubUnit::HubUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

TowerUnit::TowerUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

BridgeUnit::BridgeUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

EnergyUnit::EnergyUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

OffenseUnit::OffenseUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

CrawlerUnit::CrawlerUnit(DefenseUnit *inUnit) : DefenseUnit(inUnit) {
	setID(inUnit->getID());
	setPos(inUnit->getPosX(), inUnit->getPosY());
	setDistanceTo(inUnit->getDistanceTo());
	setState(inUnit->getState());
	setRadius(inUnit->getRadius());
	setArmor(inUnit->getArmor());
}

} // End of namespace Scumm
