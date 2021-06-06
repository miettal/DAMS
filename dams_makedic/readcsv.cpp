/* readcsv.c */
#include <ctype.h>
#include <stdio.h>
#include "headers.h"

/* CSV �ǡ������ɤ߹��� */

/*
 CSV �ǡ����ϼ��η����Ȥ���
<top-placename>\t<child>\t<grandchild>\t...\t<longitude>\t<latitude>\t<level>
<top-placename>\t<child>\t<grandchild>\t...\t<longitude>\t<latitude>\t<level>
 ľ���ιԤ�Ʊ���Ƥ�°�����硢��̾���ά���Ƥ�褤
 ��)
 �����,˭���,���ʤ�ë������,13����,15��,139.46823,35.73123,7
 ,,���İ�����,1����,1��,139.84023,35.74485,7
 ��
 �����,˭���,���ʤ�ë������,13����,15��,139.46823,35.73123,7
 �����,˭���,���İ�����,1����,1��,139.84023,35.74485,7
 ��Ʊ��

 �����̵�뤵���
 ���ѥ졼���� TAB �ޤ��� ',' �Ȥ���
 level �ϰʲ��ΤȤ���
 0 -> ̤���
 1 -> ��ƻ�ܸ�
 2 -> ������ģ
 3 -> ��Į¼�����̶�
 4 -> �������Ԥζ�
 5 -> ���
 6 -> ��������
 7 -> ���衦�̤ꡦ����
 8 -> �桦����
 9 -> �ӥ�̾
 10 -> �����ֹ�
 11 -> Ŵƻϩ��̾
 12 -> ��̾
*/

static PtrVect<char*>* args = new PtrVect<char*>(10); // °������¸������

void read_from_csv(FILE* ifp) {
  dam* d, *pparent, *added;
  PtrVect<dam*> last;
  PtrVect<char*>* args;
  char buf[BUFSIZ];
  char name[MAXSTRLEN];
  float x, y, x0, y0, x1, y1;
  int index, ele;
  long int level;
  last.Clear();
  while (fgets(buf, BUFSIZ, ifp)) {
    // longitude, latitude ���ɤ߹���
    args = get_args(buf, ',');
    if (args->Elements() <= 1) { // ���Ԥ�����Τ�ɬ��1�ˤϤʤ�
      args = get_args(buf, '\t');
    }
    ele = args->Elements();
    if (ele > 3) {
      for (int i = 0; i < ele - 3; i++) {
	char* p = args->Element(i);
	if (debug > 0) {
	  fprintf(stderr, "(%d:'%s') ", i, p);
	}
  	if (*p == '\0') {
  	  args->RemoveAt(i);
  	  i--;
  	  ele--;
	  if (debug > 0) {
	    fprintf(stderr, ":deleted ");
	  }
  	}
      }
      x = strtod(args->Element(ele - 3), NULL);
      y = strtod(args->Element(ele - 2), NULL);
      level = atol(args->Element(ele - 1));
      pparent = NULL;
      for (int i = 0; i < ele - 3; i++) {
	strcpy((char*)name, args->Element(i));
	if (last.Elements() > i && (name[0] == '\0' || !strcmp(name, "-") || !strcmp(name, last[i]->_name))) {
	  // ��������Ʊ���ǡ�������
	  pparent = last[i];
	} else {
	  long int lv;
	  if (i < ele - 4) {
	    lv = 0;
	  } else {
	    lv = level;
	  }
	  if (name[0] == '\0') {
	    fprintf(stderr, "at readcsv.c, name = NULL (wrong format csv?)\n while reading \"%s\"", buf);
	    abort();
	  }
	    
	  /*
	    if (pparent) {
	    d = new dam(name, pparent->_name, x, y, lv);
	  } else {
	    d = new dam(name, (char*)"", x, y, lv);
	  }
	  */
	  if (i > 0) {
	    d = new dam(name, args->Element(i - 1), x, y, lv);
	  } else {
	    d = new dam(name, (char*)"", x, y, lv);
	  }
	  d->_pparent = pparent;
	  if (pparent) added = pparent->AddChild(d);
	  else added = d;
	  if (last.Elements() > i) {
	    last[i] = added;
	    for (int j = last.Elements() - 1; j > i; j--) {
	      last.RemoveAt(j);
	    }
	  } else {
	    last.Add(added);
	  }
	  if (debug > 0) {
	    fprintf(stderr, "%08d: Add '%s'\n", damsArray.Elements(), d->_name);
	  }
	  pparent = added;
	  if (added == d) {
	    damsArray.Add(d);
	  } else {
	    delete d;
	    d = added;
	  }
	}
      }
      if (debug > 0) {
	fprintf(stderr, "READ %s", buf);
      }
    } else {
      if (debug > 0) {
	fprintf(stderr, "SKIP: %s", buf);
      }
    }
    args->ClearElements();
  }
}

/*
   ʸ���󤫤饻�ѥ졼���Ƕ��ڤ�줿°���ͤ���Ф�
   �ѥ�᡼�� stream    °���ͤ��ޤޤ�Ƥ���ʸ����
              separator ���ڤ�ʸ��
   �����     PtrVect<char*>* retval
              retval->Element(0) = "arg0";
	      retval->Element(1) = "arg1";
	      ...
   ��� ���Ѹ塢retval->ClearElements(); ��ƤӽФ�����
*/
PtrVect<char*>* get_args(char* stream, char separator) {
  static char buf[BUFSIZ];
  char* sp = stream, *dp, *pp;
  char* newarg;
  int len;
  args->ClearElements();
  while (*sp) {
    dp = buf;
    pp = sp;
    while(*sp && *sp != separator) {
      int kanjitype = _isKanji(sp);
      switch (kanjitype) {
      case 0:
	if (!isspace(*sp)) {
	  *dp = *sp;
	  sp++;
	  dp++;
	}
	break;
      case 1:
      case 2:
      case 4:
      case 5:
	*dp = *sp;
	dp++; sp++;
	*dp = *sp;
	dp++; sp++;
	break;
      default:
	sp++;
      }
    }
    len = dp - buf;
    newarg = new char[len + 1]; // +1 for EOS
    memcpy(newarg, buf, len);
    newarg[len] = '\0';
    args->Add(newarg);
    if (*sp == separator) sp++; // ���ѥ졼���򥹥��å�
  }
  return args;
}
