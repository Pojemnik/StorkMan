import math
import podstawy
def taśmociąg_jednostronny(x=0, y=0, warstwa=5, długość=10, grubość=0.5, obrót=0, prędkość=2, segmentowanie=10, bariery=True, widoczność_barier=True, tekstura=podstawy.domyślna_tekstura, elementy=(0,0), zwróć_tekst=False):
    s = ""
    czas = round(długość/segmentowanie/prędkość*60)
    długość_x = round(math.cos(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_x = round(math.cos(math.radians(90-obrót))*grubość,ndigits=4)
    długość_y = round(math.sin(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_y = round(-math.sin(math.radians(90-obrót))*grubość,ndigits=4)
    v = ["v",4,0,0,długość_x,długość_y,długość_x+grubość_x,długość_y+grubość_y,grubość_x,grubość_y]
    for i in range(segmentowanie-1):
        p = ["linear",3,0,0,(segmentowanie-1-i)*czas,długość_x*(segmentowanie-1-i),długość_y*(segmentowanie-1-i),0,-długość_x*i,-długość_y*i,i*czas]
        s = s + podstawy.ruchoma_platforma(x=x+długość_x*i,y=y+długość_y*i,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,ruch=p,zwróć_tekst=True)
    if bariery:
        if widoczność_barier:
            s = s + podstawy.platforma(x=x+długość_x*(segmentowanie-1),y=y+długość_y*(segmentowanie-1),warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
            s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        else:
            s = s + podstawy.bariera(x=x+długość_x*(segmentowanie-1),y=y+długość_y*(segmentowanie-1),wierzchołki=v,zwróć_tekst=True)
            s = s + podstawy.bariera(x=x,y=y,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def taśmociąg_dwustronny(x=0, y=0, warstwa=5, długość=10, grubość=0.5, rozstaw=2, obrót=0, prędkość=2, tekstura=podstawy.domyślna_tekstura, elementy=(0,0), zwróć_tekst=False):
    s = ""
    czas = round(długość/segmentowanie/prędkość*60)
    długość_x = round(math.cos(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_x = round(math.cos(math.radians(90-obrót))*grubość,ndigits=4)
    długość_y = round(math.sin(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_y = round(-math.sin(math.radians(90-obrót))*grubość,ndigits=4)
    v = ["v",4,0,0,długość_x,długość_y,długość_x+grubość_x,długość_y+grubość_y,grubość_x,grubość_y]
    for i in range(segmentowanie-1):
        p = ["linear",3,0,0,(segmentowanie-1-i)*czas,długość_x*(segmentowanie-1-i),długość_y*(segmentowanie-1-i),0,-długość_x*i,-długość_y*i,i*czas]
        s = s + podstawy.ruchoma_platforma(x=x+długość_x*i,y=y+długość_y*i,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,ruch=p,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
