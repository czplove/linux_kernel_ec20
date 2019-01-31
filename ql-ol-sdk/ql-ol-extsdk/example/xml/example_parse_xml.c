#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <libxml/xmlmemory.h>  
#include <libxml/parser.h>  

 
/* 解析文档 */  
static void parse_file(char *xml_file, char *parse_node)
{  
    /* 定义文档和节点指针 */  
    xmlDocPtr file;  
    xmlNodePtr parent_node;  
    xmlNodePtr cur_node;  
    xmlChar* key;  
          
    /* 进行解析，如果没成功，显示一个错误并停止 */  
    file = xmlParseFile(xml_file);  
    if(file == NULL){  
        fprintf(stderr, "xml file not parse successfully. \n");  
        return;  
    }  
      
    /* 获取文档根节点，若无内容则释放文档树并返回 */  
    parent_node = xmlDocGetRootElement(file);  
    if(parent_node == NULL){  
        fprintf(stderr, "empty document\n");  
        goto end;
    }  
      
    /* 遍历文档树 */  
    parent_node = parent_node->xmlChildrenNode;  
    while(parent_node != NULL)
    {  
        cur_node = parent_node->xmlChildrenNode; 
        while(cur_node != NULL)
        {
            /* 找到目标子节点 */  
            if(!xmlStrcmp(cur_node->name, (const xmlChar *)parse_node))
            {
                key = xmlNodeListGetString(file, cur_node->xmlChildrenNode, 1);  
                printf("%s: %s\n",parse_node, key);  
                xmlFree(key);
                goto end;
            }  
            cur_node = cur_node->next; /* 下一个子节点 */
        }
        parent_node = parent_node->next; /* 下一个父节点 */
    } 
    printf("this child node is not exist\n");  
    
end: 
    xmlFreeDoc(file); /* 释放文档树 */
    return;
} 
      
int main(int argc, char **argv)
{ 
    if(argc != 3){  
        printf("Parse node from xml file\n", argv[0]);  
        printf("Usage: %s xml_file parse_node\n", argv[0]);  
        exit(1); 
    }  
    parse_file(argv[1], argv[2]);

    return 0;  
}  
