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
    return 0
def podział_na_elementy(s):
    r = []
    i = 0
    k = 0
    while k>=0:
        p = s.find("<",k)
        k = s.find(" ",p)
        if k>=0:
            a = p
            t = s[p+1:k]
            p = s.find("/",k)
            k = s.find(">",k)
            if k<p:
                k = s.find(t,k)
                k = s.find(">",k)
            b = k
            r = r + [s[a:b+1],]
            i = i + 1
    r = [i,] + r
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
        t = s[k+2:p]
        if p>=0 and p<l:
            k = s.find('"',p+1)
            if t=="texture=" or t=="surface=" or t=="line=" or t=="one_sided=" or t=="sound=":
                r = r + [s[p+1:k],]
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
                    if t == "v":
                        r = r + ["v",]
                    if t == "vt":
                        r = r + ["vt",]
                    while p>0:
                        k = s.find("</"+t+">",p)
                        f = s[p+2+len(t):k]
                        n = 1
                        while n>0:
                            n = f.find(",")
                            if n>0:
                                z = z + [float(f[0:n]),]
                                f = f[n+1:]
                            else:
                                z = z + [float(f),]
                        i = i + 1
                        p=s.find("<"+t+">",k)
                    p = k + 1
                    k = s.find(">",p)
                    r = r + [i,] + z
    return r
def indeks_ruchu_elementu(e):
    if e[0]=="platform":
        return -9
    if e[0]=="object":
        return -9
    if e[0]=="animated_object":
        return -11
    if e[0]=="moving_object":
        return 9
    if e[0]=="pendulum":
        return 0
    if e[0]=="moving_platform":
        return 9
    if e[0]=="wall":
        return 0
    if e[0]=="animated_wall":
        return 0
    if e[0]=="animated_moving_platform":
        return 11
    if e[0]=="damage_zone":
        return -3
    if e[0]=="barrier":
        return -4
    if e[0]=="moving_damage_zone":
        return 3
    if e[0]=="moving_barrier":
        return 4
    if e[0]=="animated_moving_object":
        return 11
    if e[0]=="sound":
        return 0
def zapis(s):
    s_chunk = ""
    s_no_chunk = ""
    t = podział_na_elementy(s)
    for i in range(t[0]):
        t[i+1] = odczyt_elementu(t[i+1])
        if t[i+1][0]=="sound":
            s_no_chunk += zapis_elementu(t[i+1],zwróć_tekst=True)
        else:
            s_chunk += zapis_elementu(t[i+1],zwróć_tekst=True)
    if s_chunk != "":
        s_chunk = """<chunk>\n""" + s_chunk + """</chunk>"""
    s_final = s_no_chunk + s_chunk
    lvl.add(s_final)
