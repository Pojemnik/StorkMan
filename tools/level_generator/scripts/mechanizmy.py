import math
import operacje
import podstawy
global lvl
def taśmociąg_jednostronny(x=0, y=0, długość=10, grubość=0.5, obrót=0, prędkość=2, segmentowanie=10, bariery=1, widoczność_barier=1, tekstura=podstawy.domyślna_tekstura, elementy=(0,0), zwróć_tekst=False):
    s = ""
    czas = round(długość/segmentowanie/prędkość*60)
    długość_x = round(math.cos(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_x = round(math.cos(math.radians(90-obrót))*grubość,ndigits=4)
    długość_y = round(math.sin(math.radians(obrót))*długość/segmentowanie,ndigits=8)
    grubość_y = round(-math.sin(math.radians(90-obrót))*grubość,ndigits=4)
    v_x = (0,długość_x,długość_x+grubość_x,grubość_x)
    v_y = (0,długość_y,długość_y+grubość_y,grubość_y)
    for i in range(segmentowanie-1):
        p_x = (0,długość_x*(segmentowanie-1-i),-długość_x*i)
        p_y = (0,długość_y*(segmentowanie-1-i),-długość_y*i)
        p_t = ((segmentowanie-1-i)*czas,0,i*czas)
        s = s + podstawy.ruchoma_platforma(x=x+długość_x*i,y=y+długość_y*i,tekstura=tekstura,wierzchołki=4,wierzchołki_x=v_x,wierzchołki_y=v_y,znaczniki=3,znaczniki_x=p_x,znaczniki_y=p_y,znaczniki_t=p_t,zwróć_tekst=True)
    if bariery == 1:
        if widoczność_barier==1:
            s = s + podstawy.platforma(x=x+długość_x*(segmentowanie-1),y=y+długość_y*(segmentowanie-1),tekstura=tekstura,wierzchołki=4,wierzchołki_x=v_x,wierzchołki_y=v_y,zwróć_tekst=True)
            s = s + podstawy.platforma(x=x,y=y,tekstura=tekstura,wierzchołki=4,wierzchołki_x=v_x,wierzchołki_y=v_y,zwróć_tekst=True)
        else:
            s = s + podstawy.bariera(x=x+długość_x*(segmentowanie-1),y=y+długość_y*(segmentowanie-1),wierzchołki=4,wierzchołki_x=v_x,wierzchołki_y=v_y,zwróć_tekst=True)
            s = s + podstawy.bariera(x=x,y=y,wierzchołki=4,wierzchołki_x=v_x,wierzchołki_y=v_y,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
