#ifndef SCVERSION_H
#define SCVERSION_H

namespace Sci
{
/**
 * SCI versions
 * For more information, check here:
 * https://wiki.scummvm.org/index.php/Sierra_Game_Versions#SCI_Games
 */
enum SciVersion : byte {
	SCI_VERSION_NONE,
	SCI_VERSION_0_EARLY, // KQ4 early, LSL2 early, XMAS card 1988
	SCI_VERSION_0_LATE, // KQ4, LSL2, LSL3, SQ3 etc
	SCI_VERSION_01, // KQ1 and multilingual games (S.old.*)
	SCI_VERSION_1_EGA_ONLY, // SCI 1 EGA with parser (i.e. QFG2 only)
	SCI_VERSION_1_EARLY, // KQ5 floppy, SQ4 floppy, XMAS card 1990, Fairy tales, Jones floppy
	SCI_VERSION_1_MIDDLE, // LSL1, Jones CD
	SCI_VERSION_1_LATE, // Dr. Brain 1, EcoQuest 1, Longbow, PQ3, SQ1, LSL5, KQ5 CD
	SCI_VERSION_1_1, // Dr. Brain 2, EcoQuest 1 CD, EcoQuest 2, KQ6, QFG3, SQ4CD, XMAS 1992 and many more
#ifdef ENABLE_SCI32
	SCI_VERSION_2, // GK1, PQ4 floppy, QFG4 floppy
	SCI_VERSION_2_1_EARLY, // GK2 demo, KQ7, LSL6 hires, PQ4, QFG4 floppy
	SCI_VERSION_2_1_MIDDLE, // GK2, KQ7, MUMG Deluxe, Phantasmagoria 1, PQ4CD, PQ:SWAT, QFG4CD, Shivers 1, SQ6, Torin
	SCI_VERSION_2_1_LATE, // demos of LSL7, Lighthouse, RAMA
	SCI_VERSION_3 // LSL7, Lighthouse, RAMA, Phantasmagoria 2
#endif
};

/**
 * Convenience function to obtain the active SCI version.
 */
static inline SciVersion getSciVersion() {
	extern SciVersion s_sciVersion;
	assert(s_sciVersion != SCI_VERSION_NONE);
	return s_sciVersion;
}

}

#endif // SCVERSION_H
