import pysvf

# 运行 SVF 分析，获取 `SVFIR` 对象
pag = pysvf.analyze("./BASIC_array_2d_0-0.c.bc")

# 获取 `ICFG` 并遍历所有节点
icfg = pag.get_icfg()
nodes = icfg.get_nodes()

for node in nodes:
    print(node.to_string())
