from level_base import level

lamp = """<object position="{0}, {1}" texture="{2}" height="4" layer="7"/>
<light position="{0}, {3}" color="{4}, {5}, {6}" intensity="0.25"/>"""

def stairs(pos, n, size, tex):
    s = ""
    for i in range(n):
        s += """    <platform position="{0}, {1}" texture="{6}">
        <v>{2},{3}</v>
        <v>{4},{3}</v>
        <v>{4},{5}</v>
        <v>{2},{5}</v>
    </platform>
""".format(pos[0], pos[1], 0, size[0]/n*i, size[0]/n*(i+1), size[0]/n*(i+1), tex)
    return s

lvl = level('Poziom testowy python')
lvl.add(stairs((10,10),5,(10,10),"bricks,1"))
for i in range(5):
    lvl.add(
    """    <platform position="0,{0}" texture="dirt,0">
        <v>0,0</v>
        <v>30,0</v>
        <v>30,0.5</v>
        <v>0,0.5</v>
    </platform>""".format(20+i*5))
lvl.add(lamp.format(10, 20, 'lamp0', 21, 255, 255, 255))
lvl.save('out.xml')