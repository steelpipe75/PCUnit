#ifndef PCUNIT_H_INCLUDED
#define PCUNIT_H_INCLUDED

#include "PCU_Libc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCU_VERSION "1.5.0"

/* 
 * private
 */

#if !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
#if (defined(_MSC_VER) && _MSC_VER < 1400) /* VC2005 */
typedef unsigned _int64 PCU_size_t;
typedef _int64 PCU_ssize_t;
#else
typedef unsigned long long PCU_size_t;
typedef long long PCU_ssize_t;
#endif
#else
typedef size_t PCU_size_t;
typedef ptrdiff_t PCU_ssize_t;
#endif

#define PCU_TYPE_NONE      0x00000000
#define PCU_TYPE_BOOL      0x00000001
#define PCU_TYPE_NUM       0x00000002
#define PCU_TYPE_PTR       0x00000003
#define PCU_TYPE_PTR_NULL  0x00000004
#define PCU_TYPE_STR       0x00000005
#define PCU_TYPE_STRW      0x00000006
#define PCU_TYPE_DBL       0x00000007
#define PCU_TYPE_OP        0x00000008
#define PCU_TYPE_OP_INT    0x00000009
#define PCU_TYPE_OP_DBL    0x0000000a
#define PCU_TYPE_MSG       0x0000000b
#define PCU_TYPE_ADDMSG    0x0000000c
#define PCU_TYPE_FAIL      0x0000000d
#define PCU_TYPE_SETUP     0x0000000e
#define PCU_TYPE_NSTR      0x20000000
#define PCU_TYPE_NSTRW     0x40000000
#define PCU_TYPE_NOT       0x80000000

