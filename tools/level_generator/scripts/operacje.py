import podstawy
import math
global lvl
def dodanie_ruchu(obiekt, ścieżka_ruchu, zwróć_tekst=False):
    z = ""
    t = podstawy.podział_na_elementy(obiekt)
    for i in range(t[0]):
        r = podstawy.odczyt_elementu(t[i+1])
        n = podstawy.indeks_ruchu_elementu(r)
        if n<0:
            n = -n
            r = ["moving_" + r[0],] + r[1:]
            r = r[:n] + [0,] + r[n:]
        r_s = []
        cykl = True
        e_t_max = 0
        s_t_max = 0
        for i in range(r[n]):
            e_t_max = e_t_max + r[n+(i+1)*3]
        for i in range(ścieżka_ruchu[0]):
            s_t_max = s_t_max + ścieżka_ruchu[(i+1)*3]
        e_t = e_t_max
        s_t = s_t_max
        e = (2,r[n+1],r[n+2],r[n+4],r[n+5],r[n+3],r[n+3],r[n+6]) # numer klatki, startowe x, startowe y, końcowe x, końcowe y, czas, pozostały czas
        s = (2,ścieżka_ruchu[1],ścieżka_ruchu[2],ścieżka_ruchu[4],ścieżka_ruchu[5],ścieżka_ruchu[3],ścieżka_ruchu[3],ścieżka_ruchu[6])
        if r[n] == 0:
            r_s = ścieżka_ruchu
        else:
            k = 0
            while cykl or s[0]!=2 or e[0]!=2:
                if e[6] == s[6]:
                    r_s = r_s + [s[3]+e[3],]
                    r_s = r_s + [s[4]+e[4],]
                    e_t = e_t - e[6]
                    s_t = s_t - s[6]
                    if s[0] >= ścieżka_ruchu[0]:
                        s = (1,s[3],s[4],ścieżka_ruchu[1],ścieżka_ruchu[2],s[7],s[7],ścieżka_ruchu[3])
                    else:
                        s = (s[0]+1,s[3],s[4],ścieżka_ruchu[(s[0]+1)*3-2],ścieżka_ruchu[(s[0]+1)*3-1],s[7],s[7],ścieżka_ruchu[(s[0]+1)*3])
                    if e[0] >= r[n]:
                        e = (1,e[3],e[4],r[n+1],r[n+2],e[7],e[7],r[n+3])
                    else:
                        e = (e[0]+1,e[3],e[4],r[n-2+(e[0]+1)*3],r[n-1+(e[0]+1)*3],e[7],e[7],r[n+(e[0]+1)*3])
                    r_s = r_s + [min(s[6],e[6]),]
                else:
                    if e[6] > s[6]:
                        e = e[:6] + (e[6] - s[6],) + (e[7],)
                        r_s = r_s + [s[3]+e[3]+(e[1]-e[3])*(e[6]/e[5]),]
                        r_s = r_s + [s[4]+e[4]+(e[2]-e[4])*(e[6]/e[5]),]
                        e_t = e_t - s[6]
                        s_t = s_t - s[6]
                        if s[0] >= ścieżka_ruchu[0]:
                            s = (1,s[3],s[4],ścieżka_ruchu[1],ścieżka_ruchu[2],s[7],s[7],ścieżka_ruchu[3])
                        else:
                            s = (s[0]+1,s[3],s[4],ścieżka_ruchu[(s[0]+1)*3-2],ścieżka_ruchu[(s[0]+1)*3-1],s[7],s[7],ścieżka_ruchu[(s[0]+1)*3])
                        r_s = r_s + [min(s[6],e[6]),]
                    else:
                        s = s[:6] + (s[6] - e[6],) + (s[7],)
                        r_s = r_s + [e[3]+s[3]+(s[1]-s[3])*(s[6]/s[5]),]
                        r_s = r_s + [e[4]+s[4]+(s[2]-s[4])*(s[6]/s[5]),]
                        e_t = e_t - e[6]
                        s_t = s_t - e[6]
                        if e[0] >= r[n]:
                            e = (1,e[3],e[4],r[n+1],r[n+2],e[7],e[7],r[n+3])
                        else:
                            e = (e[0]+1,e[3],e[4],r[n-2+(e[0]+1)*3],r[n-1+(e[0]+1)*3],e[7],e[7],r[n+(e[0]+1)*3])
                        r_s = r_s + [min(s[6],e[6]),]
                if e_t == 0 or s_t == 0:
                    if e_t == 0 and s_t == 0:
                        cykl = False
                    if e_t == 0:
                        e_t = e_t_max
                    if s_t == 0:
                        s_t = s_t_max
                k = k + 1
            r_s = [k,] + r_s[(k-1)*3:] + r_s[:(k-1)*3]
        r = r[:n] + r_s + r[n+1+3*r[n]:]
        z = z + podstawy.zapis_elementu(e=r,zwróć_tekst=True)
    if zwróć_tekst:
        return z
    else:
        podstawy.zapis(z)