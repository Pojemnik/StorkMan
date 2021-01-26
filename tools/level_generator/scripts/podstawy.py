import math

global lvl
global domyślna_tekstura
global domyślna_animowana_tekstura
global domyślny_obiekt
global domyślny_animowany_obiekt
global domyślna_lina
global domyślna_powierzchnia

domyślna_tekstura = "bricks,0"
domyślna_animowana_tekstura = "rain45"
domyślny_obiekt = "chest0"
domyślny_animowany_obiekt = "wind0"
domyślna_lina = "rope2"
domyślna_powierzchnia = "none"
domyślny_dźwięk = "coal_grinder"

typy_platforma = [8,"position","texture","layer","rotation","flip","surface","one_sided","color"]
domyślne_wartości_platforma = [None,None,None,5,0,1,1,"none","false",255,255,255]
typy_obiekt = [8,"position","texture","height","rotation","flip","layer","color","paralax"]
domyślne_wartości_obiekt = [None,None,None,None,0,1,1,3,255,255,255,0]
typy_animowany_obiekt = [10,"position","texture","height","rotation","flip","layer","frame_time","offset","color","paralax"]
domyślne_wartości_animowany_obiekt = [None,None,None,None,0,1,1,3,1,0,255,255,255,0]
typy_wahadło = [11,"position","texture","line","length","angle","layer","rotation","flip","line_offset","surface","color"]
domyślne_wartości_wahadło = [None,None,None,None,None,5,0,1,1,None,"none",255,255,255]
typy_ruchoma_platforma = [7,"position","texture","layer","rotation","flip","surface","color"]
domyślne_wartości_ruchoma_platforma = [None,None,None,5,0,1,1,"none",255,255,255]
typy_ściana = [6,"position","texture","layer","flip","rotation","color"]
domyślne_wartości_ściana = [None,None,None,1,1,1,0,255,255,255]
typy_animowana_ściana = [8,"position","texture","layer","frame_time","offset","flip","rotation","color"]
domyślne_wartości_animowana_ściana = [None,None,None,1,1,0,1,1,0,255,255,255]
typy_animowana_platforma = [9,"position","texture","layer","rotation","flip","offset","frame_time","surface","color"]
domyślne_wartości_animowana_platforma = [None,None,None,5,0,1,1,0,1,"none",255,255,255]
typy_strefa_obrażeń = [1,"position"]
domyślne_wartości_strefa_obrażeń = [None,None]
typy_bariera = [2,"position","surface"]
domyślne_wartości_bariera = [None,None,"none"]
typy_dźwięk = [6,"position","volume","sound","attenuation","min_distance","range"]
domyślne_wartości_dźwięk = [None,None,100,None,1,1,8]
typy_strefa_zdarzeń = [2,"position","player_only"]
domyślne_wartości_strefa_zdarzeń = [None,None,False]
typy_wyzwalacz_stanu = [2,"state","trigger"]
domyślne_wartości_wyzwalacz_stanu = [None,None]
typy_czasowe_zdarzenie = [4,"start","stop","event","time"]
domyślne_wartości_czasowe_zdarzenie = [None,None,None,None]
typy_zdarzenie = [1,"name"]
domyślne_wartości_zdarzenie = [None]

def znaczniki(nazwa):
    if nazwa == "accelerated":
        return 4
    if nazwa == "linear":
        return 3
    if nazwa == "rotational":
        return 2
    if nazwa == "v":
        return 2
    if nazwa == "vt":
        return 4
    if nazwa == "d":
        return 2
    if nazwa == "e":
        return 1
    if nazwa == "et":
        return 2
    return 0
def podział_na_elementy(s):
    r = []
    i = 0
    k = 0
    while k>=0:
        p = s.find("<",k)
        k = s.find(" ",p)
        if k>=0:
            if s.find("state>",p,k)==-1 and s.find("/state>",p,k)==-1:
                a = p
                t = s[p+1:k]
                p = s.find("/",k)
                k = s.find(">",k)
                if k<p:
                    k = s.find(t,k)
                    k = s.find(">",k)
                b = k
                r = r + [s[a:b+1],]
            else:
                k = s.find("\n",p)
                if s.find("/state>",p,k)==-1:
                    r += ["<state>"]
                else:
                    r += ["</state>"]
            i = i + 1
    r = [i,] + r
    return r
