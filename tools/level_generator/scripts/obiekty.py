import math
global lvl
def wielokąt_foremny(n=6, bok=1, obrót=0, początek=0, przesunięcie_x=0, przesunięcie_y=0, pomijanie=0, pominięte=(0,1)):
    s = ""
    alfa = 180*(1-(n-2)/n)
    kierunek = obrót+180
    punkt_x = przesunięcie_x
    punkt_y = przesunięcie_y
    if(początek<2):
        punkt_x = punkt_x+math.cos(math.radians(kierunek))*(bok/2)
        punkt_y = punkt_y+math.sin(math.radians(kierunek))*(bok/2)
    if(początek<1):
        punkt_x = przesunięcie_x + math.sin(math.radians(obrót))
        punkt_y = przesunięcie_y + math.cos(math.radians(obrót))
    for i in range(n):
        if pomijanie==0 or (pomijanie==1 and pominięte[i]==1):
            s = s + """
                <v>{0},{1}</v>""".format(punkt_x, punkt_y)
        kierunek = kierunek+alfa
        punkt_x = punkt_x+math.cos(math.radians(kierunek))*bok
        punkt_y = punkt_y+math.sin(math.radians(kierunek))*bok
    return s