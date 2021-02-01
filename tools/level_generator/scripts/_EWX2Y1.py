from level_base import level

lvl = level('EWX2Y1')

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

#chłodnia kominowa
struktury.element_podłużny(x=1.5,y=50,tekstura="concrete,0",grubość=3,szerokość_tekstury=192,kolor=[128,128,128],trasa=[8,0.125,0.15,0,-4,0.125,-8,0.5,-12,1.125,-16,2,-20,3.125,-24,4.5,-28],kolizja=True)
podstawy.ściana(x=0,y=21.5,wierzchołki=["v",3,0,0,4.6,0,0,28.5],tekstura="concrete,0")

lvl.save('EWX2Y1.xml')