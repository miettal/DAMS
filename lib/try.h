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
  int Find(char* key, PtrVect<void*>& result, char cond = MATCH_EXACT) const;
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
  int Find(char* key, ObjVect<long>& result, char cond, long base);
  StaticPtrTry* pack(StaticPtrTry* newaddress, long base);
  unsigned long packedSize(void);
  long list(int i, long base);
  StaticPtrTry* next(int i, long base);
  int copy(ObjVect<long>&, long base);
  int hasMember(char* key, long base);
};

#endif /* tryH */
