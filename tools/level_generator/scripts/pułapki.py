import podstawy
import operacje
import math
def spadający_obiekt(x=0, y=0, prędkość_początkowa=0, przyspieszenie=0.02, odstęp=180, spad=10, wysokość=2, strefa=["v",4,-0.5,0,-0.5,-2,0.5,-2,0.5,0], czas_obrażeń=60, szczyt_obrażeń=15, obrażenia=100, rozdrobnienie=10, obiekt=podstawy.ruchoma_platforma(zwróć_tekst=True), uderzenie=podstawy.animowany_ruchomy_obiekt(zwróć_tekst=True), zwróć_tekst=False):
    s = ""
    prędkość_końcowa = prędkość_początkowa + (pow(4*pow(prędkość_początkowa,2)+8*(spad-wysokość)*przyspieszenie,0.5) - 2*prędkość_początkowa)/2
    prędkość = (prędkość_końcowa - prędkość_początkowa) / 2
    czas1 = round(((spad-wysokość)/prędkość))
    czas2 = round((wysokość/prędkość))
    czas3 = round(czas_obrażeń-czas2)
    czas4 = round(odstęp-czas3)
    czas_rozdrobnienia = (czas_obrażeń/rozdrobnienie)
    d_d = [0,]
    d_t = [czas1,]
    for i in range(rozdrobnienie):
        d_t += [round((i+1)*czas_rozdrobnienia)-round(i*czas_rozdrobnienia),]
        if czas_rozdrobnienia*(i+0.5) < szczyt_obrażeń:
            d_d += [round(((czas_rozdrobnienia*(i+0.5))/szczyt_obrażeń)*obrażenia),]
        else:
            d_d += [round((czas_obrażeń-(czas_rozdrobnienia*(i+0.5)))/(czas_obrażeń-szczyt_obrażeń)*obrażenia),]
    d_d += [0,]
    d_t += [czas4,]
    s = s + podstawy.strefa_obrażeń(x=x,y=y,wierzchołki=strefa,stany=rozdrobnienie+2,stany_d=d_d,stany_t=d_t,zwróć_tekst=True)
    ruch = [x,y,"accelerated",3,0,-spad,czas1,przyspieszenie,0,-wysokość,czas2+czas3+czas4,0,-1,-1,-1,-1]
    s = s + operacje.element_potomny(obiekt,"ruch punktowy",ruch,True)
    ruch = [x,y,"linear",5,0,0,czas1,-1,-1,-1,0,0,czas2+czas3,0,0,czas4,-1,-1,-1]
    s = s + operacje.element_potomny(uderzenie,"ruch punktowy",ruch,True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def emiter(x=0, y=0, warstwa=3, kolor=[255,255,255], obrót=0, kąt_emisji=30, promień_początkowy=0.2, długość=4, zakończenie=0, intensywność=3, czas_klatki=0.5, obrażenia=15, ponawianie_obrażeń=6, długość_tekstury=512, szerokość_tekstury=512, początek_emisji=podstawy.animowany_obiekt(x=-0.5,y=-0.5,wysokość=1,tekstura="AOBSteam_Cloud_Concentrated",zwróć_tekst=True), koniec_emisji=podstawy.animowany_obiekt(x=-1.5,y=-1.5,wysokość=3,tekstura="AOBSteam_Cloud_Concentrated",zwróć_tekst=True), tekstura_emisji="ATXSteam_Clouds_Concentrated", zwróć_tekst=False):
    s = ""
    n = długość/math.cos(math.radians(kąt_emisji/2))
    r = n*math.sin(math.radians(kąt_emisji/2))
    przesunięcie = długość_tekstury/intensywność
    for i in range(intensywność):
        v = ["vt",4]
        v += [math.cos(math.radians(obrót-90))*promień_początkowy,math.sin(math.radians(obrót-90))*promień_początkowy,szerokość_tekstury,przesunięcie*i+(r+promień_początkowy)/długość*szerokość_tekstury*2]
        v += [math.cos(math.radians(obrót-90))*promień_początkowy+math.cos(math.radians(obrót-kąt_emisji/2))*n,math.sin(math.radians(obrót-90))*promień_początkowy+math.sin(math.radians(obrót-kąt_emisji/2))*n,szerokość_tekstury,przesunięcie*i]
        v += [math.cos(math.radians(obrót))*długość,math.sin(math.radians(obrót))*długość,szerokość_tekstury/2,przesunięcie*i]
        v += [0,0,szerokość_tekstury/2,przesunięcie*i+(r+promień_początkowy)/długość*szerokość_tekstury*2]
        s += podstawy.animowana_ściana(x=x,y=y,warstwa=warstwa,R=kolor[0],G=kolor[1],B=kolor[2],tekstura=tekstura_emisji,wierzchołki=v,czas_klatki=czas_klatki,zwróć_tekst=True)
        v = ["vt",3]
        v += [math.cos(math.radians(obrót-90))*promień_początkowy+math.cos(math.radians(obrót-kąt_emisji/2))*n,math.sin(math.radians(obrót-90))*promień_początkowy+math.sin(math.radians(obrót-kąt_emisji/2))*n,szerokość_tekstury,przesunięcie*i]
        v += [math.cos(math.radians(obrót))*(długość+zakończenie),math.sin(math.radians(obrót))*(długość+zakończenie),szerokość_tekstury,przesunięcie*i-zakończenie/długość*szerokość_tekstury*2]
        v += [math.cos(math.radians(obrót))*długość,math.sin(math.radians(obrót))*długość,szerokość_tekstury/2,przesunięcie*i]
        s += podstawy.animowana_ściana(x=x,y=y,warstwa=warstwa,R=kolor[0],G=kolor[1],B=kolor[2],tekstura=tekstura_emisji,wierzchołki=v,czas_klatki=czas_klatki,zwróć_tekst=True)
        v = ["vt",4]
        v += [math.cos(math.radians(obrót+90))*promień_początkowy,math.sin(math.radians(obrót+90))*promień_początkowy,0,przesunięcie*i+(r+promień_początkowy)/długość*szerokość_tekstury*2]
        v += [math.cos(math.radians(obrót+90))*promień_początkowy+math.cos(math.radians(obrót+kąt_emisji/2))*n,math.sin(math.radians(obrót+90))*promień_początkowy+math.sin(math.radians(obrót+kąt_emisji/2))*n,0,przesunięcie*i]
        v += [math.cos(math.radians(obrót))*długość,math.sin(math.radians(obrót))*długość,szerokość_tekstury/2,przesunięcie*i]
        v += [0,0,szerokość_tekstury/2,przesunięcie*i+(r+promień_początkowy)/długość*szerokość_tekstury*2]
        s += podstawy.animowana_ściana(x=x,y=y,warstwa=warstwa,R=kolor[0],G=kolor[1],B=kolor[2],tekstura=tekstura_emisji,wierzchołki=v,czas_klatki=czas_klatki,zwróć_tekst=True)
        v = ["vt",3]
        v += [math.cos(math.radians(obrót+90))*promień_początkowy+math.cos(math.radians(obrót+kąt_emisji/2))*n,math.sin(math.radians(obrót+90))*promień_początkowy+math.sin(math.radians(obrót+kąt_emisji/2))*n,0,przesunięcie*i]
        v += [math.cos(math.radians(obrót))*(długość+zakończenie),math.sin(math.radians(obrót))*(długość+zakończenie),0,przesunięcie*i-zakończenie/długość*szerokość_tekstury*2]
        v += [math.cos(math.radians(obrót))*długość,math.sin(math.radians(obrót))*długość,szerokość_tekstury/2,przesunięcie*i]
        s += podstawy.animowana_ściana(x=x,y=y,warstwa=warstwa,R=kolor[0],G=kolor[1],B=kolor[2],tekstura=tekstura_emisji,wierzchołki=v,czas_klatki=czas_klatki,zwróć_tekst=True)
        v = ["v",4]
        v += [math.cos(math.radians(obrót+90))*promień_początkowy,math.sin(math.radians(obrót+90))*promień_początkowy]
        v += [math.cos(math.radians(obrót+90))*promień_początkowy+math.cos(math.radians(obrót+kąt_emisji/2))*n,math.sin(math.radians(obrót+90))*promień_początkowy+math.sin(math.radians(obrót+kąt_emisji/2))*n]
        v += [math.cos(math.radians(obrót-90))*promień_początkowy+math.cos(math.radians(obrót-kąt_emisji/2))*n,math.sin(math.radians(obrót-90))*promień_początkowy+math.sin(math.radians(obrót-kąt_emisji/2))*n]
        v += [math.cos(math.radians(obrót-90))*promień_początkowy,math.sin(math.radians(obrót-90))*promień_początkowy]
        d = ["d",2,obrażenia,ponawianie_obrażeń,0,0]
        s += podstawy.strefa_obrażeń(x=x,y=y,wierzchołki=v,obrażenia=d,zwróć_tekst=True)
    s += operacje.element_potomny(początek_emisji,"punkt",[x,y],True)
    s += operacje.element_potomny(koniec_emisji,"punkt",[x+math.cos(math.radians(obrót))*długość,y+math.sin(math.radians(obrót))*długość],True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
