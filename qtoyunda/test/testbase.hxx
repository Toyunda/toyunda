#include <QTextStream>
#include <iostream>
#include "testbase.h"

namespace testclass
{
    QTextStream	cerr(stderr);
    void inittest(const std::string s)
    {
      m_testname = s;
      m_numtest = 0;
      m_testFailed = 0;
      std::cout << "Testing : " << s << std::endl;
    }
    
    template<typename EarlGrey>void	is_r_equal(const char *at, EarlGrey totest, EarlGrey expect)
    {
     m_numtest++;
     if (!(totest == expect)) {
        cerr << "Test n°" << m_numtest << " failed at : " << at << "\n";
        cerr << "\tComparaison failed\n";
        cerr << "\tGot : " << totest << "\n";
        cerr << "\tExpected : " << expect << "\n";
        m_testFailed++;
      }
    }
    int conclude()
    {
      if (m_testFailed) {
        std::cout << "Failed " << m_testFailed << "/" << m_numtest << std::endl;
        return 1;
      }
      std::cout << "All " << m_numtest << " passed succesfuly" << std::endl;
      return 0;
    }
};