def przypisanie_tabeli_wartości(typ):
    if typ=="platform":
        return domyślne_wartości_platforma,typy_platforma
    if typ=="object":
        return domyślne_wartości_obiekt,typy_obiekt
    if typ=="animated_object":
        return domyślne_wartości_animowany_obiekt,typy_animowany_obiekt
    if typ=="moving_object":
        return domyślne_wartości_obiekt,typy_obiekt
    if typ=="pendulum":
        return domyślne_wartości_wahadło,typy_wahadło
    if typ=="moving_platform":
        return domyślne_wartości_ruchoma_platforma,typy_ruchoma_platforma
    if typ=="wall":
        return domyślne_wartości_ściana,typy_ściana
    if typ=="animated_wall":
        return domyślne_wartości_animowana_ściana,typy_animowana_ściana
    if typ=="animated_moving_platform" or typ=="moving_animated_platform":
        return domyślne_wartości_animowana_platforma,typy_animowana_platforma
    if typ=="damage_zone":
        return domyślne_wartości_strefa_obrażeń,typy_strefa_obrażeń
    if typ=="barrier":
        return domyślne_wartości_bariera,typy_bariera
    if typ=="moving_damage_zone":
        return domyślne_wartości_strefa_obrażeń,typy_strefa_obrażeń
    if typ=="moving_barrier":
        return domyślne_wartości_bariera,typy_bariera
    if typ=="animated_moving_object" or typ=="moving_animated_object":
        return domyślne_wartości_animowany_obiekt,typy_animowany_obiekt
    if typ=="sound":
        return domyślne_wartości_dźwięk,typy_dźwięk
    if typ=="moving_wall":
        return domyślne_wartości_ściana,typy_ściana
    if typ=="animated_moving_wall" or typ=="moving_animated_wall":
        return domyślne_wartości_animowana_ściana,typy_animowana_ściana
    if typ=="event_zone":
        return domyślne_wartości_strefa_zdarzeń,typy_strefa_zdarzeń
    if typ=="clickable_zone":
        return domyślne_wartości_strefa_obrażeń,typy_strefa_obrażeń
    if typ=="state_trigger":
        return domyślne_wartości_wyzwalacz_stanu,typy_wyzwalacz_stanu
    if typ=="timed_event":
        return domyślne_wartości_czasowe_zdarzenie,typy_czasowe_zdarzenie
    if typ=="event":
        return domyślne_wartości_zdarzenie,typy_zdarzenie
    return [],[]
def ilość_wartości_parametru(typ):
    if typ=="color":
        return 3
    if typ=="position" or typ=="flip" or typ=="line_offset":
        return 2
    return 1
def uzupełnianie_wartości_odczytu(t):
    r = [t[0],]
    t += [None]
    wartości,typy = przypisanie_tabeli_wartości(t[0])
    n = 1
    m = 0
    for i in range(typy[0]):
        if t[n] == typy[i+1]:
            r += t[n+1:n+1+ilość_wartości_parametru(typy[i+1])]
            n += ilość_wartości_parametru(t[n]) + 1
        else:
            r += wartości[m:m+ilość_wartości_parametru(typy[i+1])]
        m += ilość_wartości_parametru(typy[i+1])
    return r
def odczyt_ruchu(r,s,t,p,k,i):
    n = 0
    i = 0
    z = []
    if t.find("/move") == -1:
        if t.find("container") == -1:
            koniec = s.find("</move>",p)
            p = s.find('"',p)
            k = s.find('"',p+1)
            r = r + [s[p+1:k],]
            l = znaczniki(s[p+1:k])
            while p<koniec and p>0 and l!=0:
                p = s.find("<p>",k)
                if p<koniec and p>0:
                    k = s.find("</p>",p)
                    f = s[p+3:k]
                    n = 1
                    if f == "-":
                        for j in range(l):
                            z = z + [-1,]
                    else:
                        while n>0:
                            n = f.find(",")
                            if n>0:
                                z = z + [float(f[0:n]),]
                                f = f[n+1:]
                            else:
                                z = z + [float(f),]
                    i = i + 1
            if l != 0:
                r = r + [i,] + z
                p = s.find("""</move>""",k)
                k = s.find(">",p)
        else:
            r = r + ["container",]
            a = len(r)
            while s.find("""move""",k) - 1 != s.find("""/move""",k):
                p = s.find("""<move""",k)
                k = s.find(""">""",p)
                t = s[p+1:k]
                n += 1
                r,s,t,p,k,i = odczyt_ruchu(r,s,t,p,k,i)
            r = r[:a] + [n,] + r[a:]
    return r,s,t,p,k,i
