from level_base import level

lvl = level('Poziom testowy python')

import podstawy
podstawy.lvl = lvl
import operacje
import pułapki
import struktury
import mechanizmy
import obiekty

struktury.komnata()
struktury.schody(x=0.5,y=9.5)
pułapki.spadający_obiekt(x=4,y=8)
pułapki.spadający_obiekt(x=6,y=8)
mechanizmy.taśmociąg_jednostronny(x=2,y=8,długość=6)
s = podstawy.ruchoma_platforma(x=5,y=10,ruch=["container",2,"linear",3,0,0,60,0,3,60,-1,-1,-1,"linear",2,0,0,10,1,1,10],zwróć_tekst=True)
s = operacje.dodanie_ruchu(s,["rotational",3,0,20,120,20,240,20],True)
#podstawy.zapis(s)

lvl.save('level.xml')