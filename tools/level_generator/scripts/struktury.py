import podstawy
import random
import math
def schody(x=0, y=0, warstwa=5, kolor=[255,255,255], obrót=1, długość=2, wysokość=2, przesunięcie=0, stopnie=4, tekstura=podstawy.domyślna_tekstura, blokada=True, zwróć_tekst=False):
    s = ""
    v = ["v",4]
    v += [0,0]
    if obrót==1:
        v += [0,-wysokość]
        v += [przesunięcie*obrót,-wysokość]
        v += [długość*obrót,0]
    else:
        v += [długość*obrót,0]
        v += [przesunięcie*obrót,-wysokość]
        v += [0,-wysokość]
    if blokada:
        s = s + podstawy.bariera(x=x,y=y,wierzchołki=v,zwróć_tekst=True)
    else:
        s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,jednostronna=True,zwróć_tekst=True)
    for i in range(stopnie):
        v = ["v",4,]
        v += [0,-wysokość/stopnie*i]
        v += [(długość-(długość-przesunięcie)/stopnie*i)*obrót,-wysokość/stopnie*i]
        v += [(długość-(długość-przesunięcie)/stopnie*i)*obrót,-wysokość/stopnie*(i+1)]
        v += [0,-wysokość/stopnie*(i+1)]
        s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,jednostronna=True,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def drabina_jednostronna(x=0, y=0, warstwa=5, kolor=[255,255,255], obrót=1, podpórka=1, grubość=0.5, długość=6, szczeble=3, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_podpórki=podstawy.domyślna_tekstura, tekstura=podstawy.domyślna_tekstura, zwróć_tekst=False):
    s = ""
    if podpórka == 1:
        v = ["v",4]
        v += [0,0]
        v += [grubość*obrót,0]
        v += [grubość*obrót,długość]
        v += [0,długość]
        s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura_podpórki,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if szczeble >= 2:
        v = ["v",4]
        v += [0,0]
        v += [głębokość*obrót,0]
        v += [głębokość*obrót,wysokość]
        v += [0,wysokość]
        for i in range(szczeble):
            s = s + podstawy.platforma(x=x+grubość*podpórka*obrót,y=y+górny_margines+(długość-górny_margines-dolny_margines-wysokość)/(szczeble-1)*i,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def drabina_dwustronna(x=0, y=0, warstwa=5, kolor=[255,255,255], naprzemienność=1, początek=0, lewe_przycięcie=0, prawe_przycięcie=0, lewa_podpórka=1, prawa_podpórka=1, grubość_lewej=0.5, grubość_prawej=0.5, długość=6, szczeble=3, rozpiętość=2, górny_margines=0, dolny_margines=0, głębokość=0.5, wysokość=0.5, tekstura_lewej_podpórki=podstawy.domyślna_tekstura, tekstura_prawej_podpórki=podstawy.domyślna_tekstura, tekstura=podstawy.domyślna_tekstura, zwróć_tekst=False):
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
        s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura_lewej_podpórki,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
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
        s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura_prawej_podpórki,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if szczeble >= 2:
        v = ["v",4,0,0,głębokość,0,głębokość,wysokość,0,wysokość]
        for i in range(szczeble):
            s = s + podstawy.platforma(x=x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i)%2),y=y+górny_margines+delta*i,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
            if naprzemienność == 0:
                s = s + podstawy.platforma(x=x+grubość_lewej*lewa_podpórka+(rozpiętość-głębokość)*((początek+i+1)%2),y=y+górny_margines+delta*i,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def komnata(x=0, y=0, warstwa=5, warstwa_tła=3, kolor=[255,255,255], kolor_tła=[255,255,255], wysokość=10, szerokość=10, tekstura=podstawy.domyślna_tekstura, tekstura_tła=podstawy.domyślna_tekstura, grubość_górnej=0.5, grubość_dolnej=0.5, grubość_prawej=0.5, grubość_lewej=0.5, otwory_górne=[0,0], otwory_dolne=[0,0], otwory_prawe=[0,0], otwory_lewe=[0,0], tło=True, zwróć_tekst=False):
    s = ""
    v = ["v",4,0,0,grubość_lewej,0,grubość_lewej,grubość_górnej,0,grubość_górnej]
    s = s + podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_prawej,0,grubość_prawej,grubość_górnej,0,grubość_górnej]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_prawej,0,grubość_prawej,grubość_dolnej,0,grubość_dolnej]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+wysokość-grubość_dolnej,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    v = ["v",4,0,0,grubość_lewej,0,grubość_lewej,grubość_dolnej,0,grubość_dolnej]
    s = s + podstawy.platforma(x=x,y=y+wysokość-grubość_dolnej,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_lewej
    v_y = [0,0,grubość_górnej,grubość_górnej]
    for i in range(otwory_górne[0]):
        v_x = [0,otwory_górne[i*2+1]+grubość_lewej-miejsce,otwory_górne[i*2+1]+grubość_lewej-miejsce,0]
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+miejsce,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_górne[i*2+1]+otwory_górne[i*2+2]+grubość_lewej
    v_x = [0,szerokość-grubość_prawej-miejsce,szerokość-grubość_prawej-miejsce,0]
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+miejsce,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_lewej
    v_y = [0,0,grubość_dolnej,grubość_dolnej]
    for i in range(otwory_dolne[0]):
        v_x = [0,otwory_dolne[i*2+1]+grubość_lewej-miejsce,otwory_dolne[i*2+1]+grubość_lewej-miejsce,0]
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+miejsce,y=y+wysokość-grubość_dolnej,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_dolne[i*2+1]+otwory_dolne[i*2+2]+grubość_lewej
    v_x = [0,szerokość-grubość_prawej-miejsce,szerokość-grubość_prawej-miejsce,0]
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+miejsce,y=y+wysokość-grubość_dolnej,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_górnej
    v_x = [0,grubość_lewej,grubość_lewej,0]
    for i in range(otwory_lewe[0]):
        v_y = [0,0,otwory_lewe[i*2+1]+grubość_górnej-miejsce,otwory_lewe[i*2+1]+grubość_górnej-miejsce]
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x,y=y+miejsce,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_lewe[i*2+1]+otwory_lewe[i*2+2]+grubość_górnej
    v_y = [0,0,wysokość-grubość_dolnej-miejsce,wysokość-grubość_dolnej-miejsce]
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x,y=y+miejsce,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    miejsce = grubość_górnej
    v_x = [0,grubość_prawej,grubość_prawej,0]
    for i in range(otwory_prawe[0]):
        v_y = [0,0,otwory_prawe[i*2+1]+grubość_górnej-miejsce,otwory_prawe[i*2+1]+grubość_górnej-miejsce]
        v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
        s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+miejsce,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
        miejsce = otwory_prawe[i*2+1]+otwory_prawe[i*2+2]+grubość_górnej
    v_y = [0,0,wysokość-grubość_dolnej-miejsce,wysokość-grubość_dolnej-miejsce]
    v = ["v",4,v_x[0],v_y[0],v_x[1],v_y[1],v_x[2],v_y[2],v_x[3],v_y[3]]
    s = s + podstawy.platforma(x=x+szerokość-grubość_prawej,y=y+miejsce,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if tło:
        s += podstawy.ściana(x=x,y=y,wierzchołki=["v",4,0,0,0,wysokość,szerokość,wysokość,szerokość,0],tekstura=tekstura_tła,warstwa=warstwa_tła,R=kolor_tła[0],G=kolor_tła[1],B=kolor_tła[2],zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def element_podłużny(x=0, y=0, warstwa=5, kolor=[255,255,255], powierzchnia=podstawy.domyślna_powierzchnia, grubość=0.5, tekstura=podstawy.domyślna_tekstura, szerokość_tekstury=64, trasa=[4,0,0,1,1,3,2,4,3], kolizja=False, jednostronność=False, parametr_losowości=1024, zwróć_tekst=False):
    s = ""
    m = random.random()*parametr_losowości
    P = []
    L = []
    pA = []
    pB = []
    pC = []
    zamien = False
    if (trasa[3]-trasa[1])>=0:
        if (trasa[3]-trasa[1])==0:
            if (trasa[4]-trasa[2])<0:
                strona = 1
            else:
                strona = -1
        else:
            strona = 1
    else:
        strona = -1
    if trasa[1]==trasa[3]:
        wsp = (trasa[1]-trasa[3])/(trasa[4]-trasa[2])
        n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
        P += [trasa[1]+n,trasa[2]+wsp*n]
        L += [trasa[1]-n,trasa[2]-wsp*n]
    else:
        wsp = (trasa[4]-trasa[2])/(trasa[1]-trasa[3])
        n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
        P += [trasa[1]+wsp*n,trasa[2]+n]
        L += [trasa[1]-wsp*n,trasa[2]-n]
    for i in range(trasa[0]-1):
        if trasa[2+i*2]==trasa[4+i*2]:
            pA += [0,]
            pB += [1,]
            pC += [-trasa[3+i*2],]
        else:
            wsp = (trasa[1+i*2]-trasa[3+i*2])/(trasa[4+i*2]-trasa[2+i*2])
            pA += [1,]
            pB += [wsp,]
            pC += [-wsp*trasa[2+i*2]-trasa[1+i*2],]
    for i in range(trasa[0]-1):
        if i == trasa[0]-2:
            if (trasa[i*2+3]-trasa[i*2+1])>=0:
                if (trasa[i*2+3]-trasa[i*2+1])==0:
                    if (trasa[i*2+4]-trasa[i*2+2])<0:
                        strona = 1
                    else:
                        strona = -1
                else:
                    strona = 1
            else:
                strona = -1
            if trasa[1+i*2]==trasa[3+i*2]:
                wsp = (trasa[1+i*2]-trasa[3+i*2])/(trasa[4+i*2]-trasa[2+i*2])
                n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
                if zamien:
                    L += [trasa[3+i*2]+n,trasa[4+i*2]+wsp*n]
                    P += [trasa[3+i*2]-n,trasa[4+i*2]-wsp*n]
                else:
                    P += [trasa[3+i*2]+n,trasa[4+i*2]+wsp*n]
                    L += [trasa[3+i*2]-n,trasa[4+i*2]-wsp*n]
            else:
                wsp = (trasa[4+i*2]-trasa[2+i*2])/(trasa[1+i*2]-trasa[3+i*2])
                n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
                if zamien:
                    L += [trasa[3+i*2]+wsp*n,trasa[4+i*2]+n]
                    P += [trasa[3+i*2]-wsp*n,trasa[4+i*2]-n]
                else:
                    P += [trasa[3+i*2]+wsp*n,trasa[4+i*2]+n]
                    L += [trasa[3+i*2]-wsp*n,trasa[4+i*2]-n]
        else:
            if (trasa[i*2+5]-trasa[i*2+3])>=0:
                if (trasa[i*2+5]-trasa[i*2+3])==0:
                    if (trasa[i*2+6]-trasa[i*2+4])<0:
                        strona = 1
                    else:
                        strona = -1
                else:
                    strona = 1
            else:
                strona = -1
            if trasa[3+i*2]==trasa[5+i*2]:
                wsp = (trasa[3+i*2]-trasa[5+i*2])/(trasa[6+i*2]-trasa[4+i*2])
                n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
                px = trasa[5+i*2]+n
                py = trasa[6+i*2]+wsp*n
                lx = trasa[5+i*2]-n
                ly = trasa[6+i*2]-wsp*n
            else:
                wsp = (trasa[6+i*2]-trasa[4+i*2])/(trasa[3+i*2]-trasa[5+i*2])
                n = grubość/(2*pow(1+pow(wsp,2),0.5))*strona
                px = trasa[5+i*2]+wsp*n
                py = trasa[6+i*2]+n
                lx = trasa[5+i*2]-wsp*n
                ly = trasa[6+i*2]-n
            kA = pA[i] #prawa strona
            kB = pB[i]
            kC = -kA*P[i*2]-kB*P[i*2+1]
            lA = pA[i+1]
            lB = pB[i+1]
            lC = -lA*px-lB*py
            if lA*kB==kA*lB:
                P += [(P[i*2]+px)/2,(P[i*2+1]+py)/2]
            else:
                P += [(lB*kC-kB*lC)/(lA*kB-kA*lB),]
                if kB==0:
                    P += [-(lA*P[i*2+2]+lC)/lB,]
                else:
                    P += [-(kA*P[i*2+2]+kC)/kB,]
            kA = pA[i] #lewa strona
            kB = pB[i]
            kC = -kA*L[i*2]-kB*L[i*2+1]
            lA = pA[i+1]
            lB = pB[i+1]
            lC = -lA*lx-lB*ly
            if lA*kB==kA*lB:
                L += [(L[i*2]+lx)/2,(L[i*2+1]+ly)/2]
            else:
                L += [(lB*kC-kB*lC)/(lA*kB-kA*lB),]
                if kB==0:
                    L += [-(lA*L[i*2+2]+lC)/lB,]
                else:
                    L += [-(kA*L[i*2+2]+kC)/kB,]
        dP = pow(pow(P[i*2]-P[i*2+2],2)+pow(P[i*2+1]-P[i*2+3],2),0.5)
        dL = pow(pow(L[i*2]-L[i*2+2],2)+pow(L[i*2+1]-L[i*2+3],2),0.5)
        n = max(dP,dL)/grubość*szerokość_tekstury
        v = ["vt",4,P[i*2],P[i*2+1],szerokość_tekstury,m,P[i*2+2],P[i*2+3],szerokość_tekstury,m+n,L[i*2+2],L[i*2+3],0,m+n,L[i*2],L[i*2+1],0,m]
        m += n
        if kolizja:
            s += podstawy.platforma(x=x,y=y,warstwa=warstwa,tekstura=tekstura,wierzchołki=v,jednostronna=jednostronność,R=kolor[0],G=kolor[1],B=kolor[2],zwróć_tekst=True)
        else:
            s += podstawy.ściana(x=x,y=y,warstwa=warstwa,tekstura=tekstura,R=kolor[0],G=kolor[1],B=kolor[2],wierzchołki=v,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
