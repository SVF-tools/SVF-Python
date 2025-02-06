import pysvf

# 运行 SVF 分析，获取 `SVFIR` 对象
pag = pysvf.analyze("BASIC_array_2d_0-0.c.bc")

# 获取 `ICFG` 并遍历所有节点
icfg = pag.get_icfg()
nodes = icfg.get_nodes()

node_16 = icfg.get_gnode(16)
print("Node 16:", node_16.to_string())
for stmt in node_16.get_svf_stmts():
    print("Stmt::", stmt.to_string())
    print("Is CMP: " + str(stmt.is_cmp_stmt()))
    print("Is Addr: " + str(stmt.is_addr_stmt()))
