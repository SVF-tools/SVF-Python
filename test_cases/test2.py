import pysvf

pag = pysvf.analyze("BASIC_array_2d_0-0.c.bc")

icfg = pag.get_icfg()
nodes = icfg.get_nodes()

node_16 = icfg.get_gnode(16)
print("Node 16:", node_16.to_string())
for stmt in node_16.get_svf_stmts():
    print("Stmt::", stmt.to_string())
    print("Is CMP: " + str(stmt.is_cmp_stmt()))
    print("Is Addr: " + str(stmt.is_addr_stmt()))
