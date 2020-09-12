global lvl
def schody(x=0, y=0, obrót=1, długość=2, wysokość=2, przesunięcie=0, stopnie=4, tekstura="bricks,0"):
    for i in range(stopnie):
        lvl.add("""
        <platform position="{0},{1}" texture="{2}">
            <v>0,{4}</v>
            <v>{3},{4}</v>
            <v>{3},{5}</v>
            <v>0,{5}</v>
        </platform>""".format(x, y, tekstura, (długość-(długość-przesunięcie)/stopnie*i)*obrót, -wysokość/stopnie*i, -wysokość/stopnie*(i+1)))
def drabina_jednostronna(x=0, y=0, obrót=1, podpórka=1, grubość=0.5, długość=6, szczeble=3, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_podpórki="wood,1", tekstura="wood,2"):
    if podpórka == 1:
        lvl.add("""
        <platform position="{0},{1}" texture="{2}">
            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
        </platform>""".format(x, y, tekstura_podpórki, grubość*obrót, długość))
    if szczeble >= 2:
        for i in range(szczeble):
            lvl.add("""
            <platform position="{0},{1}" texture="{2}">
                <v>0,0</v>
                <v>{3},0</v>
                <v>{3},{4}</v>
                <v>0,{4}</v>
            </platform>""".format(x+grubość*podpórka*obrót, y+górny_margines+(długość-górny_margines-dolny_margines-wysokość)/(szczeble-1)*i, tekstura, głębokość*obrót, wysokość))
def drabina_dwustronna(x=0, y=0, naprzemienność=1, początek=0, lewe_przycięcie=0, prawe_przycięcie=0, lewa_podpórka=1, prawa_podpórka=1, grubość_lewej=0.5, grubość_prawej=0.5, długość=6, szczeble=3, rozpiętość=2, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_lewej_podpórki="wood,1", tekstura_prawej_podpórki="wood,1", tekstura="wood,2"):
    delta = (długość-górny_margines-dolny_margines-wysokość)/(szczeble-1)
    if lewa_podpórka == 1:
        if lewe_przycięcie == 0:
            lvl.add("""
            <platform position="{0},{1}" texture="{2}">
                <v>0,0</v>
                <v>{3},0</v>
                <v>{3},{4}</v>
                <v>0,{4}</v>
            </platform>""".format(x, y, tekstura_lewej_podpórki, grubość_lewej, długość))
        if lewe_przycięcie == 1:
            if początek == szczeble%2:
                lvl.add("""
                <platform position="{0},{1}" texture="{2}">
                    <v>0,0</v>
                    <v>{3},0</v>
                    <v>{3},{4}</v>
                    <v>0,{4}</v>
                </platform>""".format(x, y, tekstura_lewej_podpórki, grubość_lewej, długość-dolny_margines-delta))
            if początek != szczeble%2:
                lvl.add("""
                <platform position="{0},{1}" texture="{2}">
                    <v>0,0</v>
                    <v>{3},0</v>
                    <v>{3},{4}</v>
                    <v>0,{4}</v>
                </platform>""".format(x, y, tekstura_lewej_podpórki, grubość_lewej, długość-dolny_margines))
    if prawa_podpórka == 1:
        if prawe_przycięcie == 0:
            lvl.add("""
            <platform position="{0},{1}" texture="{2}">
                <v>0,0</v>
                <v>{3},0</v>
                <v>{3},{4}</v>
                <v>0,{4}</v>
            </platform>""".format(x+grubość_lewej*lewa_podpórka+rozpiętość, y, tekstura_prawej_podpórki, grubość_prawej, długość))
        if prawe_przycięcie == 1:
            if początek != szczeble%2:
                lvl.add("""
                <platform position="{0},{1}" texture="{2}">
                    <v>0,0</v>
                    <v>{3},0</v>
                    <v>{3},{4}</v>
                    <v>0,{4}</v>
                </platform>""".format(x+grubość_lewej*lewa_podpórka+rozpiętość, y, tekstura_prawej_podpórki, grubość_prawej, długość-dolny_margines-delta))
            if początek == szczeble%2:
                lvl.add("""
                <platform position="{0},{1}" texture="{2}">
                    <v>0,0</v>
                    <v>{3},0</v>
                    <v>{3},{4}</v>
                    <v>0,{4}</v>
                </platform>""".format(x+grubość_lewej*lewa_podpórka+rozpiętość, y, tekstura_prawej_podpórki, grubość_prawej, długość-dolny_margines))
    if szczeble >= 2:
        for i in range(szczeble):
            lvl.add("""
            <platform position="{0},{1}" texture="{2}">
                <v>0,0</v>
                <v>{3},0</v>
                <v>{3},{4}</v>
                <v>0,{4}</v>
            </platform>""".format(x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i)%2), y+górny_margines+delta*i, tekstura, głębokość, wysokość))
            if naprzemienność == 0:
                lvl.add("""
                <platform position="{0},{1}" texture="{2}">
                    <v>0,0</v>
                    <v>{3},0</v>
                    <v>{3},{4}</v>
                    <v>0,{4}</v>
                </platform>""".format(x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i+1)%2), y+górny_margines+delta*i, tekstura, głębokość, wysokość))