def odczyt_elementu(s):
    r = []
    z = []
    i = 0
    t = ""
    p = s.find("<")
    k = s.find(" ",p) - 1
    l = s.find(">",k)
    r = r + [s[p+1:k+1],]
    while p>=0 and p<l:
        p = s.find('"',k+1)
        t = s[k+2:p-1]
        if p>=0 and p<l:
            k = s.find('"',p+1)
            r += [t,]
            if t=="texture" or t=="surface" or t=="line" or t=="one_sided" or t=="sound" or t=="player_only" or t=="trigger" or t=="event" or t=="start" or t=="stop" or t=="name":
                r += [s[p+1:k],]
            else:
                f = s[p+1:k]
                n = 0
                while n>=0:
                    n = f.find(",")
                    if n>=0:
                        r = r + [float(f[0:n]),]
                        f = f[n+1:]
                    else:
                        r = r + [float(f),]
    r = uzupełnianie_wartości_odczytu(r)
    if s.find(r[0],k)!=-1:
        while t!="/"+r[0]:
            p = s.find("<",k)
            k = s.find(">",p)
            t = s[p+1:k]
            if t!="/"+r[0]:
                i = 0
                z = []
                if t.find("move") != -1:
                    r,s,t,p,k,i = odczyt_ruchu(r,s,t,p,k,i)
                else:
                    if t == "v" or t == "vt" or t == "d" or t == "e" or t == "et":
                        r = r + [t,]
                    while p>0:
                        k = s.find("</"+t+">",p)
                        f = s[p+2+len(t):k]
                        n = 1
                        while n>0:
                            n = f.find(",")
                            if t=="e" or t=="et":
                                if n>0:
                                    z += [f[0:n]]
                                    f = f[n+1:]
                                else:
                                    z += [f]
                            else:
                                if n>0:
                                    z = z + [float(f[0:n]),]
                                    f = f[n+1:]
                                else:
                                    z = z + [float(f),]
                        i = i + 1
                        p = s.find("<"+t+">",k)
                    p = k + 1
                    k = s.find(">",p)
                    r = r + [i,] + z
    return r
def indeks_ruchu_elementu(e):
    if e[0]=="platform":
        return -13
    if e[0]=="object":
        return -13
    if e[0]=="animated_object":
        return -15
    if e[0]=="moving_object":
        return 13
    if e[0]=="pendulum":
        return 0
    if e[0]=="moving_platform":
        return 12
    if e[0]=="wall":
        return -11
    if e[0]=="animated_wall":
        return -13
    if e[0]=="animated_moving_platform" or e[0]=="moving_animated_platform":
        return 15
    if e[0]=="damage_zone":
        return -3
    if e[0]=="barrier":
        return -4
    if e[0]=="moving_damage_zone":
        return 3
    if e[0]=="moving_barrier":
        return 4
    if e[0]=="animated_moving_object" or e[0]=="moving_animated_object":
        return 15
    if e[0]=="sound":
        return 0
    if e[0]=="event_zone":
        return 0
    if e[0]=="clickable_zone":
        return 0
    if e[0]=="moving_wall":
        return 11
    if e[0]=="animated_moving_wall" or e[0]=="moving_animated_wall":
        return 13
    if e[0]=="state_trigger":
        return 0
    if e[0]=="timed_event":
        return 0
    return 0
def indeks_wierzchołków_elementu(e):
    if e[0]=="platform":
        return 13
    if e[0]=="object":
        return -1
    if e[0]=="animated_object":
        return -1
    if e[0]=="moving_object":
        return -1
    if e[0]=="pendulum":
        return 17
    if e[0]=="moving_platform":
        return 0
    if e[0]=="wall":
        return 11
    if e[0]=="animated_wall":
        return 13
    if e[0]=="animated_moving_platform" or e[0]=="moving_animated_platform":
        return 0
    if e[0]=="damage_zone":
        return 3
    if e[0]=="barrier":
        return 4
    if e[0]=="moving_damage_zone":
        return 0
    if e[0]=="moving_barrier":
        return 0
    if e[0]=="animated_moving_object" or e[0]=="moving_animated_object":
        return -1
    if e[0]=="sound":
        return 8
    if e[0]=="event_zone":
        return 4
    if e[0]=="clickable_zone":
        return 3
    if e[0]=="moving_wall":
        return 0
    if e[0]=="animated_moving_wall":
        return 0
    if e[0]=="state_trigger":
        return -1
    if e[0]=="timed_event":
        return -1
    return 0
