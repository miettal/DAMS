/* strlib.h */
/* $BA43Q?t;z$d4A?t;z$N=hM}$J$I$r9T$&4X?t72(B */

extern const char* hihun[];
extern const char* kansuji[];
extern const char* arabic;

extern unsigned short int hexval(const char*);
extern void set_hexval(char*, unsigned short int);
extern int _isHihun(const char*);
extern int _isKansuji(const char*);
extern int _isArabsuji(const char*);
extern int _isNumeric(const char*);
extern int _isKanji(const char*);
extern int _getWcharIndex(const char*, int pos);
extern const char* indexof(const char* key, const char* string);
extern int getNumber(const char* str, int& bytes);
extern int kstrncpy(char* dst, const char* src, int maxlen);
