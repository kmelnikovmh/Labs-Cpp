#ifndef MYTEST_
#define MYTEST_

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <algorithm>

namespace mytest {
static const int DEEP_SUBCASE = 30;

// TESTCASE
class TestCase {
    static inline std::vector<std::pair<std::function<void()>, std::string>> &getStackTest();
    static inline int &getCheckFail();

public:
    static inline void saveTest(const std::function<void()> &f, const std::string &msg);
    static inline void changeCheckFail(int num);
    static inline bool CheckFailed();
    friend int run_test();
};

// SUBCASE
class SubCase {
    static inline std::vector<std::vector<std::string>> &getStackSubCase();
    std::string m_name_subcase;
    int m_deep_subcase;

public:
    static inline std::vector<int> &getCountSubCase();
    static inline void increaseCountSubCase(int deep);

    static inline bool tryInSubCase(int deep);
    static inline std::vector<int> &getEntered();
    static inline void changeEntered(int num, int deep);

    static inline std::vector<std::string> &getInCase();
    static inline void changeInCase(const std::string &name, int deep);

    static inline int &getDeep();
    static inline int &getEnteredDeep();
    static inline void increaseDeep();
    static inline void decreaseDeep();

    explicit inline SubCase(const std::string &name);
    inline ~SubCase();

    SubCase(const SubCase &) = delete;
    SubCase(SubCase &&) = delete;
    SubCase &operator=(const SubCase &) = delete;
    SubCase &operator=(SubCase &&) = delete;

    friend int run_test();
};

// CLASS DEFINITIONS
// cppcheck-suppress unusedFunction
std::vector<std::pair<std::function<void()>, std::string>>
    &TestCase::getStackTest() {
    static std::vector<std::pair<std::function<void()>, std::string>>
        stack_test;
    return stack_test;
}

int &TestCase::getCheckFail() {
    static int count_failed_check = 0;
    return count_failed_check;
}

void TestCase::changeCheckFail(int num) {
    TestCase::getCheckFail() = num;
}

bool TestCase::CheckFailed() {
    return TestCase::getCheckFail() != 0;
}

SubCase::SubCase(const std::string &name)
    : m_name_subcase(name), m_deep_subcase(getDeep()) {
    getEnteredDeep() = m_deep_subcase;
    if (getEntered()[getDeep()] == 0 && tryInSubCase(m_deep_subcase)) {
        changeInCase(name, getDeep());
        increaseDeep();
    }
};

SubCase::~SubCase() {
    if (getEntered()[m_deep_subcase] == 0 && tryInSubCase(m_deep_subcase)) {
        if (static_cast<size_t>(getCountSubCase()[m_deep_subcase + 1]) ==
            getStackSubCase()[m_deep_subcase + 1].size()) {
            getStackSubCase()[m_deep_subcase].push_back(m_name_subcase);
            getStackSubCase()[m_deep_subcase + 1].clear();
        }
        decreaseDeep();
        changeEntered(1, getDeep());
    }
    increaseCountSubCase(getDeep());
}

std::vector<std::vector<std::string>> &SubCase::getStackSubCase() {
    static std::vector<std::vector<std::string>> stack_subcase(
        DEEP_SUBCASE, std::vector<std::string>()
    );
    return stack_subcase;
}

bool SubCase::tryInSubCase(int deep) {
    return static_cast<unsigned int>(getCountSubCase()[deep]) ==
            getStackSubCase()[deep].size();
}

std::vector<int> &SubCase::getEntered() {
    static std::vector<int> entered(DEEP_SUBCASE, 0);
    return entered;
}

void SubCase::changeEntered(int num, int deep) {
    SubCase::getEntered()[deep] = num;
}

std::vector<std::string> &SubCase::getInCase() {
    static std::vector<std::string> in_case(DEEP_SUBCASE, "");
    return in_case;
}

void SubCase::changeInCase(const std::string &name, int deep) {
    SubCase::getInCase()[deep] = name;
}

std::vector<int> &SubCase::getCountSubCase() {
    static std::vector<int> count_subcase(DEEP_SUBCASE, 0);
    return count_subcase;
}

void SubCase::increaseCountSubCase(int deep) {
    ++SubCase::getCountSubCase()[deep];
}

int &SubCase::getDeep() {
    static int deep = 0;
    return deep;
}

void SubCase::increaseDeep() {
    ++(SubCase::getDeep());
}

void SubCase::decreaseDeep() {
    --(SubCase::getDeep());
}

int &SubCase::getEnteredDeep() {
    static int entered_deep = 0;
    return entered_deep;
}

// cppcheck-suppress unusedFunction
void TestCase::saveTest(
    const std::function<void()> &func,
    const std::string &msg
) {
    TestCase::getStackTest().emplace_back(func, msg);
}

// OTHER DEFINITIONS
inline void check (
    bool expr,
    const std::string &string_expr,
    const std::string &file,
    int line,
    std::optional<std::string> msg
) {
    if (expr) {
        return;
    }
    std::cerr << "  CHECK(" << string_expr << ") at " << file << ":" << line
              << " failed!\n";
    if (msg.has_value()) {
        std::cerr << "      message: " << msg.value() << "\n";
    }
    if (SubCase::getDeep() != 0 &&
        !SubCase::getInCase()[SubCase::getDeep() - 1].empty()) {
        for (int i = 0; i < 30; ++i) {
            if (SubCase::getInCase()[i].empty()) {
                break;
            }
            std::cerr << "      in subcase " << SubCase::getInCase()[i] << "\n";
        }
    }
    TestCase::changeCheckFail(1);
}

inline int run_test() {
    unsigned int count_succes_test = 0;
    const unsigned int count_tests = TestCase::getStackTest().size();
    std::vector<std::pair<std::function<void()>, std::string>> &stack =
        TestCase::getStackTest();
    std::sort(stack.begin(), stack.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.second < rhs.second;
    });
    for (const auto &pair : TestCase::getStackTest()) {
        TestCase::changeCheckFail(0);
        std::cerr << "\nRunning \"" << pair.second << "\"\n";
        SubCase::changeEntered(0, SubCase::getDeep());
        pair.first();
        while (static_cast<unsigned int>(
                   SubCase::getCountSubCase()[SubCase::getDeep()]
               ) != SubCase::getStackSubCase()[SubCase::getDeep()].size() &&
               !(TestCase::CheckFailed())) {
            SubCase::getCountSubCase() = std::vector<int>(DEEP_SUBCASE, 0);
            SubCase::getEntered() = std::vector<int>(DEEP_SUBCASE, 0);
            std::cerr << "...another subcase...\n";
            pair.first();
        }
        SubCase::getStackSubCase()[SubCase::getDeep()].clear();
        if (TestCase::getCheckFail() == 0) {
            ++count_succes_test;
        }
    }
    std::cerr << "\n===== Tests passed: " << count_succes_test << "/"
              << count_tests << " =====\n";
    return static_cast<int>(!(count_succes_test == count_tests));
}
}  // namespace mytest