def zapis(s):
    if s.find("<state>")!=-1:
        chunk_name = "dynamic_chunk"
    else:
        chunk_name = "chunk"
    s_chunk = ""
    s_no_chunk = ""
    t = podział_na_elementy(s)
    for i in range(t[0]):
        if t[i+1]=="<state>" or t[i+1]=="</state>":
            if t[i+1]=="<state>":
                if t[i+2]=="</state>":
                    s_chunk += '<state position="0,0" size="0,0">\n'
                else:
                    s_chunk += "<state>\n"
            else:
                s_chunk += "</state>\n"
        else:
            t[i+1] = odczyt_elementu(t[i+1])
            if t[i+1][0]=="sound" or t[i+1][0]=="event" or t[i+1][0]=="timed_event":
                s_no_chunk += zapis_elementu(t[i+1],zwróć_tekst=True)
            else:
                s_chunk += zapis_elementu(t[i+1],zwróć_tekst=True)
    if s_chunk != "":
        s_chunk = """<"""+chunk_name+""">\n""" + s_chunk + """</"""+chunk_name+""">"""
    s_final = s_no_chunk + s_chunk
    lvl.add(s_final)
def wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e, n):
    if e[n] == "container":
        p = n+2
        for i in range(e[n+1]):
            p += e[p+1]*znaczniki(e[p])+2
    else:
        p = e[n+1]*znaczniki(e[n])+(n+2)
    return int(p)
def zapis_elementu(e ,zwróć_tekst=False):
    s = ""
    p = 0
    v = 0
    d = 0
    a_x = []
    a_y = []
    n = indeks_ruchu_elementu(e)
    if n<=0:
        n = indeks_wierzchołków_elementu(e)
    if e[0]=="platform":
        if e[9]=="true": #SPECJALNA TRANSFORMACJA TRUE/FALSE Z XML-A DO PYTHONA
            e[9] = True
        else:
            e[9] = False
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        s = platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[n:v],True)
    if e[0]=="object":
        s = obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],True)
    if e[0]=="animated_object":
        s = animowany_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[14],True)
    if e[0]=="moving_object":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        s = ruchomy_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[n:p],True)
    if e[0]=="pendulum":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        for i in range(e[v]):
            a_x = a_x + [e[v+2*i+1],]
            a_y = a_y + [e[v+2*i+2],]
        s = wahadło(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[14],e[15],e[16],e[n:v],e[v],a_x,a_y,True)
    if e[0]=="moving_platform":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[n:p],e[p:v],True)
    if e[0]=="wall":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        s = ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[n:v],True)
    if e[0]=="animated_wall":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        s = animowana_ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[n:v],True)
    if e[0]=="animated_moving_platform" or e[0]=="moving_animated_platform":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = animowana_ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[n:p],e[p:v],True)
    if e[0]=="damage_zone":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        d = round(v+e[v+1]*znaczniki(e[v])+2)
        s = strefa_obrażeń(e[1],e[2],e[n:v],e[v:d],True)
    if e[0]=="barrier":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        s = bariera(e[1],e[2],e[3],e[n:v],True)
    if e[0]=="moving_damage_zone":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        d = round(v+e[v+1]*znaczniki(e[v])+2)
        s = ruchoma_strefa_obrażeń(e[1],e[2],e[n:p],e[p:v],e[v:d],True)
    if e[0]=="moving_barrier":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = ruchoma_bariera(e[1],e[2],e[3],e[4:p],e[p:v],True)
    if e[0]=="animated_moving_object" or e[0]=="moving_animated_object":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        s = animowany_ruchomy_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[14],e[n:p],True)
    if e[0]=="sound":
        if e[8:]==[]:
            s = dźwięk(e[1],e[2],e[3],e[4],e[5],e[6],e[7],[],[],zwróć_tekst=True)
        else:
            if e[8]=="v":
                v = round(e[n+1]*znaczniki(e[n])+(n+2))
                s = dźwięk(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8:v],e[v:],zwróć_tekst=True)
            else:
                s = dźwięk(e[1],e[2],e[3],e[4],e[5],e[6],e[7],[],e[8:],zwróć_tekst=True)
    if e[0]=="event_zone":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        d = round(v+e[v+1]*znaczniki(e[v])+2)
        s = strefa_zdarzeń(e[1],e[2],e[3],e[n:v],e[v:d],True)
    if e[0]=="clickable_zone":
        v = round(e[n+1]*znaczniki(e[n])+(n+2))
        d = round(v+e[v+1]*znaczniki(e[v])+2)
        s = strefa_klikalna(e[1],e[2],e[n:v],e[v:d],True)
    if e[0]=="moving_wall":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = ruchoma_ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[n:p],e[p:v],True)
    if e[0]=="animated_moving_wall" or e[0]=="moving_animated_wall":
        p = wyznaczanie_zakresu_ruchu_w_tablicy_elementu(e,n)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = animowana_ruchoma_ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[n:p],e[p:v],True)
    if e[0]=="state_trigger":
        s = wyzwalacz_stanu(e[1],e[2],True)
    if e[0]=="timed_event":
        s = czasowe_zdarzenie(e[1],e[2],e[3],e[4],True)
    if e[0]=="event":
        s = zdarzenie(e[1],True)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def złożenie_ruchu(ruch=["linear",2,0,0,60,1,1,60]):
    s = """\n        <move type="{0}">""".format(ruch[0])
    zaokr = 3
    if ruch[0] == "container":
        pos = 2
        for i in range(ruch[1]):
            s = s + złożenie_ruchu(ruch[pos:pos+round(ruch[pos+1]*znaczniki(ruch[pos])+2)])
            pos += ruch[pos+1]*znaczniki(ruch[pos])+2
    if ruch[0] == "linear":
        for i in range(ruch[1]):
            if ruch[i*3+4] == -1:
                s = s + """\n            <p>-</p>"""
            else:
                s = s + """\n            <p>{0},{1},{2}</p>""".format(round(ruch[i*3+2],zaokr),round(ruch[i*3+3],zaokr),round(ruch[i*3+4],zaokr))
    if ruch[0] == "accelerated":
        for i in range(ruch[1]):
            if ruch[i*4+4] == -1:
                s = s + """\n            <p>-</p>"""
            else:
                s = s + """\n            <p>{0},{1},{2},{3}</p>""".format(round(ruch[i*4+2],zaokr),round(ruch[i*4+3],zaokr),round(ruch[i*4+4],zaokr),round(ruch[i*4+5],zaokr))
    if ruch[0] == "rotational":
        for i in range(ruch[1]):
            if ruch[i*2+3] == -1:
                s = s + """\n            <p>-</p>"""
            else:
                s = s + """\n            <p>{0},{1}</p>""".format(round(ruch[i*2+2],zaokr),round(ruch[i*2+3],zaokr))
    s = s + """\n        </move>"""
    return s