unsigned long PCU_get_assert_type(unsigned long type);
size_t PCU_get_nstr_len(unsigned long type);
int PCU_get_not_flag(unsigned long type);
void PCU_assert_impl(int passed_flag, PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
void PCU_assert_num_impl(PCU_size_t expected, PCU_size_t actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
void PCU_assert_ptr_impl(const void *expected, const void *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
void PCU_assert_str_impl(const char *expected, const char *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
void PCU_assert_strw_impl(const wchar_t *expected, const wchar_t *actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
#endif
#ifndef PCU_NO_FLOATINGPOINT
void PCU_assert_double_impl(double expected, double actual, double delta, unsigned long type, const char *str_assert, const char *file, unsigned int line);
void PCU_assert_op_double_impl(int passed_flag, double expected, double actual, unsigned long type, const char *str_assert, const char *file, unsigned int line);
#endif
void PCU_msg_impl(const char *msg, unsigned long type, const char *str_assert, const char *file, unsigned int line);

void PCU_leave_test_func(void);
int PCU_last_assertion(void);


typedef struct {
	int num_asserts;
	int num_asserts_ran;
	int num_asserts_failed;
	int num_errors_setup;
	int num_errors_teardown;
	int num_msgs;
} PCU_TestResult;

typedef struct {
	int num_tests;
	int num_tests_ran;
	int num_tests_failed;
	int num_errors_initialize;
	int num_errors_cleanup;
	PCU_TestResult test_result;
} PCU_SuiteResult;

typedef struct {
	int num_suites;
	int num_suites_ran;
	int num_suites_failed;
	PCU_SuiteResult suite_result;
} PCU_Result;

typedef struct PCU_TestFailure {
	union {
		PCU_size_t num;
		char *str;
		const void *ptr;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		double dbl;
#endif
	} expected;
	union {
		PCU_size_t num;
		char *str;
		const void *ptr;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
		double dbl;
#endif
	} actual;
#if !defined(PCU_NO_FLOATINGPOINT) && !defined(PCU_NO_VSNPRINTF) && !defined(PCU_NO_LIBC)
	double delta;
#endif
	unsigned long type;
	const char *str_assert;
	const char *file;
	unsigned int line;
	int repeat;
	struct PCU_TestFailure *next;
	struct PCU_TestFailure *prev;
} PCU_TestFailure;

/* 
 * public
 */
typedef struct PCU_Test PCU_Test;
typedef struct PCU_Suite PCU_Suite;

struct PCU_Test {
	/* public */
	const char *name;
	void (*test)(void);
	int ntimes;
	/* private */
	const PCU_Suite *suite;
	PCU_TestResult result;
	PCU_TestFailure failure_list;
};

struct PCU_Suite {
	/* public */
	const char *name;
	PCU_Test *tests;
	int ntests;
	int (*setup)(void);
	int (*teardown)(void);
	int (*initialize)(void);
	int (*cleanup)(void);
	/* private */
	int initialize_error;
	int cleanup_error;
	PCU_SuiteResult result;
};

typedef PCU_Suite *(*PCU_SuiteMethod)(void);

typedef int (*PCU_Putchar)(int c);
typedef int (*PCU_Getchar)(void);

/* 
 * PCUnit API
 */
void PCU_run(const PCU_SuiteMethod *suite_methods, int num);
void PCU_set_putchar(PCU_Putchar func);
void PCU_set_getchar(PCU_Getchar func);
int PCU_repeat_counter(void);
const char *PCU_test_name(void);
const char *PCU_suite_name(void);
void PCU_enable_color(void);
void PCU_disable_color(void);
#ifndef PCU_NO_CONSOLE_RUN
void PCU_console_run(const PCU_SuiteMethod *suite_methods, int num);
#endif
#ifndef PCU_NO_STDARG
const char *PCU_format(const char *format, ...);
#else
#define PCU_format0(f)                                     PCU_format_aux0(f)
#define PCU_format1(f, a1)                                 PCU_format_aux1(f, (size_t)(a1))
#define PCU_format2(f, a1, a2)                             PCU_format_aux2(f, (size_t)(a1), (size_t)(a2))
#define PCU_format3(f, a1, a2, a3)                         PCU_format_aux3(f, (size_t)(a1), (size_t)(a2), (size_t)(a3))
#define PCU_format4(f, a1, a2, a3, a4)                     PCU_format_aux4(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4))
#define PCU_format5(f, a1, a2, a3, a4, a5)                 PCU_format_aux5(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5))
#define PCU_format6(f, a1, a2, a3, a4, a5, a6)             PCU_format_aux6(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6))
#define PCU_format7(f, a1, a2, a3, a4, a5, a6, a7)         PCU_format_aux7(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7))
#define PCU_format8(f, a1, a2, a3, a4, a5, a6, a7, a8)     PCU_format_aux8(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8))
#define PCU_format9(f, a1, a2, a3, a4, a5, a6, a7, a8, a9) PCU_format_aux9(f, (size_t)(a1), (size_t)(a2), (size_t)(a3), (size_t)(a4), (size_t)(a5), (size_t)(a6), (size_t)(a7), (size_t)(a8), (size_t)(a9))
#endif
#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
const char *PCU_formatW(const wchar_t *format, ...);
#endif


/* 
 * Assert Macros
 */

#define PCU_ASSERT_AUX(expr, str_assert)\
	PCU_assert_impl(((expr) != 0),\
		0, 0,\
		PCU_TYPE_NONE,\
		str_assert,\
		__FILE__, __LINE__)\

#define PCU_ASSERT_TRUE_AUX(expr, str_assert)\
	PCU_assert_impl(((expr) != 0),\
		0, 0,\
		PCU_TYPE_BOOL,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_FALSE_AUX(expr, str_assert)\
	PCU_assert_impl(((expr) == 0),\
		0, 0,\
		PCU_TYPE_BOOL,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_num_impl((PCU_size_t) (expected), (PCU_size_t) (actual),\
		PCU_TYPE_NUM,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_NOT_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_num_impl((PCU_size_t) (expected), (PCU_size_t) (actual),\
		PCU_TYPE_NUM | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_PTR_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_ptr_impl((const void *) (expected), (const void *) (actual),\
		PCU_TYPE_PTR,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_ptr_impl((const void *) (expected), (const void *) (actual),\
		PCU_TYPE_PTR | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_PTR_NULL_AUX(value, str_assert)\
	PCU_assert_ptr_impl(0, (const void *) (value),\
		PCU_TYPE_PTR_NULL,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_PTR_NOT_NULL_AUX(value, str_assert)\
	PCU_assert_ptr_impl(0, (const void *) (value),\
		PCU_TYPE_PTR_NULL | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_STRING_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_STR,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_STR | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n, str_assert)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_NSTR | (n),\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n, str_assert)\
	PCU_assert_str_impl((const char *) (expected), (const char *) (actual),\
		PCU_TYPE_NSTR | PCU_TYPE_NOT | (n),\
		str_assert,\
		__FILE__, __LINE__)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_STRW,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual, str_assert)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_STRW | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n, str_assert)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_NSTRW | (n),\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n, str_assert)\
	PCU_assert_strw_impl((const wchar_t *) (expected), (const wchar_t *) (actual),\
		PCU_TYPE_NSTRW | PCU_TYPE_NOT | (n),\
		str_assert,\
		__FILE__, __LINE__)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta, str_assert)\
	PCU_assert_double_impl((expected), (actual), (delta),\
		PCU_TYPE_DBL,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta, str_assert)\
	PCU_assert_double_impl((expected), (actual), (delta),\
		PCU_TYPE_DBL | PCU_TYPE_NOT,\
		str_assert,\
		__FILE__, __LINE__)

#define PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs, str_assert)\
	do {\
		const double pcu_assert_operator_aux_lhs = (double) (lhs);\
		const double pcu_assert_operator_aux_rhs = (double) (rhs);\
		PCU_assert_op_double_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			pcu_assert_operator_aux_lhs, pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP_DBL,\
			str_assert,\
			__FILE__, __LINE__);\
	} while (0)
#endif

#define PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs, str_assert)\
	do {\
		const PCU_size_t pcu_assert_operator_aux_lhs = (PCU_size_t) (lhs);\
		const PCU_size_t pcu_assert_operator_aux_rhs = (PCU_size_t) (rhs);\
		PCU_assert_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			pcu_assert_operator_aux_lhs, pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP,\
			str_assert,\
			__FILE__, __LINE__);\
	} while (0)

#define PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs, str_assert)\
	do {\
		const PCU_ssize_t pcu_assert_operator_aux_lhs = (PCU_ssize_t) (lhs);\
		const PCU_ssize_t pcu_assert_operator_aux_rhs = (PCU_ssize_t) (rhs);\
		PCU_assert_impl((pcu_assert_operator_aux_lhs op pcu_assert_operator_aux_rhs),\
			(PCU_size_t) pcu_assert_operator_aux_lhs, (PCU_size_t) pcu_assert_operator_aux_rhs,\
			PCU_TYPE_OP_INT,\
			str_assert,\
			__FILE__, __LINE__);\
	} while (0)


#if defined(PCU_NO_SETJMP) || defined(PCU_NO_LIBC)
#define PCU_LEAVE_TEST_FUNC()	return
#else
#define PCU_LEAVE_TEST_FUNC()	PCU_leave_test_func()
#endif

#define PCU_LEAVE_TEST_FUNC_IF_FAILED()	\
	do {\
		if (!PCU_last_assertion()) {\
			PCU_LEAVE_TEST_FUNC();\
		}\
	} while (0)

#define PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg)	\
	do {\
		if (!PCU_last_assertion()) {\
			PCU_msg_impl(msg, PCU_TYPE_ADDMSG , "", "", 0);\
			PCU_LEAVE_TEST_FUNC();\
		}\
	} while (0)


#define PCU_ASSERT(expr)\
	do {\
		PCU_ASSERT_AUX(expr,\
			"PCU_ASSERT(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_TRUE(expr)\
	do {\
		PCU_ASSERT_TRUE_AUX(expr,\
			"PCU_ASSERT_TRUE(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_FALSE(expr)\
	do {\
		PCU_ASSERT_FALSE_AUX(expr,\
			"PCU_ASSERT_FALSE(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_NOT_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_PTR_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_PTR_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_PTR_NOT_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_PTR_NULL(value)\
	do {\
		PCU_ASSERT_PTR_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NULL(" #value ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_PTR_NOT_NULL(value)\
	do {\
		PCU_ASSERT_PTR_NOT_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NOT_NULL(" #value ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_STRING_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_STRING_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_STRING_NOT_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_NSTRING_EQUAL(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_NSTRING_NOT_EQUAL(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_STRINGW_NOT_EQUAL(expected, actual)\
	do {\
		PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_NSTRINGW_EQUAL(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL(expected, actual, n)\
	do {\
		PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_NOT_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL(expected, actual, delta)\
	do {\
		PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_EQUAL(" #expected ", " #actual ", " #delta ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL(expected, actual, delta)\
	do {\
		PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_NOT_EQUAL(" #expected ", " #actual ", " #delta ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_OPERATOR_DOUBLE(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_DOUBLE((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)
#endif

#define PCU_ASSERT_OPERATOR(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_ASSERT_OPERATOR_INT(lhs, op, rhs)\
	do {\
		PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_INT((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)


#define PCU_ASSERT_MESSAGE(expr, msg)\
	do {\
		PCU_ASSERT_AUX(expr,\
			"PCU_ASSERT(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_TRUE_MESSAGE(expr, msg)\
	do {\
		PCU_ASSERT_TRUE_AUX(expr,\
			"PCU_ASSERT_TRUE(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_FALSE_MESSAGE(expr, msg)\
	do {\
		PCU_ASSERT_FALSE_AUX(expr,\
			"PCU_ASSERT_FALSE(" #expr ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_PTR_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_PTR_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_PTR_NOT_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_PTR_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_PTR_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_PTR_NULL_MESSAGE(value, msg)\
	do {\
		PCU_ASSERT_PTR_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NULL(" #value ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_PTR_NOT_NULL_MESSAGE(value, msg)\
	do {\
		PCU_ASSERT_PTR_NOT_NULL_AUX(value,\
			"PCU_ASSERT_PTR_NOT_NULL(" #value ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_STRING_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_STRING_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_STRING_NOT_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_STRING_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRING_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_NSTRING_EQUAL_MESSAGE(expected, actual, n, msg)\
	do {\
		PCU_ASSERT_NSTRING_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_NSTRING_NOT_EQUAL_MESSAGE(expected, actual, n, msg)\
	do {\
		PCU_ASSERT_NSTRING_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRING_NOT_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#if !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGW_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_STRINGW_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_STRINGW_NOT_EQUAL_MESSAGE(expected, actual, msg)\
	do {\
		PCU_ASSERT_STRINGW_NOT_EQUAL_AUX(expected, actual,\
			"PCU_ASSERT_STRINGW_NOT_EQUAL(" #expected ", " #actual ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_NSTRINGW_EQUAL_MESSAGE(expected, actual, n, msg)\
	do {\
		PCU_ASSERT_NSTRINGW_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_NSTRINGW_NOT_EQUAL_MESSAGE(expected, actual, n, msg)\
	do {\
		PCU_ASSERT_NSTRINGW_NOT_EQUAL_AUX(expected, actual, n,\
			"PCU_ASSERT_NSTRINGW_NOT_EQUAL(" #expected ", " #actual ", " #n ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)
#endif

#ifndef PCU_NO_FLOATINGPOINT
#define PCU_ASSERT_DOUBLE_EQUAL_MESSAGE(expected, actual, delta, msg)\
	do {\
		PCU_ASSERT_DOUBLE_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_EQUAL(" #expected ", " #actual ", " #delta ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_DOUBLE_NOT_EQUAL_MESSAGE(expected, actual, delta, msg)\
	do {\
		PCU_ASSERT_DOUBLE_NOT_EQUAL_AUX(expected, actual, delta,\
			"PCU_ASSERT_DOUBLE_NOT_EQUAL(" #expected ", " #actual ", " #delta ")");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_OPERATOR_DOUBLE_MESSAGE(lhs, op, rhs, msg)\
	do {\
		PCU_ASSERT_OPERATOR_DOUBLE_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_DOUBLE((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)
#endif

#define PCU_ASSERT_OPERATOR_MESSAGE(lhs, op, rhs, msg)\
	do {\
		PCU_ASSERT_OPERATOR_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)

#define PCU_ASSERT_OPERATOR_INT_MESSAGE(lhs, op, rhs, msg)\
	do {\
		PCU_ASSERT_OPERATOR_INT_AUX(lhs, op, rhs,\
			"PCU_ASSERT_OPERATOR_INT((" #lhs ") " #op " (" #rhs "))");\
		PCU_LEAVE_TEST_FUNC_IF_FAILED_MSG(msg);\
	} while (0)


#define PCU_FAIL(msg)\
	do {\
		PCU_assert_impl(0, (PCU_size_t)(size_t)(msg), 0, PCU_TYPE_FAIL, "PCU_FAIL", __FILE__, __LINE__);\
		PCU_LEAVE_TEST_FUNC_IF_FAILED();\
	} while (0)

#define PCU_MESSAGE(msg)\
	do {\
		PCU_msg_impl(msg, PCU_TYPE_MSG, "PCU_MESSAGE", __FILE__, __LINE__);\
	} while (0)


#if (defined(_UNICODE) || defined(UNICODE)) && !defined(PCU_NO_WCHAR) && !defined(PCU_NO_LIBC)
#define PCU_ASSERT_STRINGT_EQUAL      PCU_ASSERT_STRINGW_EQUAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL  PCU_ASSERT_STRINGW_NOT_EQUAL
#define PCU_ASSERT_NSTRINGT_EQUAL     PCU_ASSERT_NSTRINGW_EQUAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL PCU_ASSERT_NSTRINGW_NOT_EQUAL
#define PCU_formatT                   PCU_formatW
#else
#define PCU_ASSERT_STRINGT_EQUAL      PCU_ASSERT_STRING_EQUAL
#define PCU_ASSERT_STRINGT_NOT_EQUAL  PCU_ASSERT_STRING_NOT_EQUAL
#define PCU_ASSERT_NSTRINGT_EQUAL     PCU_ASSERT_NSTRING_EQUAL
#define PCU_ASSERT_NSTRINGT_NOT_EQUAL PCU_ASSERT_NSTRING_NOT_EQUAL
#define PCU_formatT                   PCU_format
#endif


/* 
 * Helper Macros
 */
#define PCU_TEST(func)                  { #func, func }
#define PCU_TEST_REPEATED(func, ntimes) { #func, func, ntimes }
#define PCU_TEST_SKIPPED(func)          { #func, func, -1 }

#ifdef __cplusplus
}
#endif

#endif /* PCUNIT_H_INCLUDED */
