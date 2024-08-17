/* Copyright (c) 2024 Diego A.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * COMPILE GUIDE
 *
 * APP BUILD
 * C:\gcc "C:\Users\Documents\makai_aio.c" -o "C:\Users\Documents\makai_aio.exe" icon.res
 *
 * ICON BUILD
 * C:\windres "C:\Users\Documents\icn.rc" -O coff -o "C:\Users\Documents\icn.res"
*/

static const char proj[] = "PROJECT MAKA II";

static const char decIntro[] = "# Makai no Bookmark Dialogue Layout v1.0\n"
"# This is a straightforward format for editing Makai no Bookmark script files.\n"
"# You can do a variety of things with it.\n"
"# By using \\ you can specify special commands.\n"
"# Here's a list of all commands:\n"
"# \\N        = Linebreak, specify more than 1 to insert new lines.\n"
"# \\BLACK    = Changes text color to black and outlines to white.\n"
"# \\BLUE1    = Changes text color to blue.\n"
"# \\BLUE2    = Changes outlines to blue.\n"
"# \\RED1\n"
"# \\RED2\n"
"# \\GREEN1\n"
"# \\GREEN2\n"
"# \\ORANGE\n"
"# \\RESTORE  = Changes text to the default color.\n";

static char LANG_ID[7][4] =
{
	"jp",
	"en",
	"es",
	"fr",
	"it",
	"pt",
	"de"
};

typedef struct _LinesData
{
	unsigned lineptr;
} LinesData;