def zapis_elementu(e ,zwróć_tekst=False):
    s = ""
    p = 0
    v = 0
    a_x = []
    a_y = []
    d_d = []
    d_t = []
    if e[0]=="platform":
        if e[9]=="true":
            e[9] = True
        else:
            e[9] = False
        v = round(e[11]*znaczniki(e[10])+12)
        s = platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10:v],True)
    if e[0]=="object":
        s = obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],True)
    if e[0]=="animated_object":
        s = animowany_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],True)
    if e[0]=="moving_object":
        if e[9] == "container":
            p = 11
            for i in range(e[10]):
                p += e[p+1]*znaczniki(e[p])+2
        else:
            p = e[10]*znaczniki(e[9])+11
        s = ruchomy_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9:p],True)
    if e[0]=="pendulum":
        v = round(e[15]*znaczniki(e[14])+16)
        for i in range(e[v]):
            a_x = a_x + [e[v+2*i+1],]
            a_y = a_y + [e[v+2*i+2],]
        s = wahadło(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[14:v],e[v],a_x,a_y,True)
    if e[0]=="moving_platform":
        if e[9] == "container":
            p = 11
            for i in range(e[10]):
                p += round(e[p+1]*znaczniki(e[p])+2)
        else:
            p = round(e[10]*znaczniki(e[9])+11)
        v = round(p+e[p+1]*znaczniki(e[p])+2)
        s = ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9:p],e[p:v],True)
    if e[0]=="wall":
        v = e[7]*znaczniki(e[6])+8
        s = ściana(e[1],e[2],e[3],e[4],e[5],e[6:v],True)
    if e[0]=="animated_wall":
        v = e[9]*znaczniki(e[8])+10
        s = animowana_ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8:v],True)
    if e[0]=="animated_moving_platform" or e[0]=="moving_animated_platform":
        if e[11] == "container":
            p = 13
            for i in range(e[12]):
                p += e[p+1]*znaczniki(e[p])+2
        else:
            p = e[12]*znaczniki(e[11])+13
        v = p+e[p+1]*znaczniki(e[p])+2
        s = animowana_ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11:p],e[p:v],True)
    if e[0]=="damage_zone":
        v = e[4]*znaczniki(e[3])+5
        for i in range(e[p]):
            d_d = d_d + [e[p+1+2*i],]
            d_t = d_t + [e[p+2+2*i],]
        s = strefa_obrażeń(e[1],e[2],e[3:v],e[p],d_d,d_t,True)
    if e[0]=="barrier":
        v = e[5]*znaczniki(e[4])+6
        s = bariera(e[1],e[2],e[3],e[4:v],True)
    if e[0]=="moving_damage_zone":
        if e[3] == "container":
            p = 5
            for i in range(e[4]):
                p += e[p+1]*znaczniki(e[p])+2
        else:
            p = e[4]*znaczniki(e[3])+5
        v = p+e[p+1]*znaczniki(e[p])+2
        for i in range(e[v]):
            d_d = d_d + [v+1+i*2,]
            d_t = d_t + [v+2+i*2,]
        s = ruchoma_strefa_obrażeń(e[1],e[2],e[3:p],e[p:v],e[v],d_d,d_t,True)
    if e[0]=="moving_barrier":
        if e[4] == "container":
            p = 6
            for i in range(e[5]):
                p += e[p+1]*znaczniki(e[p])+2
        else:
            p = e[5]*znaczniki(e[4])+6
        v = p+e[p+1]*znaczniki(e[p])+2
        s = ruchoma_bariera(e[1],e[2],e[3],e[4:p],e[p:v],True)
    if e[0]=="animated_moving_object" or e[0]=="moving_animated_object":
        if e[11] == "container":
            p = 13
            for i in range(e[12]):
                p += e[p+1]*znaczniki(e[p])+2
        else:
            p = round(e[12]*znaczniki(e[11])+13)
        s = animowany_ruchomy_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11:p],True)
    if e[0]=="sound":
        s = dźwięk(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8:],zwróć_tekst=True)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def złożenie_ruchu(ruch=["linear",2,0,0,60,1,1,60]):
    s = """\n        <move type="{0}">""".format(ruch[0])
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
                s = s + """\n            <p>{0},{1},{2}</p>""".format(ruch[i*3+2],ruch[i*3+3],ruch[i*3+4])
    if ruch[0] == "accelerated":
        for i in range(ruch[1]):
            if ruch[i*4+4] == -1:
                s = s + """\n            <p>-</p>"""
            else:
                s = s + """\n            <p>{0},{1},{2},{3}</p>""".format(ruch[i*4+2],ruch[i*4+3],ruch[i*4+4],ruch[i*4+5])
    if ruch[0] == "rotational":
        for i in range(ruch[1]):
            if ruch[i*2+3] == -1:
                s = s + """\n            <p>-</p>"""
            else:
                s = s + """\n            <p>{0},{1}</p>""".format(ruch[i*2+2],ruch[i*2+3])
    s = s + """\n        </move>"""
    return s
def złożenie_wierzchołków(wierzchołki=["v",4,0,0,0,1,1,1,1,0]):
    s = ""
    if wierzchołki != []:
        if wierzchołki[0] == "v":
            for i in range(wierzchołki[1]):
                s = s + """\n        <v>{0},{1}</v>""".format(wierzchołki[2*i+2],wierzchołki[2*i+3])
        if wierzchołki[0] == "vt":
            for i in range(wierzchołki[1]):
                s = s + """\n        <vt>{0},{1},{2},{3}</vt>""".format(wierzchołki[4*i+2],wierzchołki[4*i+3],wierzchołki[4*i+4],wierzchołki[4*i+5])
    return s
def platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, jednostronna=False, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    if jednostronna:
        tekst = "true"
    else:
        tekst = "false"
    s = s + """    <platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}" one_sided="{8}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia,tekst)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, zwróć_tekst=False):
    s = ""
    s = s + """    <object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, czas_klatki=1, przesunięcie=0, zwróć_tekst=False):
    s = ""
    s = s + """    <animated_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}" frame_time="{8}" offset="{9}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y,czas_klatki,przesunięcie)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchomy_obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, ruch=["linear",2,0,0,60,1,1,60], zwróć_tekst=False):
    s = ""
    s = s + """    <moving_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}">""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + """\n    </moving_object>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def wahadło(x=0, y=0, tekstura=domyślna_tekstura, lina=domyślna_lina, długość=5, wychylenie=30, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, początek_liny_x=256, początek_liny_y=0, powierzchnia=domyślna_powierzchnia, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zaczepienia=2, zaczepienia_x=(0,1), zaczepienia_y=(0,1), zwróć_tekst=False):
    s = ""
    s = s + """    <pendulum position="{0},{1}" texture="{2}" line="{3}" length="{4}" angle="{5}" layer="{6}" rotation="{7}" flip="{8},{9}" line_offset="{10},{11}" surface="{12}">""".format(x,y,tekstura,lina,długość,wychylenie,warstwa,rotacja,odbicie_x,odbicie_y,początek_liny_x,początek_liny_y,powierzchnia)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    for i in range(zaczepienia):
        s = s + """\n        <a>{0},{1}</a>""".format(zaczepienia_x[i],zaczepienia_y[i])
    s = s + """\n    </pendulum>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia)
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </moving_platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ściana(x=0, y=0, tekstura=domyślna_tekstura, warstwa=1, rotacja=0, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}">""".format(x,y,tekstura,warstwa,rotacja)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ściana(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=1, rotacja=0, czas_klatki=1, przesunięcie=0, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <animated_wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" frame_time="{5}" offset="{6}">""".format(x,y,tekstura,warstwa,rotacja,czas_klatki,przesunięcie)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </animated_wall>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ruchoma_platforma(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, czas_klatki=1, przesunięcie=0, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <animated_moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}" frame_time="{8}" offset="{9}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia,czas_klatki,przesunięcie)   
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </animated_moving_platform>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def strefa_obrażeń(x=0, y=0, wierzchołki=["v",4,0,0,0,1,1,1,1,0], stany=2, stany_d=[100,0], stany_t=[60,60], zwróć_tekst=False):
    s = ""
    s = s + """    <damage_zone position="{0},{1}">""".format(x,y)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    for i in range(stany):
        s = s + """\n        <d>{0},{1}</d>""".format(stany_d[i],stany_t[i])
    s = s + """\n    </damage_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def bariera(x=0, y=0, powierzchnia=domyślna_powierzchnia, wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <barrier position="{0},{1}" surface="{2}">""".format(x,y,powierzchnia)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </barrier>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_strefa_obrażeń(x=0, y=0, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], stany=2, stany_d=[100,0], stany_t=[60,60], zwróć_tekst=False):
    s = ""
    s = s + """    <moving_damage_zone position="{0},{1}">""".format(x,y)
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    for i in range(stany):
        s = s + """\n        <d>{0},{1}</d>""".format(stany_d[i],stany_t[i])
    s = s + """\n    </moving_damage_zone>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_bariera(x=0, y=0, powierzchnia=domyślna_powierzchnia, ruch=["linear",2,0,0,60,1,1,60], wierzchołki=["v",4,0,0,0,1,1,1,1,0], zwróć_tekst=False):
    s = ""
    s = s + """    <moving_barrier position="{0},{1}" surface="{2}">""".format(x,y,powierzchnia)
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + złożenie_wierzchołków(wierzchołki=wierzchołki)
    s = s + """\n    </moving_barrier>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_ruchomy_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, czas_klatki=1, przesunięcie=0, ruch=["linear",2,0,0,60,1,1,60], zwróć_tekst=False):
    s = ""
    s = s + """    <animated_moving_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}" frame_time="{8}" offset="{9}">""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y,czas_klatki,przesunięcie)
    s = s + złożenie_ruchu(ruch=ruch)
    s = s + """\n    </animated_moving_object>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def dźwięk(x=0, y=0, głośność=100, dźwięk=domyślny_dźwięk, wyciszanie=1, próg_głośności=2, zasięg=8, wierzchołki=["v",0], zwróć_tekst=False):
    s = ""
    s = s + '    <sound position="{0},{1}" volume="{2}" sound="{3}" attenuation="{4}" min_distance="{5}" range="{6}"'.format(x,y,głośność,dźwięk,wyciszanie,próg_głośności,zasięg)
    v = złożenie_wierzchołków(wierzchołki=wierzchołki)
    if v == "":
        s += "/>\n"
    else:
        s += """>\n""" + v + """    </sound>\n"""
    if zwróć_tekst:
        return s
    else:
        zapis(s)