import pysvf

pag = pysvf.analyze("./BASIC_array_2d_0-0.c.bc")

icfg = pag.get_icfg()
nodes = icfg.get_nodes()

for node in nodes:
    print(node.to_string())