def złożenie_wierzchołków(wierzchołki=["v",4,0,0,0,1,1,1,1,0]):
    s = ""
    zaokr = 3
    if wierzchołki != []:
        if wierzchołki[0] == "v":
            for i in range(wierzchołki[1]):
                s = s + """\n        <v>{0},{1}</v>""".format(round(wierzchołki[2*i+2],zaokr),round(wierzchołki[2*i+3],zaokr))
        if wierzchołki[0] == "vt":
            for i in range(wierzchołki[1]):
                s = s + """\n        <vt>{0},{1},{2},{3}</vt>""".format(round(wierzchołki[4*i+2],zaokr),round(wierzchołki[4*i+3],zaokr),round(wierzchołki[4*i+4],zaokr),round(wierzchołki[4*i+5],zaokr))
    return s
def złożenie_obrażeń(obrażenia=["d",2,100,60,0,60]):
    s = ""
    zaokr = 3
    if obrażenia[0] == "d":
        for i in range(obrażenia[1]):
            s += """\n        <d>{0},{1}</d>""".format(round(obrażenia[i*2+2],zaokr),round(obrażenia[i*2+3],zaokr))
    return s
def złożenie_zdarzeń(zdarzenia=["e",1,"event"]):
    s = ""
    if zdarzenia != []:
        if zdarzenia[0] == "e":
            for i in range(zdarzenia[1]):
                s += """\n        <e>{0}</e>""".format(zdarzenia[i+2])
        if zdarzenia[0] == "et":
            for i in range(zdarzenia[1]):
                s += """\n        <et>{0},{1}</et>""".format(zdarzenia[i*2+2],zdarzenia[i*2+3])
    return s
