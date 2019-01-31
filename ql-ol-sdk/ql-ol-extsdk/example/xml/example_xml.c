#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <libxml/xmlmemory.h>  
#include <libxml/parser.h>  
     
/* 解析quectel节点，打印title节点的内容 */  
void parseChild(xmlDocPtr doc, xmlNodePtr cur, char * parse_str)
{  
    xmlChar* key;  
    cur=cur->xmlChildrenNode;  
    while(cur != NULL)
    {  
        /* 找到title子节点 */  
        if(!xmlStrcmp(cur->name, (const xmlChar *)parse_str)){  
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);  
            printf("%s: %s\n", parse_str,key);  
            xmlFree(key);  
        }  
        cur=cur->next; /* 下一个子节点 */  
    }  
  
    return;  
}  
      
/* 解析文档 */  
static void parseDoc(char *docname, char *parse_str)
{  
    /* 定义文档和节点指针 */  
    xmlDocPtr doc;  
    xmlNodePtr cur;  
          
    /* 进行解析，如果没成功，显示一个错误并停止 */  
    doc = xmlParseFile(docname);  
    if(doc == NULL){  
        fprintf(stderr, "Document not parse successfully. \n");  
        return;  
    }  
      
    /* 获取文档根节点，若无内容则释放文档树并返回 */  
    cur = xmlDocGetRootElement(doc);  
    if(cur == NULL){  
        fprintf(stderr, "empty document\n");  
        xmlFreeDoc(doc);  
        return;  
    }  
      
    /* 确定根节点名是否为company，不是则返回 */  
    if(xmlStrcmp(cur->name, (const xmlChar *)"company")){  
        fprintf(stderr, "document of the wrong type, root node != quectel");  
        xmlFreeDoc(doc);  
        return;  
    }  
      
    /* 遍历文档树 */  
    cur = cur->xmlChildrenNode;  
    while(cur != NULL)
    {  
        /* 找到quectel子节点 */  
        if(!xmlStrcmp(cur->name, (const xmlChar *)"quectel")){
            parseChild(doc, cur, parse_str); /* 解析quectel子节点 */  
        }  
        cur = cur->next; /* 下一个子节点 */  
    }  
     
    xmlFreeDoc(doc); /* 释放文档树 */  
    return;  
} 
      
int main(int argc, char **argv)
{  
    char *docname;  
    char *parse_str;  
    if(argc <= 1){  
        printf("Usage: %s docname parse_str\n", argv[0]);  
        exit(1);  
    }  
    docname=argv[1];  
    parse_str =argv[2];  
    parseDoc(docname, parse_str);  
    
    return 0;  
}  
