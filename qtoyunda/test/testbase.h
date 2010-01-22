#ifndef H_TESTBASOUNET
#define H_TESTBASOUNET
#include <string>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define is_equal(x, y) is_r_equal(AT, x, y)


namespace testclass
{
    std::string m_testname;
    int	m_numtest;
    int  m_testFailed;
    void inittest(const std::string s);
    template<typename EarlGrey>void	is_r_equal(const char *at, EarlGrey totest, EarlGrey expect);
    int conclude();
};

#include "testbase.hxx"
#endif
