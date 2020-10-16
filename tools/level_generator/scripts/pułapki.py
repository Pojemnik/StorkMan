import podstawy
global lvl
def spadający_obiekt(x=0, y=0, prędkość=8, odstęp=180, spad=10, wysokość=2, rozmiar=1, strefa=(4,0,1,1,0,0,0,-2,-2), czas_obrażeń=60, szczyt_obrażeń=15, obrażenia=100, rozdrobnienie=10, obiekt=(4,0,1,1,0,0,0,1,1), tekstura=podstawy.domyślny_obiekt, animacja=podstawy.domyślny_animowany_obiekt, zwróć_tekst=False):
    s = ""
    czas1 = round(((spad-wysokość)/prędkość)*60)
    czas2 = round((wysokość/prędkość)*60)
    czas3 = round(czas_obrażeń-czas2)
    czas4 = round(odstęp-czas3)
    czas_rozdrobnienia = (czas_obrażeń/rozdrobnienie)
    v_x = strefa[1:strefa[0]+1]
    v_y = strefa[strefa[0]+1:2*strefa[0]+1]
    d_d = (0,)
    d_t = (czas1,)
    for i in range(rozdrobnienie):
        d_t = d_t + (round((i+1)*czas_rozdrobnienia)-round(i*czas_rozdrobnienia),)
        if czas_rozdrobnienia*(i+0.5) < szczyt_obrażeń:
            d_d = d_d + (round(((czas_rozdrobnienia*(i+0.5))/szczyt_obrażeń)*obrażenia),)
        else:
            d_d = d_d + (round((czas_obrażeń-(czas_rozdrobnienia*(i+0.5)))/(czas_obrażeń-szczyt_obrażeń)*obrażenia),)
    d_d = d_d + (0,)
    d_t = d_t + (czas4,)
    s = s + podstawy.strefa_obrażeń(x=x,y=y,wierzchołki=strefa[0],wierzchołki_x=v_x,wierzchołki_y=v_y,stany=rozdrobnienie+2,stany_d=d_d,stany_t=d_t,zwróć_tekst=True)
    v_x = obiekt[1:obiekt[0]+1]
    v_y = obiekt[obiekt[0]+1:2*obiekt[0]+1]
    p_x = (0,0,0)
    p_y = (-spad,-wysokość,-spad)
    p_t = (czas1,0,czas2+czas3+czas4)
    s = s + podstawy.ruchoma_platforma(x=x,y=y-rozmiar,wierzchołki=obiekt[0],wierzchołki_x=v_x,wierzchołki_y=v_y,znaczniki=3,znaczniki_x=p_x,znaczniki_y=p_y,znaczniki_t=p_t,zwróć_tekst=True)
    p_x = (0,0,0)
    p_y = (-spad,0,-spad)
    p_t = (czas1+czas2,0,czas3+czas4)
    s = s + podstawy.ruchomy_obiekt(x=x,y=y-rozmiar,tekstura=tekstura,wysokość=rozmiar,znaczniki=3,znaczniki_x=p_x,znaczniki_y=p_y,znaczniki_t=p_t,zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)