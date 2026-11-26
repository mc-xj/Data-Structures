#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 20
#define OK 1
#define ERROR 0

typedef struct {
    int data[MAXSIZE];
    int length;
} SqList;

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

int SeqInit(SqList *L)
{
    L->length = 0;
    return OK;
}

int SeqCreate(SqList *L, int n)
{
    int i;
    if (n < 0 || n > MAXSIZE) return ERROR;
    for (i = 0; i < n; i++) L->data[i] = (i + 1) * 10;
    L->length = n;
    return OK;
}

/* 查找返回位序，0 表示未找到。 */
int SeqLocate(SqList L, int e)
{
    int i;
    for (i = 0; i < L.length; i++)
        if (L.data[i] == e) return i + 1;
    return 0;
}

int SeqInsert(SqList *L, int i, int e)
{
    int j;
    if (L->length == MAXSIZE || i < 1 || i > L->length + 1)
        return ERROR;
    /* 从后向前移动，避免覆盖原来的元素。 */
    for (j = L->length - 1; j >= i - 1; j--)
        L->data[j + 1] = L->data[j];
    L->data[i - 1] = e;
    L->length++;
    return OK;
}

int SeqDelete(SqList *L, int i, int *e)
{
    int j;
    if (i < 1 || i > L->length) return ERROR;
    *e = L->data[i - 1];
    for (j = i; j < L->length; j++) L->data[j - 1] = L->data[j];
    L->length--;
    return OK;
}

void SeqPrint(SqList L)
{
    int i;
    printf("顺序表(长度=%d)：", L.length);
    for (i = 0; i < L.length; i++) printf("%d ", L.data[i]);
    printf("\n");
}

void LinkDestroy(LinkList *L)
{
    LNode *p;
    while (*L != NULL) {
        p = *L;
        *L = p->next;
        free(p);
    }
}

/* 尾插法建表，失败时释放已经申请的结点。 */
int LinkCreate(LinkList *L, int n)
{
    int i;
    LNode *tail, *p;
    if (n < 0) return ERROR;
    *L = malloc(sizeof(LNode));
    if (*L == NULL) {
        printf("内存分配失败。\n");
        return ERROR;
    }
    (*L)->next = NULL;
    tail = *L;
    for (i = 1; i <= n; i++) {
        p = malloc(sizeof(LNode));
        if (p == NULL) {
            printf("内存分配失败。\n");
            LinkDestroy(L);
            return ERROR;
        }
        p->data = i * 10;
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return OK;
}

LNode *LinkLocate(LinkList L, int e)
{
    LNode *p = L->next;
    while (p != NULL && p->data != e) p = p->next;
    return p;
}

int LinkInsert(LinkList L, int i, int e)
{
    int j;
    LNode *p = L, *s;
    if (i < 1) return ERROR;
    for (j = 1; p != NULL && j < i; j++) p = p->next;
    if (p == NULL) return ERROR;
    s = malloc(sizeof(LNode));
    if (s == NULL) {
        printf("内存分配失败。\n");
        return ERROR;
    }
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

int LinkDelete(LinkList L, int i, int *e)
{
    int j;
    LNode *p = L, *q;
    if (i < 1) return ERROR;
    for (j = 1; p != NULL && j < i; j++) p = p->next;
    if (p == NULL || p->next == NULL) return ERROR;
    q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    return OK;
}

void LinkPrint(LinkList L)
{
    int count = 0;
    LNode *p = L->next;
    printf("链表：");
    while (p != NULL) {
        printf("%d ", p->data);
        count++;
        p = p->next;
    }
    printf("(共 %d 个结点)\n", count);
}

void SeqDemo(void)
{
    SqList L;
    int e, pos;
    if (!SeqInit(&L) || !SeqCreate(&L, 8)) {
        printf("建表失败。\n");
        return;
    }
    SeqPrint(L);
    pos = SeqLocate(L, 50);
    if (pos) printf("元素 50 的位序 = %d\n", pos);
    else printf("未找到 50\n");
    pos = SeqLocate(L, 55);
    if (pos) printf("元素 55 的位序 = %d\n", pos);
    else printf("未找到 55，位序 = 0\n");
    printf("\n在第 4 个位置插入 45：\n");
    if (SeqInsert(&L, 4, 45)) SeqPrint(L);
    else printf("插入失败。\n");
    if (SeqDelete(&L, 6, &e)) {
        printf("被删除的元素 = %d\n", e);
        SeqPrint(L);
    } else printf("删除失败。\n");
}

void LinkDemo(void)
{
    LinkList L = NULL;
    LNode *p;
    int e;
    if (!LinkCreate(&L, 8)) return;
    LinkPrint(L);
    p = LinkLocate(L, 50);
    if (p != NULL) printf("找到结点，其值为 %d\n", p->data);
    else printf("未找到 50\n");
    p = LinkLocate(L, 55);
    if (p != NULL) printf("找到结点，其值为 %d\n", p->data);
    else printf("未找到 55\n");
    printf("\n在第 4 个位置插入 45：\n");
    if (LinkInsert(L, 4, 45)) LinkPrint(L);
    else printf("插入失败。\n");
    if (LinkDelete(L, 6, &e)) {
        printf("被删除的元素 = %d\n", e);
        LinkPrint(L);
    } else printf("删除失败。\n");
    LinkDestroy(&L);
}

int main(void)
{
    int choice;
    printf("\n========== 学号：待填写  姓名：待填写 ==========\n");
    printf("========== 实验一  线性表的表示与实现 ==========\n");
    for (;;) {
        printf("\n1. 顺序表操作\n2. 单链表操作\n0. 退出\n请选择：");
        if (scanf("%d", &choice) != 1 || choice == 0) break;
        if (choice == 1) SeqDemo();
        else if (choice == 2) LinkDemo();
        else printf("输入有误，请重新选择。\n");
    }
    return 0;
}
