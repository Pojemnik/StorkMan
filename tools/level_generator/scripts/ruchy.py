import math
import obiekty

def wstrząsy(wychylenie=0.25, wielokąt=5, obrót=0, czas_wychylenia=0.1):
    alfa = 180*(1-(wielokąt-2)/wielokąt)
    bok = math.tan(alfa/2)*wychylenie*2
    v = obiekty.wielokąt_foremny(n=wielokąt,bok=bok,obrót=obrót)
    v = v[2:]
    x = []
    y = []
    for i in range(wielokąt):
        x += [v[i*2],]
        y += [v[i*2+1],]
    p = ["linear",wielokąt]
    przeskok = round(wielokąt/2)+1
    for i in range(wielokąt):
        p += [x[(i*przeskok)%wielokąt],y[(i*przeskok)%wielokąt],czas_wychylenia]
    return p