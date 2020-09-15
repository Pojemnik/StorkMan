import math
global lvl
def taśmociąg_jednostronny(x=0, y=0, długość=10, grubość=0.5, obrót=0, prędkość=2, segmentowanie=10, bariery=1, widoczność_barier=1, tekstura="panels,0", elementy=(0,0)):
    czas = round(długość/segmentowanie/prędkość*60)
    długość_x = math.cos(math.radians(obrót))*długość/segmentowanie
    grubość_x = math.cos(math.radians(90-obrót))*grubość
    długość_y = math.sin(math.radians(obrót))*długość/segmentowanie
    grubość_y = -math.sin(math.radians(90-obrót))*grubość
    for i in range(segmentowanie-1):
        lvl.add("""
        <linear_platform position="{0},{1}" texture="{2}">
            <p>0,0,{3}</p>
            <p>{4},{5},0</p>
            <p>{6},{7},{8}</p>

            <v>0,0</v>
            <v>{9},{10}</v>
            <v>{11},{12}</v>
            <v>{13},{14}</v>
        </linear_platform>""".format(x+długość_x*i, y+długość_y*i, tekstura, (segmentowanie-1-i)*czas, długość_x*(segmentowanie-1-i), długość_y*(segmentowanie-1-i), -długość_x*i, -długość_y*i, i*czas, długość_x, długość_y, długość_x+grubość_x, długość_y+grubość_y, grubość_x, grubość_y))
    if bariery == 1:
        lvl.add("""
        <platform position="{0},{1}" texture="{2}" visible="{3}">
            <v>0,0</v>
            <v>{4},{5}</v>
            <v>{6},{7}</v>
            <v>{8},{9}</v>
        </platform>""".format(x+długość_x*(segmentowanie-1), y+długość_y*(segmentowanie-1), tekstura, widoczność_barier, długość_x, długość_y, długość_x+grubość_x, długość_y+grubość_y, grubość_x, grubość_y))
        lvl.add("""
        <platform position="{0},{1}" texture="{2}" visible="{3}">
            <v>0,0</v>
            <v>{4},{5}</v>
            <v>{6},{7}</v>
            <v>{8},{9}</v>
        </platform>""".format(x, y, tekstura, widoczność_barier, długość_x, długość_y, długość_x+grubość_x, długość_y+grubość_y, grubość_x, grubość_y))
