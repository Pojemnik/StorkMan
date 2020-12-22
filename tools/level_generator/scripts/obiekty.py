import math
def wielokąt_foremny(n=6, bok=1, obrót=0, początek=0, przesunięcie_x=0, przesunięcie_y=0, pomijanie=0, pominięte=(0,1)):
    s = ["v"]
    l = 0
    alfa = 180*(1-(n-2)/n)
    r = 1/math.tan(math.radians(alfa/2))*bok/2
    kierunek = obrót+180
    punkt_x = przesunięcie_x
    punkt_y = przesunięcie_y
    if(początek<2):
        punkt_x += math.cos(math.radians(-kierunek))*(bok/2)
        punkt_y += math.sin(math.radians(-kierunek))*(bok/2)
    if(początek<1):
        punkt_x += math.cos(math.radians(-kierunek-90))*r
        punkt_y += math.sin(math.radians(-kierunek-90))*r
    for i in range(n):
        if pomijanie==0 or (pomijanie==1 and pominięte[i]==1):
            s += [punkt_x,punkt_y]
            l += 1
        kierunek = kierunek+alfa
        punkt_x = punkt_x+math.cos(math.radians(kierunek))*bok
        punkt_y = punkt_y+math.sin(math.radians(kierunek))*bok
    s = [s[0],l] + s[1:]
    return s