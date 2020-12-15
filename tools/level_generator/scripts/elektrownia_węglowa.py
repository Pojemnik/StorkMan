import math
import podstawy
import operacje
import ruchy

def generator(x=0, y=0, obrót=1, wysokość=4, warstwa=5, turbiny=[3,"TURBINA_04","TURBINA_05","TURBINA_06"], podstawa_wzmocnienia=2, głośność=200, zasięg_dźwięku=20, czas_obrotu_wału=60, wstrząsy=[True,0.02,7,0,2], zwróć_tekst=False):
    czas_klatki = czas_obrotu_wału/4/32
    poz_x = x + wysokość/2*obrót
    s = ""
    z = ""
    s += podstawy.obiekt(x=x,y=y-wysokość/2,warstwa=warstwa,tekstura="OBJSteam_Generator",wysokość=wysokość/2,zwróć_tekst=True)
    for i in range(turbiny[0]):
        s += podstawy.animowany_obiekt(x=poz_x,y=y-wysokość*3/8,warstwa=warstwa,tekstura="AOBRotating_Cylinder",wysokość=wysokość/4,czas_klatki=czas_klatki/pow(podstawa_wzmocnienia,i),zwróć_tekst=True)
        poz_x += wysokość/4*obrót
        s += podstawy.obiekt(x=poz_x,y=y-wysokość,warstwa=warstwa,tekstura="OBJSteam_Turbine",wysokość=wysokość,zwróć_tekst=True)
        poz_x += wysokość*obrót
        z += podstawy.dźwięk(x=poz_x,y=y-wysokość/2,głośność=głośność,dźwięk=turbiny[i+1],próg_głośności=wysokość,zasięg=zasięg_dźwięku,zwróć_tekst=True)
        poz_x += wysokość*obrót
    if wstrząsy[0]:
        s = operacje.element_potomny(s,"ruch",ruchy.wstrząsy(wstrząsy[1],wstrząsy[2],wstrząsy[3],wstrząsy[4]),True) + z
    else:
        s = s + z
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
