import podstawy
global lvl
def schody(x=0, y=0, obrót=1, długość=2, wysokość=2, przesunięcie=0, stopnie=4, tekstura=podstawy.domyślna_tekstura, zwróć_tekst=False):
    s = ""
    for i in range(stopnie):
        v = ["v",4,]
        v += [0,-wysokość/stopnie*i]
        v += [(długość-(długość-przesunięcie)/stopnie*i)*obrót,-wysokość/stopnie*i]
        v += [(długość-(długość-przesunięcie)/stopnie*i)*obrót,-wysokość/stopnie*(i+1)]
        v += [0,-wysokość/stopnie*(i+1)]
        s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def drabina_jednostronna(x=0, y=0, obrót=1, podpórka=1, grubość=0.5, długość=6, szczeble=3, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_podpórki=podstawy.domyślna_tekstura, tekstura=podstawy.domyślna_tekstura, zwróć_tekst=False):
    s = ""
    if podpórka == 1:
        v = ["v",4]
        v += [0,0]
        v += [grubość*obrót,0]
        v += [grubość*obrót,długość]
        v += [0,długość]
        s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura_podpórki,wierzchołki=v,zwróć_tekst=True)
    if szczeble >= 2:
        v = ["v",4]
        v += [0,0]
        v += [głębokość*obrót,0]
        v += [głębokość*obrót,wysokość]
        v += [0,wysokość]
        for i in range(szczeble):
            s = s + podstawy.platforma(x=x+grubość*podpórka*obrót,y=y+górny_margines+(długość-górny_margines-dolny_margines-wysokość)/(szczeble-1)*i,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def drabina_dwustronna(x=0, y=0, naprzemienność=1, początek=0, lewe_przycięcie=0, prawe_przycięcie=0, lewa_podpórka=1, prawa_podpórka=1, grubość_lewej=0.5, grubość_prawej=0.5, długość=6, szczeble=3, rozpiętość=2, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_lewej_podpórki=podstawy.domyślna_tekstura, tekstura_prawej_podpórki=podstawy.domyślna_tekstura, tekstura=podstawy.domyślna_tekstura, zwróć_tekst=False):
    s = ""
    delta = (długość-górny_margines-dolny_margines-wysokość)/(szczeble-1)
    if lewa_podpórka == 1:
        v_x = (0,grubość_lewej,grubość_lewej,0)
        if lewe_przycięcie == 0:
            v_y = (0,0,długość,długość)
        if lewe_przycięcie == 1:
            if początek == szczeble%2:
                v_y = (0,0,długość-dolny_margines-delta,długość-dolny_margines-delta)
            if początek != szczeble%2:
                v_y = (0,0,długość-dolny_margines-delta,długość-dolny_margines)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura_lewej_podpórki,wierzchołki=v,zwróć_tekst=True)
    if prawa_podpórka == 1:
        v_x = (0,grubość_prawej,grubość_prawej,0)
        if prawe_przycięcie == 0:
            v_y = (0,0,długość,długość)
        if prawe_przycięcie == 1:
            if początek != szczeble%2:
                v_y = (0,0,długość-dolny_margines-delta,długość-dolny_margines-delta)
            if początek == szczeble%2:
                v_y = (0,0,długość-dolny_margines-delta,długość-dolny_margines)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura_prawej_podpórki,wierzchołki=v,zwróć_tekst=True)
    if szczeble >= 2:
        v = ["v",4,0,0,głębokość,0,głębokość,wysokość,0,wysokość]
        for i in range(szczeble):
            s = s + podstawy.platforma(x=x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i)%2),y=y+górny_margines+delta*i,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
            if naprzemienność == 0:
                s = s + podstawy.platforma(x=x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i+1)%2),y=y+górny_margines+delta*i,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def komnata(x=0, y=0, wysokość=10, szerokość=10, tekstura=podstawy.domyślna_tekstura, grubość_górnej=0.5, grubość_dolnej=0.5, grubość_prawej=0.5, grubość_lewej=0.5, otwory_górne=(0,0), otwory_dolne=(0,0), otwory_prawe=(0,0), otwory_lewe=(0,0), zwróć_tekst=False):
    s = ""
    v = ["v",4,0,0,grubość_lewej,0,grubość_lewej,grubość_górnej,0,grubość_górnej]
    s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_prawej,0,grubość_prawej,grubość_górnej,0,grubość_górnej]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_prawej,0,grubość_prawej,grubość_dolnej,0,grubość_dolnej]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+wysokość-grubość_dolnej,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_lewej,0,grubość_lewej,grubość_dolnej,0,grubość_dolnej]
    s = s + podstawy.platforma(x=x,y=y+wysokość-grubość_dolnej,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_lewej
    v_y = (0,0,grubość_górnej,grubość_górnej)
    for i in range(otwory_górne[0]):
        v_x = (0,otwory_górne[i*2+1]+grubość_lewej-miejsce,otwory_górne[i*2+1]+grubość_lewej-miejsce,0)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+miejsce,y=y,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_górne[i*2+1]+otwory_górne[i*2+2]+grubość_lewej
    v_x = (0,szerokość-grubość_prawej-miejsce,szerokość-grubość_prawej-miejsce,0)
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+miejsce,y=y,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_lewej
    v_y = (0,0,grubość_dolnej,grubość_dolnej)
    for i in range(otwory_dolne[0]):
        v_x = (0,otwory_dolne[i*2+1]+grubość_lewej-miejsce,otwory_dolne[i*2+1]+grubość_lewej-miejsce,0)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+miejsce,y=y+wysokość-grubość_dolnej,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_dolne[i*2+1]+otwory_dolne[i*2+2]+grubość_lewej
    v_x = (0,szerokość-grubość_prawej-miejsce,szerokość-grubość_prawej-miejsce,0)
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+miejsce,y=y+wysokość-grubość_dolnej,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_górnej
    v_x = (0,grubość_lewej,grubość_lewej,0)
    for i in range(otwory_lewe[0]):
        v_y = (0,0,otwory_lewe[i*2+1]+grubość_górnej-miejsce,otwory_lewe[i*2+1]+grubość_górnej-miejsce)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x,y=y+miejsce,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_lewe[i*2+1]+otwory_lewe[i*2+2]+grubość_górnej
    v_y = (0,0,wysokość-grubość_dolnej-miejsce,wysokość-grubość_dolnej-miejsce)
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x,y=y+miejsce,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_górnej
    v_x = (0,grubość_prawej,grubość_prawej,0)
    for i in range(otwory_prawe[0]):
        v_y = (0,0,otwory_prawe[i*2+1]+grubość_górnej-miejsce,otwory_prawe[i*2+1]+grubość_górnej-miejsce)
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+miejsce,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_prawe[i*2+1]+otwory_prawe[i*2+2]+grubość_górnej
    v_y = (0,0,wysokość-grubość_dolnej-miejsce,wysokość-grubość_dolnej-miejsce)
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+miejsce,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)  
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)  