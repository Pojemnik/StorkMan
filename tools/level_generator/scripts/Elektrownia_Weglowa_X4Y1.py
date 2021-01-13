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

lvl.save('Elektrownia_Weglowa_X1Y1.xml')