unsigned ConvertUTF(uint16_t code, bool patched)
{
	// These are the halfwidth values for the decoder.
	
	// NOTE: Since the game only uses halfwidth in rare cases
	// I've used the fullwidth equivalents for decoding hw hiragana and katakana
	
	// Useful refs
	// https://loc.gov/marc/specifications/specchareacc/JapaneseHiraganaKatakana.html
	// https://www.utf8-chartable.de/unicode-utf8-table.pl
	
	unsigned val = 0;
	
	switch(code) {
		case 0xE000: val = 0x20;
			break;
		
		case 0xE00B: val = 0x3F; // question mark
			break;
		case 0xE00C: val = patched ? 0x44 : 0xE381B2; // Hiragana HI, now D
			//fprintf(fp, "%s%s%s", "\xE3", "\x81", "\xB2"); // this works too
			break;
		case 0xE00D: val = patched ? 0x45 : 0xE38289; // Hiragana RA, now E
			break;
		case 0xE00E: val = patched ? 0x46 : 0xE3818C; // Hiragana GA, now F
			break;
		case 0xE00F: val = patched ? 0x47 : 0xE381AA; // Hiragana NA, now G
			break;
		case 0xE010: val = patched ? 0x4A : 0xE382AB; // Katakana KA, now J
			break;
		case 0xE011: val = patched ? 0x4B : 0xE382BF; // Katakana TA, now K
			break;
		case 0xE012: val = patched ? 0x4C : 0xE3838A; // Katakana NA, now L
			break;
		case 0xE013: val = 0x41; // Native A
			break;
		case 0xE014: val = 0x42; // Native B
			break;
		case 0xE015: val = 0x43; // Native C
			break;
		case 0xE016: val = patched ? 0x4E : 0xE38282; // Hiragana MO, now N
			break;
		case 0xE017: val = patched ? 0x50 : 0xE381A9; // Hiragana DO, now P
			break;
		case 0xE018: val = patched ? 0x51 : 0xE3828B; // Hiragana RU, now Q
			break;
		case 0xE019: val = patched ? 0x53 : 0xE38191; // Hiragana KE, now S
			break;
		case 0xE01A: val = patched ? 0x55 : 0xE381A9; // Hiragana small TU, now U
			break;
		case 0xE01B: val = patched ? 0x56 : 0xE381A6; // Hiragana TE, now V
			break;
		case 0xE01C: val = patched ? 0x57 : 0xE38184; // Hiragana I, now W
			break;
		case 0xE01D: val = 0x52; // Native R
			break;
		case 0xE01E: val = 0x4F; // Native O
			break;
		case 0xE01F: val = 0x4D; // Native M
			break;
		case 0xE020: val = patched ? 0x58 : 0xEFBDBA; // hw KO, now X
			break;
		case 0xE021: val = patched ? 0x59 : 0xEFBE9D; // Katakana SO, now Y
			break;
		case 0xE022: val = patched ? 0x5A : 0xE38397; // Katakana PU, now Z
			break;
		case 0xE023: val = patched ? 0x61 : 0xEFBE98; // Katakana RI, now a
			break;
		case 0xE024: val = patched ? 0x62 : 0xEFBDB0; // hw elongation, now b
			break;
		case 0xE025: val = patched ? 0x63 : 0xEFBE84; // Katakana TO, now c
			break;
		case 0xE026: val = 0x3A; // :
			break;
		case 0xE027: val = 0x25; // %
			break;
		case 0xE028: val = 0x2E; // . 
			break;
		case 0xE029: val = 0x48; // Native H
			break;
		case 0xE02A: val = 0x49; // Native I
			break;
		case 0xE02B: val = 0x54; // Native T
			break;
		case 0xE02C: val = patched ? 0xE280A6 : 0x78; // Some kind of cross sign, now ellipsis
			break;
		case 0xE02D: val = 0xE383BC; // Vowel elongation mark
			break;
		case 0xE02E: val = patched ? 0x64 : 0xE382A2; // Katakana A, now d
			break;
		case 0xE02F: val = patched ? 0x65 : 0xE382AF; // Katakana KU, now e
			break;
		case 0xE030: val = patched ? 0x66 : 0xE382B7; // Katakana SI, now f
			break;
		case 0xE031: val = patched ? 0x67 : 0xEFBDAE; // Katakana YO, now g
			break;
		case 0xE032: val = patched ? 0x68 : 0xE38090; // Thick bracket left , now h
			break;
		case 0xE033: val = patched ? 0x69 : 0xE38091; // Thick bracket right, now i
			break;
		case 0xE034: val = patched ? 0x6A : 0xE382B9; // Katakana SU, now j
			break;
		case 0xE035: val = patched ? 0x6B : 0xE383A9; // Katakana RA, now k
			break;
		case 0xE036: val = patched ? 0x6C : 0xE382A3; // Katakana small I, now l
			break;
		case 0xE037: val = patched ? 0x6D : 0xE383AB; // Katakana RU, now m
			break;
		case 0xE038: val = patched ? 0x6E : 0xE382B6; // Katakana ZA, now n
			break;
		case 0xE039: val = patched ? 0x6F : 0xE382AF; // Katakana KU, now o
			break;
		case 0xE03A: val = patched ? 0x70 : 0xE382AC; // Katakana GA, now p
			break;
		case 0xE03B: val = patched ? 0x71 : 0xE382AD; // Katakana KI, now q
			break;
		case 0xE03C: val = patched ? 0x72 : 0xE383A4; // Katakana YA, now r
			break;
		case 0xE03D: val = patched ? 0x73 : 0xE38381; // Katakana TI, now s
			break;
		case 0xE03E: val = patched ? 0x74 : 0xE383A1; // Katakana ME, now t
			break;
		case 0xE03F: val = patched ? 0x75 : 0xE38394; // Katakana PI, now u
			break;
		case 0xE040: val = patched ? 0x76 : 0xE38389; // Katakana DO, now v
			break;
		case 0xE041: val = patched ? 0x77 : 0xE383AF; // Katakana WA, now w
			break;
		case 0xE042: val = patched ? 0x78 : 0xE38390; // Katakana BA, now x
			break;
		case 0xE043: val = patched ? 0x79 : 0xE382A4; // Katakana I, now y
			break;
		case 0xE044: val = patched ? 0x7A : 0xE38399; // Katakana BE, now z
			break;
		case 0xE045: val = patched ? 0x27 : 0x74; // idk, it looks like a tiny t
			break;
		case 0xE046: val = patched ? 0x2C : 0x78; // Some tiny x at the top
			break;
		case 0xE047: val = patched ? 0xC3A1 : 0x67; // Some tiny g, now á
			break;
		case 0xE048: val = patched ? 0xC3A9 : 0xEFBDA2; // Top-left bracket, now é
			break;
		case 0xE049: val = patched ? 0xC3AD : 0x20; // SPACE, now í
			break;
		case 0xE04A: val = patched ? 0xC3B3 : 0x20; // SPACE, now ó
			break;
		case 0xE04B: val = patched ? 0xC3BA : 0x20; // SPACE, now ú
			break;
		case 0xE04C: val = patched ? 0xC3BC : 0x20; // SPACE, now ü
			break;
		case 0xE04D: val = patched ? 0xC3B1 : 0x20; // SPACE, now ñ
			break;
		case 0xE04E: val = patched ? 0xC2BF : 0x20; // SPACE, now ¿
			break;
		case 0xE04F: val = patched ? 0xC2A1 : 0x20; // SPACE, now ¡
			break;
		case 0xE050: val = patched ? 0x21 : 0x20; // SPACE, now !
			break;
		case 0xE051: val = patched ? 0xC3A7 : 0x20; // SPACE, now ç
			break;
		
		// Now fullwidth characters
		case 0x8000: val = 0xE38080; // SPACE FW
			break;
		
		// Now fullwidth misplaced katakana
		case 0x8098: val = 0xE383B4; // VU
			break;
		case 0x80E6: val = 0xE383B5; // KA small
			break;
		case 0x80E7: val = 0xE383B6; // KE small
			break;
		
		// Symbols
		case 0x80E8: val = 0xEFBC9F; // ?
			break;
		case 0x80E9: val = 0xEFBC81; // !
			break;
		case 0x80EA: val = 0xE383BC; // - prolonged sound?
			break;
		case 0x80EB: val = 0xE383BC; // - prolonged sound, longer?
			break;
		case 0x80EC: val = 0xE3829B; // voiced sound mark
			break;
		case 0x80ED: val = 0xE3829C; // semi-voiced sound mark
			break;
		case 0x80EE: val = 0xEFBC9C; // <
			break;
		case 0x80EF: val = 0xEFBD9C; // |
			break;
		case 0x80F0: val = 0xEFBCBF; // _
			break;
		case 0x80F1: val = 0xE38081; //0xEFBDA4 = halfwidth comma, JP , FW
			break;
		case 0x80F2: val = 0xEFBDA1; // fullstop
			break;
		case 0x80F3: val = 0xEFBC8C; // ,
			break;
		case 0x80F4: val = 0xEFBC8E; // .
			break;
		case 0x80F5: val = 0xEFBDA5; // katakana middle dot?
			break;
		case 0x80F6: val = 0xEFBC9A; // :
			break;
		case 0x80F7: val = 0xEFBC9B; // ;
			break;
		case 0x80F8: val = 0xE3809D; // some thin quotes closing
			break;
		case 0x80F9: val = 0xE3809E; // some thin quotes start
			break;
		case 0x80FA: printf("Unused code found 0x%X, report issue!\n", code); //val = 0xE3; // No idea what this is
			break;
		case 0x80FB: val = 0xEFBCBE; // ^
			break;
		case 0x80FC: val = 0xEFBC8D; // -
			break;
		case 0x80FD: val = 0xE383BD; // Katakana iteration mark
			break;
		case 0x80FE: val = 0xE383BE; // Katakana voiced iteration mark
			break;
		case 0x80FF: val = 0xE3829D; // Hiragana iteration mark
			break;
		case 0x8100: val = 0xE3829E; // Hiragana voiced iteration mark
			break;
		case 0x8101: val = 0xE38083; // Not sure which quotation symbol this is
			break;
		case 0x8102: val = 0xE4BB9D; // Kanji repetition mark
			break;
		case 0x8103: val = 0xE38085; // Ideographic iteration mark
			break;
		case 0x8104: val = 0xE38086; // diagonal with squiggly
			break;
		case 0x8105: val = 0xE38087; // diagonal with squiggly
			break;
		case 0x8106: val = 0xEFBC8D; // another hyphen-like design
			break;
		case 0x8107: val = 0xEFBC8F; // / SOLIDUS
			break;
		case 0x8108: val = 0xEFBCBC; // inverted /
			break;
		case 0x8109: val = 0xEFBD9E; // ~
			break;
		case 0x810A: val = 0xEFBC9D; // use horizontal version
			printf("Vertical double hyphen? 0x%X\n", code);
			break;
		case 0x810B: val = 0xE28BAF; // JP ellipsis, used in LZ script 32 and 33
			break;
		case 0x810C: val = 0xE280A5;
			printf("Two-dot ellipsis 0x%X, might appear as tofu!\n", code);
			break;
		case 0x810D: val = 0xEFBC87; // apostrophe right
			break;
		case 0x810E: val = 0xEFBC87; // apostrophe left TODO: find the correct values
			break;
		case 0x810F: val = 0xEFBC87; // idk, looks incomplete, doubled-sided apostrophe
			break;
		case 0x8110:
			printf("Script has unusual SPACE character! 0x%X\n", code);
			val = 0xE38080; // another space
			break;
		case 0x8111: val = 0x28; // ( TODO: use the fw value
			break;
		case 0x8112: val = 0x29; // ) TODO: use the fw value
			break;
		case 0x8113:
		case 0x8114:
		case 0x8115:
		case 0x8116:
		case 0x8117:
			printf("Script has unusual SPACE character! 0x%X\n", code);
			val = 0xE38080; // another space
			break;
		case 0x8118: val = 0xEFBD9D; // }
			break;
		case 0x8119: val = 0xE28CA9; // ( variant
			break;
		case 0x811A: val = 0xE28CAA; // ) variant
			break;
		case 0x811B: val = 0xEFBD9F; // (( white
			break;
		case 0x811C: val = 0xEFBDA0; // )) white
			break;
		case 0x811D: val = 0xEFBDA2; // hw corner bracket top
			break;
		case 0x811E: val = 0xEFBDA3; // hw corner bracket bot
			break;
		case 0x811F: val = 0xE3808E; // corner bracket white top
			break;
		case 0x8120: val = 0xE3808F; // corner bracket white bot
			break;
		case 0x8121: val = 0xE38090; // JP bracket left
			break;
		case 0x8122: val = 0xE38091; // JP bracket right
			break;
		case 0x8123: val = 0xEFBC8B; // +
			break;
		case 0x8124: val = 0xEFBC8D; // -
			break;
		case 0x8125: val = 0xC2B1; // +-
			break;
		case 0x8126: val = 0xC397; // mult sign
			break;
		case 0x8127: val = 0xC3B7; // div sign
			break;
		case 0x8128: val = 0xEFBC9D; // =
			break;
		case 0x8129: val = 0xE289A0; // crossed =
			break;
		case 0x812A: val = 0xEFBC9E; // >
			break;
		case 0x812B: val = 0xE289A6; // <=
			break;
		case 0x812C: val = 0xE289A7; // >=
			break;
		case 0x812D: val = 0xE2889E; // Inf
			break;
		case 0x812E: printf("Unusual chr! 0x%X Using fullstop.\n"); val = 0xEFBC8E; // three dots in pyramid position
			break;
		case 0x812F: val = 0xE29982; // Female
			break;
		case 0x8130: val = 0xE29980; // Male
			break;
		case 0x8131: val = 0xE3829C; // Handakuten circle
			break;
		case 0x8132: val = 0xCABC; // apostrophe?
			break;
		case 0x8133: val = 0xCBAE; // "
			break;
		case 0x8134: val = 0xE28483; // box? idk
			break;
		case 0x8135: val = 0xEFBFA5; // Yen
			break;
		case 0x8136: val = 0xEFBC84; // MONEY MONEY MONEY
			break;
		case 0x8137: val = 0xC2A2; // cent
			break;
		case 0x8138: val = 0xEFBFA1; // Euro
			break;
		case 0x8139: val = 0xEFBC85; // %
			break;
		case 0x813A: val = 0xEFBC83; // #
			break;
		case 0x813B: val = 0xEFBC86; // &
			break;
		case 0x813C: val = 0xEFBC8A; // *
			break;
		case 0x813D: val = 0xEFBCA0; // @
			break;
		case 0x813E: val = 0xC2A7; // S with squiggly
			break;
		case 0x813F: val = 0xE29CB0; // transparent star
			break;
		case 0x8140: val = 0xE29886; // white star
			break;
		case 0x8141: val = 0xE38087; // seikai, Madoka Kyono: maru!
			break;
		case 0x8142: val = 0xE2978B; // small circle
			break;
		case 0x8143: val = 0xE28CBE; // box? idk
			break;
		case 0x8144: val = 0xE29786; // transparent diamond
			break;
		case 0x8145: val = 0xE29787; // black diamond
			break;
		case 0x8146: val = 0xE296A0; // transparent square?
			break;
		case 0x8147: val = 0xE296A1; // square?
			break;
		case 0x8148: val = 0xE296B2; // transparent triangle
			break;
		case 0x8149: val = 0xE296B3; // black triangle
			break;
		case 0x814A: val = 0xE296BC; // downwards transparent triangle
			break;
		case 0x814B: val = 0xE296BD; // downwards triangle
			break;
		case 0x814C: val = 0xE280BB; // JP kome
			break;
		case 0x814D: val = 0xE38092; // JP yuubin
			break;
		case 0x814E: val = 0xE28692; // arrow pointing right
			break;
		case 0x814F: val = 0xE28690; // arrow pointing left
			break;
		case 0x8150: val = 0xE28691; // arrow pointing up
			break;
		case 0x8151: val = 0xE28693; // arrow pointing down
			break;
		case 0x8152: val = 0xE38093; // = thicker
			break;
		// I can't believe there's no symbol for music!
		
		// Kanji time, baby!
		case 0x8153: val = 0xE6B885; // Kiyo
			break;
		case 0x8154: val = 0xE9BABF; // maro
			break;
		case 0x8155: val = 0xE681B5; // Megumi
			break;
		case 0x8156: val = 0xE58D9A; // ?
			break;
		case 0x8157: val = 0xE5A3AB; // 
			break;
		case 0x8158: val = 0xE7A59E; // 
			break;
		case 0x8159: val = 0xE6AEBA; // 
			break;
		case 0x815A: val = 0xE6989F; // 
			break;
		case 0x815B: val = 0xE4BC8A; // 
			break;
		case 0x815C: val = 0xE7868A; // 
			break;
		case 0x815D: val = 0xE8998E; // 
			break;
		case 0x815E: val = 0xE9A39B; // 
			break;
		case 0x815F: val = 0xE5B7A5; // 
			break;
		case 0x8160: val = 0xE6A8B9; // 
			break;
		case 0x8161: val = 0xE6B0B4; // 
			break;
		case 0x8162: val = 0xE7B2BE; // 
			break;
		case 0x8163: val = 0xE9AD94; // 
			break;
		case 0x8164: val = 0xE789A9; // 
			break;
		case 0x8165: val = 0xE7B5B6; // 
			break;
		case 0x8166: val = 0xE6BB85; // 
			break;
		case 0x8167: val = 0xE5B19E; // 
			break;
		case 0x8168: val = 0xE680A7; // 
			break;
		case 0x8169: val = 0xE7B58C; // 
			break;
		case 0x816A: val = 0xE9A893; // 
			break;
		case 0x816B: val = 0xE580A4; // 
			break;
		case 0x816C: val = 0xE98791; // 
			break;
		case 0x816D: val = 0xE8BF91; // 
			break;
		case 0x816E: val = 0xE5AF84; // 
			break;
		case 0x816F: val = 0xE5BCB1; // 
			break;
		case 0x8170: val = 0xE5BCB7; // 
			break;
		case 0x8171: val = 0xE6B2B9; // 
			break;
		case 0x8172: val = 0xE696AD; // 
			break;
		case 0x8173: val = 0xE7A681; // 
			break;
		case 0x8174: val = 0xE6898B; // 
			break;
		case 0x8175: val = 0xE79BB8; // 
			break;
		case 0x8176: val = 0xE6B3A8; // 
			break;
		case 0x8177: val = 0xE6848F; // 
			break;
		case 0x8178: val = 0xE695B5; // 
			break;
		case 0x8179: val = 0xE6A0BC; // 
			break;
		case 0x817A: val = 0xE6AEB5; // 
			break;
		case 0x817B: val = 0xE69C80; // 
			break;
		case 0x817C: val = 0xE98080; // 
			break;
		case 0x817D: val = 0xE6B2BB; // 
			break;
		case 0x817E: val = 0xE682AA; // 
			break;
		case 0x817F: val = 0xE694BE; // 
			break;
		case 0x8180: val = 0xE58FA9; // 
			break;
		case 0x8181: val = 0xE6B097; // 
			break;
		case 0x8182: val = 0xE68C81; // 
			break;
		case 0x8183: val = 0xE7AA81; // 
			break;
		case 0x8184: val = 0xE8B5B0; // 
			break;
		case 0x8185: val = 0xE58D8A; // 
			break;
		case 0x8186: val = 0xE58886; // 
			break;
		case 0x8187: val = 0xE688A6; // 
			break;
		case 0x8188: val = 0xE7B8BE; // 
			break;
		case 0x8189: val = 0xE697A5; // 
			break;
		case 0x818A: val = 0xE8A898; // 
			break;
		case 0x818B: val = 0xE4BDBF; // 
			break;
		case 0x818C: val = 0xE8BEBC; // 
			break;
		case 0x818D: val = 0xE5AE8C; // 
			break;
		case 0x818E: val = 0xE68890; // 
			break;
		case 0x818F: val = 0xE5BE8C; // 
			break;
		case 0x8190: val = 0xE5B091; // 
			break;
		case 0x8191: val = 0xE585A8; // 
			break;
		case 0x8192: val = 0xE983A8; // 
			break;
		case 0x8193: val = 0xE4BA8B; // 
			break;
		case 0x8194: val = 0xE4BD93; // 
			break;
		case 0x8195: val = 0xE7B586; // 
			break;
		case 0x8196: val = 0xE58A9B; // 
			break;
		case 0x8197: val = 0xE59B9E; // 
			break;
		case 0x8198: val = 0xE5BEA9; // 
			break;
		case 0x8199: val = 0xE889B2; // 
			break;
		case 0x819A: val = 0xE8A8BC; // 
			break;
		case 0x819B: val = 0xE69982; // 
			break;
		case 0x819C: val = 0xE99693; // 
			break;
		case 0x819D: val = 0xE78E87; // 
			break;
		case 0x819E: val = 0xE5A4A7; // 
			break;
		case 0x819F: val = 0xE695B0; // 
			break;
		case 0x81A0: val = 0xE59088; // 
			break;
		case 0x81A1: val = 0xE7A3A8; // 
			break;
		case 0x81A2: val = 0xE5AE85; // 
			break;
		case 0x81A3: val = 0xE591A8; // 
			break;
		case 0x81A4: val = 0xE8BEBA; // 
			break;
		case 0x81A5: val = 0xE587BA; // 
			break;
		case 0x81A6: val = 0xE6B688; // 
			break;
		case 0x81A7: val = 0xE69CAC; // 
			break;
		case 0x81A8: val = 0xE5BD93; // 
			break;
		case 0x81A9: val = 0xE4B8AD; // 
			break;
		case 0x81AA: val = 0xE7B69A; // 
			break;
		case 0x81AB: val = 0xE981B8; // 
			break;
		case 0x81AC: val = 0xE68A9E; // 
			break;
		case 0x81AD: val = 0xE5BF83; // 
			break;
		case 0x81AE: val = 0xE88AB1; // 
			break;
		case 0x81AF: val = 0xE69E9C; // 
			break;
		case 0x81B0: val = 0xE5AE9F; // 
			break;
		case 0x81B1: val = 0xE5A48F; // 
			break;
		case 0x81B2: val = 0xE5A7AB; // 
			break;
		case 0x81B3: val = 0xE5A4A9; // 
			break;
		case 0x81B4: val = 0xE5B79D; // 
			break;
		case 0x81B5: val = 0xE58F8B; // 
			break;
		case 0x81B6: val = 0xE68385; // 
			break;
		case 0x81B7: val = 0xE786B1; // 
			break;
		case 0x81B8: val = 0xE5BFAB; // 
			break;
		case 0x81B9: val = 0xE8B6B3; // 
			break;
		case 0x81BA: val = 0xE79FAD; // 
			break;
		case 0x81BB: val = 0xE589A3; // 
			break;
		case 0x81BC: val = 0xE5BC93; // 
			break;
		case 0x81BD: val = 0xE79FA2; // 
			break;
		case 0x81BE: val = 0xE995B7; // 
			break;
		case 0x81BF: val = 0xE696A7; // 
			break;
		case 0x81C0: val = 0xE6A78D; // 
			break;
		case 0x81C1: val = 0xE58880; // 
			break;
		case 0x81C2: val = 0xE79FB3; // 
			break;
		case 0x81C3: val = 0xE5AE88; // 
			break;
		case 0x81C4: val = 0xE9899B; // 
			break;
		case 0x81C5: val = 0xE98A85; // 
			break;
		case 0x81C6: val = 0xE98A80; // 
			break;
		case 0x81C7: val = 0xE98984; // 
			break;
		case 0x81C8: val = 0xE98BBC; // 
			break;
		case 0x81C9: val = 0xE699B6; // 
			break;
		case 0x81CA: val = 0xE79C9F; // 
			break;
		case 0x81CB: val = 0xE6B1BA; // 
			break;
		case 0x81CC: val = 0xE4BC9A; // 
			break;
		case 0x81CD: val = 0xE799BA; // 
			break;
		case 0x81CE: val = 0xE5B195; // 
			break;
		case 0x81CF: val = 0xE8BA8D; // 
			break;
		case 0x81D0: val = 0xE587B8; // 
			break;
		case 0x81D1: val = 0xE587B9; // 
			break;
		case 0x81D2: val = 0xE584AA; // 
			break;
		case 0x81D3: val = 0xE78E8B; // 
			break;
		case 0x81D4: val = 0xE6A798; // 
			break;
		case 0x81D5: val = 0xE99BBB; // 
			break;
		case 0x81D6: val = 0xE69283; // 
			break;
		case 0x81D7: val = 0xE88196; // 
			break;
		case 0x81D8: val = 0xE6AF8D; // 
			break;
		case 0x81D9: val = 0xE5A489; // 
			break;
		case 0x81DA: val = 0xE58C96; // 
			break;
		case 0x81DB: val = 0xE9878D; // 
			break;
		case 0x81DC: val = 0xE79FA5; // 
			break;
		case 0x81DD: val = 0xE8AD98; // 
			break;
		case 0x81DE: val = 0xE6ADA6; // 
			break;
		case 0x81DF: val = 0xE98193; // 
			break;
		case 0x81E0: val = 0xE980B2; // 
			break;
		case 0x81E1: val = 0xE6B389; // 
			break;
		case 0x81E2: val = 0xE6B4BB; // 
			break;
		case 0x81E3: val = 0xE7A798; // 
			break;
		case 0x81E4: val = 0xE5A587; // 
			break;
		case 0x81E5: val = 0xE8B7A1; // 
			break;
		case 0x81E6: val = 0xE4BBB2; // 
			break;
		case 0x81E7: val = 0xE99BB7; // 
			break;
		case 0x81E8: val = 0xE799BD; // 
			break;
		case 0x81E9: val = 0xE7B499; // 
			break;
		case 0x81EA: val = 0xE4B883; // 
			break;
		case 0x81EB: val = 0xE5A495; // 
			break;
		case 0x81EC: val = 0xE7ACAC; // 
			break;
		case 0x81ED: val = 0xE585AD; // 
			break;
		case 0x81EE: val = 0xE6849F; // 
			break;
		case 0x81EF: val = 0xE9AD9A; // 
			break;
		case 0x81F0: val = 0xE581A5; // 
			break;
		case 0x81F1: val = 0xE5BAB7; // 
			break;
		case 0x81F2: val = 0xE5A085; // 
			break;
		case 0x81F3: val = 0xE8B3A2; // 
			break;
		case 0x81F4: val = 0xE58784; // 
			break;
		case 0x81F5: val = 0xE795AA; // 
			break;
		case 0x81F6: val = 0xE79BAE; // 
			break;
		case 0x81F7: val = 0xE68980; // 
			break;
		case 0x81F8: val = 0xE4B88D; // 
			break;
		case 0x81F9: val = 0xE58FAF; // 
			break;
		case 0x81FA: val = 0xE88F93; // Used in Kanchome's candy line
			break;
		case 0x81FB: val = 0xE5AD90; // 
			break;
		case 0x81FC: val = 0xE9A39F; // 
			break;
		case 0x81FD: val = 0xE4B9B3; // 
			break;
		case 0x81FE: val = 0xE985B8; // 
			break;
		case 0x81FF: val = 0xE88F8C; // 
			break;
		case 0x8200: val = 0xE9A3B2; // 
			break;
		case 0x8201: val = 0xE69699; // 
			break;
		case 0x8202: val = 0xE8A193; // 
			break;
		case 0x8203: val = 0xE795B0; // 
			break;
		case 0x8204: val = 0xE5B8B8; // 
			break;
		case 0x8205: val = 0xE887AA; // 
			break;
		case 0x8206: val = 0xE58B95; // 
			break;
		case 0x8207: val = 0xE79A84; // 
			break;
		case 0x8208: val = 0xE6B89B; // 
			break;
		case 0x8209: val = 0xE694BB; // 
			break;
		case 0x820A: val = 0xE593A1; // 
			break;
		case 0x820B: val = 0xE4B880; // 
			break;
		case 0x820C: val = 0xE5AE9A; // 
			break;
		case 0x820D: val = 0xE8A68B; // 
			break;
		case 0x820E: val = 0xE4BABA; // 
			break;
		case 0x820F: val = 0xE7A7BB; // 
			break;
		case 0x8210: val = 0xE9809F; // 
			break;
		case 0x8211: val = 0xE5BAA6; // 
			break;
		case 0x8212: val = 0xE4B88A; // 
			break;
		case 0x8213: val = 0xE9858D; // 
			break;
		case 0x8214: val = 0xE890BD; // 
			break;
		case 0x8215: val = 0xE794BA; // 
			break;
		case 0x8216: val = 0xE5B882; // 
			break;
		case 0x8217: val = 0xE8A197; // 
			break;
		case 0x8218: val = 0xE59CB0; // 
			break;
		case 0x8219: val = 0xE5A696; // 
			break;
		case 0x821A: val = 0xE5B2A9; // 
			break;
		case 0x821B: val = 0xE5B3B6; // 
			break;
		case 0x821C: val = 0xE6A3AE; // 
			break;
		case 0x821D: val = 0xE591AA; // 
			break;
		case 0x821E: val = 0xE5B1B1; // 
			break;
		case 0x821F: val = 0xE894B5; // 
			break;
		case 0x8220: val = 0xE6B8AF; // 
			break;
		case 0x8221: val = 0xE59586; // 
			break;
		case 0x8222: val = 0xE5BA97; // 
			break;
		case 0x8223: val = 0xE4BD8F; // 
			break;
		case 0x8224: val = 0xE9AB98; // Taka, one of the few kanji I know--Sara Takatsuki <3
			break;
		case 0x8225: val = 0xE5B6BA; // 
			break;
		case 0x8226: val = 0xE5B180; // 
			break;
		case 0x8227: val = 0xE79C8C; // 
			break;
		case 0x8228: val = 0xE7AB8B; // 
			break;
		case 0x8229: val = 0xE59BB3; // 
			break;
		case 0x822A: val = 0xE69BB8; // 
			break;
		case 0x822B: val = 0xE9A4A8; // 
			break;
		case 0x822C: val = 0xE6B5B7; // 
			break;
		case 0x822D: val = 0xE5B2B8; // 
			break;
		case 0x822E: val = 0xE7B79A; // 
			break;
		case 0x822F: val = 0xE9A082; // 
			break;
		case 0x8230: val = 0xE5B081; // 
			break;
		case 0x8231: val = 0xE7A9B4; // 
			break;
		case 0x8232: val = 0xE8BFBD; // 
			break;
		case 0x8233: val = 0xE686B6; // 
			break;
		case 0x8234: val = 0xE6B996; // 
			break;
		case 0x8235: val = 0xE79594; // 
			break;
		case 0x8236: val = 0xE4BB84; // 
			break;
		case 0x8237: val = 0xE69A97; // 
			break;
		case 0x8238: val = 0xE99A8E; // 
			break;
		case 0x8239: val = 0xE4BA8C; // 
			break;
		case 0x823A: val = 0xE4B889; // 
			break;
		case 0x823B: val = 0xE4B8B8; // 
			break;
		case 0x823C: val = 0xE9A2A8; // 
			break;
		case 0x823D: val = 0xE5AF86; // 
			break;
		case 0x823E: val = 0xE59FBA; // 
			break;
		case 0x823F: val = 0xE5ADA6; // 
			break;
		case 0x8240: val = 0xE6A0A1; // 
			break;
		case 0x8241: val = 0xE6A39F; // 
			break;
		case 0x8242: val = 0xE58F82; // 
			break;
		case 0x8243: val = 0xE696B0; // 
			break;
		case 0x8244: val = 0xE8A69A; // 
			break;
		case 0x8245: val = 0xE5A0B4; // 
			break;
		case 0x8246: val = 0xE7A781; // 
			break;
		case 0x8247: val = 0xE98194; // 
			break;
		case 0x8248: val = 0xE5B496; // 
			break;
		case 0x8249: val = 0xE69DA5; // 
			break;
		case 0x824A: val = 0xE4BB8A; // 
			break;
		case 0x824B: val = 0xE58395; // 
			break;
		case 0x824C: val = 0xE6B8A1; // 
			break;
		case 0x824D: val = 0xE8A18C; // 
			break;
		case 0x824E: val = 0xE585A5; // 
			break;
		case 0x824F: val = 0xE794BB; // 
			break;
		case 0x8250: val = 0xE99DA2; // 
			break;
		case 0x8251: val = 0xE9968B; // 
			break;
		case 0x8252: val = 0xE8AC8E; // 
			break;
		case 0x8253: val = 0xE58AA0; // 
			break;
		case 0x8254: val = 0xE5A4B1; // 
			break;
		case 0x8255: val = 0xE69597; // 
			break;
		case 0x8256: val = 0xE58588; // 
			break;
		case 0x8257: val = 0xE588A9; // 
			break;
		case 0x8258: val = 0xE794A8; // 
			break;
		case 0x8259: val = 0xE5BDB9; // 
			break;
		case 0x825A: val = 0xE5A0B1; // 
			break;
		case 0x825B: val = 0xE696B9; // 
			break;
		case 0x825C: val = 0xE6B395; // 
			break;
		case 0x825D: val = 0xE4BC9D; // 
			break;
		case 0x825E: val = 0xE6809D; // 
			break;
		case 0x825F: val = 0xE697A9; // 
			break;
		case 0x8260: val = 0xE69E9A; // 
			break;
		case 0x8261: val = 0xE8A385; // 
			break;
		case 0x8262: val = 0xE58299; // 
			break;
		case 0x8263: val = 0xE4BD95; // 
			break;
		case 0x8264: val = 0xE58AB9; // 
			break;
		case 0x8265: val = 0xE784A1; // 
			break;
		case 0x8266: val = 0xE980A0; // 
			break;
		case 0x8267: val = 0xE7B584; // 
			break;
		case 0x8268: val = 0xE69CAA; // 
			break;
		case 0x8269: val = 0xE99191; // 
			break;
		case 0x826A: val = 0xE78AB6; // 
			break;
		case 0x826B: val = 0xE6858B; // 
			break;
		case 0x826C: val = 0xE5BF98; // 
			break;
		case 0x826D: val = 0xE5889D; // 
			break;
		case 0x826E: val = 0xE889AF; // 
			break;
		case 0x826F: val = 0xE7A2BA; // 
			break;
		case 0x8270: val = 0xE8AA8D; // 
			break;
		case 0x8271: val = 0xE4BEA1; // 
			break;
		case 0x8272: val = 0xE7B4A0; // 
			break;
		case 0x8273: val = 0xE69D90; // 
			break;
		case 0x8274: val = 0xE8AAAC; // 
			break;
		case 0x8275: val = 0xE6988E; // 
			break;
		case 0x8276: val = 0xE4BD9C; // 
			break;
		case 0x8277: val = 0xE6ACA0; // 
			break;
		case 0x8278: val = 0xE591BC; // 
			break;
		case 0x8279: val = 0xE789B9; // 
			break;
		case 0x827A: val = 0xE4BE8B; // 
			break;
		case 0x827B: val = 0xE585B7; // 
			break;
		case 0x827C: val = 0xE59084; // 
			break;
		case 0x827D: val = 0xE782BA; // 
			break;
		case 0x827E: val = 0xE6ADA9; // 
			break;
		case 0x827F: val = 0xE9A086; // 
			break;
		case 0x8280: val = 0xE8AABF; // 
			break;
		case 0x8281: val = 0xE99B86; // 
			break;
		case 0x8282: val = 0xE6A5BD; // 
			break;
		case 0x8283: val = 0xE782B9; // 
			break;
		case 0x8284: val = 0xE78886; // 
			break;
		case 0x8285: val = 0xE8A7A6; // 
			break;
		case 0x8286: val = 0xE58F97; // 
			break;
		case 0x8287: val = 0xE5898D; // 
			break;
		case 0x8288: val = 0xE58583; // 
			break;
		case 0x8289: val = 0xE5AD98; // 
			break;
		case 0x828A: val = 0xE99990; // 
			break;
		case 0x828B: val = 0xE4BBA5; // 
			break;
		case 0x828C: val = 0xE5A496; // 
			break;
		case 0x828D: val = 0xE68EA2; // 
			break;
		case 0x828E: val = 0xE3808A; // <<
			break;
		case 0x828F: val = 0xE3808B; // >>
			break;
		case 0x8290: val = 0xE5AB8C; // 
			break;
		case 0x8291: val = 0xE6A99F; // 
			break;
		case 0x8292: val = 0xE8A9A6; // 
			break;
		case 0x8293: val = 0xE79982; // 
			break;
		case 0x8294: val = 0xE58AA9; // 
			break;
		case 0x8295: val = 0xE5A297; // 
			break;
		case 0x8296: val = 0xE5B985; // 
			break;
		case 0x8297: val = 0xE5BA83; // 
			break;
		case 0x8298: val = 0xE68C91; // 
			break;
		case 0x8299: val = 0xE4BB96; // 
			break;
		case 0x829A: val = 0xE5BDB1; // 
			break;
		case 0x829B: val = 0xE99FBF; // 
			break;
		case 0x829C: val = 0xE5B9B2; // 
			break;
		case 0x829D: val = 0xE6B889; // 
			break;
		case 0x829E: val = 0xE5A5BD; // 
			break;
		case 0x829F: val = 0xE59F9F; // 
			break;
		case 0x82A0: val = 0xE68F83; // 
			break;
		case 0x82A1: val = 0xE5A881; // 
			break;
		case 0x82A2: val = 0xE8BBBD; // 
			break;
		case 0x82A3: val = 0xE5BE97; // 
			break;
		case 0x82A4: val = 0xE58092; // 
			break;
		case 0x82A5: val = 0xE79086; // 
			break;
		case 0x82A6: val = 0xE8A7A3; // 
			break;
		case 0x82A7: val = 0xE99AA0; // 
			break;
		case 0x82A8: val = 0xE58887; // 
			break;
		case 0x82A9: val = 0xE99BA2; // 
			break;
		case 0x82AA: val = 0xE58E84; // 
			break;
		case 0x82AB: val = 0xE4BB8B; // 
			break;
		case 0x82AC: val = 0xE8B5B7; // 
			break;
		case 0x82AD: val = 0xE5908D; // 
			break;
		case 0x82AE: val = 0xE9809A; // 
			break;
		case 0x82AF: val = 0xE5B9B8; // 
			break;
		case 0x82B0: val = 0xE9818B; // 
			break;
		case 0x82B1: val = 0xE996A2; // 
			break;
		case 0x82B2: val = 0xE980A3; // 
			break;
		case 0x82B3: val = 0xE5908C; // 
			break;
		case 0x82B4: val = 0xE4BF82; // 
			break;
		case 0x82B5: val = 0xE4B896; // 
			break;
		case 0x82B6: val = 0xE7958C; // 
			break;
		case 0x82B7: val = 0xE78FBE; // 
			break;
		case 0x82B8: val = 0xE8B2BB; // 
			break;
		case 0x82B9: val = 0xE9878F; // 
			break;
		case 0x82BA: val = 0xE699B4; // 
			break;
		case 0x82BB: val = 0xE5A7BF; // 
			break;
		case 0x82BC: val = 0xE981AD; // 
			break;
		case 0x82BD: val = 0xE98187; // OCR always gives me 0xE9818E, but I don't think that's correct
			break;
		case 0x82BE: val = 0xE5AEBF; // 
			break;
		case 0x82BF: val = 0xE4BEBF; // 
			break;
		case 0x82C0: val = 0xE5B08F; // 
			break;
		case 0x82C1: val = 0xE8A382; // 
			break;
		case 0x82C2: val = 0xE5808D; // 
			break;
		case 0x82C3: val = 0xE7B3BB; // 
			break;
		case 0x82C4: val = 0xE98086; // 
			break;
		case 0x82C5: val = 0xE7A0B4; // 
			break;
		case 0x82C6: val = 0xE7A8AE; // 
			break;
		case 0x82C7: val = 0xE9A19E; // 
			break;
		case 0x82C8: val = 0xE99A8A; // 
			break;
		case 0x82C9: val = 0xE8B2B8; // 
			break;
		case 0x82CA: val = 0xE79A86; // 
			break;
		case 0x82CB: val = 0xE8A880; // 
			break;
		case 0x82CC: val = 0xE5BF99; // 
			break;
		case 0x82CD: val = 0xE591B3; // 
			break;
		case 0x82CE: val = 0xE5AEB6; // 
			break;
		case 0x82CF: val = 0xE7949F; // 
			break;
		case 0x82D0: val = 0xE59381; // 
			break;
		case 0x82D1: val = 0xE8AAAD; // 
			break;
		case 0x82D2: val = 0xE6B3A3; // 
			break;
		case 0x82D3: val = 0xE59089; // 
			break;
		case 0x82D4: val = 0xE587B6; // 
			break;
		case 0x82D5: val = 0xE58DA0; // 
			break;
		case 0x82D6: val = 0xE5B8AB; // 
			break;
		case 0x82D7: val = 0xE6ADA3; // 
			break;
		case 0x82D8: val = 0xE7BEA9; // 
			break;
		case 0x82D9: val = 0xE694AF; // 
			break;
		case 0x82DA: val = 0xE5BE81; // 
			break;
		case 0x82DB: val = 0xE69C8D; // 
			break;
		case 0x82DC: val = 0xE8BB8D; // 
			break;
		case 0x82DD: val = 0xE59BA3; // 
			break;
		case 0x82DE: val = 0xE5B9B3; // 
			break;
		case 0x82DF: val = 0xE5928C; // 
			break;
		case 0x82E0: val = 0xE79083; // 
			break;
		case 0x82E1: val = 0xE7AC91; // 
			break;
		case 0x82E2: val = 0xE5B086; // 
			break;
		case 0x82E3: val = 0xE69687; // 
			break;
		case 0x82E4: val = 0xE594B1; // 
			break;
		case 0x82E5: val = 0xE5BE85; // 
			break;
		case 0x82E6: val = 0xE4B8BB; // 
			break;
		case 0x82E7: val = 0xE98185; // 
			break;
		case 0x82E8: val = 0xE8AAB0; // 
			break;
		case 0x82E9: val = 0xE69A91; // 
			break;
		case 0x82EA: val = 0xE5A49A; // 
			break;
		case 0x82EB: val = 0xE88085; // 
			break;
		case 0x82EC: val = 0xE4B888; // 
			break;
		case 0x82ED: val = 0xE5A4AB; // 
			break;
		case 0x82EE: val = 0xE5A5B3; // 
			break;
		case 0x82EF: val = 0xE6B585; // 
			break;
		case 0x82F0: val = 0xE6B0B8; // 
			break;
		case 0x82F1: val = 0xE58AAB; // 
			break;
		case 0x82F2: val = 0xE59BB2; // 
			break;
		case 0x82F3: val = 0xE7B792; // 
			break;
		case 0x82F4: val = 0xE8A5B2; // 
			break;
		case 0x82F5: val = 0xE69CAD; // 
			break;
		case 0x82F6: val = 0xE7B590; // 
			break;
		case 0x82F7: val = 0xE5AFBE; // 
			break;
		case 0x82F8: val = 0xE68A97; // 
			break;
		case 0x82F9: val = 0xE5BF85; // 
			break;
		case 0x82FA: val = 0xE8A681; // 
			break;
		case 0x82FB: val = 0xE98195; // 
			break;
		case 0x82FC: val = 0xE8819E; // 
			break;
		case 0x82FD: val = 0xE6ADA2; // 
			break;
		case 0x82FE: val = 0xE58D81; // 
			break;
		case 0x82FF: val = 0xE5AD97; // 
			break;
		case 0x8300: val = 0xE5B7A6; // 
			break;
		case 0x8301: val = 0xE58FB3; // 
			break;
		case 0x8302: val = 0xE69BBF; // 
			break;
		case 0x8303: val = 0xE4B88B; // 
			break;
		case 0x8304: val = 0xE8A1A8; // 
			break;
		case 0x8305: val = 0xE7A4BA; // 
			break;
		case 0x8306: val = 0xE6ACA1; // 
			break;
		case 0x8307: val = 0xE883BD; // 
			break;
		case 0x8308: val = 0xE589B2; // 
			break;
		case 0x8309: val = 0xE6BC94; // 
			break;
		case 0x830A: val = 0xE68993; // 
			break;
		case 0x830B: val = 0xE5BEB4; // 
			break;
		case 0x830C: val = 0xE79EAC; // 
			break;
		case 0x830D: val = 0xE58EBB; // 
			break;
		case 0x830E: val = 0xE5808B; // 
			break;
		case 0x830F: val = 0xE8A8AD; // 
			break;
		case 0x8310: val = 0xE7B582; // 
			break;
		case 0x8311: val = 0xE4BA86; // 
			break;
		case 0x8312: val = 0xE688BB; // 
			break;
		case 0x8313: val = 0xE58699; // 
			break;
		case 0x8314: val = 0xE698BC; // 
			break;
		case 0x8315: val = 0xE7B484; // 
			break;
		case 0x8316: val = 0xE69D9F; // 
			break;
		case 0x8317: val = 0xE79D80; // 
			break;
		case 0x8318: val = 0xE4BB98; // 
			break;
		case 0x8319: val = 0xE5B18B; // 
			break;
		case 0x831A: val = 0xE88090; // 
			break;
		case 0x831B: val = 0xE6AEBF; // 
			break;
		case 0x831C: val = 0xE68EA7; // 
			break;
		case 0x831D: val = 0xE5AEA4; // 
			break;
		case 0x831E: val = 0xE8BFB7; // 
			break;
		case 0x831F: val = 0xE8B7AF; // 
			break;
		case 0x8320: val = 0xE78AAF; // 
			break;
		case 0x8321: val = 0xE7BDAA; // 
			break;
		case 0x8322: val = 0xE68BA0; // 
			break;
		case 0x8323: val = 0xE683B3; // 
			break;
		case 0x8324: val = 0xE9878E; // 
			break;
		case 0x8325: val = 0xE59091; // 
			break;
		case 0x8326: val = 0xE698A8; // 
			break;
		case 0x8327: val = 0xE78B99; // 
			break;
		case 0x8328: val = 0xE8A9B1; // 
			break;
		case 0x8329: val = 0xE6B381; // 
			break;
		case 0x832A: val = 0xE68A9C; // 
			break;
		case 0x832B: val = 0xE58FB0; // 
			break;
		case 0x832C: val = 0xE9A194; // 
			break;
		case 0x832D: val = 0xE4BCBC; // 
			break;
		case 0x832E: val = 0xE9A198; // 
			break;
		case 0x832F: val = 0xE7BDAE; // 
			break;
		case 0x8330: val = 0xE590B8; // 
			break;
		case 0x8331: val = 0xE58F96; // 
			break;
		case 0x8332: val = 0xE69C9B; // 
			break;
		case 0x8333: val = 0xE88BA6; // 
			break;
		case 0x8334: val = 0xE794A3; // 
			break;
		case 0x8335: val = 0xE8A6AA; // 
			break;
		case 0x8336: val = 0xE98083; // 
			break;
		case 0x8337: val = 0xE588A5; // 
			break;
		case 0x8338: val = 0xE6ADBB; // 
			break;
		case 0x8339: val = 0xE78783; // 
			break;
		case 0x833A: val = 0xE4BFA1; // 
			break;
		case 0x833B: val = 0xE59CA8; // 
			break;
		case 0x833C: val = 0xE99A9B; // 
			break;
		case 0x833D: val = 0xE5AE89; // 
			break;
		case 0x833E: val = 0xE9A085; // 
			break;
		case 0x833F: val = 0xE68ABC; // 
			break;
		case 0x8340: val = 0xE5B7AE; // 
			break;
		case 0x8341: val = 0xE5BF9C; // 
			break;
		case 0x8342: val = 0xE4BAA4; // 
			break;
		case 0x8343: val = 0xE68F9B; // 
			break;
		case 0x8344: val = 0xE8A9B3; // 
			break;
		case 0x8345: val = 0xE6A78B; // 
			break;
		case 0x8346: val = 0xE7A9BA; // 
			break;
		case 0x8347: val = 0xE5B084; // 
			break;
		case 0x8348: val = 0xE79BB4; // 
			break;
		case 0x8349: val = 0xE68EA5; // 
			break;
		case 0x834A: val = 0xE682A9; // 
			break;
		case 0x834B: val = 0xE9818A; // 
			break;
		case 0x834C: val = 0xE585AC; // 
			break;
		case 0x834D: val = 0xE59C92; // 
			break;
		case 0x834E: val = 0xE8BFAB; // 
			break;
		case 0x834F: val = 0xE794B7; // 
			break;
		case 0x8350: val = 0xE5A78B; // 
			break;
		case 0x8351: val = 0xE99D99; // 
			break;
		case 0x8352: val = 0xE8A6B3; // 
			break;
		case 0x8353: val = 0xE5AEA2; // 
			break;
		case 0x8354: val = 0xE58CBA; // 
			break;
		case 0x8355: val = 0xE6AE8B; // 
			break;
		case 0x8356: val = 0xE5A3B2; // 
			break;
		case 0x8357: val = 0xE8A8B1; // 
			break;
		case 0x8358: val = 0xE59B9B; // 
			break;
		case 0x8359: val = 0xE5A293; // 
			break;
		case 0x835A: val = 0xE5918A; // 
			break;
		case 0x835B: val = 0xE58FA3; // 
			break;
		case 0x835C: val = 0xE585B1; // 
			break;
		case 0x835D: val = 0xE59C9F; // 
			break;
		case 0x835E: val = 0xE981A0; // 
			break;
		case 0x835F: val = 0xE58D94; // 
			break;
		case 0x8360: val = 0xE5B185; // 
			break;
		case 0x8361: val = 0xE884B1; // 
			break;
		case 0x8362: val = 0xE59BBD; // 
			break;
		case 0x8363: val = 0xE88B97; // 
			break;
		case 0x8364: val = 0xE89189; // 
			break;
		case 0x8365: val = 0xE6BCA2; // 
			break;
		case 0x8366: val = 0xE685A2; // 
			break;
		case 0x8367: val = 0xE68D9C; // 
			break;
		case 0x8368: val = 0xE68E9B; // 
			break;
		case 0x8369: val = 0xE6A188; // 
			break;
		case 0x836A: val = 0xE8ADA6; // 
			break;
		case 0x836B: val = 0xE5AF9F; // 
			break;
		case 0x836C: val = 0xE4BBAE; // 
			break;
		case 0x836D: val = 0xE98CB2; // 
			break;
		case 0x836E: val = 0xE99FB3; // 
			break;
		case 0x836F: val = 0xE880B3; // 
			break;
		case 0x8370: val = 0xE59BB0; // 
			break;
		case 0x8371: val = 0xE7AD94; // 
			break;
		case 0x8372: val = 0xE5BFB5; // 
			break;
		case 0x8373: val = 0xE99C8A; // 
			break;
		case 0x8374: val = 0xE6A5AD; // 
			break;
		case 0x8375: val = 0xE680A8; // 
			break;
		case 0x8376: val = 0xE78D84; // 
			break;
		case 0x8377: val = 0xE58FAC; // 
			break;
		case 0x8378: val = 0xE7A0B2; // 
			break;
		case 0x8379: val = 0xE4B985; // 
			break;
		case 0x837A: val = 0xE9ACBC; // 
			break;
		case 0x837B: val = 0xE596B0; // 
			break;
		case 0x837C: val = 0xE5969A; // 
			break;
		case 0x837D: val = 0xE68891; // 
			break;
		case 0x837E: val = 0xE7A4BC; // 
			break;
		case 0x837F: val = 0xE68F90; // 
			break;
		case 0x8380: val = 0xE4BA92; // 
			break;
		case 0x8381: val = 0xE799BE; // 
			break;
		case 0x8382: val = 0xE7BEA4; // 
			break;
		case 0x8383: val = 0xE9BB84; // 
			break;
		case 0x8384: val = 0xE5809F; // 
			break;
		case 0x8385: val = 0xE58099; // 
			break;
		case 0x8386: val = 0xE8A39C; // 
			break;
		case 0x8387: val = 0xE8B2A0; // 
			break;
		case 0x8388: val = 0xE68294; // 
			break;
		case 0x8389: val = 0xE4BB95; // 
			break;
		case 0x838A: val = 0xE8BF94; // 
			break;
		case 0x838B: val = 0xE794B1; // 
			break;
		case 0x838C: val = 0xE8B5A4; // 
			break;
		case 0x838D: val = 0xE68989; // 
			break;
		case 0x838E: val = 0xE4BC91; // 
			break;
		case 0x838F: val = 0xE686A9; // 
			break;
		case 0x8390: val = 0xE88083; // 
			break;
		case 0x8391: val = 0xE79498; // 
			break;
		case 0x8392: val = 0xE5B8B0; // 
			break;
		case 0x8393: val = 0xE8BAAB; // 
			break;
		case 0x8394: val = 0xE784B6; // 
			break;
		case 0x8395: val = 0xE79599; // 
			break;
		case 0x8396: val = 0xE99D92; // 
			break;
		case 0x8397: val = 0xE4BBBB; // 
			break;
		case 0x8398: val = 0xE58585; // 
			break;
		case 0x8399: val = 0xE586A5; // 
			break;
		case 0x839A: val = 0xE69BB4; // 
			break;
		case 0x839B: val = 0xE5A5A5; // 
			break;
		case 0x839C: val = 0xE58B9D; // 
			break;
		case 0x839D: val = 0xE8B8AA; // 
			break;
		case 0x839E: val = 0xE4BBB6; // 
			break;
		case 0x839F: val = 0xE68190; // 
			break;
		case 0x83A0: val = 0xE69591; // 
			break;
		case 0x83A1: val = 0xE68C87; // 
			break;
		case 0x83A2: val = 0xE6B182; // 
			break;
		case 0x83A3: val = 0xE68DA8; // 
			break;
		case 0x83A4: val = 0xE9A0BC; // 
			break;
		case 0x83A5: val = 0xE7B5A1; // 
			break;
		case 0x83A6: val = 0xE7B994; // 
			break;
		case 0x83A7: val = 0xE99689; // 
			break;
		case 0x83A8: val = 0xE789A2; // 
			break;
		case 0x83A9: val = 0xE8AB87; // 
			break;
		case 0x83AA: val = 0xE58685; // 
			break;
		case 0x83AB: val = 0xE9A699; // 
			break;
		case 0x83AC: val = 0xE581B4; // 
			break;
		case 0x83AD: val = 0xE58692; // 
			break;
		case 0x83AE: val = 0xE999BA; // 
			break;
		case 0x83AF: val = 0xE5BC95; // 
			break;
		case 0x83B0: val = 0xE5868D; // 
			break;
		case 0x83B1: val = 0xE6B3B3; // 
			break;
		case 0x83B2: val = 0xE8B29D; // 
			break;
		case 0x83B3: val = 0xE8B2A1; // 
			break;
		case 0x83B4: val = 0xE5AE9D; // 
			break;
		case 0x83B5: val = 0xE599A8; // 
			break;
		case 0x83B6: val = 0xE58089; // 
			break;
		case 0x83B7: val = 0xE5BAAB; // 
			break;
		case 0x83B8: val = 0xE7B0A1; // 
			break;
		case 0x83B9: val = 0xE58D98; // 
			break;
		case 0x83BA: val = 0xE59B9A; // 
			break;
		case 0x83BB: val = 0xE5BCB5; // 
			break;
		case 0x83BC: val = 0xE5B188; // 
			break;
		case 0x83BD: val = 0xE6898D; // 
			break;
		case 0x83BE: val = 0xE58590; // 
			break;
		case 0x83BF: val = 0xE5A3B0; // 
			break;
		case 0x83C0: val = 0xE8ADB0; // 
			break;
		case 0x83C1: val = 0xE5909B; // 
			break;
		case 0x83C2: val = 0xE69599; // 
			break;
		case 0x83C3: val = 0xE5A5AA; // 
			break;
		case 0x83C4: val = 0xE9A18C; // 
			break;
		case 0x83C5: val = 0xE5AEB9; // 
			break;
		case 0x83C6: val = 0xE6BF83; // 
			break;
		case 0x83C7: val = 0xE58F8D; // 
			break;
		case 0x83C8: val = 0xE698A0; // 
			break;
		case 0x83C9: val = 0xE8A487; // 
			break;
		case 0x83CA: val = 0xE4BD8E; // 
			break;
		case 0x83CB: val = 0xE78987; // 
			break;
		case 0x83CC: val = 0xE680A5; // 
			break;
		case 0x83CD: val = 0xE58F8E; // 
			break;
		case 0x83CE: val = 0xE69DA1; // 
			break;
		case 0x83CF: val = 0xE685AE; // 
			break;
		case 0x83D0: val = 0xE788B6; // 
			break;
		case 0x83D1: val = 0xE58FA4; // 
			break;
		case 0x83D2: val = 0xE78CAE; // 
			break;
		case 0x83D3: val = 0xE7B4A2; // 
			break;
		case 0x83D4: val = 0xE887B3; // 
			break;
		case 0x83D5: val = 0xE79BA3; // 
			break;
		case 0x83D6: val = 0xE8A696; // 
			break;
		case 0x83D7: val = 0xE4BD90; // 
			break;
		case 0x83D8: val = 0xE88F9C; // 
			break;
		case 0x83D9: val = 0xE9A088; // 
			break;
		case 0x83DA: val = 0xE5B9B4; // 
			break;
		case 0x83DB: val = 0xE9BDA2; // 
			break;
		case 0x83DC: val = 0xE6ADB3; // 
			break;
		case 0x83DD: val = 0xE7BE8E; // 
			break;
		case 0x83DE: val = 0xE5BDBC; // 
			break;
		case 0x83DF: val = 0xE7A88B; // 
			break;
		case 0x83E0: val = 0xE4B8A1; // 
			break;
		case 0x83E1: val = 0xE8B68A; // 
			break;
		case 0x83E2: val = 0xE587A6; // 
			break;
		case 0x83E3: val = 0xE8B88A; // 
			break;
		case 0x83E4: val = 0xE6B5AE; // 
			break;
		case 0x83E5: val = 0xE8A888; // 
			break;
		case 0x83E6: val = 0xE59F8E; // 
			break;
		case 0x83E7: val = 0xE8A2AB; // 
			break;
		case 0x83E8: val = 0xE5BBB6; // 
			break;
		case 0x83E9: val = 0xE591BD; // 
			break;
		case 0x83EA: val = 0xE4BBA4; // 
			break;
		case 0x83EB: val = 0xE5B4A9; // 
			break;
		case 0x83EC: val = 0xE5BDA2; // 
			break;
		case 0x83ED: val = 0xE6858E; // 
			break;
		case 0x83EE: val = 0xE68F8F; // 
			break;
		case 0x83EF: val = 0xE7B5B5; // 
			break;
		case 0x83F0: val = 0xE5958F; // 
			break;
		case 0x83F1: val = 0xE781BD; // 
			break;
		case 0x83F2: val = 0xE99BA3; // 
			break;
		case 0x83F3: val = 0xE5BFA0; // 
			break;
		case 0x83F4: val = 0xE6B581; // 
			break;
		case 0x83F5: val = 0xE983BD; // 
			break;
		case 0x83F6: val = 0xE59BBA; // 
			break;
		case 0x83F7: val = 0xE6A899; // 
			break;
		case 0x83F8: val = 0xE8A88E; // 
			break;
		case 0x83F9: val = 0xE8829D; // 
			break;
		case 0x83FA: val = 0xE79CA0; // 
			break;
		case 0x83FB: val = 0xE58584; // 
			break;
		case 0x83FC: val = 0xE9A090; // 
			break;
		case 0x83FD: val = 0xE8B2B4; // 
			break;
		case 0x83FE: val = 0xE796B2; // 
			break;
		case 0x83FF: val = 0xE692A4; // 
			break;
		case 0x8400: val = 0xE69BB2; // 
			break;
		case 0x8401: val = 0xE981BF; // 
			break;
		case 0x8402: val = 0xE58AB4; // 
			break;
		case 0x8403: val = 0xE78AA0; // 
			break;
		case 0x8404: val = 0xE789B2; // 
			break;
		case 0x8405: val = 0xE7AE87; // 
			break;
		case 0x8406: val = 0xE58DB8; // 
			break;
		case 0x8407: val = 0xE8BC89; // 
			break;
		case 0x8408: val = 0xE58E9F; // 
			break;
		case 0x8409: val = 0xE6B08F; // 
			break;
		case 0x840A: val = 0xE58589; // 
			break;
		case 0x840B: val = 0xE6A084; // 
			break;
		case 0x840C: val = 0xE8B2B7; // 
			break;
		case 0x840D: val = 0xE8BBA2; // 
			break;
		case 0x840E: val = 0xE8BB8A; // 
			break;
		case 0x840F: val = 0xE881B7; // 
			break;
		case 0x8410: val = 0xE6A8A9; // 
			break;
		case 0x8411: val = 0xE4B9B1; // 
			break;
		case 0x8412: val = 0xE5B7BB; // 
			break;
		case 0x8413: val = 0xE5AEB3; // 
			break;
		case 0x8414: val = 0xE799BB; // 
			break;
		case 0x8415: val = 0xE9A696; // 
			break;
		case 0x8416: val = 0xE8AC80; // 
			break;
		case 0x8417: val = 0xE8A19B; // 
			break;
		case 0x8418: val = 0xE68A95; // 
			break;
		case 0x8419: val = 0xE7A8BF; // 
			break;
		case 0x841A: val = 0xE69CAB; // 
			break;
		case 0x841B: val = 0xE69DB1; // 
			break;
		case 0x841C: val = 0xE4BAAC; // 
			break;
		case 0x841D: val = 0xE58B89; // 
			break;
		case 0x841E: val = 0xE69C89; // 
			break;
		case 0x841F: val = 0xE6938D; // 
			break;
		case 0x8420: val = 0xE68AB1; // 
			break;
		case 0x8421: val = 0xE680AF; // 
			break;
		case 0x8422: val = 0xE69C88; // 
			break;
		case 0x8423: val = 0xE4B997; // 
			break;
		case 0x8424: val = 0xE8BCAA; // 
			break;
		case 0x8425: val = 0xE6B49E; // 
			break;
		case 0x8426: val = 0xE7AA9F; // 
			break;
		case 0x8427: val = 0xE6BAB6; // 
			break;
		case 0x8428: val = 0xE6849A; // 
			break;
		case 0x8429: val = 0xE7AD96; // 
			break;
		case 0x842A: val = 0xE88097; // 
			break;
		case 0x842B: val = 0xE5B88C; // 
			break;
		case 0x842C: val = 0xE59684; // 
			break;
		case 0x842D: val = 0xE6AF9B; // 
			break;
		case 0x842E: val = 0xE69C9F; // 
			break;
		case 0x842F: val = 0xE694B9; // 
			break;
		case 0x8430: val = 0xE794B3; // 
			break;
		case 0x8431: val = 0xE8A8B3; // 
			break;
		case 0x8432: val = 0xE69FBB; // 
			break;
		case 0x8433: val = 0xE782AD; // 
			break;
		case 0x8434: val = 0xE989B1; // 
			break;
		case 0x8435: val = 0xE98D94; // 
			break;
		case 0x8436: val = 0xE7AB9C; // 
			break;
		case 0x8437: val = 0xE68BBF; // 
			break;
		case 0x8438: val = 0xE99683; // 
			break;
		case 0x8439: val = 0xE6829F; // 
			break;
		case 0x843A: val = 0xE588A4; // 
			break;
		case 0x843B: val = 0xE5BBBA; // 
			break;
		case 0x843C: val = 0xE590B9; // 
			break;
		case 0x843D: val = 0xE884B3; // 
			break;
		case 0x843E: val = 0xE5838D; // 
			break;
		case 0x843F: val = 0xE5B9BB; // 
			break;
		case 0x8440: val = 0xE7B5AE; // 
			break;
		case 0x8441: val = 0xE4B88E; // 
			break;
		case 0x8442: val = 0xE5B08E; // 
			break;
		case 0x8443: val = 0xE9998D; // 
			break;
		case 0x8444: val = 0xE5A082; // 
			break;
		case 0x8445: val = 0xE98081; // 
			break;
		case 0x8446: val = 0xE58DB1; // 
			break;
		case 0x8447: val = 0xE581B6; // 
			break;
		case 0x8448: val = 0xE58D91; // 
			break;
		case 0x8449: val = 0xE5AF9D; // 
			break;
		case 0x844A: val = 0xE8AA98; // 
			break;
		case 0x844B: val = 0xE7B48D; // 
			break;
		case 0x844C: val = 0xE6839C; // 
			break;
		case 0x844D: val = 0xE6ACB2; // 
			break;
		case 0x844E: val = 0xE98D9B; // 
			break;
		case 0x844F: val = 0xE7B699; // 
			break;
		case 0x8450: val = 0xE998B2; // 
			break;
		case 0x8451: val = 0xE5BEA1; // 
			break;
		case 0x8452: val = 0xE8AA95; // 
			break;
		case 0x8453: val = 0xE68391; // 
			break;
		case 0x8454: val = 0xE88ABD; // 
			break;
		case 0x8455: val = 0xE9A083; // 
			break;
		case 0x8456: val = 0xE4BA88; // 
			break;
		case 0x8457: val = 0xE6B888; // 
			break;
		case 0x8458: val = 0xE8B3AA; // 
			break;
		case 0x8459: val = 0xE9B3A5; // 
			break;
		case 0x845A: val = 0xE68096; // 
			break;
		case 0x845B: val = 0xE68B98; // 
			break;
		case 0x845C: val = 0xE586B7; // 
			break;
		case 0x845D: val = 0xE8B79D; // 
			break;
		case 0x845E: val = 0xE88485; // 
			break;
		case 0x845F: val = 0xE5859A; // 
			break;
		case 0x8460: val = 0xE88BB1; // 
			break;
		case 0x8461: val = 0xE99B84; // 
			break;
		case 0x8462: val = 0xE899AB; // 
			break;
		case 0x8463: val = 0xE8B5A6; // 
			break;
		case 0x8464: val = 0xE8B387; // 
			break;
		case 0x8465: val = 0xE888B9; // 
			break;
		case 0x8466: val = 0xE7AAAE; // 
			break;
		case 0x8467: val = 0xE996A5; // 
			break;
		case 0x8468: val = 0xE681AF; // 
			break;
		case 0x8469: val = 0xE58BA2; // 
			break;
		case 0x846A: val = 0xE9818E; // 
			break;
		case 0x846B: val = 0xE5878D; // 
			break;
		case 0x846C: val = 0xE6B0B7; // 
			break;
		case 0x846D: val = 0xE69AB4; // 
			break;
		case 0x846E: val = 0xE888AC; // 
			break;
		case 0x846F: val = 0xE5BB83; // 
			break;
		case 0x8470: val = 0xE69B9C; // 
			break;
		case 0x8471: val = 0xE7A4BE; // 
			break;
		case 0x8472: val = 0xE7AEA1; // 
			break;
		case 0x8473: val = 0xE8BEBF; // 
			break;
		case 0x8474: val = 0xE99C87; // 
			break;
		case 0x8475: val = 0xE8838C; // 
			break;
		case 0x8476: val = 0xE59593; // 
			break;
		case 0x8477: val = 0xE5BE93; // 
			break;
		case 0x8478: val = 0xE4BFB3; // 
			break;
		case 0x8479: val = 0xE7B89B; // 
			break;
		case 0x847A: val = 0xE889A6; // 
			break;
		case 0x847B: val = 0xE4BE9B; // 
			break;
		case 0x847C: val = 0xE781B0; // 
			break;
		case 0x847D: val = 0xE787BC; // 
			break;
		case 0x847E: val = 0xE79691; // 
			break;
		case 0x847F: val = 0xE5A789; // 
			break;
		case 0x8480: val = 0xE5A6B9; // 
			break;
		case 0x8481: val = 0xE6818B; // 
			break;
		case 0x8482: val = 0xE6849B; // 
			break;
		case 0x8483: val = 0xE7B494; // 
			break;
		case 0x8484: val = 0xE58897; // 
			break;
		case 0x8485: val = 0xE59E8B; // 
			break;
		case 0x8486: val = 0xE99D9E; // 
			break;
		case 0x8487: val = 0xE5838F; // 
			break;
		case 0x8488: val = 0xE9A99A; // 
			break;
		case 0x8489: val = 0xE5868A; // 
			break;
		case 0x848A: val = 0xE7B4B0; // 
			break;
		case 0x848B: val = 0xE7A8BC; // 
			break;
		case 0x848C: val = 0xE6AD8C; // 
			break;
		case 0x848D: val = 0xE68AAB; // 
			break;
		case 0x848E: val = 0xE99CB2; // 
			break;
		case 0x848F: val = 0xE6AF92; // 
			break;
		case 0x8490: val = 0xE9ABAA; // 
			break;
		case 0x8491: val = 0xE79BBE; // 
			break;
		case 0x8492: val = 0xE5B7A8; // 
			break;
		case 0x8493: val = 0xE68A80; // 
			break;
		case 0x8494: val = 0xE5BCBE; // 
			break;
		case 0x8495: val = 0xE99D92; // 
			break;
		case 0x8496: val = 0xE8A792; // 
			break;
		case 0x8497: val = 0xE7A880; // 
			break;
		case 0x8498: val = 0xE699AE; // 
			break;
		case 0x8499: val = 0xE588BB; // 
			break;
		case 0x849A: val = 0xE58DB0; // 
			break;
		case 0x849B: val = 0xE8B2B0; // 
			break;
		case 0x849C: val = 0xE7A7B0; // 
			break;
		case 0x849D: val = 0xE58FB7; // 
			break;
		case 0x849E: val = 0xE592B2; // 
			break;
		case 0x849F: val = 0xE5BAA7; // 
			break;
		case 0x84A0: val = 0xE6B2B3; // 
			break;
		case 0x84A1: val = 0xE6A8AA; // 
			break;
		case 0x84A2: val = 0xE79CBA; // 
			break;
		case 0x84A3: val = 0xE896AC; // 
			break;
		case 0x84A4: val = 0xE7979B; // 
			break;
		case 0x84A5: val = 0xE9A0AD; // 
			break;
		case 0x84A6: val = 0xE8A99B; // 
			break;
		case 0x84A7: val = 0xE9BABB; // 
			break;
		case 0x84A8: val = 0xE98594; // 
			break;
		case 0x84A9: val = 0xE6B7B7; // 
			break;
		case 0x84AA: val = 0xE98CAF; // 
			break;
		case 0x84AB: val = 0xE69CA8; // 
			break;
		case 0x84AC: val = 0xE78BAC; // 
			break;
		case 0x84AD: val = 0xE696AC; // 
			break;
		case 0x84AE: val = 0xE7A1AC; // 
			break;
		case 0x84AF: val = 0xE9A091; // 
			break;
		case 0x84B0: val = 0xE999A4; // 
			break;
		case 0x84B1: val = 0xE5B08A; // 
			break;
		case 0x84B2: val = 0xE695AC; // 
			break;
		case 0x84B3: val = 0xE58D83; // 
			break;
		case 0x84B4: val = 0xE6AE8A; // 
			break;
		case 0x84B5: val = 0xE9BB92; // 
			break;
		case 0x84B6: val = 0xE6AF94; // 
			break;
		case 0x84B7: val = 0xE8BC83; // 
			break;
		case 0x84B8: val = 0xE79787; // 
			break;
		case 0x84B9: val = 0xE5BE90; // 
			break;
		case 0x84BA: val = 0xE4BD8D; // 
			break;
		case 0x84BB: val = 0xE8B1A1; // 
			break;
		case 0x84BC: val = 0xE6A392; // 
			break;
		case 0x84BD: val = 0xE58A9F; // 
			break;
		
		
		
		// Hiragana in MnB seems to be in order of UTF
		// now would be a good idea to loop to avoid writing so much.
	//	case 0x803F: val = 0xE38181; // Hiragana A s
	//		break;
	//	case 0x8040: val = 0xE38182; // Hiragana A
	//		break;
	}
	
	// In case these symbols or blank spaces show up, use kome
	if(code > 0x84BD && code < 0x84CD)
		val = 0xE280BB;
	
#if 1
	// Numbers HW
	if(code > 0xE000 && code < 0xE00B) {
		uint16_t i = 0;
		val = 0x2F;
		for(i = 0; i < (code - 0xE000); ++i) {
			++val;
		}
	}
	// Numbers FW
	else if(code > 0x8000 && code < 0x800B) {
		uint16_t i = 0;
		val = 0xEFBC8F;
		for(i = 0; i < (code - 0x8000); ++i) {
			++val;
		}
	}
	// Letters FW
	else if(code > 0x800A && code < 0x8025) {
		uint16_t i = 0;
		val = 0xEFBCA0;
		for(i = 0; i < (code - 0x800A); ++i) {
			++val;
		}
	}
	// Lowercase letters FW
	else if(code > 0x8024 && code < 0x803F) {
		uint16_t i = 0;
		val = 0xEFBD80;
		for(i = 0; i < (code - 0x8024); ++i) {
			++val;
		}
	}
	// Hiragana part 1
	else if(code > 0x803E && code < 0x807E) {
		uint16_t i = 0;
		val = 0xE38180;
		for(i = 0; i < (code - 0x803E); ++i) {
			//printf("SHOW: 0x%X\n", val);
			
			++val;
		}
	}
	// Hiragana part 2
	else if(code > 0x807D && code < 0x8092) {
		uint16_t i = 0;
		val = 0xE3827F;
		for(i = 0; i < (code - 0x807D); ++i) {
			++val;
		}
	}
	// Katakana p1 misplaced VU
	else if(code > 0x8091 && code < 0x8098) {
		uint16_t i = 0;
		val = 0xE382A0;
		for(i = 0; i < (code - 0x8091); ++i)
			++val;
	}
	// Katakana p2
	else if(code > 0x8098 && code < 0x80B2) {
		uint16_t i = 0;
		val = 0xE382A6;
		for(i = 0; i < (code - 0x8098); ++i)
			++val;
	}
	// Katakana p3
	else if(code > 0x80B1 && code < 0x80E6) {
		uint16_t i = 0;
		val = 0xE3837F;
		for(i = 0; i < (code - 0x80B1); ++i)
			++val;
	}
#endif
	
	return val;
}

