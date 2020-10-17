from level_base import level

lvl = level('Poziom testowy python')

import podstawy
podstawy.lvl = lvl
import operacje
operacje.lvl = lvl
import pułapki
pułapki.lvl = lvl
import struktury
struktury.lvl = lvl
import mechanizmy
mechanizmy.lvl = lvl
import obiekty
obiekty.lvl = lvl

struktury.komnata(szerokość=20,wysokość=20)
mechanizmy.taśmociąg_jednostronny(x=5,y=10)
s = mechanizmy.taśmociąg_jednostronny(x=5,y=0,zwróć_tekst=True)
s = operacje.dodanie_ruchu(s,[2,0,0,300,0,20,0],zwróć_tekst=True)
operacje.dodanie_ruchu(s,[2,-5,0,120,5,0,120])

lvl.save('level.xml')