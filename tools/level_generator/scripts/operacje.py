import podstawy
import math
def element_potomny(element, typ_nadrzędnego="punkt", nadrzędny=[0,0], zwróć_tekst=False):
    s = ""
    tablica_elementów = podstawy.podział_na_elementy(element)
    for i in range(tablica_elementów[0]):
        e = podstawy.odczyt_elementu(tablica_elementów[i+1])
        if typ_nadrzędnego == "punkt":
            e[1] += nadrzędny[0]
            e[2] += nadrzędny[1]
            e_txt = podstawy.zapis_elementu(e)
        if typ_nadrzędnego == "ruch":
            e_txt = dodanie_ruchu(podstawy.zapis_elementu(e,True),nadrzędny,True)
        if typ_nadrzędnego == "ruch punktowy":
            e[1] += nadrzędny[0]
            e[2] += nadrzędny[1]
            e_txt = dodanie_ruchu(podstawy.zapis_elementu(e,True),nadrzędny[2:],True)
        s += e_txt
    if zwróć_tekst:
        return s
    else:
        podstawy.zapis(s)
def dodanie_ruchu(obiekt, ścieżka_ruchu, zwróć_tekst=False):
    zwrot = ""
    tablica_elementów = podstawy.podział_na_elementy(obiekt)
    for j in range(tablica_elementów[0]):
        element = podstawy.odczyt_elementu(tablica_elementów[j+1])
        n = podstawy.indeks_ruchu_elementu(element)
        if n<0:
            n = -n
            element = ["moving_" + element[0],] + element[1:n] + ścieżka_ruchu + element[n:]
        else:
            if element[n] == "container" or ścieżka_ruchu[0] == "container":
                if element[n] == "container" and ścieżka_ruchu[0] == "container":
                    idx1 = n+2
                    for i in range(element[n+1]-1):
                        idx1 += element[idx1+1]*podstawy.znaczniki(element[idx1])+2
                    element = element[0:n+1] + [element[n+1]+ścieżka_ruchu[1]] + element[n+2:idx1] + ścieżka_ruchu[2:] + element[idx1:]
                else:
                    if element[n] == "container":
                        idx1 = n+2
                        for i in range(element[n+1]-1):
                            idx1 += element[idx1+1]*podstawy.znaczniki(element[idx1])+2
                        element = element[0:n+1] + [element[n+1]+1] + element[n+2:idx1] + ścieżka_ruchu + element[idx1:]
                    else:
                        element = element[0:n] + [ścieżka_ruchu[0],ścieżka_ruchu[1]+1] + ścieżka_ruchu[2:] + element[n:]
            else:
                element = element[0:n] + ["container",2] + ścieżka_ruchu + element[n:]
        zwrot = zwrot + podstawy.zapis_elementu(e=element,zwróć_tekst=True)
    if zwróć_tekst:
        return zwrot
    else:
        podstawy.zapis(zwrot)