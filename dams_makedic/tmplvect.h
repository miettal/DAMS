#ifndef tmplvectH
#define tmplvectH

#include <memory.h>
#include <strings.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULT_SIZE 10

/* �ݥ�������Υƥ�ץ졼�� */
template <class T> class PtrVect
{
protected:
  T* _data;
  int _elements;
  int _size;
  int _delta;
public:
  PtrVect(int initsize = DEFAULT_SIZE);
  ~PtrVect(void);
  int Elements(void) const { return _elements; }
  T& Element(int i);
  T& operator [ ](int i);
  void operator =(const PtrVect<T>&);
  void operator +=(const PtrVect<T>&);
  void Add(T);
  void InsertAt(int, T);
  int Find(T) const;
  void Remove(T);
  void RemoveAt(int);
  void Replace(T, T);
  void Clear(void);
  void ClearElements(void);
  void QSort(int (*fcmp)(const void *, const void *));
};

template <class T>
PtrVect<T>::PtrVect(int n)
{
  _data = (T*)malloc(sizeof(T) * n);
  _elements = 0;
  _size = n;
  _delta = n;
  memset(_data, 0, sizeof(T) * _size);
}

template <class T>
PtrVect<T>::~PtrVect()
{
  free(_data);
}

template <class T>
T& PtrVect<T>::Element(int i)
{
  if (i < 0 || i >= _elements) assert(0); //throw ("PtrVect: Illegal Array Index.");
  return _data[i];
}

template <class T>
T& PtrVect<T>::operator[](int i)
{
  if (i < 0 || i >= _elements) assert(0); //throw ("PtrVect: Illegal Array Index.");
  return _data[i];
}

template <class T>
void PtrVect<T>::operator =(const PtrVect<T>& d) {
  free(_data);
  _elements = d._elements;
  _size = d._size;
  _delta = d._delta;
  _data = (T*)malloc(sizeof(T) * _size);
  memcpy(_data, d._data, sizeof(T) * _elements);
}

template <class T>
void PtrVect<T>::operator +=(const PtrVect<T>& d) {
  for (int i = 0; i < d._elements; i++)
    Add(d._data[i]);
}

template <class T>
void PtrVect<T>::Add(T newobj)
{
  InsertAt(_elements, newobj);
}

template<class T>
void PtrVect<T>::InsertAt(int index, T newobj)
{
  if (_elements == _size) {
    // �������� _delta ���䤹
    size_t sizeoft = sizeof(T);
    T* newdata;
    newdata = (T*)realloc(_data, sizeoft * (_size + _delta));
    if (!newdata) { // realloc failed
      newdata = (T*)malloc(sizeoft * (_size + _delta));
      memmove(newdata, _data, sizeoft * _elements);
      free(_data);
      _data = newdata;
    } else {
      _data = newdata;
    }
    _size += _delta;
  }
  
  T* src, * dst;
  size_t size = sizeof(T) * (_elements - index);
  src = _data + index;
  dst = src + 1;
  memmove(dst, src, size);
  *src = newobj;
  _elements++;
}

/* obj��õ��������ǥå����ֹ���֤� */
template <class T>
int PtrVect<T>::Find(T obj) const
{
  for (int i = 0; i < _elements; i++)
    {
      if (_data[i] == obj) return i;
    }
  return -1;   // ���Ĥ���ʤ�
}

/* obj��õ���ơ��ꥹ�Ȥ��������롣
   ��ա� obj ���ؤ����󥹥��󥹤� delete ����ʤ� */
template <class T>
void PtrVect<T>::Remove(T obj)
{
  int index = Find(obj);
  if (index < 0) return;
  RemoveAt(index);
}

template <class T>
void PtrVect<T>::RemoveAt(int index)
{
  T* src, * dst;
  size_t size = sizeof(T) * (_elements - index - 1);
  dst = _data + index;
  src = dst + 1;
  memmove(dst, src, size);

  _elements--;
  _data[_elements] = NULL;
  if (_elements < _size - 2 * _delta) {
    /* �������ʤä��Τǥꥵ���� */
    T* newdata = (T*)realloc(_data, sizeof(T) * (_size - 2 * _delta));
    if (!newdata) {
      newdata = (T*)malloc(sizeof(T) * (_size - 2 * _delta));
      memmove(newdata, _data, sizeof(T) * _elements);
      free(_data);
      _data = newdata;
    } else {
      _data = newdata;
    }
    _size -= 2 * _delta;
  }
}

/* oldobj ��õ���ơ������� newobj ���֤�������
   ��ա� oldobj ���ؤ����󥹥��󥹤� delete ����ʤ� */
template <class T>
void PtrVect<T>::Replace(T oldobj, T newobj)
{
  int index = Find(oldobj);
  if (index < 0) {
    throw ("PtrVect: Replace failed.");
  }
  _data[index] = newobj;
}

/* ���٤Ƥ����Ǥ�ꥹ�Ȥ��������롣���󥹥��󥹤� delete ���ʤ� */
template <class T>
void PtrVect<T>::Clear(void)
{
  free(_data);
  _data = (T*)malloc(sizeof(T) * _delta);
  _elements = 0;
  _size = _delta;
  memset(_data, 0, sizeof(T) * _size);
}

/* ���٤Ƥ����Ǥ�ꥹ�Ȥ��������롣���󥹥��󥹤� delete ���� */
template <class T>
void PtrVect<T>::ClearElements(void)
{
  int i;
  for (i = 0; i < _elements; i++) free(_data[i]);
  Clear();
}

// �����å�������
template <class T>
void PtrVect<T>::QSort(int (*fcmp)(const void *, const void *))
{
  qsort(_data, _elements, sizeof(T), fcmp);
}

#endif /* tmplvectH */
