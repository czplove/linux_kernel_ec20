#ifndef __TEST_CASE_H__
#define __TEST_CASE_H__

typedef int (*TEST)(void);

typedef struct
{
    int  cmdIdx;
    char *funcName;
} st_api_test_case;

typedef struct
{
    char                *group_name;
    st_api_test_case    *test_cases;
    TEST                pf_test;
} func_api_test_t;

extern void show_group_help(func_api_test_t *pt_test);

#endif //__TEST_CASE_H__
