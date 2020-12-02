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
struktury.element_podłużny(x=3,y=95,warstwa=9,tekstura="TEXSteel_stick_rusted",trasa=[6,0,0,2,0,2,-2,0,-2,0,-4,2,-4],kolizja=True)
struktury.element_podłużny(x=5,y=96,warstwa=9,tekstura="TEXHydro_pipe",trasa=[3,0,0,1,1,0,2],kolizja=True)
struktury.element_podłużny(x=7,y=96,warstwa=9,tekstura="TEXHydro_pipe",trasa=[3,0,0,2,1,0,2],kolizja=True)
struktury.element_podłużny(x=9,y=96,warstwa=9,tekstura="TEXHydro_pipe_rusted",trasa=[3,0,0,3,1,0,2],kolizja=True)
struktury.element_podłużny(x=7,y=94,warstwa=9,tekstura="TEXSteel_stick",trasa=[4,0,0,1,1,2,0,3,1])
struktury.element_podłużny(x=7,y=94,warstwa=9,tekstura="TEXSteel_stick",trasa=[9,0,0,-1,-1,2,-2,3,-6,0,-8,-2,-6,-6,-6,-6,-4,-8,-4],kolizja=True)
struktury.element_podłużny(x=7,y=94,warstwa=8,tekstura="TEXElectricity_cable_red",grubość=0.1,szerokość_tekstury=32,trasa=[9,0,0,-1.5,-1.5,2,-2,3.5,-6.5,0.5,-8.5,-2,-6,-6.5,-6.5,-6,-4,-8.5,-4.5])
struktury.element_podłużny(x=7,y=94,warstwa=8,tekstura="TEXElectricity_cable_green",grubość=0.1,szerokość_tekstury=32,trasa=[9,0,0,-1,-1,2.5,-2.5,3.5,-6.5,0,-8,-2.5,-6.5,-6,-6,-6.5,-4.5,-8,-4])

lvl.save('level.xml')