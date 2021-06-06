/* maketree.c */
#include "headers.h"

// ���Ƥ� TREE ����ľ��
void make_all_tree(void) {
  int index, count = damsArray.Elements(), proc = 1;
  dam* d;
  for (int i = 0; i < count; i++) {
    if (i + 1 >= proc * count / 10) {
      fprintf(stderr, "%d%% ", proc * 10);
      proc++;
    }
    d = damsArray[i];
    if (d) {
      make_tree(d);
    }
  }
  fputc('\n', stderr);
}

// �ƻҴط��κ���
void make_tree(dam* key) {
  PtrVect<void*> list, children;
  dam* parent, *added;

  if (debug) {
    //    fprintf(stderr, "%s/%s �οƻҴط��������ޤ�\n", key->_parent, key->_name);
  }
    
  // ̤��³��󥯤η��
  if (! key->_pparent) {
    parent = get_parent(key);
    if (debug) fprintf(stderr, "  �ƤؤΥݥ��󥿤����ꤵ��Ƥ��ޤ���");
    if (parent) {
      key->_pparent = parent;
      added = parent->AddChild(key);
      if (added != key) {
	fprintf(stderr, "  make_tree: key(%s/%s) is deleted.\n", key->_parent, key->_name);
	delete key;
      }
      if (debug) fprintf(stderr, "  �Ƥ��ߤĤ���ޤ�����('%s')\n", parent->_name);
    } else {
      if (debug) fprintf(stderr, "  �Ƥ��ߤĤ���ޤ���Ǥ�����\n");
    }
  } else {
    //    if (debug) fprintf(stderr, "  �ƤؤΥݥ��󥿤��������ꤵ��Ƥ��ޤ���('%s')\n", key->_pparent->_name);
  }
}
