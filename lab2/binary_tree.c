#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode {
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

void DestroyTree(BiTree *T)
{
    if (*T == NULL) return;
    DestroyTree(&(*T)->lchild);
    DestroyTree(&(*T)->rchild);
    free(*T);
    *T = NULL;
}

/* 用状态值区分空子树和输入/分配失败，并释放构造了一半的树。 */
int CreateBiTree(BiTree *T)
{
    char ch;
    *T = NULL;
    if (scanf(" %c", &ch) != 1) return 0;
    if (ch == '#') return 1;
    *T = malloc(sizeof(BiTNode));
    if (*T == NULL) return 0;
    (*T)->data = ch;
    (*T)->lchild = (*T)->rchild = NULL;
    if (!CreateBiTree(&(*T)->lchild) || !CreateBiTree(&(*T)->rchild)) {
        DestroyTree(T);
        return 0;
    }
    return 1;
}

void PreOrder(BiTree T)
{
    if (T == NULL) return;
    printf("%c ", T->data);
    PreOrder(T->lchild);
    PreOrder(T->rchild);
}

void InOrder(BiTree T)
{
    if (T == NULL) return;
    InOrder(T->lchild);
    printf("%c ", T->data);
    InOrder(T->rchild);
}

void PostOrder(BiTree T)
{
    if (T == NULL) return;
    PostOrder(T->lchild);
    PostOrder(T->rchild);
    printf("%c ", T->data);
}

int TreeDepth(BiTree T)
{
    int left, right;
    if (T == NULL) return 0;
    left = TreeDepth(T->lchild);
    right = TreeDepth(T->rchild);
    return (left > right ? left : right) + 1;
}

int CountNodes(BiTree T)
{
    if (T == NULL) return 0;
    return CountNodes(T->lchild) + CountNodes(T->rchild) + 1;
}

int CountLeaves(BiTree T)
{
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL) return 1;
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}

int CountOneChild(BiTree T)
{
    if (T == NULL) return 0;
    return CountOneChild(T->lchild) + CountOneChild(T->rchild)
           + ((T->lchild == NULL) != (T->rchild == NULL));
}

int main(void)
{
    BiTree T = NULL;
    printf("\n========== 学号：待填写  姓名：待填写 ==========\n");
    printf("========== 实验二  二叉树的表示与实现 ==========\n");
    printf("请输入先序扩展序列（# 表示空子树，输入结束后回车）：\n");
    if (!CreateBiTree(&T)) {
        fprintf(stderr, "构造失败：序列不完整或内存不足。\n");
        return 1;
    }
    /* 一棵树结束后，只允许本行剩余部分为空白。 */
    {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            if (ch != ' ' && ch != '\t' && ch != '\r') {
                fprintf(stderr, "序列包含多余字符。\n");
                DestroyTree(&T);
                return 1;
            }
        }
    }
    printf("先序遍历："); PreOrder(T);
    printf("\n中序遍历："); InOrder(T);
    printf("\n后序遍历："); PostOrder(T);
    printf("\n树的深度：%d\n", TreeDepth(T));
    printf("结点总数：%d\n", CountNodes(T));
    printf("叶子结点数：%d\n", CountLeaves(T));
    printf("度为 1 的结点数：%d\n", CountOneChild(T));
    DestroyTree(&T);
    return 0;
}