unsigned ConvertCharJP(uint8_t text1, uint8_t text2, uint8_t text3)
{
	unsigned val = 0;
	unsigned chr = text3 | text2 << 8 | text1 << 16;
	
	//printf("SHOW24: 0x%X\n", chr);
	
	switch (chr) {
		
		case 0xE280A6: val = 0xE02C; // ...
			break;
		case 0xE383BC: val = 0xE02D; // Vowel elongation
			break;
		
		// It takes too much time to write all the JP characters
		case 0xE38080: val = 0x8000; // SPACE FW
			break;
		
		case 0xEFBC9F: val = 0x80E8; // ? FW
			break;
		case 0xEFBC81: val = 0x80E9; // ! FW
			break;
		case 0xE3829B: val = 0x80EC; // " FW
			break;
		case 0xEFBC9C: val = 0x80EE; // < FW
			break;
		case 0xE38081: val = 0x80F1; // JP ,? FW
			break;
		case 0xEFBDA4: val = 0x80F1; // JP ,? FW
			break;
		case 0xE38082: val = 0x80F2; // JP . FW
			break;
		case 0xEFBC8E: val = 0x80F4; // . FW
			break;
		case 0xEFBC9A: val = 0x80F6; // : FW
			break;
		case 0xEFBC9B: val = 0x80F7; // ; FW
			break;
		case 0xE3809E: val = 0x80F8; // right " FW
			break;
		case 0xE3809D: val = 0x80F9; // left " FW
			break;
		case 0xEFBFA5: val = 0x8135; // Yen FW
			break;
		case 0xE38087: val = 0x8141; // Seikai FW
			break;
		// Honestly, I doubt these symbols are used in the game
		// so only the ones I thought might appear are here.
		
		case 0xEFBC85: val = 0x8139; // % FW (Has HW ver.)
			break;
		
		
	//	case 0xE38181: val = 0x803F; // Hiragana A small
	//		break;
	//	case 0xE38182: val = 0x8040; // Hiragana A
	//		break;
		
	}
	
	// These cover numbers and letters in FW
	if(chr > 0xEFBC8F && chr < 0xEFBC9A) {
		uint16_t i = 0;
		val = 0x8000;
		for(i = 0; i < (chr - 0xEFBC8F); ++i) {
			++val;
		}
	}
	else if(chr > 0xEFBCA0 && chr < 0xEFBCBB) {
		uint16_t i = 0;
		val = 0x800A;
		for(i = 0; i < (chr - 0xEFBCA0); ++i) {
			++val;
		}
	}
	else if(chr > 0xEFBD80 && chr < 0xEFBD9B) {
		uint16_t i = 0;
		val = 0x8024;
		for(i = 0; i < (chr - 0xEFBD80); ++i) {
			++val;
		}
	}
	// Hiragana part 1
	else if(chr > 0xE38180 && chr < 0xE381C0) {
		uint16_t i = 0;
		val = 0x803E;
		for(i = 0; i < (chr - 0xE38180); ++i)
			++val;
	}
	// Hiragana part 2
	else if(chr > 0xE3827F && chr < 0xE38294) {
		uint16_t i = 0;
		val = 0x807D;
		for(i = 0; i < (chr - 0xE3827F); ++i) {
			++val;
		}
	}
	// Katakana p1 misplaced VU
	else if(chr > 0xE382A0 && chr < 0xE382A7) {
		uint16_t i = 0;
		val = 0x8091;
		for(i = 0; i < (chr - 0xE382A0); ++i)
			++val;
	}
	// Katakana p2
	else if(chr > 0xE382A6 && chr < 0xE382C0) {
		uint16_t i = 0;
		val = 0x8098;
		for(i = 0; i < (chr - 0xE382A6); ++i)
			++val;
	}
	// Katakana p3
	else if(chr > 0xE3837F && chr < 0xE383B4) {
		uint16_t i = 0;
		val = 0x80B1;
		for(i = 0; i < (chr - 0xE3837F); ++i)
			++val;
	}
	
	return val;
}

