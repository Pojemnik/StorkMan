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

def podział_na_elementy(s):
    r = []
    i = 0
    k = 0
    while k>=0:
        p = s.find("<",k)
        k = s.find(" ",p)
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
    r = [i-1,] + r
    return r
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
            if t=="texture=" or t=="surface=" or t=="line=":
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
                if t=='move type="linear"':
                    while p>0:
                        p = s.find("<p>",k)
                        if p>0:
                            k = s.find("</p>",p)
                            f = s[p+3:k]
                            n = 1
                            while n>0:
                                n = f.find(",")
                                if n>0:
                                    z = z + [float(f[0:n]),]
                                    f = f[n+1:]
                                else:
                                    z = z + [float(f),]
                            i = i + 1
                    r = r + [i,] + z
                    p = s.find("""</move>""",k)
                    k = s.find(">",p)
                else:
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
        return 0
    if e[0]=="moving_damage_zone":
        return 3
def zapis(s):
    t = podział_na_elementy(s)
    x_min = 0
    y_min = 0
    x_max = 0
    y_max = 0
    for i in range(t[0]):
        r = odczyt_elementu(t[i+1])
        n = indeks_ruchu_elementu(r)
        if n > 0:
            for j in range(r[n]):
                if r[n+1+j*3] < x_min:
                    x_min = r[n+1+j*3]
                if r[n+2+j*3] < y_min:
                    y_min = r[n+2+j*3]
                if r[n+1+j*3] > x_max:
                    x_max = r[n+1+j*3]
                if r[n+2+j*3] > y_max:
                    y_max = r[n+2+j*3]
    s = """<chunk left="{0}" right="{1}" top="{2}" bottom="{3}">
    """.format(abs(x_min),abs(x_max),abs(y_min),abs(y_max)) + s + """
    </chunk>"""
    lvl.add(s)