def komnata(x=0, y=0, wysokość=10, szerokość=10, tekstura="bricks,0", grubość_górnej=0.5, grubość_dolnej=0.5, grubość_prawej=0.5, grubość_lewej=0.5, otwory_górne=(0,0), otwory_dolne=(0,0), otwory_prawe=(0,0), otwory_lewe=(0,0)):
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x, y, tekstura, grubość_lewej, grubość_górnej))
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x+szerokość-grubość_prawej, y, tekstura, grubość_prawej, grubość_górnej))
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x, y+wysokość-grubość_dolnej, tekstura, grubość_lewej, grubość_dolnej))
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x+szerokość-grubość_prawej, y+wysokość-grubość_dolnej, tekstura, grubość_prawej, grubość_dolnej))
    miejsce = grubość_lewej
    for i in range(otwory_górne[0]):
        lvl.add("""<platform position="{0},{1}" texture="{2}">
            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
            </platform>""".format(x+miejsce, y, tekstura, otwory_górne[i*2+1]+grubość_lewej-miejsce, grubość_górnej))
        miejsce = otwory_górne[i*2+1]+otwory_górne[i*2+2]+grubość_lewej
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x+miejsce, y, tekstura, szerokość-grubość_prawej-miejsce, grubość_górnej))
    miejsce = grubość_lewej
    for i in range(otwory_dolne[0]):
        lvl.add("""<platform position="{0},{1}" texture="{2}">
            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
            </platform>""".format(x+miejsce, y+wysokość-grubość_dolnej, tekstura, otwory_dolne[i*2+1]+grubość_lewej-miejsce, grubość_dolnej))
        miejsce = otwory_dolne[i*2+1]+otwory_dolne[i*2+2]+grubość_lewej
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x+miejsce, y+wysokość-grubość_dolnej, tekstura, szerokość-grubość_prawej-miejsce, grubość_dolnej))
    miejsce = grubość_górnej
    for i in range(otwory_lewe[0]):
        lvl.add("""<platform position="{0},{1}" texture="{2}">
            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
            </platform>""".format(x, y+miejsce, tekstura, grubość_lewej, otwory_lewe[i*2+1]+grubość_górnej-miejsce))
        miejsce = otwory_lewe[i*2+1]+otwory_lewe[i*2+2]+grubość_górnej
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x, y+miejsce, tekstura, grubość_lewej, wysokość-grubość_dolnej-miejsce))
    miejsce = grubość_górnej
    for i in range(otwory_prawe[0]):
        lvl.add("""<platform position="{0},{1}" texture="{2}">
            <v>0,0</v>
            <v>{3},0</v>
            <v>{3},{4}</v>
            <v>0,{4}</v>
            </platform>""".format(x+szerokość-grubość_prawej, y+miejsce, tekstura, grubość_prawej, otwory_prawe[i*2+1]+grubość_górnej-miejsce))
        miejsce = otwory_prawe[i*2+1]+otwory_prawe[i*2+2]+grubość_górnej
    lvl.add("""<platform position="{0},{1}" texture="{2}">
        <v>0,0</v>
        <v>{3},0</v>
        <v>{3},{4}</v>
        <v>0,{4}</v>
        </platform>""".format(x+szerokość-grubość_prawej, y+miejsce, tekstura, grubość_prawej, wysokość-grubość_dolnej-miejsce))        