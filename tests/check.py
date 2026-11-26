"""运行 python3 tests/check.py，编译产物放在临时目录。"""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent.parent


def run(exe, data, success=True):
    result = subprocess.run([str(exe)], input=data, text=True, capture_output=True)
    assert (result.returncode == 0) == success, result.stderr
    return result.stdout


with tempfile.TemporaryDirectory() as directory:
    build = Path(directory)
    for name, source in [
        ("list", "lab1/linear_list.c"),
        ("tree", "lab2/binary_tree.c"),
        ("graph", "lab3/prim.c"),
    ]:
        subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Werror",
                        str(ROOT / source), "-o", str(build / name)], check=True)

    output = run(build / "list", "1\n2\n0\n")
    assert output.count("被删除的元素 = 50") == 2
    assert "10 20 30 45 40 60 70 80" in output

    # 直接检查空表、表满、首尾插删与非法位序。
    harness = build / "list_check.c"
    harness.write_text(
        '#include <assert.h>\n#define main lab_main\n#include "'
        + str(ROOT / "lab1/linear_list.c") + '"\n#undef main\n'
        + """
int main(void) {
    SqList s;
    LinkList l = NULL;
    int e;
    assert(SeqInit(&s));
    assert(!SeqDelete(&s, 1, &e));
    assert(!SeqInsert(&s, 0, 1));
    assert(!SeqCreate(&s, 21));
    assert(SeqInsert(&s, 1, 7));
    assert(SeqInsert(&s, 2, 8));
    assert(SeqDelete(&s, 1, &e) && e == 7);
    assert(SeqDelete(&s, 1, &e) && e == 8);
    assert(SeqCreate(&s, 20));
    assert(!SeqInsert(&s, 21, 1));
    assert(SeqLocate(s, 200) == 20);
    assert(!SeqDelete(&s, 21, &e));
    assert(LinkCreate(&l, 0));
    assert(!LinkDelete(l, 1, &e));
    assert(!LinkInsert(l, 0, 1));
    assert(!LinkInsert(l, 2, 1));
    assert(LinkInsert(l, 1, 7));
    assert(LinkInsert(l, 2, 8));
    assert(LinkLocate(l, 8)->data == 8);
    assert(LinkLocate(l, 9) == NULL);
    assert(LinkDelete(l, 2, &e) && e == 8);
    assert(LinkDelete(l, 1, &e) && e == 7);
    assert(!LinkDelete(l, 1, &e));
    LinkDestroy(&l);
    assert(l == NULL);
    return 0;
}
""", encoding="utf-8")
    subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Werror",
                    str(harness), "-o", str(build / "list_check")], check=True)
    run(build / "list_check", "")

    for data, preorder, inorder, postorder, stats in [
        ("ABD##E##C#F##\n", "A B D E C F", "D B E A C F", "D E B F C A", (3, 6, 3, 1)),
        ("#\n", "", "", "", (0, 0, 0, 0)),
        ("A##\n", "A", "A", "A", (1, 1, 1, 0)),
        ("AB###\n", "A B", "B A", "B A", (2, 2, 1, 1)),
    ]:
        output = run(build / "tree", data)
        for label, expected in zip(("先序遍历", "中序遍历", "后序遍历"),
                                   (preorder, inorder, postorder)):
            assert f"{label}：{expected}" in output, output
        for label, value in zip(("树的深度", "结点总数", "叶子结点数", "度为 1 的结点数"), stats):
            assert f"{label}：{value}\n" in output, output
    run(build / "tree", "AB#\n", False)
    run(build / "tree", "A###\n", False)

    connected = "5 7\n0 1 2\n0 2 4\n1 2 1\n1 3 5\n2 3 3\n2 4 6\n3 4 2\n"
    output = run(build / "graph", connected)
    assert "总成本 = 8 万元" in output
    for edge in ("(0, 1) 成本 = 2", "(1, 2) 成本 = 1",
                 "(2, 3) 成本 = 3", "(3, 4) 成本 = 2"):
        assert edge in output
    assert "lowcost[] = 0 0 1 5 INF" in output
    output = run(build / "graph", "4 2\n0 1 3\n2 3 4\n")
    assert "无法全部连通！" in output and "总成本" not in output
    assert "总成本 = 0 万元" in run(build / "graph", "1 0\n")
    assert "总成本 = 3000 万元" in run(build / "graph", "2 1\n0 1 3000\n")
    for data in ("9 0\n", "2 2\n", "2 1\n0 2 3\n",
                 "2 1\n0 1 0\n", "3 2\n0 1 2\n1 0 3\n"):
        run(build / "graph", data, False)

print("全部通过：严格编译、线性表边界操作、二叉树遍历统计、Prim 连通/不连通/边界及非法输入。")