def zapis_elementu(e ,zwróć_tekst=False):
    s = ""
    v_x = ()
    v_y = ()
    p_x = ()
    p_y = ()
    p_t = ()
    a_x = ()
    a_y = ()
    d_d = ()
    d_t = ()
    if e[0]=="platform":
        for i in range(e[9]):
            v_x = v_x + (e[10+2*i],)
            v_y = v_y + (e[11+2*i],)
        s = platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],v_x,v_y,True)
    if e[0]=="object":
        s = obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],True)
    if e[0]=="animated_object":
        s = animowany_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],True)
    if e[0]=="moving_object":
        for i in range(e[9]):
            p_x = p_x + (e[10+3*i],)
            p_y = p_y + (e[11+3*i],)
            p_t = p_t + (e[12+3*i],)
        s = ruchomy_obiekt(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],p_x,p_y,p_t,True)
    if e[0]=="pendulum":
        for i in range(e[14]):
            v_x = v_x + (e[15+2*i],)
            v_y = v_y + (e[16+2*i],)
        for i in range(e[15+2*e[14]]):
            a_x = a_x + (e[16+2*e[14]+2*i],)
            a_y = a_y + (e[17+2*e[14]+2*i],)
        s = wahadło(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],e[12],e[13],e[14],v_x,v_y,e[15+2*e[14]],a_x,a_y,True)
    if e[0]=="moving_platform":
        for i in range(e[9]):
            p_x = p_x + (e[10+3*i],)
            p_y = p_y + (e[11+3*i],)
            p_t = p_t + (e[12+3*i],)
        for i in range(e[10+3*e[9]]):
            v_x = v_x + (e[11+3*e[9]+2*i],)
            v_y = v_y + (e[12+3*e[9]+2*i],)
        s = ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],p_x,p_y,p_t,e[10+3*e[9]],v_x,v_y,True)
    if e[0]=="wall":
        for i in range(e[6]):
            v_x = v_x + (e[7+2*i],)
            v_y = v_y + (e[8+2*i],)
        s = ściana(e[1],e[2],e[3],e[4],e[5],e[6],v_x,v_y,True)
    if e[0]=="animated_wall":
        for i in range(e[8]):
            v_x = v_x + (e[9+2*i],)
            v_y = v_y + (e[10+2*i],)
        s = animowana_ściana(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],v_x,v_y,True)
    if e[0]=="animated_moving_platform":
        for i in range(e[11]):
            p_x = p_x + (e[12+3*i],)
            p_y = p_y + (e[13+3*i],)
            p_t = p_t + (e[14+3*i],)
        for i in range(e[12+3*e[11]]):
            v_x = v_x + (e[13+3*e[11]+2*i],)
            v_y = v_y + (e[14+3*e[11]+2*i],)
        s = animowana_ruchoma_platforma(e[1],e[2],e[3],e[4],e[5],e[6],e[7],e[8],e[9],e[10],e[11],p_x,p_y,p_t,e[12+3*e[11]],v_x,v_y,True)
    if e[0]=="damage_zone":
        for i in range(e[3]):
            v_x = v_x + (e[4+2*i],)
            v_y = v_y + (e[5+2*i],)
        for i in range(e[4+2*e[3]]):
            d_d = d_d + (e[5+2*e[3]+2*i],)
            d_t = d_t + (e[6+2*e[3]+2*i],)
        s = strefa_obrażeń(e[1],e[2],e[3],v_x,v_y,e[4+2*e[3]],d_d,d_t,True)
    if e[0]=="barrier":
        for i in range(e[4]):
            v_x = v_x + (e[5+2*i],)
            v_y = v_y + (e[6+2*i],)
        s = bariera(e[1],e[2],e[3],e[4],v_x,v_y,True)
    if e[0]=="moving_damage_zone":
        for i in range(e[3]):
            p_x = p_x + (e[4+3*i],)
            p_y = p_y + (e[5+3*i],)
            p_t = p_t + (e[6+3*i],)
        for i in range(e[4+3*e[3]]):
            v_x = v_x + (e[5+3*e[3]+2*i],)
            v_y = v_y + (e[6+3*e[3]+2*i],)
        for i in range(e[5+3*e[3]+e[4+2*e[3]]]):
            d_d = d_d + (e[6+3*e[3]+e[4+2*e[3]]+2*i],)
            d_t = d_t + (e[7+3*e[3]+e[4+2*e[3]]+2*i],)
        s = ruchoma_strefa_obrażeń(e[1],e[2],e[3],p_x,p_y,p_t,e[4+3*e[3]],v_x,v_y,e[5+3*e[3]+e[4+2*e[3]]],d_d,d_t,True)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </platform>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, zwróć_tekst=False):
    s = ""
    s = s + """<object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, czas_klatki=1, przesunięcie=0, zwróć_tekst=False):
    s = ""
    s = s + """<animated_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}" frame_time="{8}" offset="{9}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y,czas_klatki,przesunięcie)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchomy_obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), zwróć_tekst=False):
    s = ""
    s = s + """<moving_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}">""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    s = s + """
    <move type="linear">"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </move>"""
    s = s + """
    </moving_object>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def wahadło(x=0, y=0, tekstura=domyślna_tekstura, lina=domyślna_lina, długość=5, wychylenie=30, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, początek_liny_x=256, początek_liny_y=0, powierzchnia=domyślna_powierzchnia, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zaczepienia=2, zaczepienia_x=(0,1), zaczepienia_y=(0,1), zwróć_tekst=False):
    s = ""
    s = s + """<pendulum position="{0},{1}" texture="{2}" line="{3}" length="{4}" angle="{5}" layer="{6}" rotation="{7}" flip="{8},{9}" line_offset="{10},{11}" surface="{12}">""".format(x,y,tekstura,lina,długość,wychylenie,warstwa,rotacja,odbicie_x,odbicie_y,początek_liny_x,początek_liny_y,powierzchnia)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    for i in range(zaczepienia):
        s = s + """
        <a>{0},{1}</a>""".format(zaczepienia_x[i],zaczepienia_y[i])
    s = s + """
    </pendulum>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia)
    s = s + """
    <move type="linear">"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </move>"""
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </moving_platform>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ściana(x=0, y=0, tekstura=domyślna_tekstura, warstwa=1, rotacja=0, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}">""".format(x,y,tekstura,warstwa,rotacja)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </wall>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ściana(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=1, rotacja=0, czas_klatki=1, przesunięcie=0, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<animated_wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" frame_time="{5}" offset="{6}">""".format(x,y,tekstura,warstwa,rotacja,czas_klatki,przesunięcie)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </animated_wall>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ruchoma_platforma(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia=domyślna_powierzchnia, czas_klatki=1, przesunięcie=0, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<animated_moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}" frame_time="{8}" offset="{9}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia,czas_klatki,przesunięcie)   
    s = s + """
    <move type="linear">"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </move>"""
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </animated_moving_platform>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def strefa_obrażeń(x=0, y=0, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), stany=2, stany_d=(100,0), stany_t=(60,60), zwróć_tekst=False):
    s = ""
    s = s + """<damage_zone position="{0},{1}">""".format(x,y)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    for i in range(stany):
        s = s + """
        <d>{0},{1}</d>""".format(stany_d[i],stany_t[i])
    s = s + """
    </damage_zone>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def bariera(x=0, y=0, powierzchnia=domyślna_powierzchnia, wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<barrier position="{0},{1}" surface="{2}">""".format(x,y,powierzchnia)
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    s = s + """
    </barrier>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchoma_strefa_obrażeń(x=0, y=0, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), stany=2, stany_d=(100,0), stany_t=(60,60), zwróć_tekst=False):
    s = ""
    s = s + """<moving_damage_zone position="{0},{1}">""".format(x,y)
    s = s + """
    <move type="linear">"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </move>"""
    for i in range(wierzchołki):
        s = s + """
        <v>{0},{1}</v>""".format(wierzchołki_x[i],wierzchołki_y[i])
    for i in range(stany):
        s = s + """
        <d>{0},{1}</d>""".format(stany_d[i],stany_t[i])
    s = s + """
    </moving_damage_zone>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)