from level_base import level

lvl = level('Elektrownia_Weglowa_X1Y1')

import podstawy
podstawy.lvl = lvl
import operacje
import pułapki
import struktury
import mechanizmy
import obiekty
import chłodnia_kominowa
import ruchy
import elektrownia_węglowa
import grafiki

#zdarzenia

#ziemia
podstawy.platforma(x=0,y=47,wierzchołki=["v",4,0,0,0,3,50,3,50,0],tekstura="dirt,0")

lvl.save('Elektrownia_Weglowa_X1Y1.xml')