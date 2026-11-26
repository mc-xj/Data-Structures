#include <stdio.h>

#define MAXV 8
#define INF 99999
/* 每条边最多 3000，28 条边之和也小于 INF。 */
#define MAXCOST 3000

int cost[MAXV][MAXV];
int n;

void InitGraph(void)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            cost[i][j] = (i == j ? 0 : INF);
}

int ReadGraph(void)
{
    int m, k, u, v, w;
    printf("请输入点位数 n 和边数 m（1<=n<=8）：\n");
    if (scanf("%d%d", &n, &m) != 2 || n < 1 || n > MAXV)
        return 0;
    if (m < 0 || m > n * (n - 1) / 2) return 0;
    InitGraph();
    printf("请输入每条边：起点 终点 成本（编号从0开始，成本1~3000）\n");
    for (k = 0; k < m; k++) {
        if (scanf("%d%d%d", &u, &v, &w) != 3) return 0;
        if (u < 0 || u >= n || v < 0 || v >= n || u == v ||
            w < 1 || w > MAXCOST) return 0;
        if (cost[u][v] != INF) {
            printf("重复边，请检查输入。\n");
            return 0;
        }
        cost[u][v] = cost[v][u] = w;
    }
    return 1;
}

void PrintMatrix(void)
{
    int i, j;
    printf("邻接矩阵（INF 表示不可直接拉线）：\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (cost[i][j] == INF) printf("%6s", "INF");
            else printf("%6d", cost[i][j]);
        }
        printf("\n");
    }
}

void PrintState(int lowcost[], int adjvex[], int round)
{
    int i;
    printf("第 %d 轮后：lowcost[] =", round);
    for (i = 0; i < n; i++) {
        if (lowcost[i] == INF) printf(" INF");
        else printf(" %d", lowcost[i]);
    }
    printf("    adjvex[] =");
    for (i = 0; i < n; i++) printf(" %d", adjvex[i]);
    printf("\n");
}

int Prim(int start)
{
    int lowcost[MAXV], adjvex[MAXV];
    int i, j, k, min, total = 0;
    for (i = 0; i < n; i++) {
        lowcost[i] = cost[start][i];
        adjvex[i] = start;
    }
    lowcost[start] = 0;
    PrintState(lowcost, adjvex, 0);
    for (i = 1; i < n; i++) {
        min = INF;
        k = -1;
        for (j = 0; j < n; j++) {
            if (lowcost[j] != 0 && lowcost[j] < min) {
                min = lowcost[j];
                k = j;
            }
        }
        if (k == -1) return -1;
        printf("(%d, %d) 成本 = %d\n", adjvex[k], k, min);
        total += min;
        lowcost[k] = 0;
        for (j = 0; j < n; j++) {
            if (lowcost[j] != 0 && cost[k][j] < lowcost[j]) {
                lowcost[j] = cost[k][j];
                adjvex[j] = k;
            }
        }
        PrintState(lowcost, adjvex, i);
    }
    return total;
}

int main(void)
{
    int total;
    if (!ReadGraph()) {
        fprintf(stderr, "输入非法或不完整。\n");
        return 1;
    }
    PrintMatrix();
    total = Prim(0);
    if (total < 0)
        printf("无法全部连通！请检查各充电桩之间的可拉线关系。\n");
    else
        printf("最小生成树总成本 = %d 万元\n", total);
    return 0;
}