def parametr_pojedyńczy(s, n, typ, wartości, domyślne, zaokrąglenie):
    if zaokrąglenie[0] == True:
        wartości[n] = round(wartości[n],zaokrąglenie[1])
        if round(wartości[n]) == wartości[n]:
            wartości[n] = int(round(wartości[n],zaokrąglenie[1]))
    if domyślne[n]!=None:
        if wartości[n]!=domyślne[n]:
            s += typ + '="{0}" '.format(wartości[n])
    else:
        s += typ + '="{0}" '.format(wartości[n])
    n += 1
    return s,n
def parametr_podwójny(s, n, typ, wartości, domyślne, zaokrąglenie):
    if zaokrąglenie[0] == True:
        wartości[n] = round(wartości[n],zaokrąglenie[1])
        wartości[n+1] = round(wartości[n+1],zaokrąglenie[1])
        if round(wartości[n]) == wartości[n]:
            wartości[n] = int(round(wartości[n],zaokrąglenie[1]))
        if round(wartości[n+1]) == wartości[n+1]:
            wartości[n+1] = int(round(wartości[n+1],zaokrąglenie[1]))
    if domyślne[n]!=None:
        if wartości[n]!=domyślne[n] or wartości[n+1]!=domyślne[n+1]:
            s += typ + '="{0},{1}" '.format(wartości[n],wartości[n+1])
    else:
        s += typ + '="{0},{1}" '.format(wartości[n],wartości[n+1])
    n += 2
    return s,n
def parametr_potrójny(s, n, typ, wartości, domyślne, zaokrąglenie):
    if zaokrąglenie[0] == True:
        wartości[n] = round(wartości[n],zaokrąglenie[1])
        wartości[n+1] = round(wartości[n+1],zaokrąglenie[1])
        wartości[n+2] = round(wartości[n+2],zaokrąglenie[1])
        if round(wartości[n]) == wartości[n]:
            wartości[n] = int(round(wartości[n],zaokrąglenie[1]))
        if round(wartości[n+1]) == wartości[n+1]:
            wartości[n+1] = int(round(wartości[n+1],zaokrąglenie[1]))
        if round(wartości[n+2]) == wartości[n+2]:
            wartości[n+2] = int(round(wartości[n+2],zaokrąglenie[1]))
    if domyślne[n]!=None:
        if wartości[n]!=domyślne[n] or wartości[n+1]!=domyślne[n+1] or wartości[n+2]!=domyślne[n+2]:
            s += typ + '="{0},{1},{2}" '.format(wartości[n],wartości[n+1],wartości[n+2])
    else:
        s += typ + '="{0},{1},{2}" '.format(wartości[n],wartości[n+1],wartości[n+2])
    n += 3
    return s,n
def parametry(typy, wartości, domyślne):
    s = ""
    n = 0
    for i in range(typy[0]):
        if typy[i+1] == "position":
            s,n = parametr_podwójny(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "texture":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "layer":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,0])
        if typy[i+1] == "rotation":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "flip":
            s,n = parametr_podwójny(s,n,typy[i+1],wartości,domyślne,[True,0])
        if typy[i+1] == "surface":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "one_sided":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "color":
            s,n = parametr_potrójny(s,n,typy[i+1],wartości,domyślne,[True,1])
        if typy[i+1] == "height":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "frame_time":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "offset":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "line":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "length":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "angle":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "line_offset":
            s,n = parametr_podwójny(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "volume":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "sound":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "attenuation":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "min_distance":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "range":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "player_only":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "state":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,0])
        if typy[i+1] == "start":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "stop":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "event":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "trigger":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "time":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
        if typy[i+1] == "name":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[False,])
        if typy[i+1] == "paralax":
            s,n = parametr_pojedyńczy(s,n,typy[i+1],wartości,domyślne,[True,3])
    return s
def platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, jednostronna=False, R=255, G=255, B=255, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    if jednostronna: #SPECJALNA TRANSFORMACJA TRUE/FALSE Z XML-A DO PYTHONA
        tekst = "true"
    else:
        tekst = "false"
    wartości = [x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia,tekst,R,G,B]
    w,t = przypisanie_tabeli_wartości("platform")
    s += "    <platform " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki)
    s += """\n    </platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, rotacja=0, odbicie_x=1, odbicie_y=1, warstwa=3, R=255, G=255, B=255, paralaksa=0, zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,wysokość,rotacja,odbicie_x,odbicie_y,warstwa,R,G,B,paralaksa]
    w,t = przypisanie_tabeli_wartości("object")
    s += "    <object " + parametry(t,wartości,w) + "/>\n"
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, rotacja=0, odbicie_x=1, odbicie_y=1, warstwa=3, czas_klatki=1, przesunięcie=0, R=255, G=255, B=255, paralaksa=0, zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,wysokość,rotacja,odbicie_x,odbicie_y,warstwa,czas_klatki,przesunięcie,R,G,B,paralaksa]
    w,t = przypisanie_tabeli_wartości("animated_object")
    s += "    <animated_object " + parametry(t,wartości,w) + "/>\n"
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchomy_obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, rotacja=0, odbicie_x=1, odbicie_y=1, warstwa=3, R=255, G=255, B=255, paralaksa=0, ruch=["linear",2,0,0,60,1,1,60], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,wysokość,rotacja,odbicie_x,odbicie_y,warstwa,R,G,B,paralaksa]
    w,t = przypisanie_tabeli_wartości("moving_object")
    s += "    <moving_object " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch)
    s += """\n    </moving_object>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def wahadło(x=0, y=0, tekstura=domyślna_tekstura, lina=domyślna_lina, długość=5, wychylenie=30, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, początek_liny_x=256, początek_liny_y=0, powierzchnia=domyślna_powierzchnia, R=255, G=255, B=255, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zaczepienia=2, zaczepienia_x=(0,1), zaczepienia_y=(0,1), zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,lina,długość,wychylenie,warstwa,rotacja,odbicie_x,odbicie_y,początek_liny_x,początek_liny_y,powierzchnia,R,G,B]
    w,t = przypisanie_tabeli_wartości("pendulum")
    s += "    <pendulum " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki)
    for i in range(zaczepienia):
        s += """\n        <a>{0},{1}</a>""".format(zaczepienia_x[i],zaczepienia_y[i])
    s += """\n    </pendulum>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, R=255, G=255, B=255, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia,R,G,B]
    w,t = przypisanie_tabeli_wartości("moving_platform")
    s += "    <moving_platform " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki)
    s += """\n    </moving_platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ściana(x=0, y=0, tekstura=domyślna_tekstura, warstwa=1, odbicie_x=1, odbicie_y=1, rotacja=0, R=255, G=255, B=255, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,odbicie_x,odbicie_y,rotacja,R,G,B]
    w,t = przypisanie_tabeli_wartości("wall")
    s += "    <wall " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki)
    s += """\n    </wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ściana(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=1, czas_klatki=1, przesunięcie=0, odbicie_x=1, odbicie_y=1, rotacja=0, R=255, G=255, B=255, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,czas_klatki,przesunięcie,odbicie_x,odbicie_y,rotacja,R,G,B]
    w,t = przypisanie_tabeli_wartości("animated_wall")
    s += "    <animated_wall " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki)
    s += """\n    </animated_wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ruchoma_platforma(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, przesunięcie=0, czas_klatki=1, powierzchnia=domyślna_powierzchnia, R=255, G=255, B=255, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,przesunięcie,czas_klatki,powierzchnia,R,G,B]
    w,t = przypisanie_tabeli_wartości("animated_moving_platform")
    s += "    <animated_moving_platform " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki)
    s += """\n    </animated_moving_platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def strefa_obrażeń(x=0, y=0, wierzchołki=["v",4,0,0,0,1,1,1,1,0], obrażenia=["d",2,100,60,0,60], zwróć_tekst=False):
    s = ""
    wartości = [x,y]
    w,t = przypisanie_tabeli_wartości("damage_zone")
    s += "    <damage_zone " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki) + złożenie_obrażeń(obrażenia)
    s += """\n    </damage_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def bariera(x=0, y=0, powierzchnia=domyślna_powierzchnia, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,powierzchnia]
    w,t = przypisanie_tabeli_wartości("barrier")
    s += "    <barrier " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki)
    s += """\n    </barrier>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_strefa_obrażeń(x=0, y=0, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], obrażenia=["d",2,100,60,0,60], zwróć_tekst=False):
    s = ""
    wartości = [x,y]
    w,t = przypisanie_tabeli_wartości("moving_damage_zone")
    s += "    <moving_damage_zone " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki) + złożenie_obrażeń(obrażenia)
    s += """\n    </moving_damage_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_bariera(x=0, y=0, powierzchnia=domyślna_powierzchnia, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,powierzchnia]
    w,t = przypisanie_tabeli_wartości("moving_barrier")
    s += "    <moving_barrier " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki)
    s += """\n    </moving_barrier>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_ruchomy_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, rotacja=0, odbicie_x=1, odbicie_y=1, warstwa=3, czas_klatki=1, przesunięcie=0, R=255, G=255, B=255, paralaksa=0, ruch=["linear",2,0,0,60,1,1,60], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,wysokość,rotacja,odbicie_x,odbicie_y,warstwa,czas_klatki,przesunięcie,R,G,B,paralaksa]
    w,t = przypisanie_tabeli_wartości("animated_moving_object")
    s += "    <animated_moving_object " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch)
    s += """\n    </animated_moving_object>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def dźwięk(x=0, y=0, głośność=100, dźwięk=domyślny_dźwięk, wyciszanie=1, próg_głośności=2, zasięg=8, wierzchołki=["v",0], zdarzenia=["et",0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,głośność,dźwięk,wyciszanie,próg_głośności,zasięg]
    w,t = przypisanie_tabeli_wartości("sound")
    s += "    <sound " + parametry(t,wartości,w)
    v = złożenie_wierzchołków(wierzchołki) + złożenie_zdarzeń(zdarzenia)
    if v == "":
        s += "/>\n"
    else:
        s += """>""" + v + """\n    </sound>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def strefa_zdarzeń(x=0, y=0, tylko_gracz=False, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zdarzenia=["e",1,"event"], zwróć_tekst=False):
    s = ""
    if tylko_gracz: #SPECJALNA TRANSFORMACJA TRUE/FALSE Z XML-A DO PYTHONA
        tekst = "true"
    else:
        tekst = "false"
    wartości = [x,y,tekst]
    w,t = przypisanie_tabeli_wartości("event_zone")
    s += "    <event_zone " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki) + złożenie_zdarzeń(zdarzenia)
    s += """\n    </event_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def strefa_klikalna(x=0, y=0, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zdarzenia=["e",1,"event"], zwróć_tekst=False):
    s = ""
    wartości = [x,y]
    w,t = przypisanie_tabeli_wartości("clickable_zone")
    s += "    <clickable_zone " + parametry(t,wartości,w) + ">"
    s += złożenie_wierzchołków(wierzchołki) + złożenie_zdarzeń(zdarzenia)
    s += """\n    </clickable_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_ściana(x=0, y=0, tekstura=domyślna_tekstura, warstwa=1, odbicie_x=1, odbicie_y=1, rotacja=0, R=255, G=255, B=255, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,odbicie_x,odbicie_y,rotacja,R,G,B]
    w,t = przypisanie_tabeli_wartości("moving_wall")
    s += "    <moving_wall " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki)
    s += """\n    </moving_wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ruchoma_ściana(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=1, czas_klatki=1, przesunięcie=0, odbicie_x=1, odbicie_y=1, rotacja=0, R=255, G=255, B=255, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    wartości = [x,y,tekstura,warstwa,czas_klatki,przesunięcie,odbicie_x,odbicie_y,rotacja,R,G,B]
    w,t = przypisanie_tabeli_wartości("animated_moving_wall")
    s += "    <animated_moving_wall " + parametry(t,wartości,w) + ">"
    s += złożenie_ruchu(ruch) + złożenie_wierzchołków(wierzchołki)
    s += """\n    </animated_moving_wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def wyzwalacz_stanu(stan=0, wyzwalacz="event", zwróć_tekst=False):
    s = ""
    wartości = [stan,wyzwalacz]
    w,t = przypisanie_tabeli_wartości("state_trigger")
    s += "    <state_trigger " + parametry(t,wartości,w) + "/>\n"
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def czasowe_zdarzenie(start="event1", stop="event2", zdarzenie="event3", czas=60, zwróć_tekst=False):
    s = ""
    wartości = [start,stop,zdarzenie,czas]
    w,t = przypisanie_tabeli_wartości("timed_event")
    s += "    <timed_event " + parametry(t,wartości,w) + "/>\n"
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def zdarzenie(nazwa="event", zwróć_tekst=False):
    s = ""
    wartości = [nazwa]
    w,t = przypisanie_tabeli_wartości("event")
    s += "    <event " + parametry(t,wartości,w) + "/>\n"
    if zwróć_tekst:
        return s
    else:
        zapis(s)