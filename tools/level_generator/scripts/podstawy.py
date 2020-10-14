import math
global lvl
global domyślna_tekstura
global domyślna_animowana_tekstura
global domyślny_obiekt
global domyślny_animowany_obiekt
domyślna_tekstura = "bricks,0"
domyślna_animowana_tekstura = "rain45"
domyślny_obiekt = "chest0"
domyślny_animowany_obiekt = "wind0"

def zapis(s):
    s = """<chunk>
    """ + s + """
    </chunk>"""
    lvl.add(s)
def podział_na_elementy(s):
    r = ()
    i = 0
    k = 0
    while k+1<len(s):
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
        r = r + (s[a:b+1],)
        i = i + 1
    r = (i,) + r
    return r
def odczyt_elementu(s):
    r = ()
    z = ()
    i = 0
    t = ""
    p = s.find("<")
    k = s.find(" ",p)
    r = r + (s[p+1:k],)
    while p>=0:
        p = s.find('"',k+1)
        if p>=0:
            k = s.find('"',p+1)
            f = s[p+1:k]
            n = 0
            while n>=0:
                n = f.find(",")
                if n>=0:
                    r = r + (f[0:n],)
                    f = f[n+1:]
                else:
                    r = r + (f,)
    if s.find(r[0],k)!=-1:
        while t!="/"+r[0]:
            p = s.find("<",k)
            k = s.find(">",p)
            t = s[p+1:k]
            if t!="/"+r[0]:
                i = 0
                z = ()
                if t=="linear_move":
                    while p>0:
                        p = s.find("<p>",k)
                        if p>0:
                            k = s.find("</p>",p)
                            f = s[p+3:k]
                            n = 1
                            while n>0:
                                n = f.find(",")
                                if n>0:
                                    z = z + (f[0:n],)
                                    f = f[n+1:]
                                else:
                                    z = z + (f,)
                            i = i + 1
                    r = r + (i,) + z
                    p = s.find("</linear_move>",k)
                    k = s.find(">",p)
                else:
                    while p>0:
                        k = s.find("</"+t+">",p)
                        f = s[p+2+len(t):k]
                        n = 1
                        while n>0:
                            n = f.find(",")
                            if n>0:
                                z = z + (f[0:n],)
                                f = f[n+1:]
                            else:
                                z = z + (f,)
                        i = i + 1
                        p=s.find("<"+t+">",k)
                    p = k + 1
                    k = s.find(">",p)
                    r = r + (i,) + z
    return r
def platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia="none", wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
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
    s = s + """<object position="{0},{1}" texture="{2}" height="{3}" rotation="{4}" flip="{5},{6}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowany_obiekt(x=0, y=0, tekstura=domyślny_animowany_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, czas_klatki=1, przesunięcie=0, zwróć_tekst=False):
    s = ""
    s = s + """<animated_object position="{0},{1}" texture="{2}" height="{3}" rotation="{4}" flip="{5},{6}" frame_time="{7}" offset="{8}"/>""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y,czas_klatki,przesunięcie)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def ruchomy_obiekt(x=0, y=0, tekstura=domyślny_obiekt, wysokość=1, warstwa=3, rotacja=0, odbicie_x=1, odbicie_y=1, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), zwróć_tekst=False):
    s = ""
    s = s + """<moving_object position="{0},{1}" texture="{2}" height="{3}" layer="{4}" rotation="{5}" flip="{6},{7}">""".format(x,y,tekstura,wysokość,warstwa,rotacja,odbicie_x,odbicie_y)
    s = s + """
    <linear_move>"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </linear_move>"""
    s = s + """
    </moving_object>
    """
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def wahadło(x=0, y=0, tekstura=domyślna_tekstura, lina=domyślny_obiekt, długość=5, wychylenie=30, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, początek_liny=0, powierzchnia="none", wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zaczepienia=2, zaczepienia_x=(0,1), zaczepienia_y=(0,1), zwróć_tekst=False):
    s = ""
    s = s + """<pendulum position="{0},{1}" texture="{2}" line="{3}" length="{4}" angle="{5}" layer="{6}" flip="{7},{8}" line_offset="{9}" surface="{10}">""".format(x,y,tekstura,lina,długość,wychylenie,warstwa,rotacja,odbicie_x,odbicie_y,początek_liny,powierzchnia)
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
def ruchoma_platforma(x=0, y=0, tekstura=domyślna_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia="none", znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,powierzchnia)
    s = s + """
    <linear_move>"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </linear_move>"""
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
def ściana(x=0, y=0, tekstura=domyślna_tekstura, warstwa=1, rotacja=0, zwróć_tekst=False):
    s = ""
    s = s + """<wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}"/>""".format(x,y,tekstura,warstwa,rotacja)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ściana(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=1, rotacja=0, czas_klatki=1, przesunięcie=0, zwróć_tekst=False):
    s = ""
    s = s + """<animated_wall position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" frame_time="{5}" offset="{6}"/>""".format(x,y,tekstura,warstwa,rotacja,czas_klatki,przesunięcie)
    if zwróć_tekst:
        return s
    else:
        zapis(s)
def animowana_ruchoma_platforma(x=0, y=0, tekstura=domyślna_animowana_tekstura, warstwa=5, rotacja=0, odbicie_x=1, odbicie_y=1, powierzchnia="none", czas_klatki=1, przesunięcie=0, znaczniki=2, znaczniki_x=(0,1), znaczniki_y=(0,1), znaczniki_t=(60,60), wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
    s = ""
    s = s + """<animated_moving_platform position="{0},{1}" texture="{2}" layer="{3}" rotation="{4}" flip="{5},{6}" surface="{7}" frame_time="{8}" offset="{9}">""".format(x,y,tekstura,warstwa,rotacja,odbicie_x,odbicie_y,czas_klatki,przesunięcie)   
    s = s + """
    <linear_move>"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </linear_move>"""
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
def bariera(x=0, y=0, powierzchnia="none", wierzchołki=4, wierzchołki_x=(0,1,1,0), wierzchołki_y=(0,0,1,1), zwróć_tekst=False):
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
    <linear_move>"""
    for i in range(znaczniki):
        s = s + """
        <p>{0},{1},{2}</p>""".format(znaczniki_x[i],znaczniki_y[i],znaczniki_t[i])
    s = s + """
    </linear_move>"""
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