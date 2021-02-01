from level_base import level

lvl = level('RZX2Y2')

import podstawy
podstawy.lvl = lvl
import operacje
import pułapki
import struktury
import mechanizmy
import obiekty
import ruchy
import grafiki

#zdarzenia

#ziemia
podstawy.platforma(x=0,y=49,wierzchołki=["v",4,0,0,23,0,23,1,0,1],tekstura="bricks,3",powierzchnia="posadzka_hala")
podstawy.platforma(x=27,y=49,wierzchołki=["v",4,0,0,23,0,23,1,0,1],tekstura="bricks,3",powierzchnia="posadzka_hala")

lvl.save('RZX2Y2.xml')