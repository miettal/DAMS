///
/// @file
/// @brief dams���åפ���static�ؿ���
///

#ifndef _DAMS_H
#define _DAMS_H

#include <string>
#include <vector>
#include <stdexcept>

extern "C" {
  int dams_is_present(void);
}


namespace damswrapper { 
	
  ///
  /// @file
  /// @brief dams�¹Է�̤ν�������
  ///
  class AddressElement {
  private:
		
    std::string name;
		
    int level;
		
    float x;
		
    float y;
		
  public:
		
  AddressElement() : level(0), x(0.0), y(0.0) {}
		
    inline std::string get_name() const {
      return name;
    }
		
    inline void set_name(std::string v) {
      name = v;
    }
		
    inline int get_level() const {
      return level;
    }
		
    inline void set_level(int v) {
      level = v;
    }
		
    inline float get_x() const {
      return x;
    }
		
    inline void set_x(float v) {
      x = v;
    }
		
    inline float get_y() const {
      return y;
    }
		
    inline void set_y(float v) {
      y = v;
    }
  };

  ///
  /// @file
  /// @brief dams���㳰���饹��
  ///

  class DamsInitException : public std::runtime_error {
  public:
  DamsInitException(const char* msg) : runtime_error(msg) {}
  };
	
  class DamsException : public std::runtime_error {
  public:
  DamsException(const char* msg) : runtime_error(msg) {}
  };

  typedef std::vector<AddressElement> Candidate;
	
  /// @brief ��������ޤ���
  /// @param [in] damsFileName ����ե�����̾
  /// @exception �ե����뤬�����ʤ��ʤɡ�������˼��Ԥ������
  void init(const std::string& damsFileName) throw (DamsInitException);

  /// @brief �ǥե���ȤΥ����������ե����뤬¸�ߤ���г����ƽ�������ޤ���
  void init(void);

  /// @brief �����������ե�����򳫤����˽�������ޤ���
  void _init(void);

  /// @brief ��λ���ޤ���
  void final();

  /// @brief �ǥե���Ȥμ���ǥ��쥯�ȥ���֤��ޤ���
  const char* default_dic_path(void);

  /// @brief ʸ�������ɤ��Ѵ����ޤ���
  std::string utf8_to_euc(const std::string& utf_string);
  std::string euc_to_utf8(const std::string& euc_string);
	
  /// @brief ���Ϥ��Ʒ�̤��֤��ޤ���
  /// @param [out] score
  /// @param [out] tail
  /// @param [out] candidates
  /// @param [in] ������ʸ����UTF-8��ʸ����Ǥ���ɬ�פ����롣
  void retrieve(
		int& score, 
		std::string& tail, 
		std::vector<Candidate>& candidates, 
		const std::string& query) throw (DamsException);
	
  /// @brief �ǥХå��⡼�ɤ����ꤷ�ޤ����������θƤӽФ��Τ�ͭ���Ǥ����ǥե������false
  /// @param [in] flag
  void debugmode(bool flag);
	
  /// @brief ����̾������å����뤫�ɤ��������ꤷ�ޤ����������θƤӽФ��Τ�ͭ���Ǥ����ǥե������false
  /// @param [in] flag
  void set_check_new_address(bool flag);
	
  /// @brief �����������ξ�¤����ꤷ�ޤ����������θƤӽФ��Τ�ͭ���Ǥ����ǥե������10
  /// @param [in] limit
  void set_limit(int limit);
	
  /// @brief �������פ�ɬ�פȤ����٥�����ꤷ�ޤ����������θƤӽФ��Τ�ͭ���Ǥ����ǥե������5
  /// @param [in] level
  void set_exact_match_level(int level);
	
  /// @brief �������פ�ɬ�פȤ����٥��������ޤ���
  /// @return
  int get_exact_match_level();
	
  /// @brief �Ǹ�� retrieve() ���פ������֡ʥߥ��áˤ�������ޤ���
  /// @return
  long elapsedtime();

  /// @brief ���λ���ɸ�ಽ����ʸ�����������ޤ���
  /// @param [in] str ���λ���ޤ�ʸ����
  /// @return ɸ�ಽʸ����
  std::string get_standardized_string(const std::string& str);
	
}

#endif /* _DAMS_H */