unsigned ConvertCharSymb(uint8_t text1, uint8_t text2)
{
	unsigned val = 0;
	uint16_t chr = text2 | text1 << 8;
	
	//printf("SHOW16: 0x%X\n", chr);
	
	switch (chr) {
		case 0xC3A1: val = 0xE047; // á
			break;
		case 0xC3A9: val = 0xE048; // é
			break;
		case 0xC3AD: val = 0xE049; // í
			break;
		case 0xC3B3: val = 0xE04A; // ó
			break;
		case 0xC3BA: val = 0xE04B; // ú
			break;
		case 0xC3BC: val = 0xE04C; // ü
			break;
		case 0xC3B1: val = 0xE04D; // ñ
			break;
		case 0xC2BF: val = 0xE04E; // ¿
			break;
		case 0xC2A1: val = 0xE04F; // ¡
			break;
		case 0xC3A7: val = 0xE051; // ç
			break;
	}
	
	return val;
}

unsigned ConvertChar(char text)
{
  unsigned val = 0;

  switch (text) {


   //case '!': val = 0x80E9; //fullwidth
     // break;

   case '_': val = 0x80FC; //use underscore for topscore
      break;

   case '”': val = 0x8133; //alt 0148
      break;
   case '“': val = 0x8133; //alt 0147
      break;
   case '’': val = 0x8132; //alt 0146
      break;

   case '–': val = 0x80EA; //en dash alt 0150
      break;
   case '—': val = 0x80EB; //em dash alt 0151
      break;
   case '<': val = 0x80EE;
      break;
   case '|': val = 0x80EF;
      break;
   case ';': val = 0x80F7;
      break;
   case '^': val = 0x80FB; //needs quotes
      break;
   case '/': val = 0x8107;
      break;
   case '\\': val = 0x8108;
      break;
   case '~': val = 0x8109;
      break;
   case '(': val = 0x8111;
      break;
   case ')': val = 0x8112;
      break;
   case '}': val = 0x8118;
      break;
   case '[': val = 0x8121;
      break;
   case ']': val = 0x8122;
      break;
   case '+': val = 0x8123;
      break;
   case '=': val = 0x8128;
      break;
   case '>': val = 0x812A; //requires quotes
      break;
   case '\"': val = 0x8133; //quotes fullwidth
      break;
   case '¥': val = 0x813A; //alt 0165
      break;
   case '$': val = 0x8136;
      break;
//   case '%': val = 0x8139; //fullwidth
//      break;
   case '#': val = 0x813A;
      break;
   case '&': val = 0x813B; //needs to be escaped \& or in quotes depending on situation
      break;
   case '*': val = 0x813C; //needs special care
      break;
   case '@': val = 0x813D;
      break;
   case '§': val = 0x813E; //vertical double s
      break;

   case ' ': val = 0xE000; //space requires quotes
      break;
   case '0': val = 0xE001;
      break;
   case '1': val = 0xE002;
      break;
   case '2': val = 0xE003;
      break;
   case '3': val = 0xE004;
      break;
   case '4': val = 0xE005;
      break;
   case '5': val = 0xE006;
      break;
   case '6': val = 0xE007;
      break;
   case '7': val = 0xE008;
      break;
   case '8': val = 0xE009;
      break;
   case '9': val = 0xE00A;
      break;
   case '?': val = 0xE00B;
      break;
   case 'D': val = 0xE00C;
      break;
   case 'E': val = 0xE00D;
      break;
   case 'F': val = 0xE00E;
      break;
   case 'G': val = 0xE00F;
      break;
   case 'J': val = 0xE010;
      break;
   case 'K': val = 0xE011;
      break;
   case 'L': val = 0xE012;
      break;
   case 'A': val = 0xE013;
      break;
   case 'B': val = 0xE014;
      break;
   case 'C': val = 0xE015;
      break;
   case 'N': val = 0xE016;
      break;
   case 'P': val = 0xE017;
      break;
   case 'Q': val = 0xE018;
      break;
   case 'S': val = 0xE019;
      break;
   case 'U': val = 0xE01A;
      break;
   case 'V': val = 0xE01B;
      break;
   case 'W': val = 0xE01C;
      break;
   case 'R': val = 0xE01D;
      break;
   case 'O': val = 0xE01E;
      break;
   case 'M': val = 0xE01F;
      break;
   case 'X': val = 0xE020;
      break;
   case 'Y': val = 0xE021;
      break;
   case 'Z': val = 0xE022;
      break;
   case 'a': val = 0xE023;
      break;
   case 'b': val = 0xE024;
      break;
   case 'c': val = 0xE025;
      break;
   case ':': val = 0xE026;
      break;
   case '%': val = 0xE027;
      break;
   case '.': val = 0xE028;
      break;
   case 'H': val = 0xE029;
      break;
   case 'I': val = 0xE02A;
      break;
   case 'T': val = 0xE02B;
      break;
   case '…': val = 0xE02C; //x mult, replaced by ellipsis
      break;
   case '-': val = 0xE02D;
      break;
   case 'd': val = 0xE02E;
      break;
   case 'e': val = 0xE02F;
      break;
   case 'f': val = 0xE030;
      break;
   case 'g': val = 0xE031;
      break;
   case 'h': val = 0xE032;
      break;
   case 'i': val = 0xE033;
      break;
   case 'j': val = 0xE034;
      break;
   case 'k': val = 0xE035;
      break;
   case 'l': val = 0xE036;
      break;
   case 'm': val = 0xE037;
      break;
   case 'n': val = 0xE038;
      break;
   case 'o': val = 0xE039;
      break;
   case 'p': val = 0xE03A;
      break;
   case 'q': val = 0xE03B;
      break;
   case 'r': val = 0xE03C;
      break;
   case 's': val = 0xE03D;
      break;
   case 't': val = 0xE03E;
      break;
   case 'u': val = 0xE03F;
      break;
   case 'v': val = 0xE040;
      break;
   case 'w': val = 0xE041;
      break;
   case 'x': val = 0xE042;
      break;
   case 'y': val = 0xE043;
      break;
   case 'z': val = 0xE044;
      break;
   case '\'': val = 0xE045;
      break;
   case ',': val = 0xE046;
      break;
   case 'á': val = 0xE047;
      break;
   case 'é': val = 0xE048;
      break;
   case 'í': val = 0xE049;
      break;
   case 'ó': val = 0xE04A;
      break;
   case 'ú': val = 0xE04B;
      break;
   case 'ü': val = 0xE04C; //alt 0252
      break;
   case 'ñ': val = 0xE04D;
      break;
   case '¿': val = 0xE04E;
      break;
   case '¡': val = 0xE04F;
      break;
   case '!': val = 0xE050;
      break;
   case 'ç': val = 0xE051; //change for quotes?
      break;
  }

  return val;
}

