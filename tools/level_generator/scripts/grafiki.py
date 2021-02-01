import math
import podstawy
import operacje
import ruchy

def świetlówka(x=0, y=0, obrót=1, długość=2, warstwa=5, czas_klatki=1, dźwięk="LAMPA_01", głośność=100, wyciszanie=1, zasięg_dźwięku=20, przesunięcie=0, zwróć_tekst=False):
    s = ""
    tekstura = "AOBFluorescent_shining_instable"
    if przesunięcie<0:
        przesunięcie = random.random()*(-przesunięcie)
    s += podstawy.dźwięk(x=x+(długość/2)*obrót,y=y+długość/8,głośność=głośność,dźwięk=dźwięk,wyciszanie=wyciszanie,próg_głośności=długość/2,zasięg=zasięg_dźwięku,zwróć_tekst=True)
    s += podstawy.animowany_obiekt(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wysokość=długość/4,czas_klatki=czas_klatki,przesunięcie=przesunięcie,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def interaktywna_świetlówka(x=0, y=0, obrót=1, długość=2, włącznik="event1", wyłącznik="event2", pozycja_początkowa=0, warstwa=5, czas_klatki=1, dźwięk="LAMPA_01", głośność=100, wyciszanie=1, zasięg_dźwięku=20, przesunięcie=0, zwróć_tekst=False):
    s = ""
    if pozycja_początkowa==0:
        zd = ["et",3,"default","true",włącznik,"true",wyłącznik,"false"]
    else:
        zd = ["et",3,"default","false",włącznik,"true",wyłącznik,"false"]
    s += podstawy.dźwięk(x=x+(długość/2)*obrót,y=y+długość/8,głośność=głośność,dźwięk=dźwięk,wyciszanie=wyciszanie,próg_głośności=długość/2,zasięg=zasięg_dźwięku,zdarzenia=zd,zwróć_tekst=True)
    animacja = "AOBFluorescent_shining_instable"
    tekstura = "OBJFluorescent_off"
    s += "<state>\n"
    s += podstawy.animowany_obiekt(x=x,y=y,warstwa=warstwa,tekstura=animacja,wysokość=długość/4,czas_klatki=czas_klatki,przesunięcie=przesunięcie,zwróć_tekst=True)
    s += "</state>\n"
    s += "<state>\n"
    s += podstawy.obiekt(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wysokość=długość/4,zwróć_tekst=True)
    s += "</state>\n"
    s += podstawy.wyzwalacz_stanu(pozycja_początkowa,"default",True)
    s += podstawy.wyzwalacz_stanu(0,włącznik,True)
    s += podstawy.wyzwalacz_stanu(1,wyłącznik,True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def generator(x=0, y=0, obrót=1, wysokość=4, warstwa=5, turbiny=[3,"TURBINA_04","TURBINA_05","TURBINA_06"], zdarzenia_dźwięku=["et",0], podstawa_wzmocnienia=2, głośność=200, zasięg_dźwięku=20, czas_obrotu_wału=60, wstrząsy=[True,0.02,7,0,2], zwróć_tekst=False):
    czas_klatki = czas_obrotu_wału/4/32
    poz_x = x + wysokość/2*obrót
    s = ""
    z = ""
    s += podstawy.obiekt(x=x,y=y-wysokość/2,warstwa=warstwa,tekstura="OBJSteam_generator",wysokość=wysokość/2,zwróć_tekst=True)
    for i in range(turbiny[0]):
        s += podstawy.animowany_obiekt(x=poz_x,y=y-wysokość*3/8,warstwa=warstwa,tekstura="AOBRotating_cylinder",wysokość=wysokość/4,czas_klatki=czas_klatki/pow(podstawa_wzmocnienia,i),zwróć_tekst=True)
        poz_x += wysokość/4*obrót
        s += podstawy.obiekt(x=poz_x,y=y-wysokość,warstwa=warstwa,tekstura="OBJSteam_turbine",wysokość=wysokość,zwróć_tekst=True)
        poz_x += wysokość*obrót
        if turbiny[i+1]!=None:
            z += podstawy.dźwięk(x=poz_x,y=y-wysokość/2,głośność=głośność,dźwięk=turbiny[i+1],próg_głośności=wysokość,zasięg=zasięg_dźwięku,zdarzenia=zdarzenia_dźwięku,zwróć_tekst=True)
        poz_x += wysokość*obrót
    if wstrząsy[0]:
        s = operacje.element_potomny(s,"ruch",ruchy.wstrząsy(wstrząsy[1],wstrząsy[2],wstrząsy[3],wstrząsy[4]),True) + z
    else:
        s = s + z
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)