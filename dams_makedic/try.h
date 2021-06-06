#ifndef tryH
#define tryH

#include <memory.h>
#include <assert.h>
#include "tmplvect.h"

#define MATCH_EXACT      0 // �������פΤ�
#define MATCH_LONGER     1 // ���ꤷ��ʸ����ǻϤޤ�Ĺ��ʸ����
#define MATCH_SHORTER    2 // ���ꤷ��ʸ����˴ޤޤ��û��ʸ����

#define STATICS_SIZE    10 // ���פ��륵����
extern unsigned int statics[STATICS_SIZE];

typedef unsigned short int wchar;

/* �ȥ饤 */
class PtrTry
{
public:
  char _letter;
  PtrVect<void*> *_list; // ����ʸ���ǽ���ǡ����Υꥹ��
  PtrVect<PtrTry*> *_next; // ����ʸ���ؤΥꥹ��
public:
  PtrTry(char letter = '\0');
  ~PtrTry(void);
  void Add(void*, char* name);
  void Find(const char* key, PtrVect<void*>& result, char cond = MATCH_EXACT) const;
  void Remove(void*, char* key);
  void Clear(void);
  void MakeStatics(void);
  void QSort(int (*)(const void*, const void*));
};

/* ��Ū�ȥ饤 */
/* Add �� Remove �ϤǤ��ʤ� */
class StaticPtrTry
{
public:
  char _letter;
  int _nList;
  int _nNext;
  long* _pList;
  StaticPtrTry** _pNext;
public:
  StaticPtrTry(PtrTry*);
  void Find(const char* key, PtrVect<long>& result, char cond = MATCH_EXACT, long base = 0);
  StaticPtrTry* pack(StaticPtrTry* newaddress, long base = 0);
  unsigned long packedSize(void);
  long list(int i, long base = 0);
  StaticPtrTry* next(int i, long base = 0);
};

#endif /* tryH */
