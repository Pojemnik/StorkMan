import math
import random
import podstawy
def słup_chłodniczy_potrójny(x=0, y=0, warstwa=8, warstwa_poprzeczki=5, rozstaw=3, odbicie=1, wysokość=10, poprzeczki=6, szerokość=0.5, szerokość_poprzeczki=0.3, szerokość_animacji=0.2, szerokość_animacji_poprzeczki=0.15, szerokość_tekstury=64, czas_klatki=1, czas_klatki_poprzeczki=2, tekstura="TEXSteel_stick", szerokość_animowanej_tekstury=64, animowana_tekstura="ATXWater_drops_flowing", losowość=True, obecność_wody=True, zwróć_tekst=False):
    s = ""
    v = ["vt",4]
    if losowość:
        losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
    else:
        losowe_przesunięcie = 0
    v += [0,0,0,losowe_przesunięcie]
    v += [szerokość*odbicie,0,szerokość_tekstury*odbicie,losowe_przesunięcie]
    v += [szerokość*odbicie,-wysokość,szerokość_tekstury*odbicie,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    v += [0,-wysokość,0,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    s += podstawy.ściana(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    v = ["vt",4]
    if losowość:
        losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
    else:
        losowe_przesunięcie = 0
    v += [0,0,0,losowe_przesunięcie]
    v += [szerokość*odbicie,0,szerokość_tekstury*odbicie,losowe_przesunięcie]
    v += [szerokość*odbicie,-wysokość,szerokość_tekstury*odbicie,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    v += [0,-wysokość,0,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    s += podstawy.ściana(x=x+(rozstaw-szerokość)*odbicie,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if obecność_wody:
        v = ["vt",4]
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
        else:
            losowe_przesunięcie = 0
        v += [0,0,szerokość_animowanej_tekstury,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,0,0,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,-wysokość,0,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        v += [0,-wysokość,szerokość_animowanej_tekstury,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        s += podstawy.animowana_ściana(x=x-szerokość_animacji*odbicie,y=y,warstwa=warstwa,tekstura=animowana_tekstura,czas_klatki=czas_klatki,wierzchołki=v,zwróć_tekst=True)
        v = ["vt",4]
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
        else:
            losowe_przesunięcie = 0
        v += [0,0,-szerokość_animowanej_tekstury,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,0,0,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,-wysokość,0,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        v += [0,-wysokość,-szerokość_animowanej_tekstury,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        s += podstawy.animowana_ściana(x=x+(rozstaw)*odbicie,y=y,warstwa=warstwa,tekstura=animowana_tekstura,czas_klatki=czas_klatki,wierzchołki=v,zwróć_tekst=True)
    n = (pow(pow(wysokość,2) + (pow(poprzeczki*rozstaw,2) + pow(wysokość,2)) * (pow(poprzeczki*rozstaw/szerokość_poprzeczki,2)-1),0.5)-wysokość) / (pow(poprzeczki*rozstaw/szerokość_poprzeczki,2)-1)
    dł = n*rozstaw/szerokość_poprzeczki
    gr = pow(pow(dł,2)-pow(rozstaw,2),0.5)
    odchylenie = pow(pow(n,2)-pow(szerokość_poprzeczki,2),0.5)
    for i in range(poprzeczki):
        if i%2 == 0:
            przesunięcie = pow(pow(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr,2)+pow(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),2),0.5)
            if losowość:
                losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
            else:
                losowe_przesunięcie = 0
            v = ["vt",4]
            v += [0,0,szerokość_tekstury,losowe_przesunięcie]
            v += [0,n,0,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
            v += [rozstaw*odbicie,n+gr,0,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
            v += [rozstaw*odbicie,gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
            s += podstawy.platforma(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,jednostronna=True,zwróć_tekst=True)
            if obecność_wody:
                if losowość:
                    losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
                else:
                    losowe_przesunięcie = 0
                v = ["vt",4]
                v += [0,0,0,losowe_przesunięcie]
                v += [0,n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*odchylenie+losowe_przesunięcie]
                v += [rozstaw*odbicie-(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr),gr,szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*(dł+odchylenie-przesunięcie)+losowe_przesunięcie]
                v += [rozstaw*odbicie,gr,0,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*dł+losowe_przesunięcie]
                s += podstawy.animowana_ściana(x=x,y=y-wysokość+i*gr+n,warstwa=warstwa_poprzeczki,tekstura=animowana_tekstura,czas_klatki=czas_klatki_poprzeczki,wierzchołki=v,zwróć_tekst=True)
        else:
            if losowość:
                losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
            else:
                losowe_przesunięcie = 0
            v = ["vt",4]
            v += [0,n+gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
            v += [0,gr,0,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
            v += [rozstaw*odbicie,0,0,0+losowe_przesunięcie]
            v += [rozstaw*odbicie,n,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
            s += podstawy.platforma(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,jednostronna=True,zwróć_tekst=True)
            if obecność_wody:
                if losowość:
                    losowe_przesunięcie = random.random()*szerokość_animowanej_tekstury*dł/szerokość_animacji_poprzeczki
                else:
                    losowe_przesunięcie = 0
                v = ["vt",4]
                v += [n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr,gr,szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*(dł+odchylenie-przesunięcie)+losowe_przesunięcie]
                v += [0,gr,0,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*dł+losowe_przesunięcie]
                v += [rozstaw*odbicie,0,0,0+losowe_przesunięcie]
                v += [rozstaw*odbicie,n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*odchylenie+losowe_przesunięcie]
                s += podstawy.animowana_ściana(x=x,y=y-wysokość+i*gr+n,warstwa=warstwa_poprzeczki,tekstura=animowana_tekstura,czas_klatki=czas_klatki_poprzeczki,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def słup_chłodniczy_poczwórny(x=0, y=0, warstwa=8, warstwa_poprzeczki=5, rozstaw=3, odbicie=1, wysokość=10, poprzeczki=4, szerokość=0.5, szerokość_poprzeczki=0.3, szerokość_animacji=0.2, szerokość_animacji_poprzeczki=0.15, szerokość_tekstury=64, czas_klatki=1, czas_klatki_poprzeczki=2, tekstura="TEXSteel_stick", szerokość_animowanej_tekstury=64, animowana_tekstura="ATXWater_drops_flowing", losowość=True, obecność_wody=True, zwróć_tekst=False):
    s = ""
    v = ["vt",4]
    if losowość:
        losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
    else:
        losowe_przesunięcie = 0
    v += [0,0,0,losowe_przesunięcie]
    v += [szerokość*odbicie,0,szerokość_tekstury*odbicie,losowe_przesunięcie]
    v += [szerokość*odbicie,-wysokość,szerokość_tekstury*odbicie,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    v += [0,-wysokość,0,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    s += podstawy.ściana(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    v = ["vt",4]
    if losowość:
        losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
    else:
        losowe_przesunięcie = 0
    v += [0,0,0,losowe_przesunięcie]
    v += [szerokość*odbicie,0,szerokość_tekstury*odbicie,losowe_przesunięcie]
    v += [szerokość*odbicie,-wysokość,szerokość_tekstury*odbicie,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    v += [0,-wysokość,0,-szerokość_tekstury*wysokość/szerokość+losowe_przesunięcie]
    s += podstawy.ściana(x=x+(rozstaw-szerokość)*odbicie,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if obecność_wody:
        v = ["vt",4]
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
        else:
            losowe_przesunięcie = 0
        v += [0,0,szerokość_animowanej_tekstury,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,0,0,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,-wysokość,0,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        v += [0,-wysokość,szerokość_animowanej_tekstury,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        s += podstawy.animowana_ściana(x=x-szerokość_animacji*odbicie,y=y,warstwa=warstwa,tekstura=animowana_tekstura,czas_klatki=czas_klatki,wierzchołki=v,zwróć_tekst=True)
        v = ["vt",4]
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*wysokość/szerokość
        else:
            losowe_przesunięcie = 0
        v += [0,0,-szerokość_animowanej_tekstury,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,0,0,losowe_przesunięcie]
        v += [szerokość_animacji*odbicie,-wysokość,0,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        v += [0,-wysokość,-szerokość_animowanej_tekstury,-szerokość_animowanej_tekstury*wysokość/szerokość_animacji+losowe_przesunięcie]
        s += podstawy.animowana_ściana(x=x+(rozstaw)*odbicie,y=y,warstwa=warstwa,tekstura=animowana_tekstura,czas_klatki=czas_klatki,wierzchołki=v,zwróć_tekst=True)
    n = (pow(pow(wysokość,2) + (pow(poprzeczki*rozstaw,2) + pow(wysokość,2)) * (pow(poprzeczki*rozstaw/szerokość_poprzeczki,2)-1),0.5)-wysokość) / (pow(poprzeczki*rozstaw/szerokość_poprzeczki,2)-1)
    dł = n*rozstaw/szerokość_poprzeczki
    gr = pow(pow(dł,2)-pow(rozstaw,2),0.5)
    odchylenie = pow(pow(n,2)-pow(szerokość_poprzeczki,2),0.5)
    for i in range(poprzeczki):
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
        else:
            losowe_przesunięcie = 0
        v = ["vt",4]
        v += [0,0,szerokość_tekstury,losowe_przesunięcie]
        v += [0,n,0,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
        v += [rozstaw*odbicie,n+gr,0,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
        v += [rozstaw*odbicie,gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
        s += podstawy.platforma(x=x,y=y-wysokość+i*gr,wierzchołki=v,jednostronna=True,zwróć_tekst=True)
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
        else:
            losowe_przesunięcie = 0
        v = ["vt",4]
        v += [0,n+gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
        v += [0,gr,0,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
        v += [rozstaw*odbicie,0,0,0+losowe_przesunięcie]
        v += [rozstaw*odbicie,n,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
        s += podstawy.platforma(x=x,y=y-wysokość+i*gr,wierzchołki=v,jednostronna=True,zwróć_tekst=True)
        if obecność_wody:
            przesunięcie = pow(pow(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr,2)+pow(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),2),0.5)
            if losowość:
                losowe_przesunięcie = random.random()*szerokość_animowanej_tekstury*dł/szerokość_animacji_poprzeczki
            else:
                losowe_przesunięcie = 0
            v = ["vt",4]
            v += [n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr,gr,szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*(dł+odchylenie-przesunięcie)+losowe_przesunięcie]
            v += [0,gr,0,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*dł+losowe_przesunięcie]
            v += [rozstaw*odbicie,0,0,0+losowe_przesunięcie]
            v += [rozstaw*odbicie,n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*odchylenie+losowe_przesunięcie]
            s += podstawy.animowana_ściana(x=x,y=y-wysokość+i*gr+n,warstwa=warstwa_poprzeczki,tekstura=animowana_tekstura,czas_klatki=czas_klatki_poprzeczki,wierzchołki=v,zwróć_tekst=True)
            if losowość:
                losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
            else:
                losowe_przesunięcie = 0
            v = ["vt",4]
            v += [0,0,0,losowe_przesunięcie]
            v += [0,n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki),szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*odchylenie+losowe_przesunięcie]
            v += [rozstaw*odbicie-(n*(szerokość_animacji_poprzeczki/szerokość_poprzeczki)*rozstaw*odbicie/gr),gr,szerokość_animowanej_tekstury,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*(dł+odchylenie-przesunięcie)+losowe_przesunięcie]
            v += [rozstaw*odbicie,gr,0,szerokość_animowanej_tekstury/szerokość_animacji_poprzeczki*dł+losowe_przesunięcie]
            s += podstawy.animowana_ściana(x=x,y=y-wysokość+i*gr+n,warstwa=warstwa_poprzeczki,tekstura=animowana_tekstura,czas_klatki=czas_klatki_poprzeczki,wierzchołki=v,zwróć_tekst=True)
        if losowość:
            losowe_przesunięcie = random.random()*szerokość_tekstury*dł/szerokość_poprzeczki
        else:
            losowe_przesunięcie = 0
        v = ["vt",5]
        v += [0,0,szerokość_tekstury,losowe_przesunięcie]
        v += [0,n,0,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
        v += [rozstaw*odbicie*(gr-n)/2/gr,(gr+n)/2,0,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*(gr-n)/2/gr+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,(gr+n)/2,szerokość_tekstury/2,szerokość_tekstury/szerokość_poprzeczki*dł/2+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,gr/2,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*gr/2+losowe_przesunięcie]
        s += podstawy.ściana(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        v = ["vt",5]
        v += [0,gr,0,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
        v += [0,n+gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,n+gr/2,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*(n+gr/2)+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,(gr+n)/2,szerokość_tekstury/2,szerokość_tekstury/szerokość_poprzeczki*dł/2+losowe_przesunięcie]
        v += [rozstaw*odbicie*(gr-n)/2/gr,(gr+n)/2,0,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*(gr-n)/2/gr+losowe_przesunięcie]
        s += podstawy.ściana(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        v = ["vt",5]
        v += [rozstaw*odbicie,n+gr,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)+losowe_przesunięcie]
        v += [rozstaw*odbicie,gr,0,szerokość_tekstury/szerokość_poprzeczki*dł+losowe_przesunięcie]
        v += [rozstaw*odbicie*(1-(gr-n)/2/gr),(gr+n)/2,0,szerokość_tekstury/szerokość_poprzeczki*dł*(1-(gr-n)/2/gr)+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,(gr+n)/2,szerokość_tekstury/2,szerokość_tekstury/szerokość_poprzeczki*dł/2+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,n+gr/2,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*(n+gr/2)+losowe_przesunięcie]
        s += podstawy.ściana(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
        v = ["vt",5]
        v += [rozstaw*odbicie,n,0,szerokość_tekstury/szerokość_poprzeczki*odchylenie+losowe_przesunięcie]
        v += [rozstaw*odbicie,0,szerokość_tekstury,0+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,gr/2,szerokość_tekstury,szerokość_tekstury/szerokość_poprzeczki*(dł+odchylenie)*gr/2+losowe_przesunięcie]
        v += [rozstaw*odbicie/2,(gr+n)/2,szerokość_tekstury/2,szerokość_tekstury/szerokość_poprzeczki*dł/2+losowe_przesunięcie]
        v += [rozstaw*odbicie*(1-(gr-n)/2/gr),(gr+n)/2,0,szerokość_tekstury/szerokość_poprzeczki*dł*(1-(gr-n)/2/gr)+losowe_przesunięcie]
        s += podstawy.ściana(x=x,y=y-wysokość+i*gr,warstwa=warstwa_poprzeczki,tekstura=tekstura,wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)