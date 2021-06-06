#ifndef tmpltryH
#define tmpltryH

#include <memory.h>
#include <assert.h>
#include "tmplvect.h"

/* �ȥ饤�Υƥ�ץ졼�� */
template <class T> class PtrTry
{
protected:
  char _letter;
  PtrVect<T> * _line; // ����ʸ���ǽ���ǡ����Υꥹ��
  PtrTry<T> * _next; // ����ʸ���ؤΥꥹ��
public:
  PtrTry(void);
  ~PtrHash(void);
  int Add(T, char* name);
  PtrVect<T> * Find(char* key) const;
  void Remove(T, char* key);
  void Clear(void);
};

template <class T>
PtrTry<T>::PtrTry(void)
{
  _letter = '\0';
  _line = NULL;
  _next = NULL;
}

template <class T>
PtrTry<T>::~PtrTry()
{
  if (_next != NULL) {
    int count = _next->Elements();
    for (int i = 0; i < count; i++) {
      delete _next->Element(i);
    }
    _next->Clear();
  }
  _line->Clear();
}

/* key ����ꤷ�ƥǡ����򥹥�åȤ��ɲä��� */
template <class T>
int PtrHash<T>::Add(T newobj, const void* key)
{
  int index = _hashfunc(key);
  assert(0 <= index && index < _size);
  PtrVect<T>* pslot = _slots + index;
  if (!pslot) {
    /* ����åȤκ��� */
    pslot = new PtrVect<T>;
    pslot->Add(newobj);
  } else if (pslot->Find(newobj) < 0) {
    pslot->Add(newobj);
  }
  return index;
}

/* key ��õ������������ꥹ�Ȥ��֤�
   �ꥹ�Ȥ����ξ�硢 NULL ���֤� */
template <class T>
PtrVect<T>* PtrHash<T>::Find(const void* key) const
{
  int index = _hashfunc(key);
  assert(0 <= index && index < _size);
  return _slots + index;
}

/* obj��õ���ơ��ꥹ�Ȥ��������롣
   ��ա� obj ���ؤ����󥹥��󥹤� delete ����ʤ� */
template <class T>
void PtrHash<T>::Remove(T obj, const void* key)
{
  int index = _hashfunc(key);
  assert(0 <= index && index < _size);
  PtrVect<T> *pslot = _slots[index];
  if (pslot) {
    pslot->Remove(obj);
    if (pslot->Elements() == 0) {
      delete pslot;
      _slots[index] = NULL;
    }
  }
}

template <class T>
void PtrHash<T>::Clear(void)
{
  int i;
  for (i = 0; i < _size; i++) {
    if (_slots[i]) delete _slots[i];
  }
}

#endif /* tmplhashH */
