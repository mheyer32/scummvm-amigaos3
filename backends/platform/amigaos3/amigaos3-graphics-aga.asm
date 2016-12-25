   1               	#NO_APP
   4               		.text
   5               	.Ltext0:
 1608               	.LC0:
 1609 0000 3178 00   		.ascii "1x\0"
 1610               	.LC1:
 1611 0003 4E6F 726D 		.ascii "Normal\0"
 1611      616C 00
 1612 000a 0000      		.align	2
 1613               	__ZL24s_supportedGraphicsModes:
 1614 000c 0000 0000 		.long	.LC0
 1615 0010 0000 0003 		.long	.LC1
 1616 0014 0000 0000 		.long	0
 1617 0018 0000 0000 		.long	0
 1618 001c 0000 0000 		.long	0
 1619 0020 0000 0000 		.long	0
 1621               	.lcomm __ZL12emptypointer,12
 1623               		.align	2
 1624               		.globl	__ZN16OSystem_AmigaOS310hasFeatureEN7OSystem7FeatureE
 1625               	__ZN16OSystem_AmigaOS310hasFeatureEN7OSystem7FeatureE:
 1626               		.stabd	46,0,0
   1:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** /* ScummVM - Graphic Adventure Engine
   2:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  *
   3:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * ScummVM is the legal property of its developers, whose names
   4:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * are too numerous to list here. Please refer to the COPYRIGHT
   5:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * file distributed with this source distribution.
   6:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  *
   7:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * This program is free software; you can redistribute it and/or
   8:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * modify it under the terms of the GNU General Public License
   9:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * as published by the Free Software Foundation; either version 2
  10:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * of the License, or (at your option) any later version.
  11:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  12:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * This program is distributed in the hope that it will be useful,
  13:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  14:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  15:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * GNU General Public License for more details.
  16:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  17:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * You should have received a copy of the GNU General Public License
  18:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * along with this program; if not, write to the Free Software
  19:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
  20:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  *
  21:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * $URL$
  22:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  * $Id$
  23:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  *
  24:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****  */
  25:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  26:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "backends/fs/amigaos3/amigaos3-fs-node.h"
  27:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "backends/fs/fs-factory.h"
  28:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "backends/platform/amigaos3/amigaos3-aga.h"
  29:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "common/config-manager.h"
  30:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "common/debug.h"
  31:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "common/scummsys.h"
  32:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "common/textconsole.h"
  33:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include "graphics/scaler/aspect.h"
  34:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  35:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #include <proto/commodities.h>
  36:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  37:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** static const OSystem::GraphicsMode s_supportedGraphicsModes[] = {{"1x", "Normal", GFX_NORMAL}, {0, 
  38:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  39:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** static UWORD emptypointer[] = {
  40:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****   0x0000, 0x0000, /* reserved, must be NULL */
  41:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****   0x0000, 0x0000, /* 1 row of image data */
  42:backends/platform/amigaos3/amigaos3-graphics-aga.cpp ****   0x0000, 0x0000  /* reserved, must be NULL */
  43:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** };
  44:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  45:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** bool OSystem_AmigaOS3::hasFeature(OSystem::Feature f) {
 1627               		.stabd	68,0,45
 1628               	.LFBB1:
  46:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	/*if (f == OSystem::kFeatureAspectRatioCorrection) {
  47:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			return true;
  48:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}*/
  49:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  50:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (f == OSystem::kFeatureFullscreenMode) {
 1629               		.stabd	68,0,50
 1630 0024 4AAF 0008 		tst.l 8(sp)
 1631 0028 57C0      		seq d0
  51:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return true;
  52:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
  53:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  54:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return false;
  55:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1632               		.stabd	68,0,55
 1633 002a 4400      		neg.b d0
 1634 002c 4E75      		rts
 1638               	.Lscope1:
 1640               		.stabd	78,0,0
 1641 002e 0000      		.align	2
 1642               		.globl	__ZN16OSystem_AmigaOS315setFeatureStateEN7OSystem7FeatureEb
 1643               	__ZN16OSystem_AmigaOS315setFeatureStateEN7OSystem7FeatureEb:
 1644               		.stabd	46,0,0
  56:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  57:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::setFeatureState(OSystem::Feature f, bool enable) {
 1645               		.stabd	68,0,57
 1646               	.LFBB2:
 1647 0030 4E75      		rts
 1652               	.Lscope2:
 1654               		.stabd	78,0,0
 1655 0032 0000      		.align	2
 1656               		.globl	__ZN16OSystem_AmigaOS315getFeatureStateEN7OSystem7FeatureE
 1657               	__ZN16OSystem_AmigaOS315getFeatureStateEN7OSystem7FeatureE:
 1658               		.stabd	46,0,0
  58:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
  59:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::setFeatureState()");
  60:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
  61:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  62:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	/*switch (f) {
  63:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		case OSystem::kFeatureAspectRatioCorrection:
  64:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 					_videoMode.aspectRatioCorrectionRequested = enable;
  65:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			break;
  66:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}*/
  67:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
  68:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  69:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** bool OSystem_AmigaOS3::getFeatureState(OSystem::Feature f) {
 1659               		.stabd	68,0,69
 1660               	.LFBB3:
  70:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
  71:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::getFeatureState()");
  72:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  73:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
  74:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
  75:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  76:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	switch (f) {
  77:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	case OSystem::kFeatureFullscreenMode:
  78:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return true;
  79:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  80:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// case OSystem::kFeatureAspectRatioCorrection:
  81:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		//  return _videoMode.aspectRatioCorrectionRequested;
  82:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  83:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	default:
  84:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 1661               		.stabd	68,0,84
 1662 0034 4AAF 0008 		tst.l 8(sp)
 1663 0038 56C0      		sne d0
  85:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
  86:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1664               		.stabd	68,0,86
 1665 003a 5200      		addq.b #1,d0
 1666 003c 4E75      		rts
 1670               	.Lscope3:
 1672               		.stabd	78,0,0
 1673 003e 0000      		.align	2
 1674               		.globl	__ZNK16OSystem_AmigaOS325getSupportedGraphicsModesEv
 1675               	__ZNK16OSystem_AmigaOS325getSupportedGraphicsModesEv:
 1676               		.stabd	46,0,0
  87:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  88:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** const OSystem::GraphicsMode *OSystem_AmigaOS3::getSupportedGraphicsModes() const {
 1677               		.stabd	68,0,88
 1678               	.LFBB4:
  89:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return s_supportedGraphicsModes;
  90:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1679               		.stabd	68,0,90
 1680 0040 203C 0000 		move.l #__ZL24s_supportedGraphicsModes,d0
 1680      000C 
 1681 0046 4E75      		rts
 1684               	.Lscope4:
 1686               		.stabd	78,0,0
 1687               		.align	2
 1688               		.globl	__ZNK16OSystem_AmigaOS322getDefaultGraphicsModeEv
 1689               	__ZNK16OSystem_AmigaOS322getDefaultGraphicsModeEv:
 1690               		.stabd	46,0,0
  91:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  92:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** int OSystem_AmigaOS3::getDefaultGraphicsMode() const {
 1691               		.stabd	68,0,92
 1692               	.LFBB5:
  93:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return GFX_NORMAL;
  94:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1693               		.stabd	68,0,94
 1694 0048 4280      		clr.l d0
 1695 004a 4E75      		rts
 1698               	.Lscope5:
 1700               		.stabd	78,0,0
 1701               		.align	2
 1702               		.globl	__ZN16OSystem_AmigaOS318resetGraphicsScaleEv
 1703               	__ZN16OSystem_AmigaOS318resetGraphicsScaleEv:
 1704               		.stabd	46,0,0
  95:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
  96:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::resetGraphicsScale() {
 1705               		.stabd	68,0,96
 1706               	.LFBB6:
 1707 004c 206F 0004 		move.l 4(sp),a0
  97:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	setGraphicsMode(GFX_NORMAL);
 1708               		.stabd	68,0,97
 1709 0050 2250      		move.l (a0),a1
 1710 0052 42A7      		clr.l -(sp)
 1711 0054 2F08      		move.l a0,-(sp)
 1712 0056 2069 0028 		move.l 40(a1),a0
 1713 005a 4E90      		jsr (a0)
  98:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1714               		.stabd	68,0,98
 1715 005c 508F      		addq.l #8,sp
 1716 005e 4E75      		rts
 1720               	.Lscope6:
 1722               		.stabd	78,0,0
 1723               		.align	2
 1724               		.globl	__ZN16OSystem_AmigaOS319beginGFXTransactionEv
 1725               	__ZN16OSystem_AmigaOS319beginGFXTransactionEv:
 1726               		.stabd	46,0,0
  99:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 100:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::beginGFXTransaction() {
 1727               		.stabd	68,0,100
 1728               	.LFBB7:
 1729 0060 206F 0004 		move.l 4(sp),a0
 101:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 102:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::beginGFXTransaction()");
 103:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 104:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 105:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 106:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 107:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_transactionMode = kTransactionActive;
 1730               		.stabd	68,0,107
 1731 0064 7001      		moveq #1,d0
 1732 0066 2140 021C 		move.l d0,540(a0)
 108:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 109:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_transactionDetails.sizeChanged = false;
 1733               		.stabd	68,0,109
 1734 006a 4228 01D6 		clr.b 470(a0)
 110:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 111:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Store the current mode in the old mode.
 112:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_oldVideoMode = _videoMode;
 1735               		.stabd	68,0,112
 1736 006e 4878 0022 		pea 34.w
 1737 0072 4868 01D8 		pea 472(a0)
 1738 0076 4868 01FA 		pea 506(a0)
 1739 007a 4EB9 0000 		jsr _memcpy
 1739      0000 
 113:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1740               		.stabd	68,0,113
 1741 0080 4FEF 000C 		lea (12,sp),sp
 1742 0084 4E75      		rts
 1746               	.Lscope7:
 1748               		.stabd	78,0,0
 1749               	.LC2:
 1750 0086 436F 756C 		.ascii "Could not switch to video mode (%d x %d)\0"
 1750      6420 6E6F 
 1750      7420 7377 
 1750      6974 6368 
 1750      2074 6F20 
 1751 00af 00        		.align	2
 1752               		.globl	__ZN16OSystem_AmigaOS317endGFXTransactionEv
 1753               	__ZN16OSystem_AmigaOS317endGFXTransactionEv:
 1754               		.stabd	46,0,0
 114:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 115:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** OSystem::TransactionError OSystem_AmigaOS3::endGFXTransaction() {
 1755               		.stabd	68,0,115
 1756               	.LFBB8:
 1757 00b0 48E7 3822 		movem.l a6/a2/d4/d3/d2,-(sp)
 1758 00b4 246F 0018 		move.l 24(sp),a2
 116:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 117:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::endGFXTransaction()");
 118:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 119:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode != kTransactionNone);
 120:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 121:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 122:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	int errors = OSystem::kTransactionSuccess;
 123:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 124:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_transactionMode == kTransactionRollback) {
 1759               		.stabd	68,0,124
 1760 00b8 7002      		moveq #2,d0
 1761 00ba B0AA 021C 		cmp.l 540(a2),d0
 1762 00be 6600 00EE 		jne .L18
 125:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		errors |= OSystem::kTransactionSizeChangeFailed;
 126:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 127:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Revert to the last (working) mode.
 128:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.screenWidth = _oldVideoMode.screenWidth;
 1763               		.stabd	68,0,128
 1764 00c2 256A 0200 		move.l 512(a2),478(a2)
 1764      01DE 
 129:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.screenHeight = _oldVideoMode.screenHeight;
 1765               		.stabd	68,0,129
 1766 00c8 256A 0204 		move.l 516(a2),482(a2)
 1766      01E2 
 130:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 131:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Stop an endless loop if loadGFXMode() also fails with the old mode.
 132:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_oldVideoMode.setup = false;
 1767               		.stabd	68,0,132
 1768 00ce 422A 01FA 		clr.b 506(a2)
 125:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 1769               		.stabd	68,0,125
 1770 00d2 7808      		moveq #8,d4
 1771               	.L12:
 1772               	.LBB91:
 133:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 134:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 135:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_transactionDetails.sizeChanged) {
 1773               		.stabd	68,0,135
 1774 00d4 4A2A 01D6 		tst.b 470(a2)
 1775 00d8 6700 00C8 		jeq .L13
 1776               	.LBB92:
 136:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		unloadGFXMode();
 1777               		.stabd	68,0,136
 1778 00dc 2F0A      		move.l a2,-(sp)
 1779 00de 4EBA 055C 		jsr __ZN16OSystem_AmigaOS313unloadGFXModeEv
 137:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (!loadGFXMode()) {
 1780               		.stabd	68,0,137
 1781 00e2 2F0A      		move.l a2,-(sp)
 1782 00e4 4EBA 013A 		jsr __ZN16OSystem_AmigaOS311loadGFXModeEv
 1783 00e8 508F      		addq.l #8,sp
 1784 00ea 4A00      		tst.b d0
 1785 00ec 661C      		jne .L14
 138:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			if (_oldVideoMode.setup) {
 1786               		.stabd	68,0,138
 1787 00ee 4A2A 01FA 		tst.b 506(a2)
 1788 00f2 6700 00C0 		jeq .L15
 139:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				_transactionMode = kTransactionRollback;
 1789               		.stabd	68,0,139
 1790 00f6 7002      		moveq #2,d0
 1791 00f8 2540 021C 		move.l d0,540(a2)
 140:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				errors |= endGFXTransaction();
 1792               		.stabd	68,0,140
 1793 00fc 2052      		move.l (a2),a0
 1794 00fe 2F0A      		move.l a2,-(sp)
 1795 0100 2068 0040 		move.l 64(a0),a0
 1796 0104 4E90      		jsr (a0)
 1797 0106 8880      		or.l d0,d4
 1798 0108 588F      		addq.l #4,sp
 1799               	.L14:
 1800               	.LBB93:
 1801               	.LBB94:
 141:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			} else {
 142:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				error("Could not switch to video mode (%d x %d)", _videoMode.screenWidth, _videoMode.screenHeig
 143:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			}
 144:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 145:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 146:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Success.
 147:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		SetPointer(_hardwareWindow, emptypointer, 1, 16, 0, 0);
 1802               		.stabd	68,0,147
 1803 010a 2C79 0000 		move.l _IntuitionBase,a6
 1803      0000 
 1804 0110 206A 0036 		move.l 54(a2),a0
 1805 0114 43F9 0000 		lea __ZL12emptypointer,a1
 1805      0000 
 1806 011a 7001      		moveq #1,d0
 1807 011c 7210      		moveq #16,d1
 1808 011e 4242      		clr.w d2
 1809 0120 4243      		clr.w d3
 1810               	#APP
 1811 0122 4EAE FEF2 	| 147 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 1812               		jsr a6@(-0x10e:W)
 1813               	| 0 "" 2
 148:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 149:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Set current cursor position.
 150:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_mouseCursor.x = _hardwareWindow->MouseX;
 1814               	#NO_APP
 1815 0126 206A 0036 	.LBE94:
 1816 012a 3268 000E 	.LBE93:
 1817 012e 2549 023E 		.stabd	68,0,150
 151:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_mouseCursor.y = _hardwareWindow->MouseY;
 1818               		move.l 54(a2),a0
 1819 0132 3068 000C 		move.w 14(a0),a1
 1820 0136 2548 0242 		move.l a1,574(a2)
 152:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 153:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (!_overlayPalette) {
 1821               		.stabd	68,0,151
 1822 013a 4AAA 028E 		move.w 12(a0),a0
 1823 013e 661C      		move.l a0,578(a2)
 154:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			_overlayPalette = (byte *)calloc(3 * 256, sizeof(byte));
 1824               		.stabd	68,0,153
 1825 0140 4878 0001 		tst.l 654(a2)
 1826 0144 4878 0300 		jne .L16
 1827 0148 4EB9 0000 		.stabd	68,0,154
 1827      0000 
 1828 014e 2540 028E 		pea 1.w
 155:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			loadOverlayPalette();
 1829               		pea 768.w
 1830 0152 2F0A      		jsr _calloc
 1831 0154 4EBA 0946 		move.l d0,654(a2)
 1832 0158 4FEF 000C 		.stabd	68,0,155
 1833               		move.l a2,-(sp)
 156:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 157:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 158:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		setPalette((byte *)_overlayPalette, 0, 256);
 1834               		jsr __ZN16OSystem_AmigaOS318loadOverlayPaletteEv
 1835 015c 2052      		lea (12,sp),sp
 1836 015e 4878 0100 	.L16:
 1837 0162 42A7      		.stabd	68,0,158
 1838 0164 2F2A 028E 		move.l (a2),a0
 1839 0168 2F0A      		pea 256.w
 1840 016a 2068 00F4 		clr.l -(sp)
 1841 016e 4E90      		move.l 654(a2),-(sp)
 159:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 160:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (!_overlayColorMap) {
 1842               		move.l a2,-(sp)
 1843 0170 4FEF 0010 		move.l 244(a0),a0
 1844 0174 4AAA 01D2 		jsr (a0)
 1845 0178 661E      		.stabd	68,0,160
 161:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			_overlayColorMap = (byte *)calloc(65536, sizeof(byte));
 1846               		lea (16,sp),sp
 1847 017a 4878 0001 		tst.l 466(a2)
 1848 017e 2F3C 0001 		jne .L17
 1848      0000 
 1849 0184 4EB9 0000 		.stabd	68,0,161
 1849      0000 
 1850 018a 2540 01D2 		pea 1.w
 162:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			loadOverlayColorMap();
 1851               		move.l #65536,-(sp)
 1852 018e 2F0A      		jsr _calloc
 1853 0190 4EBA 09EA 		move.l d0,466(a2)
 1854 0194 4FEF 000C 		.stabd	68,0,162
 1855               		move.l a2,-(sp)
 163:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 164:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 165:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.setup = true;
 1856               		jsr __ZN16OSystem_AmigaOS319loadOverlayColorMapEv
 1857 0198 157C 0001 		lea (12,sp),sp
 1857      01D8 
 166:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 167:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_screenChangeCount++;
 1858               	.L17:
 1859 019e 52AA 0220 		.stabd	68,0,165
 1860               		move.b #1,472(a2)
 1861               		.stabd	68,0,167
 1862               		addq.l #1,544(a2)
 168:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 169:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 170:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_transactionMode = kTransactionNone;
 1863               	.L13:
 1864 01a2 42AA 021C 	.LBE92:
 171:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 172:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return (OSystem::TransactionError)errors;
 173:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1865               	.LBE91:
 1866 01a6 2004      		.stabd	68,0,170
 1867 01a8 4CDF 441C 		clr.l 540(a2)
 1868 01ac 4E75      		.stabd	68,0,173
 1869               		move.l d4,d0
 122:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 1870               		movem.l (sp)+,d2/d3/d4/a2/a6
 1871 01ae 4284      		rts
 1872 01b0 6000 FF22 	.L18:
 1873               		.stabd	68,0,122
 1874               		clr.l d4
 1875               		jra .L12
 142:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			}
 1876               	.L15:
 1877 01b4 2F2A 01E2 	.LBB96:
 1878 01b8 2F2A 01DE 	.LBB95:
 1879 01bc 487A FEC8 		.stabd	68,0,142
 1880 01c0 4EB9 0000 		move.l 482(a2),-(sp)
 1880      0000 
 1881               		move.l 478(a2),-(sp)
 1882               		pea .LC2
 1883               		jsr __Z5errorPKcz
 1884               	.LBE95:
 1885               	.LBE96:
 1905 01c6 0000      	.Lscope8:
 1907               		.stabd	78,0,0
 1908               		.align	2
 174:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 175:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** bool OSystem_AmigaOS3::setGraphicsMode(int mode) {
 1909               		.globl	__ZN16OSystem_AmigaOS315setGraphicsModeEi
 1910               	__ZN16OSystem_AmigaOS315setGraphicsModeEi:
 1911 01c8 206F 0004 		.stabd	46,0,0
 1912 01cc 222F 0008 		.stabd	68,0,175
 176:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 177:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::setGraphicsMode(%d)", mode);
 178:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 179:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 180:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionActive);
 181:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 182:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_oldVideoMode.setup && _oldVideoMode.mode == mode) {
 1913               	.LFBB9:
 1914 01d0 1028 01FA 		move.l 4(sp),a0
 1915 01d4 6706      		move.l 8(sp),d1
 1916 01d6 B2A8 01FC 		.stabd	68,0,182
 1917 01da 670E      		move.b 506(a0),d0
 1918               		jeq .L20
 183:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return true;
 184:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 185:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 186:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (mode != GFX_NORMAL) {
 1919               		cmp.l 508(a0),d1
 1920 01dc 4A81      		jeq .L19
 1921 01de 6608      	.L20:
 187:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		warning("Unsupported GFX mode %d", mode);
 188:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 189:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 190:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 191:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.mode = mode;
 1922               		.stabd	68,0,186
 1923 01e0 42A8 01DA 		tst.l d1
 192:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 193:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return true;
 1924               		jne .L22
 1925 01e4 7001      		.stabd	68,0,191
 1926 01e6 4E75      		clr.l 474(a0)
 1927               		.stabd	68,0,193
 188:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 1928               		moveq #1,d0
 1929 01e8 4200      		rts
 1930               	.L22:
 194:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1931               		.stabd	68,0,188
 1932 01ea 4E75      		clr.b d0
 1933               	.L19:
 1934               		.stabd	68,0,194
 1935               		rts
 1941               	.Lscope9:
 1943               		.stabd	78,0,0
 1944               		.align	2
 195:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 196:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** int OSystem_AmigaOS3::getGraphicsMode() const {
 1945               		.globl	__ZNK16OSystem_AmigaOS315getGraphicsModeEv
 1946               	__ZNK16OSystem_AmigaOS315getGraphicsModeEv:
 197:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 198:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 199:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return _videoMode.mode;
 200:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 1947               		.stabd	46,0,0
 1948 01ec 206F 0004 		.stabd	68,0,196
 1949 01f0 2028 01DA 	.LFBB10:
 1950 01f4 4E75      		.stabd	68,0,200
 1951               		move.l 4(sp),a0
 1952               		move.l 474(a0),d0
 1953               		rts
 1957 01f6 0000      	.Lscope10:
 1959               		.stabd	78,0,0
 1960               		.align	2
 201:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 202:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::initSize(uint w, uint h, const Graphics::PixelFormat *format) {
 1961               		.globl	__ZN16OSystem_AmigaOS38initSizeEjjPKN8Graphics11PixelFormatE
 1962               	__ZN16OSystem_AmigaOS38initSizeEjjPKN8Graphics11PixelFormatE:
 1963 01f8 206F 0004 		.stabd	46,0,0
 1964 01fc 222F 0008 		.stabd	68,0,202
 1965 0200 202F 000C 	.LFBB11:
 203:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 204:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::initSize(%d, %d)", w, h);
 205:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 206:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionActive);
 207:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 208:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 209:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Avoid redundant res changes
 210:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	/*if (_videoMode.aspectRatioCorrectionRequested) {
 211:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			if (w == _videoMode.screenWidth && real2Aspect(h) == _videoMode.scaledHeight) {
 212:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			return;
 213:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			}
 214:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {*/
 215:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w == _videoMode.screenWidth && h == _videoMode.screenHeight) {
 1966               		move.l 4(sp),a0
 1967 0204 B2A8 01DE 		move.l 8(sp),d1
 1968 0208 6606      		move.l 12(sp),d0
 1969 020a B0A8 01E2 		.stabd	68,0,215
 1970 020e 670E      		cmp.l 478(a0),d1
 1971               		jne .L28
 216:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 217:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 218:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	//}
 219:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 220:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.screenWidth = w;
 1972               		cmp.l 482(a0),d0
 1973 0210 2141 01DE 		jeq .L27
 221:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.screenHeight = h;
 1974               	.L28:
 1975 0214 2140 01E2 		.stabd	68,0,220
 222:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 223:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_transactionDetails.sizeChanged = true;
 1976               		move.l d1,478(a0)
 1977 0218 117C 0001 		.stabd	68,0,221
 1977      01D6 
 1978               		move.l d0,482(a0)
 1979 021e 4E75      		.stabd	68,0,223
 1980               		move.b #1,470(a0)
 1981               	.L27:
 1982               		rts
 1991               	.Lscope11:
 1993               		.stabd	78,0,0
 1994               		.align	2
 224:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 225:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 226:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** bool OSystem_AmigaOS3::loadGFXMode() {
 1995               		.globl	__ZN16OSystem_AmigaOS311loadGFXModeEv
 1996               	__ZN16OSystem_AmigaOS311loadGFXModeEv:
 1997 0220 4FEF FFF4 		.stabd	46,0,0
 1998 0224 48E7 203E 		.stabd	68,0,226
 1999 0228 246F 0028 	.LFBB12:
 227:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 228:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::loadGFXMode()");
 229:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 230:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "New video mode requested - width = %d, height = %d", _videoMode.screenWidth, _videoMode.
 231:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 232:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 233:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Reset.
 234:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	//_videoMode.aspectRatioCorrectionInUse = false;
 235:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 236:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Only aspect scale 200 pixel heights.
 237:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	/*if (_videoMode.aspectRatioCorrectionRequested && _videoMode.screenHeight == 200) {
 238:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		debug(5, "Aspect ratio correction requested and allowed");
 239:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.aspectRatioCorrectionInUse = true;
 240:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 241:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Adjust screen height.
 242:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				//_videoMode.screenHeight = real2Aspect(_videoMode.screenHeight);
 243:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				_videoMode.scaledHeight = real2Aspect(_videoMode.screenHeight);
 244:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				debug(5, "Aspect corrected video mode - width = %d, height = %d", _videoMode.screenWidth,
 245:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.screenHeight);
 246:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}*/
 247:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 248:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Default overlay size to the same as the (game) screen.
 249:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.overlayScreenHeight = _videoMode.screenHeight;
 2000               		lea (-12,sp),sp
 2001 022c 202A 01E2 		movem.l a6/a5/a4/a3/a2/d2,-(sp)
 250:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 251:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Increase for a bigger overlay.
 252:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_videoMode.overlayScreenHeight < 256) {
 2002               		move.l 40(sp),a2
 2003 0230 0C80 0000 		.stabd	68,0,249
 2003      00FF 
 2004 0236 633C      		move.l 482(a2),d0
 249:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2005               		.stabd	68,0,252
 2006 0238 2540 01E6 		cmp.l #255,d0
 2007               		jls .L31
 253:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_videoMode.overlayScreenHeight = 256;
 254:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 255:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Hack - should come from tooltypes
 256:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// FIXME: what is this whole overlay confuzzle? Why do we have multiple members
 257:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// with the same data?
 258:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.overlayWidth = _videoMode.screenWidth;
 2008               		.stabd	68,0,249
 2009 023c 202A 01DE 		move.l d0,486(a2)
 2010 0240 2540 01EA 	.L32:
 259:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_videoMode.overlayHeight = _videoMode.overlayScreenHeight;
 2011               		.stabd	68,0,258
 2012 0244 222A 01E6 		move.l 478(a2),d0
 2013 0248 2541 01EE 		move.l d0,490(a2)
 260:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 261:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayWidth = _videoMode.overlayWidth;
 2014               		.stabd	68,0,259
 2015 024c 2540 0224 		move.l 486(a2),d1
 262:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayHeight = _videoMode.overlayHeight;
 2016               		move.l d1,494(a2)
 2017 0250 2541 0228 		.stabd	68,0,261
 263:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "Allocating sctreen - width = %d, height = %d", _videoMode.screenWidth, _videoMode.overla
 264:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 265:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the hardware screen.
 266:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_hardwareScreen = createHardwareScreen(_videoMode.screenWidth, _videoMode.overlayScreenHeight);
 2018               		move.l d0,548(a2)
 2019 0254 2F01      		.stabd	68,0,262
 2020 0256 2F00      		move.l d1,552(a2)
 2021 0258 2F0A      		.stabd	68,0,266
 2022 025a 4EBA 01BC 		move.l d1,-(sp)
 2023 025e 2540 0174 		move.l d0,-(sp)
 267:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (!_hardwareScreen) {
 2024               		move.l a2,-(sp)
 2025 0262 4FEF 000C 		jsr __ZN16OSystem_AmigaOS320createHardwareScreenEjj
 2026 0266 6616      		move.l d0,372(a2)
 2027               		.stabd	68,0,267
 268:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 2028               		lea (12,sp),sp
 2029 0268 4200      		jne .L33
 2030               	.L35:
 269:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 270:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 271:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::loadGFXMode() 2");
 272:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 273:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Setup double/triple buffering.
 274:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (unsigned s = 0; s < NUM_SCREENBUFFERS; ++s) {
 275:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_hardwareScreenBuffer[s] = AllocScreenBuffer(_hardwareScreen, NULL, s == 0 ? SB_SCREEN_BITMAP : 0
 276:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (!_hardwareScreenBuffer[s]) {
 277:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// FIXME: free all the stuff that has been allocated!
 278:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			return false;
 279:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 280:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// WriteChunkyPixels wants a RastPort structure, so create one for each buffer
 281:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		InitRastPort(&_screenRastPorts[s]);
 282:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_screenRastPorts[s].BitMap = _hardwareScreenBuffer[s]->sb_BitMap;
 283:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 284:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::loadGFXMode() 3");
 285:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 286:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// 0 is shown, 1 is the new backbuffer
 287:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_currentScreenBuffer = 1;
 288:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 289:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::loadGFXMode() 5");
 290:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 291:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the hardware window.
 292:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_hardwareWindow = createHardwareWindow(_videoMode.screenWidth, _videoMode.overlayScreenHeight, _ha
 293:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (!_hardwareWindow) {
 294:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 295:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 296:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 297:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the surface that contains the 8 bit game data
 298:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_screen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createForma
 299:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 300:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the screen used by the scaler/shaker.
 301:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_tmpscreen.create(_videoMode.screenWidth, _videoMode.screenHeight, Graphics::PixelFormat::createFo
 302:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 303:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the 8bit overlay surface
 304:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayscreen8.create(_videoMode.screenWidth, _videoMode.overlayScreenHeight,
 305:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						   Graphics::PixelFormat::createFormatCLUT8());
 306:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 307:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the 16bit overlay surface
 308:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayscreen16.create(_videoMode.screenWidth, _videoMode.overlayScreenHeight, _overlayFormat);
 309:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 310:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return true;
 311:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2031               		.stabd	68,0,268
 2032 026a 4CDF 7C04 		clr.b d0
 2033 026e 4FEF 000C 	.L30:
 2034 0272 4E75      		.stabd	68,0,311
 2035               		movem.l (sp)+,d2/a2/a3/a4/a5/a6
 253:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 2036               		lea (12,sp),sp
 2037 0274 257C 0000 		rts
 2037      0100 01E6 
 2038 027c 60BE      	.L31:
 2039               		.stabd	68,0,253
 2040 027e 4BEA 003A 		move.l #256,486(a2)
 2041 0282 49EA 004A 		jra .L32
 267:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 2042               	.L33:
 2043 0286 97CB      		lea (58,a2),a5
 2044               		lea (74,a2),a4
 2045               		.stabd	68,0,267
 2046               		sub.l a3,a3
 2047               	.L36:
 275:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (!_hardwareScreenBuffer[s]) {
 2048               	.LBB118:
 2049 0288 200B      	.LBB119:
 2050 028a 57C0      	.LBB120:
 2051 028c 49C0      		.stabd	68,0,275
 2052               		move.l a3,d0
 2053 028e 2C79 0000 		seq d0
 2053      0000 
 2054 0294 206A 0174 		extb.l d0
 2055 0298 93C9      	.LBB121:
 2056 029a 4480      		move.l _IntuitionBase,a6
 2057               		move.l 372(a2),a0
 2058 029c 4EAE FD00 		sub.l a1,a1
 2059 02a0 240D      		neg.l d0
 2060               	#APP
 2061 02a2 2A80      	| 275 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2062               		jsr a6@(-0x300:W)
 276:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// FIXME: free all the stuff that has been allocated!
 2063               	| 0 "" 2
 2064 02a4 67C2      	#NO_APP
 2065               		move.l a5,d2
 2066               	.LBE121:
 281:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_screenRastPorts[s].BitMap = _hardwareScreenBuffer[s]->sb_BitMap;
 2067               		move.l d0,(a5)
 2068 02a6 2C79 0000 	.LBE120:
 2068      0000 
 2069 02ac 43EC FFFC 		.stabd	68,0,276
 2070               		jeq .L35
 2071 02b0 4EAE FF3A 	.LBB122:
 2072 02b4 588D      	.LBB123:
 2073               		.stabd	68,0,281
 2074               		move.l _GfxBase,a6
 282:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 2075               		lea (-4,a4),a1
 2076 02b6 2242      	#APP
 2077 02b8 2051      	| 281 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2078 02ba 2890      		jsr a6@(-0xc6:W)
 2079               	| 0 "" 2
 274:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_hardwareScreenBuffer[s] = AllocScreenBuffer(_hardwareScreen, NULL, s == 0 ? SB_SCREEN_BITMAP : 0
 2080               	#NO_APP
 2081 02bc 528B      		addq.l #4,a5
 2082 02be 49EC 0064 	.LBE123:
 2083 02c2 7003      	.LBE122:
 2084 02c4 B08B      		.stabd	68,0,282
 2085 02c6 66C0      		move.l d2,a1
 2086               		move.l (a1),a0
 287:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2087               		move.l (a0),(a4)
 2088 02c8 157C 0001 	.LBE119:
 2088      0172 
 292:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (!_hardwareWindow) {
 2089               		.stabd	68,0,274
 2090 02ce 2F2A 0174 		addq.l #1,a3
 2091 02d2 2F2A 01E6 		lea (100,a4),a4
 2092 02d6 2F2A 01DE 		moveq #3,d0
 2093 02da 2F0A      		cmp.l a3,d0
 2094 02dc 4EBA 0276 		jne .L36
 2095 02e0 2540 0036 	.LBE118:
 293:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return false;
 2096               		.stabd	68,0,287
 2097 02e4 4FEF 0010 		move.b #1,370(a2)
 2098 02e8 6700 FF7E 		.stabd	68,0,292
 2099               		move.l 372(a2),-(sp)
 2100               		move.l 486(a2),-(sp)
 2101               		move.l 478(a2),-(sp)
 2102               		move.l a2,-(sp)
 2103               		jsr __ZN16OSystem_AmigaOS320createHardwareWindowEjjP6Screen
 2104               		move.l d0,54(a2)
   1:./graphics/pixelformat.h **** /* ScummVM - Graphic Adventure Engine
   2:./graphics/pixelformat.h ****  *
   3:./graphics/pixelformat.h ****  * ScummVM is the legal property of its developers, whose names
   4:./graphics/pixelformat.h ****  * are too numerous to list here. Please refer to the COPYRIGHT
   5:./graphics/pixelformat.h ****  * file distributed with this source distribution.
   6:./graphics/pixelformat.h ****  *
   7:./graphics/pixelformat.h ****  * This program is free software; you can redistribute it and/or
   8:./graphics/pixelformat.h ****  * modify it under the terms of the GNU General Public License
   9:./graphics/pixelformat.h ****  * as published by the Free Software Foundation; either version 2
  10:./graphics/pixelformat.h ****  * of the License, or (at your option) any later version.
  11:./graphics/pixelformat.h ****  *
  12:./graphics/pixelformat.h ****  * This program is distributed in the hope that it will be useful,
  13:./graphics/pixelformat.h ****  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  14:./graphics/pixelformat.h ****  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  15:./graphics/pixelformat.h ****  * GNU General Public License for more details.
  16:./graphics/pixelformat.h ****  *
  17:./graphics/pixelformat.h ****  * You should have received a copy of the GNU General Public License
  18:./graphics/pixelformat.h ****  * along with this program; if not, write to the Free Software
  19:./graphics/pixelformat.h ****  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
  20:./graphics/pixelformat.h ****  *
  21:./graphics/pixelformat.h ****  */
  22:./graphics/pixelformat.h **** 
  23:./graphics/pixelformat.h **** #ifndef GRAPHICS_PIXELFORMAT_H
  24:./graphics/pixelformat.h **** #define GRAPHICS_PIXELFORMAT_H
  25:./graphics/pixelformat.h **** 
  26:./graphics/pixelformat.h **** #include "common/scummsys.h"
  27:./graphics/pixelformat.h **** #include "common/str.h"
  28:./graphics/pixelformat.h **** 
  29:./graphics/pixelformat.h **** namespace Graphics {
  30:./graphics/pixelformat.h **** 
  31:./graphics/pixelformat.h **** /** Template to expand from an n-bit component to an 8-bit component */
  32:./graphics/pixelformat.h **** template<int depth>
  33:./graphics/pixelformat.h **** struct ColorComponent {
  34:./graphics/pixelformat.h **** };
  35:./graphics/pixelformat.h **** 
  36:./graphics/pixelformat.h **** template<>
  37:./graphics/pixelformat.h **** struct ColorComponent<0> {
  38:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  39:./graphics/pixelformat.h **** 		return 0;
  40:./graphics/pixelformat.h **** 	}
  41:./graphics/pixelformat.h **** };
  42:./graphics/pixelformat.h **** 
  43:./graphics/pixelformat.h **** template<>
  44:./graphics/pixelformat.h **** struct ColorComponent<1> {
  45:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  46:./graphics/pixelformat.h **** 		value &= 1;
  47:./graphics/pixelformat.h **** 		return value |
  48:./graphics/pixelformat.h **** 		       (value << 1) |
  49:./graphics/pixelformat.h **** 		       (value << 2) |
  50:./graphics/pixelformat.h **** 		       (value << 3) |
  51:./graphics/pixelformat.h **** 		       (value << 4) |
  52:./graphics/pixelformat.h **** 		       (value << 5) |
  53:./graphics/pixelformat.h **** 		       (value << 6) |
  54:./graphics/pixelformat.h **** 		       (value << 7);
  55:./graphics/pixelformat.h **** 	}
  56:./graphics/pixelformat.h **** };
  57:./graphics/pixelformat.h **** 
  58:./graphics/pixelformat.h **** template<>
  59:./graphics/pixelformat.h **** struct ColorComponent<2> {
  60:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  61:./graphics/pixelformat.h **** 		value &= 3;
  62:./graphics/pixelformat.h **** 		return value |
  63:./graphics/pixelformat.h **** 		       (value << 2) |
  64:./graphics/pixelformat.h **** 		       (value << 4) |
  65:./graphics/pixelformat.h **** 		       (value << 6);
  66:./graphics/pixelformat.h **** 	}
  67:./graphics/pixelformat.h **** };
  68:./graphics/pixelformat.h **** 
  69:./graphics/pixelformat.h **** template<>
  70:./graphics/pixelformat.h **** struct ColorComponent<3> {
  71:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  72:./graphics/pixelformat.h **** 		value &= 7;
  73:./graphics/pixelformat.h **** 		return (value << 5) |
  74:./graphics/pixelformat.h **** 		       (value << 2) |
  75:./graphics/pixelformat.h **** 		       (value >> 1);
  76:./graphics/pixelformat.h **** 	}
  77:./graphics/pixelformat.h **** };
  78:./graphics/pixelformat.h **** 
  79:./graphics/pixelformat.h **** template<>
  80:./graphics/pixelformat.h **** struct ColorComponent<4> {
  81:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  82:./graphics/pixelformat.h **** 		value &= 15;
  83:./graphics/pixelformat.h **** 		return value |
  84:./graphics/pixelformat.h **** 		       (value << 4);
  85:./graphics/pixelformat.h **** 	}
  86:./graphics/pixelformat.h **** };
  87:./graphics/pixelformat.h **** 
  88:./graphics/pixelformat.h **** template<>
  89:./graphics/pixelformat.h **** struct ColorComponent<5> {
  90:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
  91:./graphics/pixelformat.h **** 		value &= 31;
  92:./graphics/pixelformat.h **** 		return (value << 3) |
  93:./graphics/pixelformat.h **** 		       (value >> 2);
  94:./graphics/pixelformat.h **** 	}
  95:./graphics/pixelformat.h **** };
  96:./graphics/pixelformat.h **** 
  97:./graphics/pixelformat.h **** template<>
  98:./graphics/pixelformat.h **** struct ColorComponent<6> {
  99:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
 100:./graphics/pixelformat.h **** 		value &= 63;
 101:./graphics/pixelformat.h **** 		return (value << 2) |
 102:./graphics/pixelformat.h **** 		       (value >> 4);
 103:./graphics/pixelformat.h **** 	}
 104:./graphics/pixelformat.h **** };
 105:./graphics/pixelformat.h **** 
 106:./graphics/pixelformat.h **** template<>
 107:./graphics/pixelformat.h **** struct ColorComponent<7> {
 108:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
 109:./graphics/pixelformat.h **** 		value &= 127;
 110:./graphics/pixelformat.h **** 		return (value << 1) |
 111:./graphics/pixelformat.h **** 		       (value >> 6);
 112:./graphics/pixelformat.h **** 	}
 113:./graphics/pixelformat.h **** };
 114:./graphics/pixelformat.h **** 
 115:./graphics/pixelformat.h **** template<>
 116:./graphics/pixelformat.h **** struct ColorComponent<8> {
 117:./graphics/pixelformat.h **** 	static inline uint expand(uint value) {
 118:./graphics/pixelformat.h **** 		return value & 255;
 119:./graphics/pixelformat.h **** 	}
 120:./graphics/pixelformat.h **** };
 121:./graphics/pixelformat.h **** 
 122:./graphics/pixelformat.h **** /**
 123:./graphics/pixelformat.h ****  * A pixel format description.
 124:./graphics/pixelformat.h ****  *
 125:./graphics/pixelformat.h ****  * Like ColorMasks it includes the given values to create colors from RGB
 126:./graphics/pixelformat.h ****  * values and to retrieve RGB values from colors.
 127:./graphics/pixelformat.h ****  *
 128:./graphics/pixelformat.h ****  * Unlike ColorMasks it is not dependend on knowing the exact pixel format
 129:./graphics/pixelformat.h ****  * on compile time.
 130:./graphics/pixelformat.h ****  *
 131:./graphics/pixelformat.h ****  * A minor difference between ColorMasks and PixelFormat is that ColorMasks
 132:./graphics/pixelformat.h ****  * stores the bit count per channel in 'kFooBits', while PixelFormat stores
 133:./graphics/pixelformat.h ****  * the loss compared to 8 bits per channel in '#Loss'. It also doesn't
 134:./graphics/pixelformat.h ****  * contain mask values.
 135:./graphics/pixelformat.h ****  */
 136:./graphics/pixelformat.h **** struct PixelFormat {
 137:./graphics/pixelformat.h **** 	byte bytesPerPixel; /**< Number of bytes used in the pixel format. */
 138:./graphics/pixelformat.h **** 
 139:./graphics/pixelformat.h **** 	byte rLoss, gLoss, bLoss, aLoss; /**< Precision loss of each color component. */
 140:./graphics/pixelformat.h **** 	byte rShift, gShift, bShift, aShift; /**< Binary left shift of each color component in the pixel v
 141:./graphics/pixelformat.h **** 
 142:./graphics/pixelformat.h **** 	inline PixelFormat() {
 143:./graphics/pixelformat.h **** 		bytesPerPixel =
 144:./graphics/pixelformat.h **** 		rLoss = gLoss = bLoss = aLoss =
 145:./graphics/pixelformat.h **** 		rShift = gShift = bShift = aShift = 0;
 146:./graphics/pixelformat.h **** 	}
 147:./graphics/pixelformat.h **** 
 148:./graphics/pixelformat.h **** 	inline PixelFormat(byte BytesPerPixel,
 149:./graphics/pixelformat.h **** 						byte RBits, byte GBits, byte BBits, byte ABits,
 150:./graphics/pixelformat.h **** 						byte RShift, byte GShift, byte BShift, byte AShift) {
 151:./graphics/pixelformat.h **** 		bytesPerPixel = BytesPerPixel;
 2105               		.stabd	68,0,293
 2106 02ec 1F7C 0001 		lea (16,sp),sp
 2106      001B 
 152:./graphics/pixelformat.h **** 		rLoss = 8 - RBits, gLoss = 8 - GBits, bLoss = 8 - BBits, aLoss = 8 - ABits;
 2107               		jeq .L35
 2108 02f2 1F7C 0008 	.LBB124:
 2108      001C 
 2109 02f8 1F7C 0008 	.LBB125:
 2109      001D 
 2110 02fe 1F7C 0008 	.LBB126:
 2110      001E 
 2111 0304 1F7C 0008 	.LBB127:
 2111      001F 
 153:./graphics/pixelformat.h **** 		rShift = RShift, gShift = GShift, bShift = BShift, aShift = AShift;
 2113 030a 4280      	.Ltext1:
 2114 030c 1F40 0020 		.stabd	68,0,151
 2115 0310 1F40 0021 		move.b #1,27(sp)
 2116 0314 1F40 0022 		.stabd	68,0,152
 2117 0318 1F40 0023 		move.b #8,28(sp)
 2118               		move.b #8,29(sp)
 2119               		move.b #8,30(sp)
 2120               		move.b #8,31(sp)
 2121               		.stabd	68,0,153
 2122               		clr.l d0
 2123               		move.b d0,32(sp)
 298:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2124               		move.b d0,33(sp)
 2125 031c 49EF 001B 		move.b d0,34(sp)
 2126 0320 2F0C      		move.b d0,35(sp)
 2127 0322 202A 01E2 	.LBE127:
 2128 0326 0280 0000 	.LBE126:
 2128      FFFF 
 2129 032c 2F00      	.LBE125:
 2130 032e 202A 01DE 	.LBE124:
 2132 0338 2F00      	.Ltext2:
 2133 033a 486A 0178 		.stabd	68,0,298
 2134 033e 47F9 0000 		lea (27,sp),a4
 2134      0000 
 2135 0344 4E93      		move.l a4,-(sp)
 2136               		move.l 482(a2),d0
 2137               		and.l #65535,d0
 2138               		move.l d0,-(sp)
 2139               		move.l 478(a2),d0
 2140               		and.l #65535,d0
 2141               		move.l d0,-(sp)
 151:./graphics/pixelformat.h **** 		rLoss = 8 - RBits, gLoss = 8 - GBits, bLoss = 8 - BBits, aLoss = 8 - ABits;
 2142               		pea 376(a2)
 2143 0346 1F7C 0001 		lea __ZN8Graphics7Surface6createEttRKNS_11PixelFormatE,a3
 2143      002B 
 152:./graphics/pixelformat.h **** 		rShift = RShift, gShift = GShift, bShift = BShift, aShift = AShift;
 2144               		jsr (a3)
 2145 034c 1F7C 0008 	.LBB128:
 2145      002C 
 2146 0352 1F7C 0008 	.LBB129:
 2146      002D 
 2147 0358 1F7C 0008 	.LBB130:
 2147      002E 
 2148 035e 1F7C 0008 	.LBB131:
 2148      002F 
 2150 0364 4280      	.Ltext3:
 2151 0366 1F40 0030 		.stabd	68,0,151
 2152 036a 1F40 0031 		move.b #1,43(sp)
 2153 036e 1F40 0032 		.stabd	68,0,152
 2154 0372 1F40 0033 		move.b #8,44(sp)
 2155               		move.b #8,45(sp)
 2156               		move.b #8,46(sp)
 2157               		move.b #8,47(sp)
 2158               		.stabd	68,0,153
 2159               		clr.l d0
 2160               		move.b d0,48(sp)
 301:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2161               		move.b d0,49(sp)
 2162 0376 2F0C      		move.b d0,50(sp)
 2163 0378 202A 01E2 		move.b d0,51(sp)
 2164 037c 0280 0000 	.LBE131:
 2164      FFFF 
 2165 0382 2F00      	.LBE130:
 2166 0384 202A 01DE 	.LBE129:
 2167 0388 0280 0000 	.LBE128:
 2167      FFFF 
 2169 0390 486A 018C 	.Ltext4:
 2170 0394 4E93      		.stabd	68,0,301
 2171               		move.l a4,-(sp)
 2172               		move.l 482(a2),d0
 2173               		and.l #65535,d0
 2174               		move.l d0,-(sp)
 2175               		move.l 478(a2),d0
 2176               		and.l #65535,d0
 151:./graphics/pixelformat.h **** 		rLoss = 8 - RBits, gLoss = 8 - GBits, bLoss = 8 - BBits, aLoss = 8 - ABits;
 2177               		move.l d0,-(sp)
 2178 0396 1F7C 0001 		pea 396(a2)
 2178      003B 
 152:./graphics/pixelformat.h **** 		rShift = RShift, gShift = GShift, bShift = BShift, aShift = AShift;
 2179               		jsr (a3)
 2180 039c 1F7C 0008 	.LBB132:
 2180      003C 
 2181 03a2 1F7C 0008 	.LBB133:
 2181      003D 
 2182 03a8 1F7C 0008 	.LBB134:
 2182      003E 
 2183 03ae 1F7C 0008 	.LBB135:
 2183      003F 
 2185 03b4 4280      	.Ltext5:
 2186 03b6 1F40 0040 		.stabd	68,0,151
 2187 03ba 1F40 0041 		move.b #1,59(sp)
 2188 03be 1F40 0042 		.stabd	68,0,152
 2189 03c2 1F40 0043 		move.b #8,60(sp)
 2190               		move.b #8,61(sp)
 2191               		move.b #8,62(sp)
 2192               		move.b #8,63(sp)
 2193               		.stabd	68,0,153
 2194               		clr.l d0
 2195               		move.b d0,64(sp)
 305:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2196               		move.b d0,65(sp)
 2197 03c6 4FEF 001C 		move.b d0,66(sp)
 2198 03ca 2E8C      		move.b d0,67(sp)
 304:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						   Graphics::PixelFormat::createFormatCLUT8());
 2199               	.LBE135:
 2200 03cc 202A 01E6 	.LBE134:
 2201 03d0 0280 0000 	.LBE133:
 2201      FFFF 
 305:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2202               	.LBE132:
 304:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						   Graphics::PixelFormat::createFormatCLUT8());
 2204               	.Ltext6:
 2205 03d8 202A 01DE 		.stabd	68,0,305
 2206 03dc 0280 0000 		lea (28,sp),sp
 2206      FFFF 
 305:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2207               		move.l a4,(sp)
 2208 03e2 2F00      		.stabd	68,0,304
 2209 03e4 486A 01A0 		move.l 486(a2),d0
 2210 03e8 4E93      		and.l #65535,d0
 308:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 2211               		.stabd	68,0,305
 2212 03ea 486A 01C9 		move.l d0,-(sp)
 2213 03ee 202A 01E6 		.stabd	68,0,304
 2214 03f2 0280 0000 		move.l 478(a2),d0
 2214      FFFF 
 2215 03f8 2F00      		and.l #65535,d0
 2216 03fa 202A 01DE 		.stabd	68,0,305
 2217 03fe 0280 0000 		move.l d0,-(sp)
 2217      FFFF 
 2218 0404 2F00      		pea 416(a2)
 2219 0406 486A 01B4 		jsr (a3)
 2220 040a 4E93      		.stabd	68,0,308
 310:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2221               		pea 457(a2)
 2222 040c 4FEF 0020 		move.l 486(a2),d0
 2223 0410 7001      		and.l #65535,d0
 2224 0412 6000 FE56 		move.l d0,-(sp)
 2225               		move.l 478(a2),d0
 2226               		and.l #65535,d0
 2227               		move.l d0,-(sp)
 2228               		pea 436(a2)
 2229               		jsr (a3)
 2230               		.stabd	68,0,310
 2231               		lea (32,sp),sp
 2232               		moveq #1,d0
 2233               		jra .L30
 312:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 313:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** struct Screen *OSystem_AmigaOS3::createHardwareScreen(uint width, uint height) {
 2261 0418 4FEF FFA0 	.Lscope12:
 2263 0420 262F 0080 		.stabd	78,0,0
 2264 0424 242F 0084 		.align	2
 2265               		.globl	__ZN16OSystem_AmigaOS320createHardwareScreenEjj
 314:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Create the hardware screen.
 315:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	struct Screen *screen = NULL;
 316:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ULONG modeId = INVALID_ID;
 317:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 318:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::createHardwareScreen()");
 319:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	modeId = BestModeID(BIDTAG_NominalWidth, width, BIDTAG_NominalHeight, height, BIDTAG_DesiredWidth,
 2266               	__ZN16OSystem_AmigaOS320createHardwareScreenEjj:
 2267 0428 45EF 001C 		.stabd	46,0,0
 2268 042c 4878 0034 		.stabd	68,0,313
 2269 0430 42A7      	.LFBB13:
 2270 0432 2F0A      		lea (-96,sp),sp
 2271 0434 47F9 0000 		movem.l a6/a3/a2/d4/d3/d2,-(sp)
 2271      0000 
 2272 043a 4E93      		move.l 128(sp),d3
 2273 043c 2F7C 8000 		move.l 132(sp),d2
 2273      0004 0028 
 2274 0444 2F7C 8000 	.LBB136:
 2274      0005 0030 
 2275 044c 2F7C 8000 		.stabd	68,0,319
 2275      0006 0038 
 2276 0454 2F7C 8000 		lea (28,sp),a2
 2276      0007 0040 
 2277 045c 2F7C 8000 		pea 52.w
 2277      0008 0048 
 2278 0464 7008      		clr.l -(sp)
 2279 0466 2F40 004C 		move.l a2,-(sp)
 2280 046a 2F7C 8000 		lea _memset,a3
 2280      0009 0050 
 2281 0472 2F7C 0002 		jsr (a3)
 2281      1000 0054 
 2282 047a 2F43 002C 		move.l #-2147483644,40(sp)
 2283 047e 2F42 0034 		move.l #-2147483643,48(sp)
 2284 0482 2F43 003C 		move.l #-2147483642,56(sp)
 2285 0486 2F42 0044 		move.l #-2147483641,64(sp)
 2286               		move.l #-2147483640,72(sp)
 2287               		moveq #8,d0
 2288 048a 2C79 0000 		move.l d0,76(sp)
 2288      0000 
 2289 0490 204A      		move.l #-2147483639,80(sp)
 2290               		move.l #135168,84(sp)
 2291 0492 4EAE FBE6 		move.l d3,44(sp)
 2292 0496 2800      		move.l d2,52(sp)
 2293               		move.l d3,60(sp)
 2294               		move.l d2,68(sp)
 2295               	.LBB137:
 320:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						BIDTAG_DesiredHeight, height, BIDTAG_Depth, AGA_VIDEO_DEPTH, BIDTAG_MonitorID, PAL_MONITOR_ID
 321:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						TAG_END);
 322:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 323:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::createHardwareScreen() mode id %x", modeId);
 324:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 325:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ULONG errorCode = 0;
 2296               	.LBB138:
 2297 0498 42AF 0024 		move.l _GfxBase,a6
 2298               		move.l a2,a0
 326:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 327:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (modeId != INVALID_ID) {
 2299               	#APP
 2300 049c 4FEF 000C 	| 319 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2301 04a0 70FF      		jsr a6@(-0x41a:W)
 2302 04a2 B084      	| 0 "" 2
 2303 04a4 6700 00AA 	#NO_APP
 2304               		move.l d0,d4
 328:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		screen = OpenScreenTags(NULL, SA_Depth, AGA_VIDEO_DEPTH, SA_DisplayID, modeId, SA_Width, width, S
 2305               	.LBE138:
 2306 04a8 4878 005C 	.LBE137:
 2307 04ac 42A7      	.LBE136:
 2308 04ae 2F0A      		.stabd	68,0,325
 2309 04b0 4E93      		clr.l 36(sp)
 2310 04b2 2F7C 8000 	.LBB139:
 2310      0025 0028 
 2311 04ba 7008      		.stabd	68,0,327
 2312 04bc 2F40 002C 		lea (12,sp),sp
 2313 04c0 2F7C 8000 		moveq #-1,d0
 2313      0032 0030 
 2314 04c8 2F7C 8000 		cmp.l d4,d0
 2314      0023 0038 
 2315 04d0 2F7C 8000 		jeq .L46
 2315      0024 0040 
 2316 04d8 2F7C 8000 	.LBB140:
 2316      002D 0048 
 2317 04e0 700F      		.stabd	68,0,328
 2318 04e2 2F40 004C 		pea 92.w
 2319 04e6 2F7C 8000 		clr.l -(sp)
 2319      0038 0050 
 2320 04ee 7001      		move.l a2,-(sp)
 2321 04f0 2F40 0054 		jsr (a3)
 2322 04f4 2F7C 8000 		move.l #-2147483611,40(sp)
 2322      0036 0058 
 2323 04fc 2F7C 8000 		moveq #8,d0
 2323      003E 0060 
 2324 0504 2F7C 8000 		move.l d0,44(sp)
 2324      003F 0068 
 2325 050c 2F40 006C 		move.l #-2147483598,48(sp)
 2326 0510 2F7C 8000 		move.l #-2147483613,56(sp)
 2326      0039 0070 
 2327 0518 2F7C 8000 		move.l #-2147483612,64(sp)
 2327      002A 0078 
 2328 0520 2F44 0034 		move.l #-2147483603,72(sp)
 2329 0524 2F43 003C 		moveq #15,d0
 2330 0528 2F42 0044 		move.l d0,76(sp)
 2331 052c 41EF 0024 		move.l #-2147483592,80(sp)
 2332 0530 2F48 007C 		moveq #1,d0
 2333               		move.l d0,84(sp)
 2334               		move.l #-2147483594,88(sp)
 2335 0534 2C79 0000 		move.l #-2147483586,96(sp)
 2335      0000 
 2336 053a 91C8      		move.l #-2147483585,104(sp)
 2337 053c 224A      		move.l d0,108(sp)
 2338               		move.l #-2147483591,112(sp)
 2339 053e 4EAE FD9C 		move.l #-2147483606,120(sp)
 2340               		move.l d4,52(sp)
 2341               		move.l d3,60(sp)
 2342               		move.l d2,68(sp)
 2343 0542 4FEF 000C 		lea (36,sp),a0
 2344               		move.l a0,124(sp)
 2345               	.LBB141:
 329:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 								height, SA_Type, CUSTOMSCREEN, SA_Quiet, TRUE, SA_ShowTitle, FALSE, SA_Draggable, FALSE,
 330:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 								SA_Exclusive, TRUE, SA_AutoScroll, FALSE, SA_ErrorCode, (Tag)&errorCode, TAG_END);
 331:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 332:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 333:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::createHardwareScreen() screen %p error code %u", screen, errorCode);
 334:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 335:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return screen;
 336:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2346               	.LBB142:
 2347 0546 4CDF 4C1C 		move.l _IntuitionBase,a6
 2348 054a 4FEF 0060 		sub.l a0,a0
 2349 054e 4E75      		move.l a2,a1
 2350               	#APP
 315:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ULONG modeId = INVALID_ID;
 2351               	| 328 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2352 0550 4280      		jsr a6@(-0x264:W)
 335:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2353               	| 0 "" 2
 2354 0552 60F2      	#NO_APP
 2355               	.LBE142:
 2356               	.LBE141:
 2357               	.LBE140:
 2358               		lea (12,sp),sp
 2359               	.L44:
 2360               	.LBE139:
 2361               		.stabd	68,0,336
 2362               		movem.l (sp)+,d2/d3/d4/a2/a3/a6
 2363               		lea (96,sp),sp
 2364               		rts
 2365               	.L46:
 2366               		.stabd	68,0,315
 2367               		clr.l d0
 2368               		.stabd	68,0,335
 2369               		jra .L44
 337:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 338:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** struct Window *OSystem_AmigaOS3::createHardwareWindow(uint width, uint height, struct Screen *scree
 339:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return OpenWindowTags(NULL, WA_Left, 0, WA_Top, 0, WA_Width, width, WA_Height, height, SA_AutoScro
 2409 0560 4878 007C 	.Lscope13:
 2411 0566 2F0A      		.stabd	78,0,0
 2412 0568 4EB9 0000 		.align	2
 2412      0000 
 2413 056e 2F7C 8000 		.globl	__ZN16OSystem_AmigaOS320createHardwareWindowEjjP6Screen
 2413      0064 0014 
 2414 0576 2F7C 8000 	__ZN16OSystem_AmigaOS320createHardwareWindowEjjP6Screen:
 2414      0065 001C 
 2415 057e 2F7C 8000 		.stabd	46,0,0
 2415      0066 0024 
 2416 0586 2F7C 8000 		.stabd	68,0,338
 2416      0067 002C 
 2417 058e 2F7C 8000 	.LFBB14:
 2417      0039 0034 
 2418 0596 2F7C 8000 		lea (-124,sp),sp
 2418      0070 003C 
 2419 059e 2F7C 8000 		move.l a6,-(sp)
 2419      0085 0044 
 2420 05a6 7001      		move.l a2,-(sp)
 2421 05a8 2F40 0048 	.LBB143:
 2422 05ac 2F7C 8000 		.stabd	68,0,339
 2422      0088 004C 
 2423 05b4 2F40 0050 		lea (8,sp),a2
 2424 05b8 2F7C 8000 		pea 124.w
 2424      0082 0054 
 2425 05c0 2F7C 8000 		clr.l -(sp)
 2425      0089 005C 
 2426 05c8 2F40 0060 		move.l a2,-(sp)
 2427 05cc 2F7C 8000 		jsr _memset
 2427      008C 0064 
 2428 05d4 2F40 0068 		move.l #-2147483548,20(sp)
 2429 05d8 2F7C 8000 		move.l #-2147483547,28(sp)
 2429      0087 006C 
 2430 05e0 2F40 0070 		move.l #-2147483546,36(sp)
 2431 05e4 2F7C 8000 		move.l #-2147483545,44(sp)
 2431      0086 0074 
 2432 05ec 2F40 0078 		move.l #-2147483591,52(sp)
 2433 05f0 2F7C 8000 		move.l #-2147483536,60(sp)
 2433      008A 007C 
 2434 05f8 2F40 0080 		move.l #-2147483515,68(sp)
 2435 05fc 2F7C 8000 		moveq #1,d0
 2435      006A 0084 
 2436 0604 2F7C 0000 		move.l d0,72(sp)
 2436      0418 0088 
 2437 060c 2F6F 0098 		move.l #-2147483512,76(sp)
 2437      0028 
 2438 0612 2F6F 009C 		move.l d0,80(sp)
 2438      0030 
 2439 0618 2F6F 00A0 		move.l #-2147483518,84(sp)
 2439      0040 
 2440               		move.l #-2147483511,92(sp)
 2441               		move.l d0,96(sp)
 2442 061e 2C79 0000 		move.l #-2147483508,100(sp)
 2442      0000 
 2443 0624 91C8      		move.l d0,104(sp)
 2444 0626 224A      		move.l #-2147483513,108(sp)
 2445               		move.l d0,112(sp)
 2446 0628 4EAE FDA2 		move.l #-2147483514,116(sp)
 2447               		move.l d0,120(sp)
 2448               		move.l #-2147483510,124(sp)
 2449               		move.l d0,128(sp)
 2450 062c 4FEF 000C 		move.l #-2147483542,132(sp)
 340:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  WA_CustomScreen, (Tag)screen, WA_Backdrop, TRUE, WA_Borderless, TRUE, WA_DragBar, FALSE,
 341:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  WA_Activate, TRUE, WA_SimpleRefresh, TRUE, WA_NoCareRefresh, TRUE, WA_ReportMouse, TRUE,
 342:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  WA_RMBTrap, TRUE, WA_IDCMP, IDCMP_RAWKEY | IDCMP_MOUSEMOVE | IDCMP_MOUSEBUTTONS, TAG_END);
 343:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2451               		move.l #1048,136(sp)
 2452 0630 245F      		move.l 152(sp),40(sp)
 2453 0632 2C5F      		move.l 156(sp),48(sp)
 2454 0634 4FEF 007C 		move.l 160(sp),64(sp)
 2455 0638 4E75      	.LBB144:
 2456               	.LBB145:
 2457               		move.l _IntuitionBase,a6
 2458               		sub.l a0,a0
 2459               		move.l a2,a1
 2460               	#APP
 2461               	| 339 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2462               		jsr a6@(-0x25e:W)
 2463               	| 0 "" 2
 2464               	#NO_APP
 2465               	.LBE145:
 2466               	.LBE144:
 2467               	.LBE143:
 2468               		lea (12,sp),sp
 2469               		.stabd	68,0,343
 2470               		move.l (sp)+,a2
 2471               		move.l (sp)+,a6
 2472               		lea (124,sp),sp
 2473               		rts
 344:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 345:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::unloadGFXMode() {
 346:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_screen.free();
 347:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_tmpscreen.free();
 348:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 349:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayscreen8.free();
 2491               	.Lscope14:
 2493 065a 4E93      		.stabd	78,0,0
 350:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayscreen16.free();
 2494               		.align	2
 2495 065c 486A 01B4 		.globl	__ZN16OSystem_AmigaOS313unloadGFXModeEv
 2496 0660 4E93      	__ZN16OSystem_AmigaOS313unloadGFXModeEv:
 2497               		.stabd	46,0,0
 351:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 352:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_hardwareWindow) {
 2498               		.stabd	68,0,345
 2499 0662 206A 0036 	.LFBB15:
 2500 0666 4FEF 0010 		movem.l a6/a3/a2,-(sp)
 2501 066a 2208      		move.l 16(sp),a2
 2502 066c 6716      		.stabd	68,0,346
 2503               		pea 376(a2)
 2504               		lea __ZN8Graphics7Surface4freeEv,a3
 2505               		jsr (a3)
 353:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		ClearPointer(_hardwareWindow);
 2506               		.stabd	68,0,347
 2507 066e 2C79 0000 		pea 396(a2)
 2507      0000 
 2508               		jsr (a3)
 2509 0674 4EAE FFC4 		.stabd	68,0,349
 2510               		pea 416(a2)
 2511               		jsr (a3)
 2512               		.stabd	68,0,350
 2513               		pea 436(a2)
 354:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CloseWindow(_hardwareWindow);
 2514               		jsr (a3)
 2515 0678 206A 0036 	.LBB146:
 2516               		.stabd	68,0,352
 2517 067c 4EAE FFB8 		move.l 54(a2),a0
 2518               		lea (16,sp),sp
 2519               		move.l a0,d1
 355:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_hardwareWindow = NULL;
 2520               		jeq .L49
 2521 0680 42AA 0036 	.LBB147:
 2522               	.LBB148:
 2523               	.LBB149:
 2524               		.stabd	68,0,353
 2525               		move.l _IntuitionBase,a6
 356:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 357:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 358:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_hardwareScreenBuffer[0] != NULL) {
 2526               	#APP
 2527 0684 226A 003A 	| 353 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2528 0688 2209      		jsr a6@(-0x3c:W)
 2529 068a 6732      	| 0 "" 2
 2530               	#NO_APP
 2531               	.LBE149:
 2532               	.LBE148:
 359:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[0]);
 2533               	.LBB150:
 2534 068c 2C79 0000 	.LBB151:
 2534      0000 
 2535 0692 206A 0174 		.stabd	68,0,354
 2536               		move.l 54(a2),a0
 2537 0696 4EAE FCF4 	#APP
 2538               	| 354 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2539               		jsr a6@(-0x48:W)
 2540               	| 0 "" 2
 360:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		WaitTOF();
 2541               	#NO_APP
 2542 069a 2C79 0000 	.LBE151:
 2542      0000 
 2543               	.LBE150:
 2544 06a0 4EAE FEF2 		.stabd	68,0,355
 2545               		clr.l 54(a2)
 2546               	.L49:
 361:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		WaitTOF();
 2547               	.LBE147:
 2548               	.LBE146:
 2549 06a4 4EAE FEF2 	.LBB152:
 2550               		.stabd	68,0,358
 2551               		move.l 58(a2),a1
 2552               		move.l a1,d1
 362:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[0]);
 2553               		jeq .L50
 2554 06a8 2C79 0000 	.LBB153:
 2554      0000 
 2555 06ae 206A 0174 	.LBB154:
 2556 06b2 226A 003A 	.LBB155:
 2557               		.stabd	68,0,359
 2558 06b6 4EAE FCFA 		move.l _IntuitionBase,a6
 2559               		move.l 372(a2),a0
 2560               	#APP
 363:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_hardwareScreenBuffer[0] = NULL;
 2561               	| 359 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2562 06ba 42AA 003A 		jsr a6@(-0x30c:W)
 2563               	| 0 "" 2
 2564               	#NO_APP
 2565               	.LBE155:
 2566               	.LBE154:
 2567               	.LBB156:
 364:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 365:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 366:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (unsigned s = 1; s < NUM_SCREENBUFFERS; ++s) {
 367:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (_hardwareScreenBuffer[s]) {
 2568               		.stabd	68,0,360
 2569 06be 226A 003E 		move.l _GfxBase,a6
 2570 06c2 2209      	#APP
 2571 06c4 6712      	| 360 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2572               		jsr a6@(-0x10e:W)
 2573               	| 0 "" 2
 2574               	#NO_APP
 368:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[s]);
 2575               	.LBE156:
 2576 06c6 2C79 0000 	.LBB157:
 2576      0000 
 2577 06cc 206A 0174 		.stabd	68,0,361
 2578               	#APP
 2579 06d0 4EAE FCFA 	| 361 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2580               		jsr a6@(-0x10e:W)
 2581               	| 0 "" 2
 369:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			_hardwareScreenBuffer[s] = NULL;
 2582               	#NO_APP
 2583 06d4 42AA 003E 	.LBE157:
 2584               	.LBB158:
 2585               	.LBB159:
 367:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[s]);
 2586               		.stabd	68,0,362
 2587 06d8 226A 0042 		move.l _IntuitionBase,a6
 2588 06dc 2209      		move.l 372(a2),a0
 2589 06de 6712      		move.l 58(a2),a1
 2590               	#APP
 2591               	| 362 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2592               		jsr a6@(-0x306:W)
 368:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			FreeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[s]);
 2593               	| 0 "" 2
 2594 06e0 2C79 0000 	#NO_APP
 2594      0000 
 2595 06e6 206A 0174 	.LBE159:
 2596               	.LBE158:
 2597 06ea 4EAE FCFA 		.stabd	68,0,363
 2598               		clr.l 58(a2)
 2599               	.L50:
 2600               	.LBE153:
 2601 06ee 42AA 0042 	.LBE152:
 2602               	.LBB160:
 2603               	.LBB161:
 2604               		.stabd	68,0,367
 2605               		move.l 62(a2),a1
 2606               		move.l a1,d1
 370:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 371:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 372:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 373:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_hardwareScreen) {
 2607               		jeq .L51
 2608 06f2 206A 0174 	.LBB162:
 2609 06f6 2208      	.LBB163:
 2610 06f8 670E      	.LBB164:
 2611               		.stabd	68,0,368
 2612               		move.l _IntuitionBase,a6
 2613               		move.l 372(a2),a0
 374:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CloseScreen(_hardwareScreen);
 2614               	#APP
 2615 06fa 2C79 0000 	| 368 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2615      0000 
 2616               		jsr a6@(-0x306:W)
 2617 0700 4EAE FFBE 	| 0 "" 2
 2618               	#NO_APP
 2619               	.LBE164:
 375:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_hardwareScreen = NULL;
 2620               	.LBE163:
 2621 0704 42AA 0174 		.stabd	68,0,369
 2622               		clr.l 62(a2)
 2623               	.L51:
 2624               	.LBE162:
 376:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 377:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2625               		.stabd	68,0,367
 2626 0708 4CDF 4C00 		move.l 66(a2),a1
 2627 070c 4E75      		move.l a1,d1
 2628               		jeq .L52
 2629               	.LBB167:
 2630               	.LBB166:
 2631               	.LBB165:
 2632               		.stabd	68,0,368
 2633               		move.l _IntuitionBase,a6
 2634               		move.l 372(a2),a0
 2635               	#APP
 2636               	| 368 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2637               		jsr a6@(-0x306:W)
 2638               	| 0 "" 2
 2639               	#NO_APP
 2640               	.LBE165:
 2641               	.LBE166:
 2642               		.stabd	68,0,369
 2643               		clr.l 66(a2)
 2644               	.L52:
 2645               	.LBE167:
 2646               	.LBE161:
 2647               	.LBE160:
 2648               	.LBB168:
 2649               		.stabd	68,0,373
 2650               		move.l 372(a2),a0
 2651               		move.l a0,d1
 2652               		jeq .L48
 2653               	.LBB169:
 2654               	.LBB170:
 2655               	.LBB171:
 2656               		.stabd	68,0,374
 2657               		move.l _IntuitionBase,a6
 2658               	#APP
 2659               	| 374 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2660               		jsr a6@(-0x42:W)
 2661               	| 0 "" 2
 2662               	#NO_APP
 2663               	.LBE171:
 2664               	.LBE170:
 2665               		.stabd	68,0,375
 2666               		clr.l 372(a2)
 2667               	.L48:
 2668               	.LBE169:
 2669               	.LBE168:
 2670               		.stabd	68,0,377
 2671               		movem.l (sp)+,a2/a3/a6
 2672               		rts
 378:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 379:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::setPalette(const byte *colors, uint start, uint num) {
 380:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 381:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3:setPalette()");
 382:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 383:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "setPalette() - start = %d", start);
 384:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "setPalette() - num = %d", num);
 385:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 386:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(colors);
 387:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 388:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 389:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *dst = (byte *)(_currentPalette + (3 * start));
 390:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	CopyMem((byte *)colors, dst, (num * 3));
 391:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 392:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (start < _paletteDirtyStart) {
 393:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_paletteDirtyStart = start;
 394:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 395:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 396:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (start + num > _paletteDirtyEnd) {
 397:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_paletteDirtyEnd = start + num;
 398:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 399:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2750 0756 4CDF 440C 	.Lscope15:
 2752               		.stabd	78,0,0
 2753               		.align	2
 2754               		.globl	__ZN16OSystem_AmigaOS310setPaletteEPKhjj
 2755               	__ZN16OSystem_AmigaOS310setPaletteEPKhjj:
 2756               		.stabd	46,0,0
 2757               		.stabd	68,0,379
 2758               	.LFBB16:
 2759               		movem.l a6/a2/d3/d2,-(sp)
 2760               		move.l 20(sp),a2
 2761               		move.l 28(sp),d2
 2762               		move.l 32(sp),d3
 2763               		.stabd	68,0,389
 2764               		move.l d2,d0
 2765               		add.l d0,d0
 2766               		move.l d0,a1
 2767               		add.l d2,a1
 2768               	.LBB172:
 2769               		.stabd	68,0,390
 2770               		move.l d3,d0
 2771               		add.l d0,d0
 2772               	.LBB173:
 2773               		move.l _SysBase,a6
 2774               		move.l 24(sp),a0
 2775               		add.l 646(a2),a1
 2776               		add.l d3,d0
 2777               	#APP
 2778               	| 390 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
   1:./backends/platform/amigaos3/amigaos3-aga.h **** /* ScummVM - Graphic Adventure Engine
   2:./backends/platform/amigaos3/amigaos3-aga.h ****  *
   3:./backends/platform/amigaos3/amigaos3-aga.h ****  * ScummVM is the legal property of its developers, whose names
   4:./backends/platform/amigaos3/amigaos3-aga.h ****  * are too numerous to list here. Please refer to the COPYRIGHT
   5:./backends/platform/amigaos3/amigaos3-aga.h ****  * file distributed with this source distribution.
   6:./backends/platform/amigaos3/amigaos3-aga.h ****  *
   7:./backends/platform/amigaos3/amigaos3-aga.h ****  * This program is free software; you can redistribute it and/or
   8:./backends/platform/amigaos3/amigaos3-aga.h ****  * modify it under the terms of the GNU General Public License
   9:./backends/platform/amigaos3/amigaos3-aga.h ****  * as published by the Free Software Foundation; either version 2
  10:./backends/platform/amigaos3/amigaos3-aga.h ****  * of the License, or (at your option) any later version.
  11:./backends/platform/amigaos3/amigaos3-aga.h **** 
  12:./backends/platform/amigaos3/amigaos3-aga.h ****  * This program is distributed in the hope that it will be useful,
  13:./backends/platform/amigaos3/amigaos3-aga.h ****  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  14:./backends/platform/amigaos3/amigaos3-aga.h ****  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  15:./backends/platform/amigaos3/amigaos3-aga.h ****  * GNU General Public License for more details.
  16:./backends/platform/amigaos3/amigaos3-aga.h **** 
  17:./backends/platform/amigaos3/amigaos3-aga.h ****  * You should have received a copy of the GNU General Public License
  18:./backends/platform/amigaos3/amigaos3-aga.h ****  * along with this program; if not, write to the Free Software
  19:./backends/platform/amigaos3/amigaos3-aga.h ****  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
  20:./backends/platform/amigaos3/amigaos3-aga.h ****  *
  21:./backends/platform/amigaos3/amigaos3-aga.h ****  * $URL$
  22:./backends/platform/amigaos3/amigaos3-aga.h ****  * $Id$
  23:./backends/platform/amigaos3/amigaos3-aga.h ****  *
  24:./backends/platform/amigaos3/amigaos3-aga.h ****  */
  25:./backends/platform/amigaos3/amigaos3-aga.h **** 
  26:./backends/platform/amigaos3/amigaos3-aga.h **** #ifndef PLATFORM_AMIGAOS3_H
  27:./backends/platform/amigaos3/amigaos3-aga.h **** #define PLATFORM_AMIGAOS3_H
  28:./backends/platform/amigaos3/amigaos3-aga.h **** 
  29:./backends/platform/amigaos3/amigaos3-aga.h **** #include <time.h>
  30:./backends/platform/amigaos3/amigaos3-aga.h **** 
  31:./backends/platform/amigaos3/amigaos3-aga.h **** #include "audio/mixer_intern.h"
  32:./backends/platform/amigaos3/amigaos3-aga.h **** #include "backends/events/amigaos3/amigaos3-events.h"
  33:./backends/platform/amigaos3/amigaos3-aga.h **** #include "backends/log/log.h"
  34:./backends/platform/amigaos3/amigaos3-aga.h **** #include "backends/mixer/amigaos3/amigaos3-mixer.h"
  35:./backends/platform/amigaos3/amigaos3-aga.h **** 
  36:./backends/platform/amigaos3/amigaos3-aga.h **** #include "common/scummsys.h"
  37:./backends/platform/amigaos3/amigaos3-aga.h **** #include "common/system.h"
  38:./backends/platform/amigaos3/amigaos3-aga.h **** #include "graphics/palette.h"
  39:./backends/platform/amigaos3/amigaos3-aga.h **** #include "graphics/surface.h"
  40:./backends/platform/amigaos3/amigaos3-aga.h **** 
  41:./backends/platform/amigaos3/amigaos3-aga.h **** // Amiga includes.
  42:./backends/platform/amigaos3/amigaos3-aga.h **** #include <proto/dos.h>
  43:./backends/platform/amigaos3/amigaos3-aga.h **** #include <proto/exec.h>
  44:./backends/platform/amigaos3/amigaos3-aga.h **** #include <proto/graphics.h>
  45:./backends/platform/amigaos3/amigaos3-aga.h **** #include <proto/intuition.h>
  46:./backends/platform/amigaos3/amigaos3-aga.h **** 
  47:./backends/platform/amigaos3/amigaos3-aga.h **** #define PALETTE_SIZE 768
  48:./backends/platform/amigaos3/amigaos3-aga.h **** 
  49:./backends/platform/amigaos3/amigaos3-aga.h **** enum { GFX_NORMAL = 0 };
  50:./backends/platform/amigaos3/amigaos3-aga.h **** 
  51:./backends/platform/amigaos3/amigaos3-aga.h **** class OSystem_AmigaOS3 : public OSystem, PaletteManager {
  52:./backends/platform/amigaos3/amigaos3-aga.h **** public:
  53:./backends/platform/amigaos3/amigaos3-aga.h **** 	OSystem_AmigaOS3();
  54:./backends/platform/amigaos3/amigaos3-aga.h **** 
  55:./backends/platform/amigaos3/amigaos3-aga.h **** 	~OSystem_AmigaOS3();
  56:./backends/platform/amigaos3/amigaos3-aga.h **** 
  57:./backends/platform/amigaos3/amigaos3-aga.h **** 	/**
  58:./backends/platform/amigaos3/amigaos3-aga.h **** 	 * Pre-initialize backend. It should be called after
  59:./backends/platform/amigaos3/amigaos3-aga.h **** 	 * instantiating the backend. Early needed managers are
  60:./backends/platform/amigaos3/amigaos3-aga.h **** 	 * created here.
  61:./backends/platform/amigaos3/amigaos3-aga.h **** 	 */
  62:./backends/platform/amigaos3/amigaos3-aga.h **** 	void init(int audioThreadPriority);
  63:./backends/platform/amigaos3/amigaos3-aga.h **** 
  64:./backends/platform/amigaos3/amigaos3-aga.h **** 	// Override functions from ModularBackend and OSystem
  65:./backends/platform/amigaos3/amigaos3-aga.h **** 	void initBackend();
  66:./backends/platform/amigaos3/amigaos3-aga.h **** 
  67:./backends/platform/amigaos3/amigaos3-aga.h **** 	void quit();
  68:./backends/platform/amigaos3/amigaos3-aga.h **** 
  69:./backends/platform/amigaos3/amigaos3-aga.h **** 	// Logging
  70:./backends/platform/amigaos3/amigaos3-aga.h **** 	void logMessage(LogMessageType::Type type, const char *message);
  71:./backends/platform/amigaos3/amigaos3-aga.h **** 
  72:./backends/platform/amigaos3/amigaos3-aga.h **** 	Common::SeekableReadStream *createConfigReadStream();
  73:./backends/platform/amigaos3/amigaos3-aga.h **** 	Common::WriteStream *createConfigWriteStream();
  74:./backends/platform/amigaos3/amigaos3-aga.h **** 
  75:./backends/platform/amigaos3/amigaos3-aga.h **** 	virtual uint32 getMillis(bool skipRecord = false);
  76:./backends/platform/amigaos3/amigaos3-aga.h **** 
  77:./backends/platform/amigaos3/amigaos3-aga.h **** 	void delayMillis(uint msecs);
  78:./backends/platform/amigaos3/amigaos3-aga.h **** 
  79:./backends/platform/amigaos3/amigaos3-aga.h **** 	void getTimeAndDate(TimeDate &td) const;
  80:./backends/platform/amigaos3/amigaos3-aga.h **** 
  81:./backends/platform/amigaos3/amigaos3-aga.h **** 	bool hasFeature(OSystem::Feature f);
  82:./backends/platform/amigaos3/amigaos3-aga.h **** 	void setFeatureState(OSystem::Feature f, bool enable);
  83:./backends/platform/amigaos3/amigaos3-aga.h **** 	bool getFeatureState(OSystem::Feature f);
  84:./backends/platform/amigaos3/amigaos3-aga.h **** 
  85:./backends/platform/amigaos3/amigaos3-aga.h **** 	const OSystem::GraphicsMode *getSupportedGraphicsModes() const;
  86:./backends/platform/amigaos3/amigaos3-aga.h **** 	int getDefaultGraphicsMode() const;
  87:./backends/platform/amigaos3/amigaos3-aga.h **** 	bool setGraphicsMode(int mode);
  88:./backends/platform/amigaos3/amigaos3-aga.h **** 	void resetGraphicsScale();
  89:./backends/platform/amigaos3/amigaos3-aga.h **** 	int getGraphicsMode() const;
  90:./backends/platform/amigaos3/amigaos3-aga.h **** 
  91:./backends/platform/amigaos3/amigaos3-aga.h **** 	void initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL);
  92:./backends/platform/amigaos3/amigaos3-aga.h **** 
  93:./backends/platform/amigaos3/amigaos3-aga.h **** 	int getScreenChangeID() const { return _screenChangeCount; }
  94:./backends/platform/amigaos3/amigaos3-aga.h **** 
  95:./backends/platform/amigaos3/amigaos3-aga.h **** 	void beginGFXTransaction();
  96:./backends/platform/amigaos3/amigaos3-aga.h **** 	OSystem::TransactionError endGFXTransaction();
  97:./backends/platform/amigaos3/amigaos3-aga.h **** 
  98:./backends/platform/amigaos3/amigaos3-aga.h **** 	inline int16 getHeight() { return _videoMode.screenHeight; }
  99:./backends/platform/amigaos3/amigaos3-aga.h **** 	inline int16 getWidth() { return _videoMode.screenWidth; }
 100:./backends/platform/amigaos3/amigaos3-aga.h **** 
 101:./backends/platform/amigaos3/amigaos3-aga.h **** 	inline PaletteManager *getPaletteManager() { return this; }
 102:./backends/platform/amigaos3/amigaos3-aga.h **** 
 103:./backends/platform/amigaos3/amigaos3-aga.h **** 	void setPalette(const byte *colors, uint start, uint num);
 2779               		jsr a6@(-0x270:W)
 2780               	| 0 "" 2
 2781 075c 70E8      	#NO_APP
 2782 075e D1AF 0004 	.LBE173:
 2783 0762 60AC      	.LBE172:
 2784               		.stabd	68,0,392
 2785               		cmp.l 662(a2),d2
 2786               		jcc .L70
 2787               		.stabd	68,0,393
 2788               		move.l d2,662(a2)
 2789               	.L70:
 400:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 401:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::grabPalette(byte *colors, uint start, uint num) {
 2790               		.stabd	68,0,396
 2791               		add.l d3,d2
 2792 0764 2F0E      		cmp.l 666(a2),d2
 2793 0766 2F02      		jls .L69
 2794 0768 242F 0014 		.stabd	68,0,397
 2795 076c 202F 0018 		move.l d2,666(a2)
 2796               	.L69:
 402:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 403:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(colors);
 404:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 405:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 406:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	CopyMem(_currentPalette + (3 * start), colors, 3 * num);
 2797               		.stabd	68,0,399
 2798 0770 2202      		movem.l (sp)+,d2/d3/a2/a6
 2799 0772 D281      		rts
 407:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2818               	.Lscope16:
 2820               		.stabd	78,0,0
 2821               		.align	2
 2822               		.globl	__ZThn24_N16OSystem_AmigaOS310setPaletteEPKhjj
 2823               	__ZThn24_N16OSystem_AmigaOS310setPaletteEPKhjj:
 2824               		.stabd	46,0,0
 2826               	.Ltext7:
 2827               		.stabd	68,0,103
 2828               	.LFBB17:
 2829               		moveq #-24,d0
 2830               		add.l d0,4(sp)
 2831               		jra __ZN16OSystem_AmigaOS310setPaletteEPKhjj
 2832               		.align	2
 2833               		.globl	__ZN16OSystem_AmigaOS311grabPaletteEPhjj
 2834               	__ZN16OSystem_AmigaOS311grabPaletteEPhjj:
 2835               		.stabd	46,0,0
 2837               	.Ltext8:
 2838               		.stabd	68,0,401
 2839 079a 0000      	.LFBB18:
 2840               		move.l a6,-(sp)
 2841               		move.l d2,-(sp)
 2842               		move.l 20(sp),d2
 2843               		move.l 24(sp),d0
 2844               	.LBB174:
 104:./backends/platform/amigaos3/amigaos3-aga.h **** 	void grabPalette(byte *colors, uint start, uint num);
 2845               		.stabd	68,0,406
 2846               		move.l d2,d1
 2847 079c 70E8      		add.l d1,d1
 2848 079e D1AF 0004 		move.l d1,a0
 2849 07a2 60C0      		add.l d2,a0
 2850               		move.l d0,d1
 2851               		add.l d1,d1
 2852               	.LBB175:
 2853               		move.l _SysBase,a6
 2854               		move.l 12(sp),a1
 2855               		add.l 646(a1),a0
 408:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 409:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::updatePalette() {
 2856               		move.l 16(sp),a1
 2857               		add.l d1,d0
 2858 07a4 48E7 383A 	#APP
 2859 07a8 246F 0020 	| 406 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 410:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 411:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "updatePalette()");
 412:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 413:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "updatePalette() - _paletteDirtyStart = %d", _paletteDirtyStart);
 414:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "updatePalette() - _paletteDirtyEnd = %d", _paletteDirtyEnd);
 415:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 416:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 417:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	uint j = 1;
 418:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *color = (byte *)(_currentPalette + 3 * _paletteDirtyStart);
 2860               		jsr a6@(-0x270:W)
 2861 07ac 222A 0296 	| 0 "" 2
 2862 07b0 2001      	#NO_APP
 2863 07b2 D080      	.LBE175:
 2864 07b4 2040      	.LBE174:
 2865 07b6 D1C1      		.stabd	68,0,407
 2866 07b8 D1EA 0286 		move.l (sp)+,d2
 2867               		move.l (sp)+,a6
 419:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 420:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (uint i = _paletteDirtyStart; i < _paletteDirtyEnd; i++) {
 2868               		rts
 421:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_agaPalette[j] = color[0] << 24;
 422:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_agaPalette[j + 1] = color[1] << 24;
 2887 07e4 2383 B808 	.Lscope18:
 423:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_agaPalette[j + 2] = color[2] << 24;
 2889 07e8 4283      		.stabd	78,0,0
 2890 07ea 1628 0002 		.align	2
 2891 07ee E9AB      		.globl	__ZThn24_N16OSystem_AmigaOS311grabPaletteEPhjj
 2892 07f0 2383 C800 	__ZThn24_N16OSystem_AmigaOS311grabPaletteEPhjj:
 424:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 425:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		j += 3;
 426:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		color += 3;
 2893               		.stabd	46,0,0
 420:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_agaPalette[j] = color[0] << 24;
 2895               	.Ltext9:
 2896 07f6 5282      		.stabd	68,0,104
 2897 07f8 264C      	.LFBB19:
 2898 07fa 60CC      		moveq #-24,d0
 2899               		add.l d0,4(sp)
 2900               		jra __ZN16OSystem_AmigaOS311grabPaletteEPhjj
 427:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 428:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 429:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	uint numberOfEntries = (_paletteDirtyEnd - _paletteDirtyStart);
 2901               		.align	2
 2902 07fc 9081      		.globl	__ZN16OSystem_AmigaOS313updatePaletteEv
 430:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 431:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_agaPalette[0] = (numberOfEntries << 16) + _paletteDirtyStart;
 2903               	__ZN16OSystem_AmigaOS313updatePaletteEv:
 2904 07fe 2400      		.stabd	46,0,0
 2906 0802 4242      	.Ltext10:
 2907 0804 D481      		.stabd	68,0,409
 2908 0806 2282      	.LFBB20:
 432:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 433:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Terminator: NEEDED
 434:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_agaPalette[((numberOfEntries * 3) + 1)] = 0x00000000;
 2909               		movem.l a6/a4/a3/a2/d4/d3/d2,-(sp)
 2910 0808 2200      		move.l 32(sp),a2
 2911 080a D281      		.stabd	68,0,418
 2912 080c D081      		move.l 662(a2),d1
 2913 080e 42B1 0C04 		move.l d1,d0
 2914               		add.l d0,d0
 2915               		move.l d0,a0
 435:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 436:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	LoadRGB32(&_hardwareScreen->ViewPort, _agaPalette);
 2916               		add.l d1,a0
 2917 0812 2C79 0000 		add.l 646(a2),a0
 2917      0000 
 2918 0818 307C 002C 	.LBB176:
 2919 081c D1EA 0174 		.stabd	68,0,420
 2920               		move.l 666(a2),d0
 2921 0820 4EAE FC8E 		move.l d1,d2
 2922               		sub.l a3,a3
 2923               		move.l 658(a2),a1
 437:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 438:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Reset.
 439:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_paletteDirtyStart = 256;
 2924               	.L75:
 2925 0824 257C 0000 		lea (12,a3),a4
 2925      0100 0296 
 440:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_paletteDirtyEnd = 0;
 2926               		cmp.l d2,d0
 2927 082c 42AA 029A 		jls .L74
 441:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 2928               		.stabd	68,0,421
 2929 0830 4CDF 5C1C 		clr.l d3
 2930 0834 4E75      		move.b (a0),d3
 2931               		moveq #24,d4
 2932               		lsl.l d4,d3
 2933               		move.l d3,4(a1,a3.l)
 2934               		.stabd	68,0,422
 2935               		clr.l d3
 2936               		move.b 1(a0),d3
 2937               		lsl.l d4,d3
 2938               		move.l d3,8(a1,a3.l)
 2939               		.stabd	68,0,423
 2940               		clr.l d3
 2941               		move.b 2(a0),d3
 2942               		lsl.l d4,d3
 2943               		move.l d3,(a1,a4.l)
 2944               		.stabd	68,0,426
 2945               		addq.l #3,a0
 2946               		.stabd	68,0,420
 2947               		addq.l #1,d2
 2948               		move.l a4,a3
 2949               		jra .L75
 2950               	.L74:
 2951               	.LBE176:
 2952               		.stabd	68,0,429
 2953 0836 0000      		sub.l d1,d0
 2954               		.stabd	68,0,431
 2955               		move.l d0,d2
 2956               		swap d2
 442:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 443:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) {
 2957               		clr.w d2
 2958               		add.l d1,d2
 2959 0838 48E7 3E22 		move.l d2,(a1)
 2960 083c 246F 0020 		.stabd	68,0,434
 2961 0840 206F 0024 		move.l d0,d1
 2962 0844 2A2F 0028 		add.l d1,d1
 2963 0848 2C2F 0034 		add.l d1,d0
 2964 084c 282F 0038 		clr.l 4(a1,d0.l*4)
 2965               	.LBB177:
 2966               	.LBB178:
 2967               		.stabd	68,0,436
 2968               		move.l _GfxBase,a6
   1:./graphics/surface.h **** /* ScummVM - Graphic Adventure Engine
   2:./graphics/surface.h ****  *
   3:./graphics/surface.h ****  * ScummVM is the legal property of its developers, whose names
   4:./graphics/surface.h ****  * are too numerous to list here. Please refer to the COPYRIGHT
   5:./graphics/surface.h ****  * file distributed with this source distribution.
   6:./graphics/surface.h ****  *
   7:./graphics/surface.h ****  * This program is free software; you can redistribute it and/or
   8:./graphics/surface.h ****  * modify it under the terms of the GNU General Public License
   9:./graphics/surface.h ****  * as published by the Free Software Foundation; either version 2
  10:./graphics/surface.h ****  * of the License, or (at your option) any later version.
  11:./graphics/surface.h ****  *
  12:./graphics/surface.h ****  * This program is distributed in the hope that it will be useful,
  13:./graphics/surface.h ****  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  14:./graphics/surface.h ****  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  15:./graphics/surface.h ****  * GNU General Public License for more details.
  16:./graphics/surface.h ****  *
  17:./graphics/surface.h ****  * You should have received a copy of the GNU General Public License
  18:./graphics/surface.h ****  * along with this program; if not, write to the Free Software
  19:./graphics/surface.h ****  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
  20:./graphics/surface.h ****  *
  21:./graphics/surface.h ****  */
  22:./graphics/surface.h **** 
  23:./graphics/surface.h **** #ifndef GRAPHICS_SURFACE_H
  24:./graphics/surface.h **** #define GRAPHICS_SURFACE_H
  25:./graphics/surface.h **** 
  26:./graphics/surface.h **** #include "common/scummsys.h"
  27:./graphics/surface.h **** 
  28:./graphics/surface.h **** namespace Common {
  29:./graphics/surface.h **** struct Rect;
  30:./graphics/surface.h **** }
  31:./graphics/surface.h **** 
  32:./graphics/surface.h **** #include "graphics/pixelformat.h"
  33:./graphics/surface.h **** 
  34:./graphics/surface.h **** namespace Graphics {
  35:./graphics/surface.h **** 
  36:./graphics/surface.h **** /**
  37:./graphics/surface.h ****  * An arbitrary graphics surface, which can be the target (or source) of blit
  38:./graphics/surface.h ****  * operations, font rendering, etc.
  39:./graphics/surface.h ****  */
  40:./graphics/surface.h **** struct Surface {
  41:./graphics/surface.h **** 	/*
  42:./graphics/surface.h **** 	 * IMPORTANT implementation specific detail:
  43:./graphics/surface.h **** 	 *
  44:./graphics/surface.h **** 	 * ARM code relies on the layout of the first 3 of these fields. Do not
  45:./graphics/surface.h **** 	 * change them.
  46:./graphics/surface.h **** 	 */
  47:./graphics/surface.h **** 
  48:./graphics/surface.h **** 	/**
  49:./graphics/surface.h **** 	 * The width of the surface.
  50:./graphics/surface.h **** 	 */
  51:./graphics/surface.h **** 	uint16 w;
  52:./graphics/surface.h **** 
  53:./graphics/surface.h **** 	/**
  54:./graphics/surface.h **** 	 * The height of the surface.
  55:./graphics/surface.h **** 	 */
  56:./graphics/surface.h **** 	uint16 h;
  57:./graphics/surface.h **** 
  58:./graphics/surface.h **** 	/**
  59:./graphics/surface.h **** 	 * The number of bytes a pixel line has.
  60:./graphics/surface.h **** 	 *
  61:./graphics/surface.h **** 	 * Note that this might not equal w * bytesPerPixel.
  62:./graphics/surface.h **** 	 */
  63:./graphics/surface.h **** 	uint16 pitch;
  64:./graphics/surface.h **** 
  65:./graphics/surface.h **** protected:
  66:./graphics/surface.h **** 	/**
  67:./graphics/surface.h **** 	 * The surface's pixel data.
  68:./graphics/surface.h **** 	 */
  69:./graphics/surface.h **** 	void *pixels;
  70:./graphics/surface.h **** 
  71:./graphics/surface.h **** public:
  72:./graphics/surface.h **** 	/**
  73:./graphics/surface.h **** 	 * The pixel format of the surface.
  74:./graphics/surface.h **** 	 */
  75:./graphics/surface.h **** 	PixelFormat format;
  76:./graphics/surface.h **** 
  77:./graphics/surface.h **** 	/**
  78:./graphics/surface.h **** 	 * Construct a simple Surface object.
  79:./graphics/surface.h **** 	 */
  80:./graphics/surface.h **** 	Surface() : w(0), h(0), pitch(0), pixels(0), format() {
  81:./graphics/surface.h **** 	}
  82:./graphics/surface.h **** 
  83:./graphics/surface.h **** 	/**
  84:./graphics/surface.h **** 	 * Return a pointer to the pixel data.
  85:./graphics/surface.h **** 	 *
  86:./graphics/surface.h **** 	 * @return Pointer to the pixel data.
  87:./graphics/surface.h **** 	 */
  88:./graphics/surface.h **** 	inline const void *getPixels() const {
  89:./graphics/surface.h **** 		return pixels;
  90:./graphics/surface.h **** 	}
  91:./graphics/surface.h **** 
  92:./graphics/surface.h **** 	/**
  93:./graphics/surface.h **** 	 * Return a pointer to the pixel data.
  94:./graphics/surface.h **** 	 *
  95:./graphics/surface.h **** 	 * @return Pointer to the pixel data.
  96:./graphics/surface.h **** 	 */
  97:./graphics/surface.h **** 	inline void *getPixels() {
  98:./graphics/surface.h **** 		return pixels;
  99:./graphics/surface.h **** 	}
 100:./graphics/surface.h **** 
 101:./graphics/surface.h **** 	/**
 102:./graphics/surface.h **** 	 * Sets the pixel data.
 103:./graphics/surface.h **** 	 *
 104:./graphics/surface.h **** 	 * Note that this is a simply a setter. Be careful what you are doing!
 105:./graphics/surface.h **** 	 *
 106:./graphics/surface.h **** 	 * @param newPixels The new pixel data.
 107:./graphics/surface.h **** 	 */
 108:./graphics/surface.h **** 	void setPixels(void *newPixels) { pixels = newPixels; }
 109:./graphics/surface.h **** 
 110:./graphics/surface.h **** 	/**
 111:./graphics/surface.h **** 	 * Return a pointer to the pixel at the specified point.
 112:./graphics/surface.h **** 	 *
 113:./graphics/surface.h **** 	 * @param x The x coordinate of the pixel.
 114:./graphics/surface.h **** 	 * @param y The y coordinate of the pixel.
 115:./graphics/surface.h **** 	 * @return Pointer to the pixel.
 116:./graphics/surface.h **** 	 */
 117:./graphics/surface.h **** 	inline const void *getBasePtr(int x, int y) const {
 118:./graphics/surface.h **** 		return (const byte *)(pixels) + y * pitch + x * format.bytesPerPixel;
 119:./graphics/surface.h **** 	}
 120:./graphics/surface.h **** 
 121:./graphics/surface.h **** 	/**
 122:./graphics/surface.h **** 	 * Return a pointer to the pixel at the specified point.
 123:./graphics/surface.h **** 	 *
 124:./graphics/surface.h **** 	 * @param x The x coordinate of the pixel.
 125:./graphics/surface.h **** 	 * @param y The y coordinate of the pixel.
 126:./graphics/surface.h **** 	 * @return Pointer to the pixel.
 127:./graphics/surface.h **** 	 */
 128:./graphics/surface.h **** 	inline void *getBasePtr(int x, int y) {
 129:./graphics/surface.h **** 		return static_cast<byte *>(pixels) + y * pitch + x * format.bytesPerPixel;
 2969               		move.w #44,a0
 2970 0850 4280      		add.l 372(a2),a0
 2971 0852 302A 017C 	#APP
 2972 0856 4C2F 0800 	| 436 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 2972      0030 
 2973 085c 4282      		jsr a6@(-0x372:W)
 2974 085e 142A 0182 	| 0 "" 2
 2975 0862 4C2F 2800 	#NO_APP
 2975      002C 
 2976 0868 D480      	.LBE178:
 2977 086a D4AA 017E 	.LBE177:
 2978               		.stabd	68,0,439
 2979               		move.l #256,662(a2)
 2980               		.stabd	68,0,440
 2981               		clr.l 666(a2)
 2982               		.stabd	68,0,441
 444:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 445:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "copyRectToScreen()");
 446:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "copyRectToScreen() - pitch = %d", pitch);
 447:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "copyRectToScreen() - x = %d", x);
 448:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "copyRectToScreen() - y = %d", y);
 449:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "copyRectToScreen() - w = %d", w);
 450:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(5, "copyRectToScreen() - h = %d", h);
 451:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 452:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 453:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(buf);
 454:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 455:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(x >= 0 && x < _videoMode.screenWidth);
 456:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(y >= 0 && y < _videoMode.screenHeight);
 457:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(h > 0 && y + h <= _videoMode.screenHeight);
 458:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(w > 0 && x + w <= _videoMode.screenWidth);
 459:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 460:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 461:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *dst = (byte *)_screen.getBasePtr(x, y);
 462:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 463:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_videoMode.screenWidth == pitch && pitch == w) {
 2983               		movem.l (sp)+,d2/d3/d4/a2/a3/a4/a6
 2984 086e BAAA 01DE 		rts
 464:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CopyMemQuick((byte *)buf, dst, w * h);
 465:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {
 466:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		const byte *src = (const byte *)buf;
 467:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		do {
 468:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			CopyMem((void *)src, dst, w);
 469:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src += pitch;
 470:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			dst += _videoMode.screenWidth;  //_screen.pitch;
 471:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		} while (--h);
 472:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 473:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3004               	.Lscope20:
 3006 0890 2608      		.stabd	78,0,0
 3007               		.align	2
 3008               		.globl	__ZN16OSystem_AmigaOS316copyRectToScreenEPKviiiii
 3009               	__ZN16OSystem_AmigaOS316copyRectToScreenEPKviiiii:
 3010               		.stabd	46,0,0
 3011               		.stabd	68,0,443
 468:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src += pitch;
 3012               	.LFBB21:
 3013 0892 2C79 0000 		movem.l a6/a2/d6/d5/d4/d3/d2,-(sp)
 3013      0000 
 3014 0898 2043      		move.l 32(sp),a2
 3015 089a 2242      		move.l 36(sp),a0
 3016 089c 2006      		move.l 40(sp),d5
 3017               		move.l 52(sp),d6
 3018 089e 4EAE FD90 		move.l 56(sp),d4
 3019               	.LBB188:
 3020               	.LBB189:
 469:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			dst += _videoMode.screenWidth;  //_screen.pitch;
 3022 08a2 D685      	.Ltext11:
 470:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		} while (--h);
 3023               		.stabd	68,0,129
 3024 08a4 D4AA 01DE 		clr.l d0
 3025               		move.w 380(a2),d0
 467:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			CopyMem((void *)src, dst, w);
 3026               		muls.l 48(sp),d0
 3027 08a8 5384      		clr.l d2
 3028 08aa 66E6      		move.b 386(a2),d2
 3029 08ac 60DC      		muls.l 44(sp),d2
 3030               		add.l d0,d2
 3031               		add.l 382(a2),d2
 3032               	.LBE189:
 3033               	.LBE188:
 3034               	.LBB190:
 3036               	.Ltext12:
 3037               		.stabd	68,0,463
 3038               		cmp.l 478(a2),d5
 3039               		jne .L81
 3040               		cmp.l d5,d6
 3041               		jne .L81
 3042               	.LBB191:
 3043               	.LBB192:
 3044               		.stabd	68,0,464
 3045               		move.l _SysBase,a6
 3046               		move.l d2,a1
 3047               		move.l d4,d0
 3048               		muls.l d5,d0
 3049               	#APP
 3050               	| 464 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3051               		jsr a6@(-0x276:W)
 3052               	| 0 "" 2
 3053               	#NO_APP
 3054               	.L76:
 3055               	.LBE192:
 3056               	.LBE191:
 3057               	.LBE190:
 3058               		.stabd	68,0,473
 3059               		movem.l (sp)+,d2/d3/d4/d5/d6/a2/a6
 3060               		rts
 3061               	.L81:
 3062               	.LBB197:
 3063               		move.l a0,d3
 3064               	.L79:
 3065               	.LBB193:
 3066               	.LBB194:
 3067               	.LBB195:
 3068               	.LBB196:
 3069               		.stabd	68,0,468
 3070               		move.l _SysBase,a6
 3071               		move.l d3,a0
 3072               		move.l d2,a1
 3073               		move.l d6,d0
 3074 08ae 0000      	#APP
 3075               	| 468 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3076               		jsr a6@(-0x270:W)
 3077               	| 0 "" 2
 474:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 475:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::fillScreen(uint32 col) {
 3078               	#NO_APP
 3079               	.LBE196:
 3080 08b0 206F 0004 	.LBE195:
 3081 08b4 2028 017E 		.stabd	68,0,469
 476:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_screen.getPixels()) {
 3082               		add.l d5,d3
 3083 08b8 671C      		.stabd	68,0,470
 477:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		memset(_screen.getPixels(), (int)col, (_videoMode.screenWidth * _videoMode.screenHeight));
 3084               		add.l 478(a2),d2
 3085 08ba 2228 01DE 	.LBE194:
 3086 08be 4C28 1800 		.stabd	68,0,467
 3086      01E2 
 3087 08c4 2F01      		subq.l #1,d4
 3088 08c6 2F2F 000C 		jne .L79
 3089 08ca 2F00      		jra .L76
 3090 08cc 4EB9 0000 	.LBE193:
 3090      0000 
 3091 08d2 4FEF 000C 	.LBE197:
 478:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 479:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 480:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 481:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::updateScreen() {
 482:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 483:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(9, "OSystem_AmigaOS3::updateScreen()");
 484:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 485:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 486:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	static UBYTE *src;
 487:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 488:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_mouseCursor.visible) {
 489:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		drawMouse();
 490:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 491:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 492:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_overlayVisible) {
 493:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		src = (UBYTE *)_overlayscreen8.getPixels();
 494:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		assert(_videoMode.overlayWidth <= _videoMode.screenWidth);
 495:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		assert(_videoMode.overlayHeight <= _videoMode.overlayScreenHeight);
 496:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		WriteChunkyPixels(&_screenRastPorts[_currentScreenBuffer], 0, 0, _videoMode.overlayWidth - 1,
 3131 0906 2001      	.Lscope21:
 3133 090a D081      		.stabd	78,0,0
 3134 090c E788      		.align	2
 3135 090e D081      		.globl	__ZN16OSystem_AmigaOS310fillScreenEj
 3136 0910 E588      	__ZN16OSystem_AmigaOS310fillScreenEj:
 3137 0912 282B 01EA 		.stabd	46,0,0
 3138               		.stabd	68,0,475
 3139 0916 2C79 0000 	.LFBB22:
 3139      0000 
 3140 091c 41F3 0846 		move.l 4(sp),a0
 3141 0920 4280      		move.l 382(a0),d0
 3142 0922 4281      		.stabd	68,0,476
 3143 0924 2404      		jeq .L83
 3144 0926 5382      		.stabd	68,0,477
 3145 0928 262B 01EE 		move.l 478(a0),d1
 3146 092c 5383      		muls.l 482(a0),d1
 3147               		move.l d1,-(sp)
 3148               		move.l 12(sp),-(sp)
 3149               		move.l d0,-(sp)
 3150               		jsr _memset
 3151               		lea (12,sp),sp
 3152               	.L83:
 497:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  _videoMode.overlayHeight - 1, src, _videoMode.overlayWidth);
 498:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {
 499:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		if (_currentShakePos != _newShakePos) {
 500:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// Set the 'dirty area' to black.
 501:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			memset(_tmpscreen.getBasePtr(0, (_videoMode.screenHeight - _newShakePos)), 0,
 502:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				   (_videoMode.screenWidth * _newShakePos));
 503:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 504:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src = (UBYTE *)_screen.getBasePtr(0, _newShakePos);
 505:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			byte *dst = (byte *)_tmpscreen.getBasePtr(0, 0);
 506:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 507:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			CopyMemQuick(src, dst, (_videoMode.screenWidth * (_videoMode.screenHeight - _newShakePos)));
 508:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 509:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// Reset.
 510:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			_currentShakePos = _newShakePos;
 511:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 512:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src = (UBYTE *)_tmpscreen.getPixels();
 513:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		} else {
 514:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src = (UBYTE *)_screen.getPixels();
 515:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 516:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 517:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		WriteChunkyPixels(&_screenRastPorts[_currentScreenBuffer], 0, 0, _videoMode.screenWidth - 1,
 3153               		rts
 3158               	.Lscope22:
 518:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  _videoMode.screenHeight - 1, src, _videoMode.screenWidth);
 519:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 520:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 521:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Check whether the palette was changed.
 522:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_paletteDirtyEnd != 0) {
 3160 0932 4AAB 029A 		.stabd	78,0,0
 3161 0936 6708      	.lcomm __ZZN16OSystem_AmigaOS312updateScreenEvE3src,4
 523:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		updatePalette();
 3163 0938 2F0B      		.align	2
 3164 093a 4EBA FE68 		.globl	__ZN16OSystem_AmigaOS312updateScreenEv
 3165 093e 588F      	__ZN16OSystem_AmigaOS312updateScreenEv:
 3166               		.stabd	46,0,0
 524:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 525:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_mouseCursor.visible) {
 3167               		.stabd	68,0,481
 3168 0940 4A2B 0230 	.LFBB23:
 3169 0944 6708      		movem.l a6/a3/a2/d4/d3/d2,-(sp)
 526:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		undrawMouse();
 3170               		move.l 28(sp),a3
 3171 0946 2F0B      		.stabd	68,0,488
 3172 0948 4EBA 06EE 		tst.b 560(a3)
 3173 094c 588F      		jeq .L89
 3174               		.stabd	68,0,489
 3175               		move.l a3,-(sp)
 3176               		jsr __ZN16OSystem_AmigaOS39drawMouseEv
 527:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 528:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 529:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[_currentScreenBuffer])) {
 3177               		addq.l #4,sp
 3178 094e 102B 0172 	.L89:
 3179 0952 49C0      	.LBB217:
 3180               		.stabd	68,0,492
 3181 0954 2C79 0000 		tst.b 456(a3)
 3181      0000 
 3182 095a 206B 0174 		jeq .L90
 3183 095e 2273 0C3A 		move.l 422(a3),a2
 3184               	.LBB218:
 3185 0962 4EAE FCF4 		.stabd	68,0,493
 3186               		move.l a2,__ZZN16OSystem_AmigaOS312updateScreenEvE3src
 3187               	.LBB219:
 3188 0966 4A80      		.stabd	68,0,496
 3189 0968 6726      		move.b 370(a3),d1
 530:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Flip.
 531:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_currentScreenBuffer = (_currentScreenBuffer + 1) % NUM_SCREENBUFFERS;
 3190               		extb.l d1
 3191 096a 102B 0172 		move.l d1,d0
 3192 096e 49C0      		add.l d0,d0
 3193 0970 5280      		add.l d1,d0
 3194 0972 2400      		lsl.l #3,d0
 3195 0974 4C3C 2C01 		add.l d1,d0
 3195      5555 5556 
 3196 097c 2400      		lsl.l #2,d0
 3197 097e D482      		move.l 490(a3),d4
 3198 0980 9582      	.LBB220:
 3199 0982 9282      		move.l _GfxBase,a6
 3200 0984 2401      		lea 70(a3,d0.l),a0
 3201 0986 D482      		clr.l d0
 3202 0988 D282      		clr.l d1
 3203 098a 9001      		move.l d4,d2
 3204 098c 1740 0172 		subq.l #1,d2
 3205               		move.l 494(a3),d3
 3206               		subq.l #1,d3
 532:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 533:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3207               	.L100:
 3208 0990 4CDF 4C1C 	.LBE220:
 3209 0994 4E75      	.LBE219:
 3210               	.LBE218:
 3211               	.LBB221:
 3212               	.LBB222:
 499:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// Set the 'dirty area' to black.
 3213               		.stabd	68,0,517
 3214 0996 202B 02A2 	#APP
 3215 099a B0AB 029E 	| 517 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3216 099e 6700 00AC 		jsr a6@(-0x420:W)
 3217               	| 0 "" 2
 502:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 3218               	#NO_APP
 3219 09a2 2200      	.LBE222:
 3220 09a4 4C2B 1800 	.LBE221:
 3220      01DE 
 3221 09aa 2F01      	.LBE217:
 3222 09ac 42A7      		.stabd	68,0,522
 501:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				   (_videoMode.screenWidth * _newShakePos));
 3223               		tst.l 666(a3)
 3224 09ae 222B 01E2 		jeq .L94
 3225 09b2 9280      		.stabd	68,0,523
 3226 09b4 2001      		move.l a3,-(sp)
 3227               		jsr __ZN16OSystem_AmigaOS313updatePaletteEv
 3228               		addq.l #4,sp
 3229               	.L94:
 3230               		.stabd	68,0,525
 3231               		tst.b 560(a3)
 3232 09b6 4281      		jeq .L95
 3233 09b8 322B 0190 		.stabd	68,0,526
 3234 09bc 4C01 0800 		move.l a3,-(sp)
 3235               		jsr __ZN16OSystem_AmigaOS311undrawMouseEv
 3236               		addq.l #4,sp
 3237               	.L95:
 3238               	.LBB234:
 502:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 3239               	.LBB235:
 3240 09c0 D0AB 0192 		.stabd	68,0,529
 3241 09c4 2F00      		move.b 370(a3),d0
 3242 09c6 4EB9 0000 		extb.l d0
 3242      0000 
 504:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			byte *dst = (byte *)_tmpscreen.getBasePtr(0, 0);
 3243               	.LBB236:
 3244 09cc 202B 02A2 		move.l _IntuitionBase,a6
 3245               		move.l 372(a3),a0
 3246               		move.l 58(a3,d0.l*4),a1
 3247               	#APP
 3248               	| 529 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3249               		jsr a6@(-0x30c:W)
 3250 09d0 4281      	| 0 "" 2
 3251 09d2 322B 017C 	#NO_APP
 3252 09d6 4C00 1800 	.LBE236:
 3253 09da 206B 017E 	.LBE235:
 3254 09de D1C1      		tst.l d0
 3255               		jeq .L88
 3256               		.stabd	68,0,531
 3257               		move.b 370(a3),d0
 3258               		extb.l d0
 504:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			byte *dst = (byte *)_tmpscreen.getBasePtr(0, 0);
 3259               		addq.l #1,d0
 3260 09e0 23C8 0000 		move.l d0,d2
 3260      000C 
 3261               		muls.l #1431655766,d1:d2
 507:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 3262               		move.l d0,d2
 3263 09e6 222B 01E2 		add.l d2,d2
 3264 09ea 9280      		subx.l d2,d2
 3265 09ec 2001      		sub.l d2,d1
 3266               		move.l d1,d2
 3267 09ee 2C79 0000 		add.l d2,d2
 3267      0000 
 3268 09f4 226B 0192 		add.l d2,d1
 3269 09f8 4C2B 0800 		sub.b d1,d0
 3269      01DE 
 3270               		move.b d0,370(a3)
 3271 09fe 4EAE FD8A 	.L88:
 3272               	.LBE234:
 3273               		.stabd	68,0,533
 510:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 3274               		movem.l (sp)+,d2/d3/d4/a2/a3/a6
 3275 0a02 276B 02A2 		rts
 3275      029E 
 512:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		} else {
 3276               	.L90:
 3277 0a08 23EB 0192 	.LBB237:
 3277      0000 000C 
 3278               	.LBB224:
 3279 0a10 4FEF 000C 		.stabd	68,0,499
 3280               		move.l 674(a3),d0
 3281               		cmp.l 670(a3),d0
 3282               		jeq .L92
 517:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 						  _videoMode.screenHeight - 1, src, _videoMode.screenWidth);
 3283               	.LBB225:
 3284 0a14 122B 0172 		.stabd	68,0,502
 3285 0a18 49C1      		move.l d0,d1
 3286 0a1a 2001      		muls.l 478(a3),d1
 3287 0a1c D080      		move.l d1,-(sp)
 3288 0a1e D081      		clr.l -(sp)
 3289 0a20 E788      		.stabd	68,0,501
 3290 0a22 D081      		move.l 482(a3),d1
 3291 0a24 E588      		sub.l d0,d1
 3292 0a26 282B 01DE 		move.l d1,d0
 3293               	.LBB226:
 3294 0a2a 2C79 0000 	.LBB227:
 3294      0000 
 3296 0a34 4280      	.Ltext13:
 3297 0a36 4281      		.stabd	68,0,129
 3298 0a38 2404      		clr.l d1
 3299 0a3a 5382      		move.w 400(a3),d1
 3300 0a3c 262B 01E2 		muls.l d1,d0
 3301 0a40 5383      	.LBE227:
 3302 0a42 2479 0000 	.LBE226:
 3302      000C 
 3304               	.Ltext14:
 3305               		.stabd	68,0,502
 3306               		add.l 402(a3),d0
 3307               		move.l d0,-(sp)
 514:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 3308               		jsr _memset
 3309 0a4c 23EB 017E 		.stabd	68,0,504
 3309      0000 000C 
 3310 0a54 60BE      		move.l 674(a3),d0
 3311               	.LBB228:
 3312               	.LBB229:
 3314               	.Ltext15:
 3315               		.stabd	68,0,129
 3316               		clr.l d1
 3317               		move.w 380(a3),d1
 3318               		muls.l d0,d1
 3319               		move.l 382(a3),a0
 3320               		add.l d1,a0
 3321               	.LBE229:
 3322               	.LBE228:
 3324               	.Ltext16:
 3325               		.stabd	68,0,504
 3326               		move.l a0,__ZZN16OSystem_AmigaOS312updateScreenEvE3src
 3327               	.LBB230:
 3328               		.stabd	68,0,507
 3329               		move.l 482(a3),d1
 3330               		sub.l d0,d1
 3331               		move.l d1,d0
 3332               	.LBB231:
 3333               		move.l _SysBase,a6
 3334               		move.l 402(a3),a1
 3335               		muls.l 478(a3),d0
 3336               	#APP
 3337               	| 507 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3338               		jsr a6@(-0x276:W)
 3339               	| 0 "" 2
 3340               	#NO_APP
 3341               	.LBE231:
 3342               	.LBE230:
 3343               		.stabd	68,0,510
 3344               		move.l 674(a3),670(a3)
 3345               		.stabd	68,0,512
 3346               		move.l 402(a3),__ZZN16OSystem_AmigaOS312updateScreenEvE3src
 3347               	.LBE225:
 3348               		lea (12,sp),sp
 3349               	.L93:
 3350               	.LBE224:
 3351               	.LBB232:
 3352               		.stabd	68,0,517
 3353               		move.b 370(a3),d1
 3354               		extb.l d1
 3355               		move.l d1,d0
 3356               		add.l d0,d0
 3357               		add.l d1,d0
 3358               		lsl.l #3,d0
 3359               		add.l d1,d0
 3360               		lsl.l #2,d0
 3361               		move.l 478(a3),d4
 3362               	.LBB223:
 3363               		move.l _GfxBase,a6
 3364               		lea 70(a3,d0.l),a0
 3365               		clr.l d0
 3366               		clr.l d1
 3367               		move.l d4,d2
 3368               		subq.l #1,d2
 3369               		move.l 482(a3),d3
 3370               		subq.l #1,d3
 3371               		move.l __ZZN16OSystem_AmigaOS312updateScreenEvE3src,a2
 3372               		jra .L100
 3373               	.L92:
 3374               	.LBE223:
 3375               	.LBE232:
 3376               	.LBB233:
 3377               		.stabd	68,0,514
 3378               		move.l 382(a3),__ZZN16OSystem_AmigaOS312updateScreenEvE3src
 3379               		jra .L93
 3380               	.LBE233:
 3381               	.LBE237:
 534:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 535:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::setShakePos(int shakeOffset) {
 536:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 537:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 538:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 539:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 540:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_newShakePos = shakeOffset;
 541:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 542:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 543:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark -
 544:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark--- Overlays ---
 545:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark -
 546:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 547:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::loadOverlayPalette() {
 548:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Load overlay palette file.
 549:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	FILE *paletteFile;
 550:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 551:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	paletteFile = fopen("overlay.pal", "r");
 552:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (paletteFile == NULL) {
 553:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		error("Could not load the palette file");
 554:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {
 555:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Skip forward 3 rows.
 556:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		char temp[100];
 557:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		fgets(temp, 100, paletteFile);
 558:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		fgets(temp, 100, paletteFile);
 559:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		fgets(temp, 100, paletteFile);
 560:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 561:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Read the palette data.
 562:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		int red, green, blue;
 563:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 564:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		byte *color = _overlayPalette;
 3449 0aee 2468 028E 	.Lscope23:
 3451 0af6 49EF 0020 		.stabd	78,0,0
 3452 0afa 47EF 001C 		.align	2
 3453 0afe 4DEF 0018 		.globl	__ZN16OSystem_AmigaOS311setShakePosEi
 3454 0b02 4BF9 0000 	__ZN16OSystem_AmigaOS311setShakePosEi:
 3454      0000 
 3455               		.stabd	46,0,0
 565:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 566:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		while (fscanf(paletteFile, "%d %d %d", &red, &green, &blue) != EOF) {
 3456               		.stabd	68,0,535
 3457 0b08 2F0C      	.LFBB24:
 3458 0b0a 2F0B      		.stabd	68,0,540
 3459 0b0c 2F0E      		move.l 4(sp),a0
 3460 0b0e 487A FF82 		move.l 8(sp),674(a0)
 3461 0b12 2F02      		rts
 567:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color[0] = red;
 3466               	.Lscope24:
 568:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color[1] = green;
 3468               		.stabd	78,0,0
 3469 0b22 156F 001F 	.LC3:
 3469      0001 
 569:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color[2] = blue;
 3470               		.ascii "r\0"
 3471 0b28 156F 0023 	.LC4:
 3471      0002 
 570:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 571:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color += 3;
 3472               		.ascii "overlay.pal\0"
 3473 0b2e 568A      	.LC5:
 566:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color[0] = red;
 3474               		.ascii "Could not load the palette file\0"
 3475 0b30 60D6      	.LC6:
 3476               		.ascii "%d %d %d\0"
 572:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 573:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 574:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		fclose(paletteFile);
 3477               		.align	2
 3478 0b32 2F02      		.globl	__ZN16OSystem_AmigaOS318loadOverlayPaletteEv
 3479 0b34 4EB9 0000 	__ZN16OSystem_AmigaOS318loadOverlayPaletteEv:
 3479      0000 
 3480               		.stabd	46,0,0
 3481               		.stabd	68,0,547
 575:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 576:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3482               	.LFBB25:
 3483 0b3a 588F      		lea (-112,sp),sp
 3484 0b3c 4CDF 7C04 		movem.l a6/a5/a4/a3/a2/d2,-(sp)
 3485 0b40 4FEF 0070 		.stabd	68,0,551
 3486 0b44 4E75      		pea .LC3
 3487               		pea .LC4
 3488               		jsr _fopen
 3489               		move.l d0,d2
 3490               	.LBB238:
 3491               		.stabd	68,0,552
 3492               		addq.l #8,sp
 3493               		jne .L103
 3494               		.stabd	68,0,553
 3495               		pea .LC5
 3496               		jsr __Z5errorPKcz
 3497               	.L103:
 3498               	.LBB239:
 3499               		.stabd	68,0,557
 3500               		move.l d0,-(sp)
 3501               		pea 100.w
 3502               		lea (44,sp),a3
 3503               		move.l a3,-(sp)
 3504 0b46 6F76 6572 		lea _fgets,a2
 3504      6C61 792E 
 3504      6D61 7000 
 3505               		jsr (a2)
 3506 0b52 436F 756C 		.stabd	68,0,558
 3506      6420 6E6F 
 3506      7420 6C6F 
 3506      6164 2074 
 3506      6865 206F 
 3507               		move.l d2,-(sp)
 3508 0b76 2564 00   		pea 100.w
 3509 0b79 0000 00   		move.l a3,-(sp)
 3510               		jsr (a2)
 3511               		.stabd	68,0,559
 3512               		move.l d2,-(sp)
 577:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 578:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::loadOverlayColorMap() {
 3513               		pea 100.w
 3514               		move.l a3,-(sp)
 3515 0b7c 598F      		jsr (a2)
 3516 0b7e 48E7 3030 		.stabd	68,0,564
 3517 0b82 246F 0018 		move.l 176(sp),a0
 579:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 580:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "generateOverlayColorMap()");
 581:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 582:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 583:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	int color8;
 584:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 585:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Load overlay map file.
 586:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	FILE *mapFile;
 587:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 588:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	mapFile = fopen("overlay.map", "r");
 3518               		move.l 654(a0),a2
 3519 0b86 487A FEDC 		lea (36,sp),sp
 3520 0b8a 487A FFBA 		lea (32,sp),a4
 3521 0b8e 4EB9 0000 		lea (28,sp),a3
 3521      0000 
 3522 0b94 2400      		lea (24,sp),a6
 589:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (mapFile == NULL) {
 3523               		lea _fscanf,a5
 3524 0b96 508F      	.L105:
 3525 0b98 672A      		.stabd	68,0,566
 3526 0b9a 4283      		move.l a4,-(sp)
 3527 0b9c 47F9 0000 		move.l a3,-(sp)
 3527      0000 
 3528               		move.l a6,-(sp)
 590:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		error("Could not load the overlay map file");
 591:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 592:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 593:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	int i = 0;
 594:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 595:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	while (fscanf(mapFile, "%d", &color8) != EOF) {
 3529               		pea .LC6
 3530 0ba2 486F 0010 		move.l d2,-(sp)
 3531 0ba6 487A FFCE 		jsr (a5)
 3532 0baa 2F02      		lea (20,sp),sp
 3533 0bac 4E93      		addq.l #1,d0
 3534 0bae 4FEF 000C 		jeq .L104
 3535 0bb2 5280      		.stabd	68,0,567
 3536 0bb4 6718      		move.b 27(sp),(a2)
 596:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_overlayColorMap[i] = color8;
 3537               		.stabd	68,0,568
 3538 0bb6 206A 01D2 		move.b 31(sp),1(a2)
 3539 0bba 11AF 0013 		.stabd	68,0,569
 3539      3800 
 597:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		i++;
 3540               		move.b 35(sp),2(a2)
 3541 0bc0 5283      		.stabd	68,0,571
 595:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_overlayColorMap[i] = color8;
 3542               		addq.l #3,a2
 3543 0bc2 60DE      		.stabd	68,0,566
 3544               		jra .L105
 590:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 3545               	.L104:
 3546 0bc4 487A FF8C 		.stabd	68,0,574
 3547 0bc8 4EB9 0000 		move.l d2,-(sp)
 3547      0000 
 3548               		jsr _fclose
 598:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 599:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 600:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	fclose(mapFile);
 3549               	.LBE239:
 3550 0bce 2F02      	.LBE238:
 3551 0bd0 4EB9 0000 		.stabd	68,0,576
 3551      0000 
 601:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3552               		addq.l #4,sp
 3553 0bd6 588F      		movem.l (sp)+,d2/a2/a3/a4/a5/a6
 3554 0bd8 4CDF 0C0C 		lea (112,sp),sp
 3555 0bdc 588F      		rts
 3569               	.Lscope25:
 3571               		.stabd	78,0,0
 602:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 603:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::showOverlay() {
 3572               	.LC7:
 3573               		.ascii "overlay.map\0"
 3574 0be0 2F0A      	.LC8:
 3575 0be2 246F 0008 		.ascii "Could not load the overlay map file\0"
 604:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 605:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 606:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 607:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 608:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_overlayVisible) {
 3576               	.LC9:
 3577 0be6 4A2A 01C8 		.ascii "%d\0"
 3578 0bea 6630      		.align	2
 609:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 610:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 611:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 612:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayVisible = true;
 3579               		.globl	__ZN16OSystem_AmigaOS319loadOverlayColorMapEv
 3580 0bec 157C 0001 	__ZN16OSystem_AmigaOS319loadOverlayColorMapEv:
 3580      01C8 
 613:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 614:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Make a backup of the current game palette.
 615:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	memcpy(_gamePalette, _currentPalette, PALETTE_SIZE);
 3581               		.stabd	46,0,0
 3582 0bf2 4878 0300 		.stabd	68,0,578
 3583 0bf6 2F2A 0286 	.LFBB26:
 3584 0bfa 2F2A 028A 		subq.l #4,sp
 3585 0bfe 4EB9 0000 		movem.l a3/a2/d3/d2,-(sp)
 3585      0000 
 616:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 617:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Set the overlay palette.
 618:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	setPalette((byte *)_overlayPalette, 0, 256);
 3586               		move.l 24(sp),a2
 3587 0c04 2052      		.stabd	68,0,588
 3588 0c06 4878 0100 		pea .LC3
 3589 0c0a 42A7      		pea .LC7
 3590 0c0c 2F2A 028E 		jsr _fopen
 3591 0c10 2F0A      		move.l d0,d2
 3592 0c12 2068 00F4 		.stabd	68,0,589
 3593 0c16 4E90      		addq.l #8,sp
 3594 0c18 4FEF 001C 		jeq .L107
 3595               		clr.l d3
 619:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3596               		lea _fscanf,a3
 3597 0c1c 245F      	.L108:
 3598 0c1e 4E75      		.stabd	68,0,595
 3599               		pea 16(sp)
 3600               		pea .LC9
 3601               		move.l d2,-(sp)
 3602               		jsr (a3)
 3603               		lea (12,sp),sp
 3604               		addq.l #1,d0
 3605               		jeq .L109
 3606               		.stabd	68,0,596
 3607               		move.l 466(a2),a0
 3608               		move.b 19(sp),(a0,d3.l)
 620:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 621:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::hideOverlay() {
 3609               		.stabd	68,0,597
 3610               		addq.l #1,d3
 3611 0c20 48E7 3022 		.stabd	68,0,595
 3612 0c24 246F 0014 		jra .L108
 622:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 623:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 624:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 625:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 626:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (!_overlayVisible) {
 3613               	.L107:
 3614 0c28 4A2A 01C8 		.stabd	68,0,590
 3615 0c2c 6700 0090 		pea .LC8
 627:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 628:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 629:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 630:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	clearOverlay();
 3616               		jsr __Z5errorPKcz
 3617 0c30 2052      	.L109:
 3618 0c32 2F0A      		.stabd	68,0,600
 3619 0c34 2068 007C 		move.l d2,-(sp)
 3620 0c38 4E90      		jsr _fclose
 3621 0c3a 588F      		.stabd	68,0,601
 3622 0c3c 7403      		addq.l #4,sp
 3623               		movem.l (sp)+,d2/d3/a2/a3
 3624               		addq.l #4,sp
 3625               		rts
 631:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 632:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	UBYTE *src = (UBYTE *)_overlayscreen8.getPixels();
 633:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 634:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// since the overlay is taller than the game screen height, need to make sure
 635:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// we're clearing all the screens (in particular the bottoms)
 636:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (unsigned s = 0; s < NUM_SCREENBUFFERS; ++s) {
 637:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		ClearScreen(&_screenRastPorts[_currentScreenBuffer]);
 3634 0c4c D081      	.Lscope26:
 3636               		.stabd	78,0,0
 3637 0c50 2C79 0000 		.align	2
 3637      0000 
 3638 0c56 43F2 0846 		.globl	__ZN16OSystem_AmigaOS311showOverlayEv
 3639               	__ZN16OSystem_AmigaOS311showOverlayEv:
 3640 0c5a 4EAE FFD0 		.stabd	46,0,0
 3641               		.stabd	68,0,603
 3642               	.LFBB27:
 3643               		move.l a2,-(sp)
 638:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		ChangeScreenBuffer(_hardwareScreen, _hardwareScreenBuffer[_currentScreenBuffer]);
 3644               		move.l 8(sp),a2
 3645 0c5e 102A 0172 		.stabd	68,0,608
 3646 0c62 49C0      		tst.b 456(a2)
 3647               		jne .L110
 3648 0c64 2C79 0000 		.stabd	68,0,612
 3648      0000 
 3649 0c6a 206A 0174 		move.b #1,456(a2)
 3650 0c6e 2272 0C3A 		.stabd	68,0,615
 3651               		pea 768.w
 3652 0c72 4EAE FCF4 		move.l 646(a2),-(sp)
 3653               		move.l 650(a2),-(sp)
 3654               		jsr _memcpy
 639:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_currentScreenBuffer = (_currentScreenBuffer + 1) % NUM_SCREENBUFFERS;
 3655               		.stabd	68,0,618
 3656 0c76 102A 0172 		move.l (a2),a0
 3657 0c7a 49C0      		pea 256.w
 3658 0c7c 5280      		clr.l -(sp)
 3659 0c7e 2600      		move.l 654(a2),-(sp)
 3660 0c80 4C3C 3C01 		move.l a2,-(sp)
 3660      5555 5556 
 3661 0c88 2600      		move.l 244(a0),a0
 3662 0c8a D683      		jsr (a0)
 3663 0c8c 9783      		lea (28,sp),sp
 3664 0c8e 9283      	.L110:
 3665 0c90 2601      		.stabd	68,0,619
 3666 0c92 D683      		move.l (sp)+,a2
 3667 0c94 D283      		rts
 3671               	.Lscope27:
 636:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		ClearScreen(&_screenRastPorts[_currentScreenBuffer]);
 3673 0c9e 669E      		.stabd	78,0,0
 3674               		.align	2
 640:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 641:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 642:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_overlayVisible = false;
 3675               		.globl	__ZN16OSystem_AmigaOS311hideOverlayEv
 3676 0ca0 4280      	__ZN16OSystem_AmigaOS311hideOverlayEv:
 3677 0ca2 1540 01C8 		.stabd	46,0,0
 643:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 644:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Reset the game palette.
 645:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	setPalette((byte *)_gamePalette, 0, 256);
 3678               		.stabd	68,0,621
 3679 0ca6 2052      	.LFBB28:
 3680 0ca8 4878 0100 		movem.l a6/a2/d3/d2,-(sp)
 3681 0cac 2F00      		move.l 20(sp),a2
 3682 0cae 2F2A 028A 		.stabd	68,0,626
 3683 0cb2 2F0A      		tst.b 456(a2)
 3684 0cb4 2068 00F4 		jeq .L112
 3685 0cb8 4E90      		.stabd	68,0,630
 3686 0cba 4FEF 0010 		move.l (a2),a0
 3687               		move.l a2,-(sp)
 646:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3688               		move.l 124(a0),a0
 3689 0cbe 4CDF 440C 		jsr (a0)
 3690 0cc2 4E75      		addq.l #4,sp
 3691               		moveq #3,d2
 3692               	.L114:
 3693               	.LBB240:
 3694               	.LBB241:
 3695               	.LBB242:
 3696               		.stabd	68,0,637
 3697               		move.b 370(a2),d1
 3698               		extb.l d1
 3699               		move.l d1,d0
 3700               		add.l d0,d0
 3701               		add.l d1,d0
 3702               		lsl.l #3,d0
 3703               		add.l d1,d0
 3704               		lsl.l #2,d0
 3705               	.LBB243:
 3706               		move.l _GfxBase,a6
 3707               		lea 70(a2,d0.l),a1
 3708               	#APP
 3709               	| 637 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3710               		jsr a6@(-0x30:W)
 3711               	| 0 "" 2
 3712               	#NO_APP
 3713               	.LBE243:
 3714               	.LBE242:
 3715               	.LBB244:
 3716               		.stabd	68,0,638
 3717               		move.b 370(a2),d0
 647:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 648:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::clearOverlay() {
 3718               		extb.l d0
 3719               	.LBB245:
 3720 0cc4 206F 0004 		move.l _IntuitionBase,a6
 649:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (!_overlayVisible) {
 3721               		move.l 372(a2),a0
 3722 0cc8 4A28 01C8 		move.l 58(a2,d0.l*4),a1
 3723 0ccc 671C      	#APP
 650:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 651:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 652:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 653:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Set the background to black.
 654:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *src = (byte *)_overlayscreen8.getPixels();
 655:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	memset(src, 0, (_videoMode.screenWidth * _videoMode.overlayScreenHeight));
 3724               	| 638 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 3725 0cce 2028 01DE 		jsr a6@(-0x30c:W)
 3726 0cd2 4C28 0800 	| 0 "" 2
 3726      01E6 
 3727 0cd8 2F00      	#NO_APP
 3728 0cda 42A7      	.LBE245:
 3729 0cdc 2F28 01A6 	.LBE244:
 3730 0ce0 4EB9 0000 		.stabd	68,0,639
 3730      0000 
 3731 0ce6 4FEF 000C 		move.b 370(a2),d0
 3732               		extb.l d0
 3733 0cea 4E75      		addq.l #1,d0
 3734               		move.l d0,d3
 3735               		muls.l #1431655766,d1:d3
 3736               		move.l d0,d3
 3737               		add.l d3,d3
 3738               		subx.l d3,d3
 3739               		sub.l d3,d1
 3740               		move.l d1,d3
 3741               		add.l d3,d3
 3742               		add.l d3,d1
 3743               		sub.b d1,d0
 656:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 657:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 658:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::grabOverlay(void *buf, int pitch) {
 3744               		move.b d0,370(a2)
 3745               		subq.l #1,d2
 3746 0cec 2F02      	.LBE241:
 3747 0cee 206F 0008 		.stabd	68,0,636
 3748 0cf2 202F 000C 		jne .L114
 659:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 660:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 661:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 662:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 663:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Grab the overlay.
 664:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	memcpy(buf, _overlayscreen16.getPixels(),
 665:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		   (_videoMode.screenWidth * _videoMode.overlayScreenHeight) * _overlayscreen16.format.bytesPerPi
 3749               	.LBE240:
 3750 0cf6 2428 01DE 		.stabd	68,0,642
 3751 0cfa 4C28 2800 		clr.l d0
 3751      01E6 
 3752 0d00 4281      		move.b d0,456(a2)
 3753 0d02 1228 01BE 		.stabd	68,0,645
 3754 0d06 4C01 2800 		move.l (a2),a0
 3755 0d0a 2F42 0010 		pea 256.w
 3756 0d0e 2F68 01BA 		move.l d0,-(sp)
 3756      000C 
 3757 0d14 2F40 0008 		move.l 650(a2),-(sp)
 666:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3758               		move.l a2,-(sp)
 3759 0d18 241F      		move.l 244(a0),a0
 665:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3760               		jsr (a0)
 3761 0d1a 60FF FFFF 		lea (16,sp),sp
 3761      F2E4 
 3762               	.L112:
 3763               		.stabd	68,0,646
 3764               		movem.l (sp)+,d2/d3/a2/a6
 3765               		rts
 667:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 668:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h) {
 669:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 670:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "copyRectToOverlay()");
 671:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 672:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(_transactionMode == kTransactionNone);
 673:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 674:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 675:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Clip the coordinates
 676:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (x < 0) {
 677:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 678:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 679:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 680:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (y < 0) {
 3786               	.Lscope28:
 3788 0d3e 6D76      		.stabd	78,0,0
 681:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 682:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 683:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 684:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w > _videoMode.screenWidth - x) {
 3789               		.align	2
 3790 0d40 2029 01DE 		.globl	__ZN16OSystem_AmigaOS312clearOverlayEv
 3791 0d44 9084      	__ZN16OSystem_AmigaOS312clearOverlayEv:
 3792 0d46 B280      		.stabd	46,0,0
 3793 0d48 6302      		.stabd	68,0,648
 685:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		w = _videoMode.screenWidth - x;
 3794               	.LFBB29:
 3795 0d4a 2200      		move.l 4(sp),a0
 3796               		.stabd	68,0,649
 686:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 687:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 688:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (h > _videoMode.overlayScreenHeight - y) {
 3797               		tst.b 456(a0)
 3798 0d4c 2029 01E6 		jeq .L116
 3799 0d50 9083      		.stabd	68,0,655
 3800 0d52 B480      		move.l 478(a0),d0
 3801 0d54 6302      		muls.l 486(a0),d0
 689:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		h = _videoMode.overlayScreenHeight - y;
 3802               		move.l d0,-(sp)
 3803 0d56 2400      		clr.l -(sp)
 3804               		move.l 422(a0),-(sp)
 690:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 691:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 692:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w <= 0 || h <= 0) {
 3805               		jsr _memset
 3806 0d58 4A81      		lea (12,sp),sp
 3807 0d5a 6F5A      	.L116:
 3808 0d5c 4A82      		rts
 3812               	.Lscope29:
 3814               		.stabd	78,0,0
 3815 0d60 4280      		.align	2
 3816 0d62 3029 01A4 		.globl	__ZN16OSystem_AmigaOS311grabOverlayEPvi
 3817 0d66 4C03 0800 	__ZN16OSystem_AmigaOS311grabOverlayEPvi:
 3818 0d6a 4283      		.stabd	46,0,0
 3819 0d6c 1629 01AA 		.stabd	68,0,658
 3820 0d70 4C04 3800 	.LFBB30:
 3821 0d74 2040      		move.l d2,-(sp)
 3822 0d76 D1C3      		move.l 8(sp),a0
 3823 0d78 2A01      		move.l 12(sp),d0
 3824 0d7a DA85      		.stabd	68,0,665
 3825               		move.l 478(a0),d2
 3826               		muls.l 486(a0),d2
 3827               		clr.l d1
 3828               		move.b 446(a0),d1
 693:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 694:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 695:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 696:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	const OverlayColor *src = (const OverlayColor *)buf;
 697:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *dst = (byte *)_overlayscreen8.getBasePtr(x, y);
 3829               		muls.l d1,d2
 3830 0d7c D1E9 01A6 		move.l d2,16(sp)
 696:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *dst = (byte *)_overlayscreen8.getBasePtr(x, y);
 3831               		move.l 442(a0),12(sp)
 3832 0d80 282F 0024 		move.l d0,8(sp)
 3833               		.stabd	68,0,666
 698:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 699:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	OverlayColor color16;
 700:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte color8;
 701:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 702:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (uint r = 0; r < h; r++) {
 3834               		move.l (sp)+,d2
 3835 0d84 4283      		.stabd	68,0,665
 3836               		jra _memcpy
 3843               	.Lscope30:
 703:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		for (uint c = 0; c < w; c++) {
 3845 0d8e B280      		.stabd	78,0,0
 3846 0d90 6712      		.align	2
 704:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color16 = *src;
 705:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 706:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color8 = _overlayColorMap[color16];
 3847               		.globl	__ZN16OSystem_AmigaOS317copyRectToOverlayEPKviiiii
 3848 0d92 4286      	__ZN16OSystem_AmigaOS317copyRectToOverlayEPKviiiii:
 3849 0d94 3C1A      		.stabd	46,0,0
 707:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 708:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			*dst = color8;
 3850               		.stabd	68,0,668
 3851 0d96 2669 01D2 	.LFBB31:
 3852 0d9a 11B3 6800 		movem.l a3/a2/d6/d5/d4/d3/d2,-(sp)
 3852      0800 
 703:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color16 = *src;
 3853               		move.l 32(sp),a1
 3854 0da0 5280      		move.l 44(sp),d4
 3855 0da2 60EA      		move.l 48(sp),d3
 3856               		move.l 52(sp),d1
 3857               		move.l 56(sp),d2
 709:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 710:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// Add a column.
 711:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			src++;
 712:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			dst++;
 713:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 714:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 715:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// add a row.
 716:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		dst += (_videoMode.screenWidth - w);
 3858               		.stabd	68,0,676
 3859 0da4 2029 01DE 		tst.l d4
 3860 0da8 D1C0      		jlt .L119
 3861 0daa 9081      		.stabd	68,0,680
 717:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		src += (_videoMode.screenWidth - w);
 3862               		tst.l d3
 3863 0dac D080      		jlt .L119
 3864 0dae D085      		.stabd	68,0,684
 3865 0db0 D880      		move.l 478(a1),d0
 3866               		sub.l d4,d0
 702:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		for (uint c = 0; c < w; c++) {
 3867               		cmp.l d0,d1
 3868 0db2 5283      		jls .L121
 3869 0db4 60D0      		.stabd	68,0,685
 3870               		move.l d0,d1
 3871               	.L121:
 718:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 719:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3872               		.stabd	68,0,688
 3873 0db6 4CDF 0C7C 		move.l 486(a1),d0
 3874 0dba 4E75      		sub.l d3,d0
 3875               		cmp.l d0,d2
 3876               		jls .L122
 3877               		.stabd	68,0,689
 3878               		move.l d0,d2
 3879               	.L122:
 3880               		.stabd	68,0,692
 3881               		tst.l d1
 3882               		jle .L119
 3883               		tst.l d2
 3884               		jle .L119
 3885               	.LBB251:
 3886               	.LBB252:
 3888               	.Ltext17:
 3889               		.stabd	68,0,129
 3890               		clr.l d0
 3891               		move.w 420(a1),d0
 3892               		muls.l d3,d0
 3893               		clr.l d3
 3894               		move.b 426(a1),d3
 3895               		muls.l d4,d3
 3896               		move.l d0,a0
 3897               		add.l d3,a0
 3898               		move.l d1,d5
 3899               		add.l d5,d5
 3900               	.LBE252:
 3901               	.LBE251:
 3903               	.Ltext18:
 3904               		.stabd	68,0,697
 720:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 721:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark -
 722:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark--- Mouse ---
 723:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #pragma mark -
 724:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 725:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** bool OSystem_AmigaOS3::showMouse(bool visible) {
 3905               		add.l 422(a1),a0
 3906               		.stabd	68,0,696
 3907 0dbc 206F 0004 		move.l 36(sp),d4
 3908 0dc0 222F 0008 	.LBB253:
 726:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_mouseCursor.visible == visible) {
 3909               		.stabd	68,0,702
 3910 0dc4 1028 0230 		clr.l d3
 3911 0dc8 B200      	.L125:
 3912 0dca 6704      		cmp.l d3,d2
 727:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return visible;
 728:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 729:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 730:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	bool last = _mouseCursor.visible;
 731:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.visible = visible;
 3913               		jeq .L119
 3914 0dcc 1141 0230 		move.l d4,a2
 3915               		clr.l d0
 732:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 733:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	return last;
 734:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3916               	.L124:
 3917 0dd0 4E75      	.LBB254:
 3918               	.LBB255:
 3919               		.stabd	68,0,703
 3920               		cmp.l d0,d1
 3921               		jeq .L123
 3922               		.stabd	68,0,706
 3923               		clr.l d6
 3924               		move.w (a2)+,d6
 3925 0dd2 0000      		.stabd	68,0,708
 3926               		move.l 466(a1),a3
 3927               		move.b (a3,d6.l),(a0,d0.l)
 3928               		.stabd	68,0,703
 735:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 736:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::warpMouse(int x, int y) {
 3929               		addq.l #1,d0
 3930               		jra .L124
 3931 0dd4 4FEF FFE0 	.L123:
 3932 0dd8 2F0E      	.LBE255:
 737:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	struct InputEvent ie;
 738:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	struct IEPointerPixel pp;
 739:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 740:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_NextEvent = NULL;
 3933               		.stabd	68,0,716
 3934 0dda 4280      		move.l 478(a1),d0
 3935 0ddc 2F40 000E 		add.l d0,a0
 741:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_Class = IECLASS_NEWPOINTERPOS;
 3936               		sub.l d1,d0
 3937 0de0 1F7C 0013 		.stabd	68,0,717
 3937      0012 
 742:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_SubClass = IESUBCLASS_PIXEL;
 3938               		add.l d0,d0
 3939 0de6 1F7C 0001 		add.l d5,d0
 3939      0013 
 743:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_Code = 0;
 3940               		add.l d0,d4
 3941 0dec 3F40 0014 	.LBE254:
 744:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_Qualifier = 0;
 3942               		.stabd	68,0,702
 3943 0df0 3F40 0016 		addq.l #1,d3
 745:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	ie.ie_EventAddress = &pp;
 3944               		jra .L125
 3945 0df4 41EF 0006 	.L119:
 3946 0df8 2F48 0018 	.LBE253:
 746:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 747:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	pp.iepp_Screen = _hardwareScreen;
 3947               		.stabd	68,0,719
 3948 0dfc 206F 0028 		movem.l (sp)+,d2/d3/d4/d5/d6/a2/a3
 3949 0e00 2F68 0174 		rts
 3949      0006 
 748:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	pp.iepp_Position.X = x;
 749:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	pp.iepp_Position.Y = y;
 750:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 751:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	AddIEvents(&ie);
 752:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 3973               	.Lscope31:
 3975               		.stabd	78,0,0
 3976               		.align	2
 3977               		.globl	__ZN16OSystem_AmigaOS39showMouseEb
 3978               	__ZN16OSystem_AmigaOS39showMouseEb:
 3979               		.stabd	46,0,0
 3980               		.stabd	68,0,725
 3981               	.LFBB32:
 3982               		move.l 4(sp),a0
 3983               		move.l 8(sp),d1
 3984               		.stabd	68,0,726
 3985               		move.b 560(a0),d0
 3986               		cmp.b d0,d1
 3987               		jeq .L126
 3988               		.stabd	68,0,731
 3989               		move.b d1,560(a0)
 3990               	.L126:
 753:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 754:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::setMouseCursor(const void *buf, uint w, uint h, int hotspot_x, int hotspot_y
 755:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 									  bool dontScale, const Graphics::PixelFormat *format) {
 3991               		.stabd	68,0,734
 3992               		rts
 3997 0e38 242F 003C 	.Lscope32:
 756:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 757:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::setMouseCursor(w = %d, h = %d)", w, h);
 758:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 759:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	assert(buf);
 760:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 761:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 762:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Sanity check.
 763:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w == 0 || h == 0) {
 3999 0e3c 4A83      		.stabd	78,0,0
 4000 0e3e 6700 00DE 		.align	2
 4001 0e42 4A82      		.globl	__ZN16OSystem_AmigaOS39warpMouseEii
 4002 0e44 6700 00D8 	__ZN16OSystem_AmigaOS39warpMouseEii:
 764:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 765:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 766:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 767:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Check to see if we need to recreate the surfaces.
 768:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w != _mouseCursor.w || h != _mouseCursor.h) {
 4003               		.stabd	46,0,0
 4004 0e48 B6AA 0236 		.stabd	68,0,736
 4005 0e4c 6608      	.LFBB33:
 4006 0e4e B4AA 023A 		lea (-32,sp),sp
 4007 0e52 6700 0098 		move.l a6,-(sp)
 4008               		.stabd	68,0,740
 4009               		clr.l d0
 4010               		move.l d0,14(sp)
 4011               		.stabd	68,0,741
 4012               		move.b #19,18(sp)
 4013               		.stabd	68,0,742
 4014               		move.b #1,19(sp)
 151:./graphics/pixelformat.h **** 		rLoss = 8 - RBits, gLoss = 8 - GBits, bLoss = 8 - BBits, aLoss = 8 - ABits;
 4015               		.stabd	68,0,743
 4016 0e56 1F7C 0001 		move.w d0,20(sp)
 4016      0023 
 152:./graphics/pixelformat.h **** 		rShift = RShift, gShift = GShift, bShift = BShift, aShift = AShift;
 4017               		.stabd	68,0,744
 4018 0e5c 1F7C 0008 		move.w d0,22(sp)
 4018      0024 
 4019 0e62 1F7C 0008 		.stabd	68,0,745
 4019      0025 
 4020 0e68 1F7C 0008 		lea (6,sp),a0
 4020      0026 
 4021 0e6e 1F7C 0008 		move.l a0,24(sp)
 4021      0027 
 4022               		.stabd	68,0,747
 4023 0e74 4280      		move.l 40(sp),a0
 4024 0e76 1F40 0028 		move.l 372(a0),6(sp)
 4025 0e7a 1F40 0029 		.stabd	68,0,748
 4026 0e7e 1F40 002A 		move.w 46(sp),10(sp)
 4027 0e82 1F40 002B 		.stabd	68,0,749
 4028               		move.w 50(sp),12(sp)
 4029               	.LBB256:
 4030               	.LBB257:
 4031               		.stabd	68,0,751
 4032               		move.l _CxBase,a6
 4033               		lea (14,sp),a0
 769:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_mouseCursor.surface.create(w, h, Graphics::PixelFormat::createFormatCLUT8());
 4034               	#APP
 4035 0e86 2A02      	| 751 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 4036 0e88 0285 0000 		jsr a6@(-0xb4:W)
 4036      FFFF 
 4037 0e8e 2803      	| 0 "" 2
 4038 0e90 0284 0000 	#NO_APP
 4038      FFFF 
 4039 0e96 49EF 0023 	.LBE257:
 4040 0e9a 2F0C      	.LBE256:
 4041 0e9c 2F05      		.stabd	68,0,752
 4042 0e9e 2F04      		move.l (sp)+,a6
 4043 0ea0 486A 024E 		lea (32,sp),sp
 4044 0ea4 47F9 0000 		rts
 4044      0000 
 151:./graphics/pixelformat.h **** 		rLoss = 8 - RBits, gLoss = 8 - GBits, bLoss = 8 - BBits, aLoss = 8 - ABits;
 152:./graphics/pixelformat.h **** 		rShift = RShift, gShift = GShift, bShift = BShift, aShift = AShift;
 4062 0ed0 1F40 0039 	.Lscope33:
 4064 0ed8 1F40 003B 		.stabd	78,0,0
 4065               		.align	2
 4066               		.globl	__ZN16OSystem_AmigaOS314setMouseCursorEPKvjjiijbPKN8Graphics11PixelFormatE
 4067               	__ZN16OSystem_AmigaOS314setMouseCursorEPKvjjiijbPKN8Graphics11PixelFormatE:
 4068               		.stabd	46,0,0
 4069               		.stabd	68,0,755
 4070               	.LFBB34:
 770:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		_mouseCursorMask.surface.create(w, h, Graphics::PixelFormat::createFormatCLUT8());
 4071               		lea (-12,sp),sp
 4072 0edc 2F0C      		movem.l a6/a4/a3/a2/d5/d4/d3/d2,-(sp)
 4073 0ede 2F05      		move.l 48(sp),a2
 4074 0ee0 2F04      		move.l 56(sp),d3
 4075 0ee2 486A 0272 		move.l 60(sp),d2
 4076 0ee6 4E93      		.stabd	68,0,763
 4077 0ee8 4FEF 0020 		tst.l d3
 4078               		jeq .L129
 771:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 772:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 773:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.w = w;
 4079               		tst.l d2
 4080 0eec 2543 0236 		jeq .L129
 774:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.h = h;
 4081               		.stabd	68,0,768
 4082 0ef0 2542 023A 		cmp.l 566(a2),d3
 775:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.hotX = hotspot_x;
 4083               		jne .L131
 4084 0ef4 256F 0040 		cmp.l 570(a2),d2
 4084      0246 
 776:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.hotY = hotspot_y;
 4085               		jeq .L132
 4086 0efa 256F 0044 	.L131:
 4086      024A 
 777:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.keyColor = keycolor;
 4087               	.LBB270:
 4088 0f00 256F 0048 	.LBB271:
 4088      0232 
 4089               	.LBB272:
 4090               	.LBB273:
 778:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 779:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	CopyMem((void *)buf, _mouseCursor.surface.getPixels(), w * h);
 4092 0f06 2C79 0000 	.Ltext19:
 4092      0000 
 4093 0f0c 206F 0034 		.stabd	68,0,151
 4094 0f10 226A 0254 		move.b #1,35(sp)
 4095 0f14 2003      		.stabd	68,0,152
 4096 0f16 4C02 0800 		move.b #8,36(sp)
 4097               		move.b #8,37(sp)
 4098 0f1a 4EAE FD90 		move.b #8,38(sp)
 4099               		move.b #8,39(sp)
 4100               		.stabd	68,0,153
 4101               		clr.l d0
 780:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 4102               		move.b d0,40(sp)
 4103 0f1e 4CDF 5C3C 		move.b d0,41(sp)
 4104 0f22 4FEF 000C 		move.b d0,42(sp)
 4105 0f26 4E75      		move.b d0,43(sp)
 4106               	.LBE273:
 4107               	.LBE272:
 4108               	.LBE271:
 4109               	.LBE270:
 4111               	.Ltext20:
 4112               		.stabd	68,0,769
 4113               		move.l d2,d5
 4114               		and.l #65535,d5
 4115               		move.l d3,d4
 4116               		and.l #65535,d4
 4117               		lea (35,sp),a4
 4118               		move.l a4,-(sp)
 4119               		move.l d5,-(sp)
 4120               		move.l d4,-(sp)
 4121               		pea 590(a2)
 4122               		lea __ZN8Graphics7Surface6createEttRKNS_11PixelFormatE,a3
 4123               		jsr (a3)
 4124               	.LBB274:
 4125               	.LBB275:
 4126               	.LBB276:
 4127               	.LBB277:
 4129               	.Ltext21:
 4130               		.stabd	68,0,151
 4131               		move.b #1,51(sp)
 4132               		.stabd	68,0,152
 4133               		move.b #8,52(sp)
 4134               		move.b #8,53(sp)
 4135               		move.b #8,54(sp)
 781:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 782:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::setMouseCursorPosition(uint x, uint y) {
 4136               		move.b #8,55(sp)
 4137               		.stabd	68,0,153
 4138 0f28 206F 0004 		clr.l d0
 783:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.x = x;
 4139               		move.b d0,56(sp)
 4140 0f2c 216F 0008 		move.b d0,57(sp)
 4140      023E 
 784:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursor.y = y;
 4141               		move.b d0,58(sp)
 4142 0f32 216F 000C 		move.b d0,59(sp)
 4142      0242 
 4143 0f38 4E75      	.LBE277:
 4144               	.LBE276:
 4145               	.LBE275:
 4146               	.LBE274:
 4148               	.Ltext22:
 4149               		.stabd	68,0,770
 4150               		move.l a4,-(sp)
 4151               		move.l d5,-(sp)
 4152 0f3a 0000      		move.l d4,-(sp)
 4153               		pea 626(a2)
 4154               		jsr (a3)
 4155               		lea (32,sp),sp
 785:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 786:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 787:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::drawMouse() {
 4156               	.L132:
 4157               		.stabd	68,0,773
 4158 0f3c 48E7 3C3A 		move.l d3,566(a2)
 4159 0f40 246F 0024 		.stabd	68,0,774
 788:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 789:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::drawMouse()");
 790:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 791:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 792:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	uint w = _mouseCursor.w;
 4160               		move.l d2,570(a2)
 4161 0f44 206A 0236 		.stabd	68,0,775
 793:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	uint h = _mouseCursor.h;
 4162               		move.l 64(sp),582(a2)
 4163 0f48 202A 023A 		.stabd	68,0,776
 794:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 795:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	int x = (_mouseCursor.x - _mouseCursor.hotX);
 4164               		move.l 68(sp),586(a2)
 4165 0f4c 2A2A 023E 		.stabd	68,0,777
 4166 0f50 9AAA 0246 		move.l 72(sp),562(a2)
 796:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	int y = (_mouseCursor.y - _mouseCursor.hotY);
 4167               	.LBB278:
 4168 0f54 242A 0242 	.LBB279:
 4169 0f58 94AA 024A 		.stabd	68,0,779
 4170 0f5c 2202      		move.l _SysBase,a6
 4171 0f5e 286A 0254 		move.l 52(sp),a0
 797:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 798:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *mousePixels = (byte *)_mouseCursor.surface.getPixels();
 799:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 800:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Clip the coordinates
 801:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (x < 0) {
 4172               		move.l 596(a2),a1
 4173 0f62 4A85      		move.l d3,d0
 4174 0f64 6C06      		muls.l d2,d0
 802:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		w += x;
 4175               	#APP
 4176 0f66 D1C5      	| 779 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 803:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		mousePixels -= x;
 4177               		jsr a6@(-0x270:W)
 4178 0f68 99C5      	| 0 "" 2
 804:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		x = 0;
 4179               	#NO_APP
 4180 0f6a 4285      	.L129:
 4181               	.LBE279:
 805:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 806:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 807:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (y < 0) {
 4182               	.LBE278:
 4183 0f6c 4A82      		.stabd	68,0,780
 4184 0f6e 6C10      		movem.l (sp)+,d2/d3/d4/d5/a2/a3/a4/a6
 808:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		h += y;
 4185               		lea (12,sp),sp
 4186 0f70 D082      		rts
 809:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		mousePixels -= (y * _mouseCursor.surface.pitch);
 810:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		y = 0;
 811:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 812:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 813:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w > _videoMode.screenWidth - x) {
 814:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		w = _videoMode.screenWidth - x;
 815:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 816:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 817:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (h > _videoMode.screenHeight - y) {
 818:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		h = _videoMode.screenHeight - y;
 819:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 820:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 821:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (w <= 0 || h <= 0) {
 817:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		h = _videoMode.screenHeight - y;
 4210 0f9c 2600      	.Lscope34:
 4212               		.stabd	78,0,0
 4213 0f9e 4A83      		.align	2
 4214 0fa0 6772      		.globl	__ZN16OSystem_AmigaOS322setMouseCursorPositionEjj
 822:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Nothing to do.
 823:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		return;
 824:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 825:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 826:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Setup the cursor mask.
 827:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursorMask.x = x;
 4215               	__ZN16OSystem_AmigaOS322setMouseCursorPositionEjj:
 4216 0fa2 2545 0262 		.stabd	46,0,0
 828:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursorMask.y = y;
 4217               		.stabd	68,0,782
 4218 0fa6 2541 0266 	.LFBB35:
 829:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursorMask.w = w;
 4219               		move.l 4(sp),a0
 4220 0faa 2544 026A 		.stabd	68,0,783
 830:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	_mouseCursorMask.h = h;
 4221               		move.l 8(sp),574(a0)
 4222 0fae 2543 026E 		.stabd	68,0,784
 4223 0fb2 242A 0278 		move.l 12(sp),578(a0)
 4224               		rts
 4230               	.Lscope35:
 4232               		.stabd	78,0,0
 4233               		.align	2
 831:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 832:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *maskPixels = (byte *)_mouseCursorMask.surface.getPixels();
 833:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 834:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Set the starting point of the screen we will be drawing to.
 835:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *screenPixels = NULL;
 836:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 837:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_overlayVisible) {
 4234               		.globl	__ZN16OSystem_AmigaOS39drawMouseEv
 4235 0fb8 4A2A 01C8 	__ZN16OSystem_AmigaOS39drawMouseEv:
 4236 0fbc 675C      		.stabd	46,0,0
 4237               		.stabd	68,0,787
 4238               	.LFBB36:
 4239               		movem.l a6/a4/a3/a2/d5/d4/d3/d2,-(sp)
 4240               		move.l 36(sp),a2
 4241               		.stabd	68,0,792
 4242 0fbe 302A 01A4 		move.l 566(a2),a0
 4243 0fc2 4C01 0800 		.stabd	68,0,793
 4244 0fc6 4281      		move.l 570(a2),d0
 4245 0fc8 122A 01AA 		.stabd	68,0,795
 4246 0fcc 4C01 5800 		move.l 574(a2),d5
 4247 0fd0 2640      		sub.l 582(a2),d5
 4248 0fd2 D7C5      		.stabd	68,0,796
 4249 0fd4 D7EA 01A6 		move.l 578(a2),d2
 4250               		sub.l 586(a2),d2
 4251               		move.l d2,d1
 4252               		move.l 596(a2),a4
 4253               		.stabd	68,0,801
 4254               		tst.l d5
 4255               		jge .L141
 4256               		.stabd	68,0,802
 4257               		add.l d5,a0
 838:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		screenPixels = (byte *)_overlayscreen8.getBasePtr(x, y);
 839:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {
 840:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		screenPixels = (byte *)_screen.getBasePtr(x, y);
 841:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 842:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 843:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	// Draw it.
 844:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte color;
 845:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 846:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	do {
 847:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Save a copy of this row before it's overwritten.
 848:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CopyMem(screenPixels, maskPixels, w);
 4258               		.stabd	68,0,803
 4259 0fd8 2C79 0000 		sub.l d5,a4
 4259      0000 
 4260 0fde 204B      		.stabd	68,0,804
 4261 0fe0 2242      		clr.l d5
 4262 0fe2 2004      	.L141:
 4263               		.stabd	68,0,807
 4264 0fe4 4EAE FD90 		tst.l d2
 4265               		jge .L142
 4266               		.stabd	68,0,808
 4267               		add.l d2,d0
 849:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 850:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		for (uint c = 0; c < w; c++) {
 4268               		.stabd	68,0,809
 4269 0fe8 4280      		clr.l d1
 4270               		move.w 594(a2),d1
 851:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color = *mousePixels;
 4271               		muls.l d1,d2
 4272 0fea 1234 0800 		sub.l d2,a4
 852:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 853:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			if (color != _mouseCursor.keyColor) {
 4273               		.stabd	68,0,810
 4274 0fee 4285      		clr.l d1
 4275 0ff0 1A01      	.L142:
 4276 0ff2 BAAA 0232 		.stabd	68,0,813
 4277 0ff6 6704      		move.l 478(a2),d4
 854:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				// Set the color.
 855:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 				*screenPixels = color;
 4278               		sub.l d5,d4
 4279 0ff8 1781 0800 		cmp.l d4,a0
 4280               		jcc .L143
 850:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			color = *mousePixels;
 4281               		move.l a0,d4
 4282 0ffc 5280      	.L143:
 4283 0ffe B084      		.stabd	68,0,821
 4284 1000 66E8      		tst.l d4
 4285               		jeq .L140
 856:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			}
 857:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 858:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			// Add a column.
 859:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			mousePixels++;
 860:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 			screenPixels++;
 861:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		}
 862:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 863:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// add a row.
 864:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		maskPixels += w;
 4286               		.stabd	68,0,817
 4287 1002 D484      		move.l 482(a2),d3
 865:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		mousePixels += (_mouseCursor.surface.pitch - w);
 4288               		sub.l d1,d3
 4289 1004 4280      		cmp.l d3,d0
 4290 1006 302A 0252 		jcc .L145
 4291 100a D9C0      		move.l d0,d3
 866:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		screenPixels += (_videoMode.screenWidth - w);
 4292               	.L145:
 4293 100c D7EA 01DE 		.stabd	68,0,821
 4294               		tst.l d3
 846:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		// Save a copy of this row before it's overwritten.
 4295               		jeq .L140
 4296 1010 5383      		.stabd	68,0,827
 4297 1012 66C4      		move.l d5,610(a2)
 4298               		.stabd	68,0,828
 867:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} while (--h);
 868:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 4299               		move.l d1,614(a2)
 4300 1014 4CDF 5C3C 		.stabd	68,0,829
 4301 1018 4E75      		move.l d4,618(a2)
 4302               		.stabd	68,0,830
 4303               		move.l d3,622(a2)
 4304               		move.l 632(a2),d2
 4305               	.LBB288:
 4306               	.LBB289:
 4308 101a 302A 017C 	.Ltext23:
 4309 101e 4C01 0800 		.stabd	68,0,129
 4310 1022 4281      		clr.l d0
 4311 1024 122A 0182 	.LBE289:
 4312 1028 4C01 5800 	.LBE288:
 4314 102e D7C5      	.Ltext24:
 4315 1030 D7EA 017E 		.stabd	68,0,837
 4316 1034 60A2      		tst.b 456(a2)
 4317               		jeq .L147
 4318               	.LBB291:
 4319               	.LBB290:
 4321               	.Ltext25:
 4322               		.stabd	68,0,129
 4323               		move.w 420(a2),d0
 4324               		muls.l d1,d0
 4325               		clr.l d1
 4326               		move.b 426(a2),d1
 4327               		muls.l d1,d5
 4328               		move.l d0,a3
 4329               		add.l d5,a3
 4330               		add.l 422(a2),a3
 4331               	.L151:
 4332               	.LBE290:
 4333               	.LBE291:
 4334               	.LBB292:
 4335               	.LBB293:
 4336               	.LBB294:
 4338               	.Ltext26:
 4339               		.stabd	68,0,848
 4340               		move.l _SysBase,a6
 4341               		move.l a3,a0
 4342               		move.l d2,a1
 4343               		move.l d4,d0
 4344               	#APP
 4345               	| 848 "backends/platform/amigaos3/amigaos3-graphics-aga.cpp" 1
 4346 1036 0000      		jsr a6@(-0x270:W)
 4347               	| 0 "" 2
 4348               	#NO_APP
 4349               	.LBE294:
 4350               	.LBE293:
 4351               	.LBB295:
 869:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 870:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** void OSystem_AmigaOS3::undrawMouse() {
 4352               		.stabd	68,0,850
 4353               		clr.l d0
 4354 1038 48E7 3822 	.L150:
 4355 103c 246F 0018 		.stabd	68,0,851
 4356               		move.b (a4,d0.l),d1
 4357               		.stabd	68,0,853
 4358               		clr.l d5
 4359               		move.b d1,d5
 4360               		cmp.l 562(a2),d5
 4361 1040 4280      		jeq .L149
 4362               		.stabd	68,0,855
 4363               		move.b d1,(a3,d0.l)
 4364               	.L149:
 4365               		.stabd	68,0,850
 871:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #ifndef NDEBUG
 872:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	debug(4, "OSystem_AmigaOS3::undrawMouse()");
 873:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** #endif
 874:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 875:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *dst = NULL;
 876:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 877:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	if (_overlayVisible) {
 4366               		addq.l #1,d0
 4367 1042 4A2A 01C8 		cmp.l d4,d0
 4368 1046 6746      		jne .L150
 4369               	.LBE295:
 4370               		.stabd	68,0,864
 4371               		add.l d4,d2
 4372               		.stabd	68,0,865
 4373               		clr.l d0
 4374 1048 302A 01A4 		move.w 594(a2),d0
 4375 104c 4C2A 0800 		add.l d0,a4
 4375      0266 
 4376 1052 4282      		.stabd	68,0,866
 4377 1054 142A 01AA 		add.l 478(a2),a3
 4378 1058 4C2A 2800 	.LBE292:
 4378      0262 
 4379 105e D480      		.stabd	68,0,846
 4380 1060 D4AA 01A6 		subq.l #1,d3
 4381               		jne .L151
 4382 1064 282A 0278 	.L140:
 4383               		.stabd	68,0,868
 4384               		movem.l (sp)+,d2/d3/d4/d5/a2/a3/a4/a6
 4385               		rts
 4386               	.L147:
 4387               	.LBB296:
 878:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		dst = (byte *)_overlayscreen8.getBasePtr(_mouseCursorMask.x, _mouseCursorMask.y);
 879:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	} else {
 880:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		dst = (byte *)_screen.getBasePtr(_mouseCursorMask.x, _mouseCursorMask.y);
 881:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 882:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 883:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	byte *src = (byte *)_mouseCursorMask.surface.getPixels();
 884:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 
 885:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	for (uint i = 0; i < _mouseCursorMask.h; i++) {
 4388               	.LBB297:
 4390               	.Ltext27:
 4391 106a B6AA 026E 		.stabd	68,0,129
 4392 106e 643C      		move.w 380(a2),d0
 4393               		muls.l d1,d0
 4394               		clr.l d1
 4395               		move.b 386(a2),d1
 886:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CopyMem(src, dst, _mouseCursorMask.w);
 4396               		muls.l d1,d5
 4397 1070 2C79 0000 		move.l d0,a3
 4397      0000 
 4398 1076 2044      		add.l d5,a3
 4399 1078 2242      		add.l 382(a2),a3
 4400 107a 202A 026A 		jra .L151
 4401               	.LBE297:
 4402 107e 4EAE FD90 	.LBE296:
 887:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		dst += _videoMode.screenWidth;
 888:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		src += _mouseCursorMask.w;
 885:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 		CopyMem(src, dst, _mouseCursorMask.w);
 4427 10aa 60B8      	.Lscope36:
 4429               		.stabd	78,0,0
 4430               		.align	2
 4431               		.globl	__ZN16OSystem_AmigaOS311undrawMouseEv
 4432               	__ZN16OSystem_AmigaOS311undrawMouseEv:
 889:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** 	}
 890:backends/platform/amigaos3/amigaos3-graphics-aga.cpp **** }
 4433               		.stabd	46,0,0
 4435 10b0 4E75      	.Ltext28:
 4436               		.stabd	68,0,870
 4437               	.LFBB37:
 4438               		movem.l a6/a2/d4/d3/d2,-(sp)
 4439               		move.l 24(sp),a2
 4440               	.LBB306:
 4441               	.LBB307:
 4443               	.Ltext29:
 4444               		.stabd	68,0,129
 4445               		clr.l d0
 4446               	.LBE307:
 4447               	.LBE306:
 4449               	.Ltext30:
 4450               		.stabd	68,0,877
 4451               		tst.b 456(a2)
 4452               		jeq .L159
 4453               	.LBB309:
 4454               	.LBB308:
 4456               	.Ltext31:
 4457               		.stabd	68,0,129
 4458               		move.w 420(a2),d0
