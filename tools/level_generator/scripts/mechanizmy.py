global lvl
def taśmociąg_jednostronny(x=0, y=0, długość=10, gruość=0.5, prędkość=2, segmentowanie=10, tekstura="panels,0", elementy=(0,0))
    czas = round(długość/segmentowanie/prędkość*60)
    for i in range(segmentowanie):
        lvl.add("""
        <platform position="{0},{1}" texture="{2}">
            <p></p>
            <p></p>
            <p></p>

            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
        </platform>""".format(x, y, tekstura_podpórki, grubość*obrót, długość))


    