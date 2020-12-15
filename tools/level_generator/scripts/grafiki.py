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
