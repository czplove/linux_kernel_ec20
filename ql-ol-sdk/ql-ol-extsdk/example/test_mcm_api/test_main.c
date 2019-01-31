#include "ql_oe.h"
#include "test_base.h"


#define FUNC_GROUP_MAX 10

typedef struct
{
    char                *group_name;
    st_api_test_case    *test_cases;
    TEST                pf_test;
} func_api_test_group_t;


func_api_test_group_t g_test_list[FUNC_GROUP_MAX] = {0};

void add_test_node(func_api_test_t *pt_test)
{
    int i;
    for(i=0; i<FUNC_GROUP_MAX; i++)
    {
        if(g_test_list[i].group_name == NULL)
        {
            g_test_list[i].group_name   = pt_test->group_name;
            g_test_list[i].test_cases   = pt_test->test_cases;
            g_test_list[i].pf_test      = pt_test->pf_test;
            break;
        }
    }
}


void show_group_help(func_api_test_t *pt_test)
{
    int i;

    printf("Group Name:%s, Supported test cases:\n", pt_test->group_name);
    for(i = 0; ; i++)
    {
        if(pt_test->test_cases[i].cmdIdx == -1)
        {
            break;
        }
        printf("%d:\t%s\n", pt_test->test_cases[i].cmdIdx, pt_test->test_cases[i].funcName);
    }
}



void show_group_list()
{
    int i;

    printf("Test groups:\n");
    for(i = 0; i<FUNC_GROUP_MAX; i++)
    {
        if(g_test_list[i].group_name != NULL)
        {
            printf("%d:\t%s\n", i, g_test_list[i].group_name);
        }
    }
}

extern func_api_test_t t_atc_test;
extern func_api_test_t t_data_test;
extern func_api_test_t t_dm_test;
extern func_api_test_t t_gps_test;
extern func_api_test_t t_mobap_test;
extern func_api_test_t t_nw_test;
extern func_api_test_t t_sim_test;
extern func_api_test_t t_sms_test;
extern func_api_test_t t_voice_test;
int main(int argc, char *argv[])
{
    int cmdIdx = 0, caseIdx = 0;

    add_test_node(&t_atc_test);
    add_test_node(&t_data_test);
    add_test_node(&t_dm_test);
    add_test_node(&t_gps_test);
    add_test_node(&t_mobap_test);
    add_test_node(&t_nw_test);
    add_test_node(&t_sim_test);
    add_test_node(&t_sms_test);
    add_test_node(&t_voice_test);
    
    show_group_list();
    while(1)
    {
        printf("please input command index(-1 exit): ");
        scanf("%d", &cmdIdx);
        if(cmdIdx == -1)
        {
            break;
        }
        if((cmdIdx >= FUNC_GROUP_MAX) || (g_test_list[cmdIdx].group_name == NULL))
        {
            show_group_list();
        }
        else
        {
            g_test_list[cmdIdx].pf_test();
            show_group_list();
        }
    }
    return 0;
}


