/* makealias.c */
#include "headers.h"
#include "aliastree.h"

static dam* _search_tree(PtrVect<char*>*);
static void _regist_tree(PtrVect<char*>* to, PtrVect<char*>* from, dam* dfrom, int flag);
static aliasTree _linkedTree("root");
static aliasTree _aliasTree("root");

// �����ꥢ���ǡ������ɤߤ���
void read_aliases(FILE* ifp) {
  int i, ele;
  PtrVect<char*>* args;
  PtrVect<char*> p0, p1;
  char buf[BUFSIZ];

  /* aliasTree �κ��� */
  rewind(ifp);
  while (fgets(buf, BUFSIZ, ifp)) {
    if (debug) {
      //      fprintf(stderr, "line: %s", buf);
    }
    p0.Clear();
    p1.Clear();
    args = get_args(buf, ',');
    if (args->Elements() <= 1) {
      args = get_args(buf, '\t');
    }
    ele = args->Elements();
    if (buf[0] == '!') continue; /* ̵�� */
    for (i = 0; i < ele && strcmp(args->Element(i), "="); i++)
      p0.Add(args->Element(i));
    i++;
    for (;i < ele; i++)
      p1.Add(args->Element(i));
    args->Clear();
    /* p0, p1 �� aliasTree �˵�Ͽ */
    _aliasTree.addDescendant(&p0);
    _aliasTree.addDescendant(&p1);
  }

  /* �����ꥢ����󥯳��� */
  rewind(ifp);
  while (fgets(buf, BUFSIZ, ifp)) {
    if (debug) {
      //      fprintf(stderr, "line: %s", buf);
    }
    // p0 �˸Ť���̾��p1�˿�������̾���ɤ߹���
    p0.Clear();
    p1.Clear();
    args = get_args(buf, ',');
    if (args->Elements() <= 1) {
      args = get_args(buf, '\t');
    }
    ele = args->Elements();
    if (!strcmp(args->Element(0), "!")) {
      continue;
#if 0
      for (i = 1; i < ele; i++)
	p0.Add(args->Element(i));
      cut_alias(&p0);
#endif /* 0 */
    } else {
      for (i = 0; i < ele && strcmp(args->Element(i), "="); i++)
	p0.Add(args->Element(i));
      i++; // skip "="
      for (; i < ele; i++)
	p1.Add(args->Element(i));
      args->Clear();
      make_alias(&p0, &p1);
    }
  }
  rewind(ifp);
}

// �����ꥢ���ν���
void make_alias(PtrVect<char*>* p0, PtrVect<char*>* p1) {
  int i;
  dam *d0, *d1;
  aliasTree *at0, *at1;

  d0 = _search_tree(p0);
  d1 = _search_tree(p1);

  /* �����ꥢ���ڤΥ����å� */
  at0 = _linkedTree.searchDescendant(p0);  // p0 �Ͽ������ɲúѤ�
  at1 = _linkedTree.searchDescendant(p1);  // p1 �Ͽ������ɲúѤ�
  if (d0 && at0) {
    d0 = NULL;
    if (debug) {
      for (i = 0; i < p0->Elements(); i++) {
	fprintf(stderr, "%s ", E2U(p0->Element(i)));
      }
      fprintf(stderr, " has been registered already.\n");
    }
  }
  if (d1 && at1) {
    d1 = NULL;
    if (debug) {
      for (i = 0; i < p1->Elements(); i++) {
	fprintf(stderr, "%s ", E2U(p1->Element(i)));
      }
      fprintf(stderr, " has been registered already.\n");
    }
  }

  if (debug && (d0 || d1)) {
    fprintf(stderr, "alias: p0 = %x, ", d0);
    for (i = 0; i < p0->Elements(); i++) {
      fprintf(stderr, "%s/", E2U(p0->Element(i)));
    }
    fprintf(stderr, ": p1 = %x, ", d1);
    for (i = 0; i < p1->Elements(); i++) {
      fprintf(stderr, "%s/", E2U(p1->Element(i)));
    }
    fprintf(stderr, "\n");
  }
  if (d0) {
    _regist_tree(p1, p0, d0, 1);
    _linkedTree.addDescendant(p1);
  } else if (d1) {
    _regist_tree(p0, p1, d1, 0);
    _linkedTree.addDescendant(p0);
  }
}

// ���פʥ����ꥢ���κ��
void cut_alias(PtrVect<char*>* p) {
  PtrVect<void*> list;
  int i;
  dam *d, *parent;

  abort(); /* this function is obsoleted */
}

// ���ع�¤�ڤ��� p ��ɽ�������̾���ؤ򸡺�
dam* _search_tree(PtrVect<char*>* p) {
  int i, j, count;
  PtrVect<void*> list;
  dam* d;

  damsTryName.Find(standardize(p->Element(0)), list);
  count = list.Elements();
  for (i = 0; i < count; i++) {
    d = (dam*) list[i];
    if (d->_pparent) {
      if (debug) fprintf(stderr, "in _search_tree, skipping %s|%s|%s\n", E2U(d->_pparent->_parent), E2U(d->_pparent->_name), E2U(d->_name));
      continue; /* �����ꥢ���κǾ����̾�ˤϿƤ��ʤ��Ϥ� */
    }
    for (j = 1; j < p->Elements(); j++) {
      if ((d = d->SearchChild(p->Element(j))) == NULL) break;
    }
    if (d) return d;
  }
  return NULL;
}