const char name[] = "Diego A.";
const char noise[] = "MISAKA NETWORK";
const char atk[] = "CHAOS CONTROL";
const char def[] = "IMAGINE BREAKER";
const char quote1[] = "The time-space rift is expanding... There's no more time, I need to hurry.";
const char quote2[] = "Sounds good but you should know only the Power Rangers can control the Mega Voyger.";
const char quote3[] = "Step aside Mercury, it's not you I'm after!";

const char spell1[] = "Zaker";
const char spell2[] = "Rashield";
const char spell3[] = "Jikerdor";
const char spell4[] = "Bao Zakeruga";
const char spell5[] = "Zakeruga";
const char spell6[] = "Rauzaruk";
const char spell7[] = "Zaguruzemu";

const char rando0[] = "Sparkling Wide Pressure";
const char rando1[] = "Exodia the Forbidden One";
const char rando2[] = "Diffusion Wave Motion";

const char end1[] = "You foolish man, I am a Jito-Ryo master. Allow me to reunite you with your ancestors.";
const char end2[] = "Jump on panel number one, it will take you to panels two and three.";

int main (int argc, char *argv[])
{
  int i = 0;
  int x = 0;
  int fullLimit = 0;
  int halfLimit = 0;
  int halfLimit1 = 0; //for recording game string len
  int fullLimit1 = 0;
  bool lineBroke = false;
  int checksum = 0;

  //verify data
  
  //printf("SUM: 0x%X\n\n", checksum);
#if 0
  if(checksum != 0x2A8C) {
     printf("\n\n");
     printf("An error has been detected.\n");
     printf("You do not have enough power-from-within!\n");
     return 0;
  }
#endif

  char outName[128] = {0};
  int langID = 1; // English
  bool isROMpatched = true;
  bool uncmpScripts = false;
  bool altVO = false;
  bool useCheats = false;
  bool verbose = false;

  printf("\n\n");
  printf("~ Konjiki no Gash Bell!! Makai no Bookmark: AIO Script Converter ~  by %s\n\n\n", name);
  
  //printf("Arg count: %d \n\n", argc);

  if(argc > 1) {
    for(i = 1; i < argc; i++) {
		
		if(strcmp(argv[i], "-o") == 0) // name of the output, this is optional
        {
                ++i;   //skip arg
                x = 0; //reset parse
				//printf("\n");
				
				sprintf(outName, "%s", argv[i]);
				
                printf("Set output filename to: %s\n\n", outName);
        }
		
		if(strcmp(argv[i], "-l") == 0) // language to encode to from the text file
        {
                ++i;   //skip arg
                x = 0; //reset parse
				//printf("\n");
				
				if(strcmp("English", argv[i]) == 0)
					langID = 1;
				else if(strcmp("Spanish", argv[i]) == 0)
					langID = 2;
				else if(strcmp("French", argv[i]) == 0)
					langID = 3;
				else if(strcmp("Italian", argv[i]) == 0)
					langID = 4;
				else if(strcmp("Portuguese", argv[i]) == 0)
					langID = 5;
				else if(strcmp("German", argv[i]) == 0)
					langID = 6;
				else if(strcmp("Japanese", argv[i]) == 0)
					langID = 0;
				
                //printf("Language set to: %s\n\n", LANG_GET[langID]);
                printf("Language set to: %s\n\n", argv[i]);
        }
		
		if(strcmp(argv[i], "--not-patched") == 0) // if the ROM is not patched some characters represent JP characters
        {
                ++i;   //skip arg
                x = 0; //reset parse
				//printf("\n");
				
				isROMpatched = false;
				printf("Decoding set to original Japanese characters.\n\n");
        }
		
		if(strcmp(argv[i], "--no-compression") == 0) // enables hack that lets cmp scripts load from the rom
		{
			++i;   //skip arg
			x = 0; //reset parse
			
			uncmpScripts = true;
		}
		
		if(strcmp(argv[i], "--alt-vo") == 0) // inserts dub
		{
			++i;   //skip arg
			x = 0; //reset parse
			
			altVO = true;
		}
		
		if(strcmp(argv[i], "--cheats") == 0) // implements easy playing
		{
			++i;   //skip arg
			x = 0; //reset parse
			
			useCheats = true;
		}
		
		if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) // Print more info on console
        {
                ++i;   //skip arg
                x = 0; //reset parse
				//printf("\n");
				
				verbose = true;
				printf("Verbosity set.\n\n");
        }
		
		if(strcmp(argv[i], "-e") == 0) // open a script text file and encode it to a bin file
        {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("\n");
                printf("Script Encoding Mode\n\n");
				
				int chrWarn = 0;
				int fileSz = 0;
				
				FILE *fp = fopen(argv[i], "rb");
				if(fp == NULL) {
					printf("Failed to open file!\n\nAre you reading the wrong spell book?\n\n");
					break;
				}
				
				// get file size
				fseek(fp, 0, SEEK_END);
				fileSz = ftell(fp);
				
				fseek(fp, 0, SEEK_SET);
				
				bool linesFound = false;
				unsigned lines = 0;
				char searchID[8] = {0};
				char txtRead[8] = {0};
				sprintf(searchID, "%s: ", LANG_ID[langID]);
				
				int p = 0;
				for(p = 0; p < fileSz; ++p) {
					fseek(fp, p, SEEK_SET);
					fread(txtRead, 1, 4, fp);
					
					if(!linesFound && strncmp(txtRead, "ln: ", 4) == 0) {
						fread(txtRead, 1, 4, fp);
						lines = atoi(txtRead);
						linesFound = true;
						
						// This is just to prevent incorrect scripts from going overboard
						if(lines > 500)
							lines = 500;
						
						printf("Lines specified by script: %d\n", lines);
					}
				}
				
				if(lines == 0) {
					printf("No lines to encode!\n");
					fclose(fp);
					break;
				}
				
				// Build the base binary
				LinesData * ptrTable = (LinesData *) malloc((lines*4)+4);
				
				// Set the number of lines
				ptrTable[0].lineptr = lines;
				
				// Should always be 0 but doesn't have to be
				ptrTable[1].lineptr = 0;
				
				// Allocate for encoded string
				uint8_t dstBuf[96*1024] = {0};
				
				//show me
				//printf("Show size: 0x%X\n", fileSz);
				
				// Position of dst, needed to deal with the mixed int lengths
				int s = 0;
				
				bool dontNul = false;
				char tmpLine[512] = {0};
				int lineCNT = 0;
				int curPTR = 0;
				int a = 0;
				for(p = 0; p < fileSz; ++p) {
					fseek(fp, p, SEEK_SET);
					fread(txtRead, 1, 4, fp);
					if(linesFound && strncmp(txtRead, searchID, 4) == 0) {
						if(!verbose)
							printf("Found string!\n");
						++lineCNT;
						
						// Reset after every line read
						curPTR = 0;
						a = 0;
						
						while(1) {
							int linebrk = 0;
							++a;
							fseek(fp, p+a, SEEK_SET);
							fread(&linebrk, 1, 1, fp);
							
							// This keeps track of the string size
							++curPTR;
							
							if(linebrk == 0x0D || linebrk == 0x0A || linebrk == 0 || curPTR > 512)
								break;
						}
						fseek(fp, p+4, SEEK_SET);
						fread(tmpLine, 1, curPTR-4, fp);
						
						if(verbose)
							printf("String: %s, Size: %d, Line: %d\n", tmpLine, curPTR, lineCNT);
						
						int e = 0;
						for(e = 0; e < curPTR-4; ++e, s+=2) {
							// Check if there's a command
							if(tmpLine[e] == '\\') {
								if(verbose)
									printf("Using special command.\n");
								
								uint8_t val_1 = 0;
								uint8_t val_2 = 0;
								uint8_t val_3 = 0;
								
								if(strncmp(&tmpLine[e+1], "RESTORE", 7) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x31;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 7;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "BLACK", 5) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x32;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 5;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "BLUE1", 5) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x33;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 5;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "BLUE2", 5) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x34;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 5;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "RED1", 4) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x35;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 4;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "RED2", 4) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x36;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 4;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "GREEN1", 6) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x37;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 6;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "GREEN2", 6) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x38;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 6;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "ORANGE", 6) == 0) {
									val_1 = 0x25;
									val_2 = 0x49;
									val_3 = 0x39;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									dstBuf[s+2] = val_3;
									
									e += 6;
									s += 1;
								}
								else if(strncmp(&tmpLine[e+1], "N", 1) == 0) {
									val_1 = 0x25;
									val_2 = 0x4E;
									dstBuf[s] = val_1;
									dstBuf[s+1] = val_2;
									
									e += 1;
									
									// TODO: Tell which linebreak is the one exceeding
									// Reset character limit to warn if the line exceeds
									if(chrWarn > 154)
										printf("Line %d exceeds the screen limit!\n", lineCNT);
									chrWarn = 0;
								}
								
								// TODO: Implement the rest of the symbols?
								
								
							} else if(tmpLine[e] != 0) {
								// Security
						//		if(ConvertChar(tmpLine[e]) == 0)
						//			break;
								
								uint8_t val = 0;
								
								// Handle Fullwidth encoding
								if((tmpLine[e] & 0xF0) == 0xC0) {
									// Symbol or tilde - 2 bytes
									if(ConvertCharSymb(tmpLine[e], tmpLine[e+1]) == 0)
										break;
									
									val = ConvertCharSymb(tmpLine[e], tmpLine[e+1]) >> 8;
									dstBuf[s] = val;
								} else if((tmpLine[e] & 0xF0) == 0xE0) {
									// JP character - 3 bytes
									if(ConvertCharJP(tmpLine[e], tmpLine[e+1], tmpLine[e+2]) == 0)
										break;
									
									val = ConvertCharJP(tmpLine[e], tmpLine[e+1], tmpLine[e+2]) >> 8;
									dstBuf[s] = val;
								} else {
									if(ConvertChar(tmpLine[e]) == 0)
										break;
									
									val = ConvertChar(tmpLine[e]) >> 8;
									dstBuf[s] = val;
								}
								
								if((val & 0xF0) == 0xE0)
									chrWarn += 7;
								else if((val & 0xF0) == 0x80)
									chrWarn += 14;
								
								// Diffusion Wave Motion
								if((tmpLine[e] & 0xF0) == 0xC0) {
									val = ConvertCharSymb(tmpLine[e], tmpLine[e+1]);
									dstBuf[s+1] = val;
									
									// end by skipping 1 byte
									++e;
								} else if((tmpLine[e] & 0xF0) == 0xE0) {
									val = ConvertCharJP(tmpLine[e], tmpLine[e+1], tmpLine[e+2]);
									dstBuf[s+1] = val;
									
									// end by skipping 2 bytes
									e += 2;
								} else {
									val = ConvertChar(tmpLine[e]);
									dstBuf[s+1] = val;
								}
							} else {
								// No choice but to break
								dontNul = true;
								break;
							}
						}
						
						// Show character limit warn if necessary
						if(chrWarn > 154)
							printf("Line %d exceeds the screen limit!\n", lineCNT);
						chrWarn = 0;
					//	printf("SHOW: %d\n", chrWarn);
						
						// When the line is done write null bytes until it's 32-bit aligned
						if(!dontNul) {
							uint8_t adv = 2;
							
							while((s+adv) % 4 != 0) {
								dstBuf[s+adv] = 0;
								++adv;
							}
							s += adv;
						}
						dontNul = false;
						
						// Perfect time to add the pointer
						if((lineCNT+1) < (lines+1))
							ptrTable[lineCNT+1].lineptr = s;
						
						// Clear the buffer for the next line
						memset(tmpLine, 0, sizeof(tmpLine));
						
						// Let the ln tag control how many lines to process
						if(lineCNT == lines)
							break;
					}
				}
				
				// Create a file to write to
				FILE *fb = fopen(outName[0] == 0 ? "out_makai.bin" : outName, "wb");
				fwrite(ptrTable, 1, (lines*4)+4, fb);
				fwrite(&dstBuf[0], 1, s, fb);
				fclose(fb);
				
				printf("Script size: %d bytes\n", s);
				
				if(ptrTable)
					free(ptrTable);
				fclose(fp);
				break;
				
				
        } else if(strcmp(argv[i], "-d") == 0) // open a script bin and decode it to a text file
        {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("\n");
                printf("Script Decoding Mode\n\n");
				
				int fileSz = 0;
				FILE *fp = fopen(argv[i], "rb");
				if(fp == NULL) {
					printf("Failed to open file!\n\nAre you casting the right spell, human?!\n\n");
					break;
				}
				
				// get file size
				fseek(fp, 0, SEEK_END);
				fileSz = ftell(fp);
				
				fseek(fp, 0, SEEK_SET);
				
				// Only one way to read a string correctly
				// by mimicking the game and using the ptr table.
				
				int begin_strings = 0;
				
				int lines = 0;
				fread(&lines, 1, 4, fp);
				
				if(lines == 0) {
					printf("This file has no strings.\n\n");
					fclose(fp);
					break;
				}
				else if(lines > 500) {
					printf("This file has too many strings.\n\nYou simply don't have the power-from-within...\n\n");
					fclose(fp);
					break;
				}
				
				printf("This BIN has %d valid lines.\n", lines);
				
				// Save the start position of the strings
				begin_strings = (lines*4) + 4;
				
				// Create a buffer to store the pointer list so we don't have to seek the file back n forth
				LinesData * ptrTable = (LinesData *) malloc((lines*4)+4);
				if(!ptrTable)
					printf("Could not allocate memory...\n");
				
				// Copy the pointer table to the buf
				fread(ptrTable, 4, lines, fp); // it's stopping at line 174... on a 341 line file
				// oh no, I fell for the old 'opened file but not in binary mode' nonsense
			/*	unsigned sTab = 0;
				for(sTab; sTab<lines; ++sTab) {
					unsigned tmp = 0;
					fread(&tmp, 4, 1, fp);
					ptrTable[sTab].lineptr = tmp;
				}*/
				//ptrTable[174].lineptr = 0x1A18;
				// It's already borked??
				//printf("SHOW AFTER READ: 0x%X\n", ptrTable[174].lineptr);
				
				
				// Create a file to write to
				FILE *fc = fopen(outName[0] == 0 ? "script_dec.txt" : outName, "w");
				
				// Write header info
				fprintf(fc, "%s%s%s%s\n\nln: %d\n\n", "\xEF", "\xBB", "\xBF", decIntro, lines);
				
				//printf("Entering Danger Zone!\n");
				
				int s = 0;
				for(s = 0; s < lines; ++s) {
					// Seek to the start of each line
					fseek(fp, ptrTable[s].lineptr + begin_strings, SEEK_SET);
					
					fprintf(fc, "%d\n%s: ", s+1, LANG_ID[langID]);
					
					// Now convert the string until null is found
					int readCHR = 0;
					int c = 0;
					//for(c = 0; c < (ptrTable[s+1].lineptr - ptrTable[s].lineptr); c+=2) {
					for(c = 0; c < fileSz; c+=2) {
						// Time to make some assumptions.
						// Characters seem to always be 2 bytes, while color codes are 3 bytes
						// therefore I'll just check for 2 bytes and if it starts with 0x25 it's a color switch
						// BUT watch out for LINEBREAKS, they start with 0x25 but are 2 bytes.
						// BUT ALSO working with LE here so I'm writing the values backwards.
						fread(&readCHR, 1, 2, fp);
						
						//if(readCHR == 0x22E0)
						//	printf("Read chr: 0x%X\n", readCHR);
						
						if((readCHR & 0xF0) == 0xE0) { // halfwidth bank
							// Conversion easy...
							//fprintf(fc, "tx");
							
							//printf("Found halfwidth! 0x%X\n", readCHR);
							uint16_t val = (readCHR >> 8) | (readCHR << 8);
							
							//printf("Found! 0x%X\n", val);
							int res = ConvertUTF(val, isROMpatched);
							
							if(res != 0 && (res & 0xFF0000) != 0) { // JP chr
								// Convert to LE but discard 00
								//res = (res>>16) | (res<<16) | (res & 0xFF00);
								res = ((res>>16) & 0xFF) | ((res<<16) & 0xFF0000) | (res & 0xFF00);
								
								fwrite(&res, 1, 3, fc);
							} else if(res != 0 && (res & 0xFF00) != 0) { // special chr like accents
								// LE byte swap
								res = (res >> 8) | (res << 8);
								
								fwrite(&res, 1, 2, fc);
							} else if(res != 0 && (res & 0xFF) != 0) // normal chr
								fwrite(&res, 1, 1, fc);
							
							//if(ConvertUTF(val, isROMpatched) != 0)
							//	fprintf(fc, "%s%01x", "\\x",(unsigned char)ConvertUTF(val, isROMpatched));
						}
						else if((readCHR & 0xF0) == 0x80) { // fullwidth bank
							// Conversion hard!
							//fprintf(fc, "fx");
							
							// Same exact code as above
							uint16_t val = (readCHR >> 8) | (readCHR << 8);
							
							int res = ConvertUTF(val, isROMpatched);
							
							if(res != 0 && (res & 0xFF0000) != 0) { // JP chr
								// Convert to LE but discard 00
								res = ((res>>16) & 0xFF) | ((res<<16) & 0xFF0000) | (res & 0xFF00);
								
								fwrite(&res, 1, 3, fc);
							} else if(res != 0 && (res & 0xFF00) != 0) { // special chr like accents
								// LE byte swap
								res = (res >> 8) | (res << 8);
								
								fwrite(&res, 1, 2, fc);
							} else if(res != 0 && (res & 0xFF) != 0) // normal chr
								fwrite(&res, 1, 1, fc);
						}
						else if(readCHR == 0x4E25) // linebreak
							fprintf(fc, "\\N");
						else if(readCHR == 0x4925) { // color code
							// 3 bytes so skip 1
							++c;
							
							// copy the 3rd byte to figure out which color it is
							readCHR = 0;
							fread(&readCHR, 1, 1, fp);
							
							if(readCHR == 0x31)
								fprintf(fc, "\\RESTORE");
							else if(readCHR == 0x32)
								fprintf(fc, "\\BLACK");
							else if(readCHR == 0x33)
								fprintf(fc, "\\BLUE1");
							else if(readCHR == 0x34)
								fprintf(fc, "\\BLUE2");
							else if(readCHR == 0x35)
								fprintf(fc, "\\RED1");
							else if(readCHR == 0x36)
								fprintf(fc, "\\RED2");
							else if(readCHR == 0x37)
								fprintf(fc, "\\GREEN1");
							else if(readCHR == 0x38)
								fprintf(fc, "\\GREEN2");
							else if(readCHR == 0x39)
								fprintf(fc, "\\ORANGE");
						}
						
						if((readCHR & 0xFF) == 0) {
							fprintf(fc, "\n\n");
							break;
						}
					}
				}
				
				//printf("Outside Danger Zone!\n");
				
				fclose(fp);
				fclose(fc);
				if(ptrTable)
					free(ptrTable);
				
				break;
				
		}
		else if(strcmp(argv[i], "-i") == 0) // inject scripts, sprites, and other data to a rom
		{
			++i;   //skip arg
			x = 0; //reset parse
				
			printf("ROM Data Insertion Mode\n\n");
			printf("Loading ROM: %s\n\n", argv[i]);
			
			int valRead = 0;
			int fileSz = 0;
			FILE *fp = fopen(argv[i], "rb");
			if(fp == NULL) {
				printf("Failed to open file!\n\nAre you casting the right spell, human?!\n\n");
				break;
			}
			
			// get file size
			fseek(fp, 0, SEEK_END);
			fileSz = ftell(fp);
			
			fseek(fp, 0, SEEK_SET);
			
			if(fileSz < (8*1024*1024)) {
				printf("Not a valid file... Size is too small.\n");
				break;
			}
			
			// Check the first bytes to see if this is a ROM
			fread(&valRead, 1, 4, fp);
			
			if(valRead != 0xEA00002E)
				printf("This might not be the correct ROM.\n");
			
			fseek(fp, 0xA8, SEEK_SET);
			fread(&valRead, 1, 4, fp);
			
			if(valRead != 0x4B52414D)
				printf("This is definitely not the correct ROM.\n");
			
			printf("Preparing patches...\n\n");
			
			if(uncmpScripts)
				printf("Enabled uncompressed script loading hack.\n\n");
			
			// rewind
			fseek(fp, 0, SEEK_SET);
			
			uint32_t curSize = 0;
			uint32_t curPos = 0x7DA0C0; // start por for inserting data
			
			uint32_t romSize = 8*1024*1024; // Once everything is done this should be 16MB
			uint8_t* romBuf = malloc(romSize);
			fread(romBuf, 1, romSize, fp);
			fclose(fp);
			
			// Apply patches to mem buffer
			
			// Cheats
			if(useCheats) {
				// Infinite SA orbs
				romBuf[0x5594] = 0; // thumb instruction to reduce orbs
				
				// Reduce delay before mashing A/B on SA screens
				romBuf[0x57C08] = 8;
				
				// Easy press A or B to fill SA meter quickly
				romBuf[0x57CAC] = 2; // A
				romBuf[0x57C3E] = 2; // B
				
				// Infinite HP
				romBuf[0x839C] = 0;
				romBuf[0x839D] = 0;
				
				// Infinite strength-from-within
				romBuf[0x7F76] = 0; // thumb inst, strh r0,[r6,#0x4]
				romBuf[0x7F77] = 0;
				
				// Quick charge spell
				//romBuf[0x7FA2] = 4; // og: 1
				
				printf("Applied cheats.\n");
			}
			
			// swap shiori with playerdata
			romBuf[0x60A2A] = 0x76; // x pos Bookmarks
			romBuf[0x60A38] = 0x9E;
			romBuf[0x60A5A] = 0x04; // extend
			romBuf[0x60AD2] = 0xB0; // x pos Data
			romBuf[0x60AE0] = 0x88;
			romBuf[0x60B02] = 0x02; // reduce
			
			// during gameplay/field
			romBuf[0x6119E] = 0x76; // x pos Bookmarks
			romBuf[0x611AC] = 0x9E;
			romBuf[0x611CE] = 0x04; // extend
			romBuf[0x61246] = 0xB0; // x pos Data
			romBuf[0x61254] = 0x88;
			romBuf[0x61276] = 0x02; // reduce
			
			printf("Applied shiori sprite swap hack.\n");
			
			// Load compressed scripts directly from the ROM
		#if 1
			if(uncmpScripts) {
				romBuf[0x06BADC] = 0;
				romBuf[0x06BADD] = 0;
				romBuf[0x06BADE] = 0;
				romBuf[0x06BADF] = 0;
			}
		#endif
			
			// Open first file to insert
			// HW font
			fp = fopen("GFX/MnB_halfwidthCHARSET_0x6328B8.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open HW font!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				fread(&romBuf[0x6328B8], 1, curSize, fp);
				fclose(fp);
				//printf("Applied halfwidth font. 0x%X\n", curSize);
				printf("Applied halfwidth font.\n");
			}
			
			// Replace the spells text that appear during gameplay
			curSize = 0;
			fp = fopen("GFX/MnB_fieldSpells_0x29C670.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open field spells!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				fread(&romBuf[0x29C670], 1, curSize, fp);
				fclose(fp);
				printf("Applied field spells sprites.\n");
			}
			
			// NOTE: Last 4 bytes after each of the PMenu names are not part of the original
			// compressed file, but I don't know what importance this has.
			// To be safe, limiting it to the compressed data only.
			
			// Pause Menu names
			curSize = 0;
			fp = fopen("GFX/GFX_MENU/GFX_MENU_EN/ZATCH_KIYO_MENU_0x2A5E98.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open Zatch-Kiyo PauseMenu names!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x4C8) {
					fread(&romBuf[0x2A5E98], 1, curSize, fp);
					printf("Applied Zatch-Kiyo PauseMenu sprite.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x28DEB4] = curPos;
					romBuf[0x28DEB5] = curPos >> 8;
					romBuf[0x28DEB6] = curPos >> 16;
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed Zatch-Kiyo PauseMenu sprite.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_MENU/GFX_MENU_EN/TIA_MEGUMI_MENU_0x2A6364.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open Tia-Megumi PauseMenu names!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3C8) {
					fread(&romBuf[0x2A6364], 1, curSize, fp);
					printf("Applied Tia-Megumi PauseMenu sprite.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x28DEB8] = curPos;
					romBuf[0x28DEB9] = curPos >> 8;
					romBuf[0x28DEBA] = curPos >> 16;
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed Tia-Megumi PauseMenu sprite.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_MENU/GFX_MENU_EN/KANCHO_FOL_MENU_0x2A6730.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open Kancho-Folgore PauseMenu names!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x460) {
					fread(&romBuf[0x2A6730], 1, curSize, fp);
					printf("Applied Kancho-Folgore PauseMenu sprite.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x28DEBC] = curPos;
					romBuf[0x28DEBD] = curPos >> 8;
					romBuf[0x28DEBE] = curPos >> 16;
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed Kancho-Folgore PauseMenu sprite.\n");
				}
				fclose(fp);
			}
			
			// Initial Pause Menu GFX - Status/Bookmarks/Save/Data, Bookmark types, stats, etc.
			curSize = 0;
			fp = fopen("GFX/MISC/CMP/0x2A3DB4_file0008_PauseMenu.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x2A3DB4_file0008' PauseMenu GFX!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x14F4) {
					fread(&romBuf[0x2A3DB4], 1, curSize, fp);
					printf("Applied PauseMenu GFX sprites 1.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x85CF4] = curPos;
					romBuf[0x85CF5] = curPos >> 8;
					romBuf[0x85CF6] = curPos >> 16;
					
					romBuf[0x2926EC] = curPos;
					romBuf[0x2926ED] = curPos >> 8;
					romBuf[0x2926EE] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed PauseMenu GFX sprites 1.\n");
				}
				fclose(fp);
			}
			
			// Pause Menu GFX - Status/Bookmarks/Save/Data - used for reloading
			curSize = 0;
			fp = fopen("GFX/MISC/CMP/0x2AABC4_file0018_PauseMenuLess.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x2AABC4_file0018' PauseMenu GFX!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x6C4) {
					fread(&romBuf[0x2AABC4], 1, curSize, fp);
					printf("Applied PauseMenu GFX sprites 2.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x61538] = curPos;
					romBuf[0x61539] = curPos >> 8;
					romBuf[0x6153A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed PauseMenu GFX sprites 2.\n");
				}
				fclose(fp);
			}
			
			// SA - Support Actions
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/01_bao_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '01_bao_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E0) {
					fread(&romBuf[0x56A730], 1, curSize, fp);
					printf("Applied 01_BaoZakeruga GFX.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x26042C] = curPos;
					romBuf[0x26042D] = curPos >> 8;
					romBuf[0x26042E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 01_BaoZakeruga GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/02_zaker_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '02_zaker_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x308) {
					fread(&romBuf[0x56AB80], 1, curSize, fp);
					printf("Applied 02_Zaker GFX.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x260438] = curPos;
					romBuf[0x260439] = curPos >> 8;
					romBuf[0x26043A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 02_Zaker GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/03_zakeruga_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '03_zakeruga_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x358) {
					fread(&romBuf[0x56AEFC], 1, curSize, fp);
					printf("Applied 03_Zakeruga GFX.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x260444] = curPos;
					romBuf[0x260445] = curPos >> 8;
					romBuf[0x260446] = curPos >> 16;
					
					romBuf[0x260450] = curPos;
					romBuf[0x260451] = curPos >> 8;
					romBuf[0x260452] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 03_Zakeruga GFX.\n");
				}
				fclose(fp);
			}
			// NOTE: 3 and 4 both point to zakeruga
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/05_rauzaruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '05_rauzaruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x39C) {
					fread(&romBuf[0x56B2C4], 1, curSize, fp);
					printf("Applied 05_Rauzaruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x26045C] = curPos;
					romBuf[0x26045D] = curPos >> 8;
					romBuf[0x26045E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 05_Rauzaruk GFX.\n");
				}
				fclose(fp);
			}
			
			//0x26042C = pointer list, (n*0xC)-0xC
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/07_saisu_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '07_saisu_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x2EC) {
					fread(&romBuf[0x56B810], 1, curSize, fp);
					printf("Applied 07_Saisu GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 7;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 07_Saisu GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/08_gigalaseoshi_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '08_gigalaseoshi_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E8) {
					fread(&romBuf[0x56BB6C], 1, curSize, fp);
					printf("Applied 08_GigaLaSeoshi GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 8;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 08_GigaLaSeoshi GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/09_saifogeo_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '09_saifogeo_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3AC) {
					fread(&romBuf[0x56BFC4], 1, curSize, fp);
					printf("Applied 09_Saifogeo GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 9;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 09_Saifogeo GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/10_poruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '10_poruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x358) {
					fread(&romBuf[0x56C3E0], 1, curSize, fp);
					printf("Applied 10_Poruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 10;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 10_Poruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/11_dikaporuk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '11_dikaporuk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3A8) {
					fread(&romBuf[0x56C7A8], 1, curSize, fp);
					printf("Applied 11_Dikaporuk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 11;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 11_Dikaporuk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/12_chichi_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '12_chichi_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x364) {
					fread(&romBuf[0x56CBC0], 1, curSize, fp);
					printf("Applied 12_Chichiwomoge GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 12;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 12_Chichiwomoge GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/13_okashi_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '13_okashi_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3F8) {
					fread(&romBuf[0x56CF94], 1, curSize, fp);
					printf("Applied 13_Sweets GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 13;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 13_Sweets GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/14_drink_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '14_drink_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x42C) {
					fread(&romBuf[0x56D3F8], 1, curSize, fp);
					printf("Applied 14_Drink GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 14;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 14_Drink GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/15_koporuk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '15_koporuk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x30C) {
					fread(&romBuf[0x56D890], 1, curSize, fp);
					printf("Applied 15_Koporuk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 15;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 15_Koporuk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/16_goredoruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '16_goredoruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x38C) {
					fread(&romBuf[0x56DC0C], 1, curSize, fp);
					printf("Applied 16_GoRedoruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 16;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 16_GoRedoruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/17_garredoruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '17_garredoruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3A0) {
					fread(&romBuf[0x56E008], 1, curSize, fp);
					printf("Applied 17_GarRedoruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 17;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 17_GarRedoruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/18_goboren_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '18_goboren_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x37C) {
					fread(&romBuf[0x56E418], 1, curSize, fp);
					printf("Applied 18_GoBoren GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 18;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 18_GoBoren GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/19_raodiboren_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '19_raodiboren_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E8) {
					fread(&romBuf[0x56E804], 1, curSize, fp);
					printf("Applied 19_RaoDiBoren GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 19;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 19_RaoDiBoren GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/20_ganzuboren_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '20_ganzuboren_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E4) {
					fread(&romBuf[0x56EC5C], 1, curSize, fp);
					printf("Applied 20_GanzuBoren GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 20;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 20_GanzuBoren GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/21_zegar_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '21_zegar_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x334) {
					fread(&romBuf[0x56F0B0], 1, curSize, fp);
					printf("Applied 21_Zegar GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 21;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 21_Zegar GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/22_ganzuzegar_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '22_ganzuzegar_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E4) {
					fread(&romBuf[0x56F454], 1, curSize, fp);
					printf("Applied 22_GanzuZegar GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 22;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 22_GanzuZegar GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/23_amzegar_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '23_amzegar_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3B0) {
					fread(&romBuf[0x56F8A8], 1, curSize, fp);
					printf("Applied 23_AmZegar GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 23;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 23_AmZegar GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/24_zegaruga_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '24_zegaruga_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x39C) {
					fread(&romBuf[0x56FCC8], 1, curSize, fp);
					printf("Applied 24_Zegaruga GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 24;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 24_Zegaruga GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/25_rajazerusen_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '25_rajazerusen_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3BC) {
					fread(&romBuf[0x5700D4], 1, curSize, fp);
					printf("Applied 25_RajaZerusen GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 25;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 25_RajaZerusen GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/26_zerusen_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '26_zerusen_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x334) {
					fread(&romBuf[0x570500], 1, curSize, fp);
					printf("Applied 26_Zerusen GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 26;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 26_Zerusen GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/27_koboruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '27_koboruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x328) {
					fread(&romBuf[0x5708A0], 1, curSize, fp);
					printf("Applied 27_Koboruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 27;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 27_Koboruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/28_mikormazegaruga_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '28_mikormazegaruga_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3FC) {
					fread(&romBuf[0x570C38], 1, curSize, fp);
					printf("Applied 28_MikorMaZegaruga GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 28;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 28_MikorMaZegaruga GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/29_giganozegar_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '29_giganozegar_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3A4) {
					fread(&romBuf[0x5710A4], 1, curSize, fp);
					printf("Applied 29_GiganoZegar GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 29;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 29_GiganoZegar GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/30_shudoruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '30_shudoruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3A8) {
					fread(&romBuf[0x5714B8], 1, curSize, fp);
					printf("Applied 30_Shudoruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 30;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					// Bari placeholders
					pos = 34;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 30_Shudoruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/31_goshudoruk_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '31_goshudoruk_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3B8) {
					fread(&romBuf[0x5718D0], 1, curSize, fp);
					printf("Applied 31_GoShudoruk GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 31;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					// Bari placeholders
					pos = 35;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 31_GoShudoruk GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/32_merumeru_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '32_merumeru_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x414) {
					fread(&romBuf[0x571CF8], 1, curSize, fp);
					printf("Applied 32_MeruMeruMe GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 32;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					// Bari placeholders
					pos = 36;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 32_MeruMeruMe GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/33_groovy_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '33_groovy_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x370) {
					fread(&romBuf[0x572178], 1, curSize, fp);
					printf("Applied 33_Groovy GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 33;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					// Bari placeholders
					pos = 37;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 33_Groovy GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/38_reis_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '38_reis_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x2C4) {
					fread(&romBuf[0x572558], 1, curSize, fp);
					printf("Applied 38_Reis GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 38;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 38_Reis GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/39_gravirei_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '39_gravirei_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x31C) {
					fread(&romBuf[0x57288C], 1, curSize, fp);
					printf("Applied 39_Gravirei GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 39;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 39_Gravirei GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/40_babergravidon_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '40_babergravidon_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x31C) {
					fread(&romBuf[0x57288C], 1, curSize, fp);
					printf("Applied 40_BaberGravidon GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 40;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 40_BaberGravidon GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/41_riorreis_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '41_riorreis_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x38C) {
					fread(&romBuf[0x573088], 1, curSize, fp);
					printf("Applied 41_RiorReis GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 41;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 41_RiorReis GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/42_iongravirei_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '42_iongravirei_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3E4) {
					fread(&romBuf[0x573484], 1, curSize, fp);
					printf("Applied 42_IonGravirei GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 42;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 42_IonGravirei GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/43_bidomgravirei_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '43_bidomgravirei_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x3BC) {
					fread(&romBuf[0x5738D8], 1, curSize, fp);
					printf("Applied 43_BidomGravirei GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 43;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 43_BidomGravirei GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/44_giganoreis_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '44_giganoreis_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x390) {
					fread(&romBuf[0x573D04], 1, curSize, fp);
					printf("Applied 44_GiganoReis GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 44;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 44_GiganoReis GFX.\n");
				}
				fclose(fp);
			}
			
			curSize = 0;
			fp = fopen("GFX/GFX_SA/GFX_SA_EN/45_diogagravidon_yorogi.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '45_diogagravidon_yorogi'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x404) {
					fread(&romBuf[0x574104], 1, curSize, fp);
					printf("Applied 45_DiogaGravidon GFX.\n");
				} else {
					// Repoint, size is bigger than original
					uint8_t pos = 45;
					romBuf[0x26042C+((pos*12)-12)]   = curPos;
					romBuf[0x26042C+((pos*12)-12)+1] = curPos >> 8;
					romBuf[0x26042C+((pos*12)-12)+2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed 45_DiogaGravidon GFX.\n");
				}
				fclose(fp);
			}
			// SA GFX END
			
			// NOTE: all scripts will be longer than the original, but still
			// I'll keep the original addresses just in case.
			
			// Script names
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/makai_names_0x60DDAC.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open 'makai_names_0x60DDAC'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x344) {
					fread(&romBuf[0x60DDAC], 1, curSize, fp);
					printf("Applied script names.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292364] = curPos;
					romBuf[0x292365] = curPos >> 8;
					romBuf[0x292366] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script names.\n");
				}
				fclose(fp);
			}
			
			// Script unk1
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x60E0F0_unk1.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x60E0F0_unk1'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x6B8) {
					fread(&romBuf[0x60E0F0], 1, curSize, fp);
					printf("Applied script unk1.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292368] = curPos;
					romBuf[0x292369] = curPos >> 8;
					romBuf[0x29236A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script unk1.\n");
				}
				fclose(fp);
			}
			
			// Script playdata
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x60E7A8_playdata.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x60E7A8_playdata'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x320) {
					fread(&romBuf[0x60E7A8], 1, curSize, fp);
					printf("Applied script playdata.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x29236C] = curPos;
					romBuf[0x29236D] = curPos >> 8;
					romBuf[0x29236E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script playdata.\n");
				}
				fclose(fp);
			}
			
			// Script unk3, skipped
			
			// Script bookmark names
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x60EBA0_Bookmark_names.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x60EBA0_Bookmark_names'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x1428) {
					fread(&romBuf[0x60EBA0], 1, curSize, fp);
					printf("Applied script bookmark names.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292374] = curPos;
					romBuf[0x292375] = curPos >> 8;
					romBuf[0x292376] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script bookmark names.\n");
				}
				fclose(fp);
			}
			
			// Script SA names
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x60FFC8_SA_names.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x60FFC8_SA_names'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x7F0) {
					fread(&romBuf[0x60FFC8], 1, curSize, fp);
					printf("Applied script SA names.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292378] = curPos;
					romBuf[0x292379] = curPos >> 8;
					romBuf[0x29237A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script SA names.\n");
				}
				fclose(fp);
			}
			
			// Script Areas
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611B90_Areas.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611B90_Areas'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0xD8) {
					fread(&romBuf[0x611B90], 1, curSize, fp);
					printf("Applied script areas.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292384] = curPos;
					romBuf[0x292385] = curPos >> 8;
					romBuf[0x292386] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script areas.\n");
				}
				fclose(fp);
			}
			
			// Script city
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611C68_Area_city.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611C68_Area_city'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x94) {
					fread(&romBuf[0x611C68], 1, curSize, fp);
					printf("Applied script city.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292388] = curPos;
					romBuf[0x292389] = curPos >> 8;
					romBuf[0x29238A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script city.\n");
				}
				fclose(fp);
			}
			
			// Script halls
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611CFC_Area_halls.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611CFC_Area_halls'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x94) {
					fread(&romBuf[0x611CFC], 1, curSize, fp);
					printf("Applied script halls.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x29238C] = curPos;
					romBuf[0x29238D] = curPos >> 8;
					romBuf[0x29238E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script halls.\n");
				}
				fclose(fp);
			}
			
			// Script coast
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611D90_Area_coast.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611D90_Area_coast'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x94) {
					fread(&romBuf[0x611D90], 1, curSize, fp);
					printf("Applied script coast.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292390] = curPos;
					romBuf[0x292391] = curPos >> 8;
					romBuf[0x292392] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script coast.\n");
				}
				fclose(fp);
			}
			
			// Script forest
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611E24_Area_forest.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611E24_Area_forest'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x84) {
					fread(&romBuf[0x611E24], 1, curSize, fp);
					printf("Applied script forest.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292394] = curPos;
					romBuf[0x292395] = curPos >> 8;
					romBuf[0x292396] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script forest.\n");
				}
				fclose(fp);
			}
			
			// Script floors
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611EA8_Area_floors.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611EA8_Area_floors'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x84) {
					fread(&romBuf[0x611EA8], 1, curSize, fp);
					printf("Applied script floors.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292398] = curPos;
					romBuf[0x292399] = curPos >> 8;
					romBuf[0x29239A] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script floors.\n");
				}
				fclose(fp);
			}
			
			// Script secret
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611F2C_Area_secret.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611F2C_Area_secret'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x84) {
					fread(&romBuf[0x611F2C], 1, curSize, fp);
					printf("Applied script secret.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x29239C] = curPos;
					romBuf[0x29239D] = curPos >> 8;
					romBuf[0x29239E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script secret.\n");
				}
				fclose(fp);
			}
			
			// Script school
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x611FB0_Area_school.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x611FB0_Area_school'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x84) {
					fread(&romBuf[0x611FB0], 1, curSize, fp);
					printf("Applied script school.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x2923A0] = curPos;
					romBuf[0x2923A1] = curPos >> 8;
					romBuf[0x2923A2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script school.\n");
				}
				fclose(fp);
			}
			
			// Script library
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x612034_library.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x612034_library'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x64) {
					fread(&romBuf[0x612034], 1, curSize, fp);
					printf("Applied script library.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x2923A4] = curPos;
					romBuf[0x2923A5] = curPos >> 8;
					romBuf[0x2923A6] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script library.\n");
				}
				fclose(fp);
			}
			
			// Script 2lines
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x6107B8_2lines.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x6107B8_2lines'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x24) {
					fread(&romBuf[0x6107B8], 1, curSize, fp);
					printf("Applied script 2lines.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x29237C] = curPos;
					romBuf[0x29237D] = curPos >> 8;
					romBuf[0x29237E] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script 2lines.\n");
				}
				fclose(fp);
			}
			
			// Script bookmark descriptions
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/0x6107DC_Bookmark_desc.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open '0x6107DC_Bookmark_desc'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x13B4) {
					fread(&romBuf[0x6107DC], 1, curSize, fp);
					printf("Applied script BookmarkDescs.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x292380] = curPos;
					romBuf[0x292381] = curPos >> 8;
					romBuf[0x292382] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script BookmarkDescs.\n");
				}
				fclose(fp);
			}
			
			// Script 01
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/script_01.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open 'script_01'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x1831) {
					fread(&romBuf[0x612098], 1, curSize, fp);
					printf("Applied script Script01.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x2923A8] = curPos;
					romBuf[0x2923A9] = curPos >> 8;
					romBuf[0x2923AA] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script Script01.\n");
				}
				fclose(fp);
			}
			
			// Script 02
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/script_02.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open 'script_02'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x1129) {
					fread(&romBuf[0x6138CC], 1, curSize, fp);
					printf("Applied script Script02.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x2923AC] = curPos;
					romBuf[0x2923AD] = curPos >> 8;
					romBuf[0x2923AE] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script Script02.\n");
				}
				fclose(fp);
			}
			
			// Script 03
			curSize = 0;
			fp = fopen("SCRIPTS/ENG/BIN/script_03.bin", "rb");
			if(fp == NULL) {
				printf("Failed to open 'script_03'!\n\n");
				//break;
			} else {
				fseek(fp, 0, SEEK_END);
				curSize = ftell(fp);
				fseek(fp, 0, SEEK_SET);
				if(curSize <= 0x1980) {
					fread(&romBuf[0x6149F8], 1, curSize, fp);
					printf("Applied script Script03.\n");
				} else {
					// Repoint, size is bigger than original
					romBuf[0x2923B0] = curPos;
					romBuf[0x2923B1] = curPos >> 8;
					romBuf[0x2923B2] = curPos >> 16;
					
					fread(&romBuf[curPos], 1, curSize, fp);
					curPos += curSize;
					printf("Applied and repointed script Script03.\n");
				}
				fclose(fp);
			}
			
			
			// VO
			if(altVO) {
				// English lines from MB/MF/anime
				;
				
				// Spanish lines from anime
				
			}
			
			// Create file to save the patched data
			fp = fopen("makai_patched.gba", "wb");
			fwrite(romBuf, 1, romSize, fp);
			fclose(fp);
			
			free(romBuf);
			
			//printf("Beginning patches...\n");
			
			printf("Finished!\n");
			
			
			//fseek(fp, 0, SEEK_SET);
			
			break;
		}
		else if(strcmp(argv[i], "-x") == 0) // load a rom and extract all HUFF/LZ files
		{
				++i;   //skip arg
				x = 0; //reset parse
				
				printf("HUFFMAN+LZ77 Extraction Mode\n\n");
				printf("Loading ROM: %s\n\n", argv[i]);
				
				int valRead = 0;
				int fileSz = 0;
				FILE *fp = fopen(argv[i], "rb");
				if(fp == NULL) {
					printf("Failed to open file!\n\nAre you casting the right spell, human?!\n\n");
					break;
				}
				
				// get file size
				fseek(fp, 0, SEEK_END);
				fileSz = ftell(fp);
				
				fseek(fp, 0, SEEK_SET);
				
				if(fileSz < (8*1024*1024)) {
					printf("Not a valid file... Size is too small.\n");
					break;
				}
				
				// Check the first bytes to see if this is a ROM
				fread(&valRead, 1, 4, fp);
				
				if(valRead != 0xEA00002E)
					printf("This might not be the correct ROM.\n");
				
				fseek(fp, 0xA8, SEEK_SET);
				fread(&valRead, 1, 4, fp);
				
				if(valRead != 0x4B52414D)
					printf("This is definitely not the correct ROM.\n");
				
				printf("Scanning ROM for compressed files...\n");
				
				int foundCMP = 0;
				uint32_t find = 0;
				for(find = 0x2F0; find < fileSz; find+=4) {
					fseek(fp, find, SEEK_SET);
					fread(&valRead, 1, 4, fp);
					
					if(((valRead & 0xFF0000FF) == 0x40) && (valRead & 0xFFFF00) > 0x50000) {
						// check huffman
						fread(&valRead, 1, 4, fp);
						if((valRead & 0xFF) == 0x24) { // It's huffman, take the shot
							//++foundCMP;
							
							//printf("Show me ptr calc: 0x%X\n", (find << 8 | 8)); // le duhhh
						//	printf("Show me ptr calc: 0x%X\n", (find | 8 << 24));
							
							// Extra check, see if it has a pointer
							fseek(fp, 0, SEEK_SET);
							bool foundPTR = false;
							int vq = 0;
							for(vq = 0x2F0; vq < fileSz; vq+=4) {
								fseek(fp, vq, SEEK_SET);
								fread(&valRead, 1, 4, fp);
								
								if(valRead == (find | 8 << 24)) { // found pointer
									foundPTR = true;
									++foundCMP;
									break;
								}
							}
							
							if(!foundPTR)
								continue;
							
							// NOTE: this will write 1344 files and it's like a 20 min process
							
							// don't write for now
							//continue;
							
							// write it to a file
							uint16_t unSz = 0; // no idea how else to do this
							fseek(fp, find, SEEK_SET);
							fread(&valRead, 1, 4, fp);
							
							unSz = valRead >> 8;
							
							// show me
						//	printf("File is: 0x%X bytes.\n", unSz);
							
							char fileName[64] = {0};
							uint8_t* extBuf = malloc(unSz);
							
							fread(extBuf, 1, unSz, fp);
							
							sprintf(fileName, "0x%X_file%04d.bin", find, foundCMP);
							
							FILE* fext = fopen(fileName, "wb");
							fwrite(extBuf, 1, unSz, fext);
							fclose(fext);
							free(extBuf);
							
							// skip reading what we already know is not valid
							//find += unSz; // yeah this is not a good idea
							
							// Only do one file for now
							//break;
						}
					}
				}
				
				printf("Found %d compressed files.\n", foundCMP);
				
				fclose(fp);
				break;
        }
	   else if(strcmp(argv[i], "-t") == 0) // Print a string given as argument in MAKAI format
       {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("\n");
                printf("Makai String Printing Mode\n\n");
				
       for(x = 0; x < strlen(argv[i]); x++) {
          if(ConvertChar(argv[i][x]) != 0) {
             //need to handle special characters or commands
             if(strcmp(argv[i], "/br") == 0) {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("Found command BREAK!\n");
                printf("%X", 0x254E);
                //half is 7px, full is 14px
                if((halfLimit*7) + (fullLimit*14) > 154 && !lineBroke) {
                   //printf("\nExceeded first line.\n");
                   halfLimit1 = halfLimit;
                   fullLimit1 = fullLimit;
                }

                lineBroke = true;
                halfLimit = fullLimit = 0;
             }
             if(strcmp(argv[i], "/help") == 0) //print all commands and instructions
             {
                ++i;   //skip arg
                x = 0; //reset parse
                //printf("\n");
                printf("USAGE INSTRUCTIONS:\nTo write spaces, your argument must be enclosed in quotes. ");
                printf("Since each space without quotes is another argument.\nArguments are used to do other commands.\n");
                printf("EXAMPLE: Text2Makai \"This is \" /blue1 Bari.\n\n");
                printf("Notice the space between \"This is \" and /blue1 and Bari.\n");
                printf("These are three arguments, first is basic text ending in a space, second is a command to turn the text outline blue\nand the final argument is more text but will show with a blue outline in-game.\n\n");

                printf("These are the accepted commands:\n/black   = Turns text black and outline white.\n");
                printf("/blue1   = Turns outline blue.\n");
                printf("/blue2   = Turns text blue and outline white.\n");
                printf("/red1    = Turns outline red.\n");
                printf("/red2    = Turns text red and outline white.\n");
                printf("/green1  = Turns outline green.\n");
                printf("/green2  = Turns text green and outline white.\n");
                printf("/orange  = Turns outline orange.\n");
                printf("/restore = Turns text black and outline white.\n");
                printf("/br      = Creates a linebreak for dialogue text. Only two lines will display, more breaks will increase dialogue.\n\n");

                printf("If the text length exceeds for either line 1 or line 2, a simple notification will be printed.\n");
                printf("This limit is calculated not by the amount of characters but by their pixel width.\n");
                printf("Full-width is 14 pixels wide, while half-width is 7 pixels wide.\n");

                //printf("\n");
             }
             if(strcmp(argv[i], "/restore") == 0) //white text, black outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254931);
             }
             if(strcmp(argv[i], "/black") == 0) //black text, white outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254932);
             }
             if(strcmp(argv[i], "/blue1") == 0) //blue outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254933);
             }
             if(strcmp(argv[i], "/blue2") == 0) //blue text, white outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254934);
             }
             if(strcmp(argv[i], "/red1") == 0) //red outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254935);
             }
             if(strcmp(argv[i], "/red2") == 0) //red text, white outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254936);
             }
             if(strcmp(argv[i], "/green1") == 0) //green outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254937);
             }
             if(strcmp(argv[i], "/green2") == 0) //green text, white outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254938);
             }
             if(strcmp(argv[i], "/orange") == 0) //orange outline
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x254939);
             }

             //support a few symbols
             if(strcmp(argv[i], "/inf") == 0) //infinity
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x812D);
             }
             if(strcmp(argv[i], "/m") == 0) //male
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x812F);
             }
             if(strcmp(argv[i], "/f") == 0) //female
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8130);
             }
             if(strcmp(argv[i], "/cel") == 0) //celcius
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8134);
             }
             if(strcmp(argv[i], "/star") == 0) //transparent star
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x813F);
             }
             if(strcmp(argv[i], "/wstar") == 0) //white star
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8140);
             }
             if(strcmp(argv[i], "/circle") == 0 || strcmp(argv[i], "/maru") == 0)
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8141);
             }
             if(strcmp(argv[i], "/dia") == 0) //dia
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8144);
             }
             if(strcmp(argv[i], "/wdia") == 0) //dia filled
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8145);
             }
             if(strcmp(argv[i], "/square") == 0)
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8146);
             }
             if(strcmp(argv[i], "/wsquare") == 0)
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8147);
             }
             if(strcmp(argv[i], "/triangle") == 0)
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8148);
             }
             if(strcmp(argv[i], "/wtriangle") == 0)
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8149);
             }
             if(strcmp(argv[i], "/right") == 0) //arrow
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x814E);
             }
             if(strcmp(argv[i], "/left") == 0) //arrow
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x814F);
             }
             if(strcmp(argv[i], "/up") == 0) //arrow
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8150);
             }
             if(strcmp(argv[i], "/down") == 0) //arrow
             {
                ++i;   //skip arg
                x = 0; //reset parse
                printf("%X", 0x8151);
             }

             printf("%04X", ConvertChar(argv[i][x]));

             if(lineBroke && ConvertChar(argv[i][x]) == 0x254E)
                printf("WARNING: You already did a linebreak!\n\n");

             if(ConvertChar(argv[i][x]) >> 8 == 0xE0)
                ++halfLimit;
             else if((ConvertChar(argv[i][x]) & 0xF000) == 0x8000)
                ++fullLimit;
          }
        }
		}
      }
      //half is 7px, full is 14px
      if(!lineBroke && (halfLimit*7) + (fullLimit*14) > 154)
          printf("\nYou have exceeded the 1st dialogue box 154 pixel limit by %d pixels.\n", (halfLimit*7) + (fullLimit*14));

      //only if a linebreak happened
      if(lineBroke && (halfLimit1*7) + (fullLimit1*14) > 154)
          printf("\nYou have exceeded the 1st dialogue box 154 pixel limit by %d pixels.\n", (halfLimit1*7) + (fullLimit1*14));
      if(lineBroke && (halfLimit*7) + (fullLimit*14) > 154)
          printf("\nYou have exceeded the 2nd dialogue box 154 pixel limit by %d pixels.\n", (halfLimit*7) + (fullLimit*14));
  }
  else
  {
     printf("No characters to convert.\n");
     printf("Use /help to list usage instructions.\n");
  }

  printf("\n");

  return 0;
}
