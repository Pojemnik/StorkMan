import podstawy
import operacje
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