// ���ع�¤�ڤ� p ��ɽ�������̾���ؤ���Ͽ
void _regist_tree(PtrVect<char*>* p, PtrVect<char*>* from, dam* org, int flag) {
  int i, j, count;
  PtrVect<void*> list;
  dam *d, *child, *parent, *added, *reserved;
  char* name;
  float x, y;
  int level;
  aliasTree* cur_from, * cur_to;

  x = org->_x;
  y = org->_y;
  level = org->_level;

  /* ���ꤹ�륨���ꥢ����ɽ�� */
  if (debug) {
    for (i = 0; i < p->Elements(); i++) {
      fprintf(stderr, "%s/", E2U(p->Element(i)));
    }
    if (flag) {
      fprintf(stderr, "<-");
    } else {
      fprintf(stderr, "->");
    }
    for (i = 0; i < from->Elements(); i++) {
      fprintf(stderr, "%s/", E2U(from->Element(i)));
    }
    fprintf(stderr, "\n");
  }
  cur_from = _aliasTree.searchDescendant(from);
  cur_to   = _aliasTree.searchDescendant(p);
  if (!cur_from) {
    fprintf(stderr, "The original path was not found in _aliasTree.\n");
    abort();
  }

  /* �Ǿ�̤���̾��ȥ饤����ǥå������鸡������ */
  damsTryName.Find(standardize(p->Element(0)), list);
  if (list.Elements() == 0) {
    d = new dam(p->Element(0), (char*)"", x, y, 0);
    reserved = d;
    //    damsArray.Add(d);
    //    damsTryName.Add(d, d->key());
  } else {
    int count = list.Elements();
    for (i = 0; i < count; i++) {
      d = (dam*) list[i];
      if (d->_pparent) {
	if (debug) fprintf(stderr, "in _regist_tree, skipping %s|%s|%s\n", E2U(d->_pparent->_parent), E2U(d->_pparent->_name), E2U(d->_name));
	d = NULL;
      } else {
	reserved = NULL;
	break;
      }
    }
    if (d == NULL) {
      fprintf(stderr, "All original nodes have parents..\n");
      abort();
    }
  }

  /* �Ǿ�̤��鳬���ڹ�¤�򤿤ɤ� */
  if (debug) fprintf(stderr, "Looking for descendant. ");
  for (i = 1; i < p->Elements(); i++) {
    parent = d;
    name = p->Element(i);
    d = d->SearchChild(name);
    if (debug) fprintf(stderr, " %s", E2U(name));
    if (!d) {
      if (debug) fprintf(stderr, ":added");
      d = new dam(p->Element(i), parent->_name, x, y, 0);
      d->_pparent = parent;
      added = parent->AddChild(d);
      if (d == reserved) reserved = NULL;
      if (added != d) {
	delete d;
	d = added;
      } else {
	if (parent->_level <= 5 || _isNumeric(d->key()) < 0) damsTryName.Add(d, d->key());
	damsArray.Add(d);
      }
    } else {
      if (debug) fprintf(stderr, ":found");
    }
  }
  if (debug) fprintf(stderr, "\n");
  parent = d;
  if (reserved) {
    damsArray.Add(reserved);
    damsTryName.Add(reserved, reserved->key());
  }
  
  /* �ǲ��̤���Ʊ��̾������ʬ������å����� */
  if (d != org) {
    if (d == 0) d->_level = org->_level;
    /* �������Ρ��� d �� org �Ρ��ɤλҤ��ɲ� */
    if (org->_pchildren) {
      if (!d->_pchildren) d->_pchildren = new PtrVect<dam*>;
      char* parentname = d->_name;
      int len = strlen(parentname);
      // *(d->_pchildren) += *(org->_pchildren);
      for (i = 0; i < org->_pchildren->Elements(); i++) {
	child = org->GetChild(i);
	if (debug) fprintf(stderr, "'%s/%s/%s'='%s/%s/%s' ", E2U(org->_parent), E2U(org->_name), E2U(child->_name), E2U(d->_parent), E2U(d->_name), E2U(child->_name));
	if (cur_from->searchChild(child->key()) || cur_to->searchChild(child->key())) {
	  /* ���λҥΡ��ɤϸ�ǽ�������� */
	  if (debug) {
	    fprintf(stderr, "Skipping %s as it is listed on _aliasTree\n", E2U(child->_name));
	  }
	} else {
	  added = d->AddChild(child);
	  if (flag) added->_pparent = d;
	  if (added != child) {
	    /* �Ρ��� d �ˤϴ��� child ��Ʊ̾�λҥΡ��ɤ��ޤޤ�Ƥ��� */
	    if (debug) fprintf(stderr, "Warning: node '%s/%s' has a child named %s already.\n", E2U(d->_parent), E2U(d->_name), E2U(child->_name));
	    org->_pchildren->Replace(child, added);
	    damsTryName.Remove(child, child->key());
	    damsTryParent.Remove(child, child->_parent);
	    child = added;
	  }
	  if (flag) {
	    /* �Ť��Ρ��ɤλҥΡ��ɤ��鿷�����Ρ��ɤ˥�󥯤�ľ�� */
	    delete[] child->_parent;
	    child->_parent = new char[len + 1];
	    strcpy(child->_parent, parentname);
	    child->_pparent = d;
	    if (debug) fprintf(stderr, "(->'%s')\n", E2U(d->_name));
	  } else {
	    if (debug) fprintf(stderr, "(: '%s')\n", E2U(org->_name));
	  }
	}
      }
    }
  }
}
