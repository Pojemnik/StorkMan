import math
import podstawy
import operacje
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
def dźwignia(x=0, y=0, promień=0.5, promień_strefy=1, warstwa=5, zdarzenia=["event1","event2"], zdarzenia_wewnętrzne=["L1","L2","L3","L4"], czas_przełożenia=30, wydłużenie=1, pozycja_początkowa=0, obrót=1, zwróć_tekst=False):
    s1 = ""
    s1 += podstawy.zdarzenie(zdarzenia_wewnętrzne[0],True)
    s1 += podstawy.zdarzenie(zdarzenia_wewnętrzne[1],True)
    s1 += podstawy.zdarzenie(zdarzenia_wewnętrzne[2],True)
    s1 += podstawy.zdarzenie(zdarzenia_wewnętrzne[3],True)
    s2 = podstawy.obiekt(x=x,y=y-promień,odbicie_x=obrót,wysokość=2*promień,warstwa=warstwa,tekstura="OBJLever_Base",zwróć_tekst=True)
    stany = [4,"","","",""]
    v = ["v",4]
    v += [0,-promień_strefy]
    v += [promień_strefy*obrót,-promień_strefy]
    v += [promień_strefy*obrót,promień_strefy]
    v += [0,promień_strefy]
    stany[1] += podstawy.strefa_klikalna(x=x,y=y,wierzchołki=v,zdarzenia=["e",2,zdarzenia[0],zdarzenia_wewnętrzne[3]],zwróć_tekst=True)
    stany[1] += podstawy.obiekt(x=x,y=y,odbicie_x=obrót,wysokość=2*promień,warstwa=warstwa,tekstura="OBJLever_Arm_Red",rotacja=90*-obrót,zwróć_tekst=True)
    stany[2] += podstawy.strefa_klikalna(x=x,y=y,wierzchołki=v,zdarzenia=["e",2,zdarzenia[1],zdarzenia_wewnętrzne[2]],zwróć_tekst=True)
    stany[2] += podstawy.obiekt(x=x,y=y,odbicie_x=obrót,wysokość=2*promień,warstwa=warstwa,tekstura="OBJLever_Arm_Red",rotacja=0,zwróć_tekst=True)
    stany[3] += podstawy.ruchomy_obiekt(x=x,y=y,odbicie_x=obrót,wysokość=2*promień,warstwa=warstwa,tekstura="OBJLever_Arm_Red",rotacja=0,ruch=["rotational",2,0,czas_przełożenia+wydłużenie,90*-obrót,czas_przełożenia],zwróć_tekst=True)
    stany[4] += podstawy.ruchomy_obiekt(x=x,y=y,odbicie_x=obrót,wysokość=2*promień,warstwa=warstwa,tekstura="OBJLever_Arm_Red",rotacja=90*-obrót,ruch=["rotational",2,0,czas_przełożenia+wydłużenie,90*obrót,0],zwróć_tekst=True)
    tekst = ""
    tekst += podstawy.wyzwalacz_stanu(0,zdarzenia_wewnętrzne[0],True)
    tekst += podstawy.wyzwalacz_stanu(1,zdarzenia_wewnętrzne[1],True)
    tekst += podstawy.wyzwalacz_stanu(2,zdarzenia_wewnętrzne[2],True)
    tekst += podstawy.wyzwalacz_stanu(3,zdarzenia_wewnętrzne[3],True)
    tekst += podstawy.czasowe_zdarzenie(2,zdarzenia_wewnętrzne[0],czas_przełożenia,True)
    tekst += podstawy.czasowe_zdarzenie(3,zdarzenia_wewnętrzne[1],czas_przełożenia,True)
    if zwróć_tekst:
        return stany,tekst,pozycja_początkowa,s1,s2
    else:
        podstawy.zapis(s1)
        podstawy.zapis_stanów(stany,tekst,pozycja_początkowa)
        podstawy.zapis(s2)
def przesuwalny_element(element=podstawy.platforma(zwróć_tekst=True), zdarzenia=["event1","event2"], zdarzenia_wewnętrzne=["event3","event4"], czas_przesunięcia=30, wydłużenie=1, przesunięcie=[1,1], zwróć_tekst=False):
    s = ""
    s += podstawy.zdarzenie(zdarzenia_wewnętrzne[0],True)
    s += podstawy.zdarzenie(zdarzenia_wewnętrzne[1],True)
    stany = [4,"","","",""]
    stany[1] += element
    stany[2] += operacje.element_potomny(element,"punkt",[przesunięcie[0],przesunięcie[1]],True)
    stany[3] += operacje.element_potomny(element,"ruch",["linear",2,0,0,czas_przesunięcia+wydłużenie,przesunięcie[0],przesunięcie[1],0],True)
    stany[4] += operacje.element_potomny(element,"ruch",["linear",2,przesunięcie[0],przesunięcie[1],czas_przesunięcia+wydłużenie,0,0,0],True)
    tekst = ""
    tekst += podstawy.wyzwalacz_stanu(0,zdarzenia_wewnętrzne[1],True)
    tekst += podstawy.wyzwalacz_stanu(1,zdarzenia_wewnętrzne[0],True)
    tekst += podstawy.wyzwalacz_stanu(2,zdarzenia[0],True)
    tekst += podstawy.wyzwalacz_stanu(3,zdarzenia[1],True)
    tekst += podstawy.czasowe_zdarzenie(2,zdarzenia_wewnętrzne[0],czas_przesunięcia,True)
    tekst += podstawy.czasowe_zdarzenie(3,zdarzenia_wewnętrzne[1],czas_przesunięcia,True)
    if zwróć_tekst:
        return stany,tekst,0,s
    else:
        podstawy.zapis(s)
        podstawy.zapis_stanów(stany,tekst,0)