// MACROS
// NOLINTBEGIN(cppcoreguidelines-macro-usage,-warnings-as-errors)
#define CHECK(expr)                                                         \
    ::mytest::check(                                                        \
        static_cast<bool>(expr), #expr, __FILE__, __LINE__, ::std::nullopt  \
    )
#define CHECK_MESSAGE(expr, msg)                                            \
    ::mytest::check(static_cast<bool>(expr), #expr, __FILE__, __LINE__, msg)

// ЭТО ПОЛЕ БРАНИ, ТУТ ПОГИБЛО НЕСКОЛЬКО ЧАСОВ ЧЕЛОВЕЧЕСКОЙ ЖИЗНИ ВПУСТУЮ
#define MYTEST_INTERNAL_NEW_NAMED(name_x, name_y) name_x##name_y
#define MYTEST_INTERNAL_NEW_NAME1(name_x, name_y)                           \
    MYTEST_INTERNAL_NEW_NAMED(name_x, name_y)

#define MYTEST_INTERNAL_NEW_VAR                                             \
    MYTEST_INTERNAL_NEW_NAME1(mytest_var_, __LINE__)

#define MYTEST_INTERNAL_NEW_FUNC                                            \
    MYTEST_INTERNAL_NEW_NAME1(mytest_func_, __LINE__)

#define MYTEST_INTERNAL_NEW_SUBCASE                                         \
    MYTEST_INTERNAL_NEW_NAME1(mytest_subcase_, __LINE__)

#define TEST_CASE_REGISTER(func, msg)                                       \
    static int MYTEST_INTERNAL_NEW_VAR =                                    \
        ((::mytest::TestCase::saveTest(func, msg)), 0)

#define TEST_CASE(msg)                                                      \
    static void MYTEST_INTERNAL_NEW_FUNC();                                 \
    TEST_CASE_REGISTER(MYTEST_INTERNAL_NEW_FUNC, msg);                      \
    void MYTEST_INTERNAL_NEW_FUNC()

#define SUBCASE(msg)                                                        \
    if (::mytest::SubCase MYTEST_INTERNAL_NEW_SUBCASE(msg);                 \
        ::mytest::SubCase::getEntered(                                      \
        )[::mytest::SubCase::getEnteredDeep()] == 0 &&                      \
        ::mytest::SubCase::tryInSubCase(::mytest::SubCase::getEnteredDeep()))
// NOLINTEND(cppcoreguidelines-macro-usage,-warnings-as-errors)


#ifdef MYTEST_MAIN_
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif

    return mytest::run_test();
}
#endif // MYTEST_MAIN_

#endif // MYTEST_
