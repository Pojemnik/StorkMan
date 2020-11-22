from level_base import level

lvl = level('Poziom testowy python')

import podstawy
podstawy.lvl = lvl
import operacje
import pułapki
import struktury
import mechanizmy
import obiekty
import chłodnia_kominowa

podstawy.platforma(x=0,y=95,wierzchołki=["v",4,0,0,10,0,10,5,0,5],tekstura="dirt,0")
struktury.komnata(x=10,y=80,wysokość=20,szerokość=43,otwory_lewe=[2,1,5,11,5],otwory_górne=[1,1,38],grubość_lewej=1,grubość_prawej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="concrete,0")
podstawy.platforma(x=53,y=95,wierzchołki=["v",4,0,0,47,0,47,5,0,5],tekstura="dirt,0")
podstawy.platforma(x=11,y=87,wierzchołki=["v",4,0,0,0,2,41,2,41,0],tekstura="concrete,0")
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=12,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=17,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=22,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=27,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=32,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=37,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=42,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=47,y=99)

lvl.save('level.xml')