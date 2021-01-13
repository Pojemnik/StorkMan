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