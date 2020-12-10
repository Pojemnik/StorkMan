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
import ruchy
import elektrownia_węglowa

elektrownia_węglowa.generator(x=12,y=15.5,warstwa=11)
struktury.schody(x=15,y=15.5,wysokość=4,długość=4,stopnie=8,tekstura="bricks,0",blokada=False)
struktury.schody(x=22,y=15.5,wysokość=4,długość=4,stopnie=8,tekstura="paving,0")
podstawy.platforma(x=0,y=95,wierzchołki=["v",4,0,0,10,0,10,5,0,5],tekstura="dirt,0")
podstawy.platforma(x=53,y=95.5,wierzchołki=["v",4,0,0,47,0,47,4.5,0,4.5],tekstura="dirt,0")
podstawy.platforma(x=11,y=87,wierzchołki=["v",4,0,0,0,2,41,2,41,0],tekstura="concrete,0")
podstawy.ściana(x=60,y=75,wierzchołki=["v",4,0,0,0,20,40,20,40,0],tekstura="asphalt,0")
struktury.komnata(x=10,y=80,wysokość=20,szerokość=43,otwory_lewe=[2,1,5,11,5],otwory_prawe=[2,1,5,11,5],otwory_górne=[1,1,38],grubość_lewej=1,grubość_prawej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="concrete,0")
struktury.komnata(x=60,y=75,warstwa=9,wysokość=20.5,szerokość=40,otwory_lewe=[1,16,3],otwory_prawe=[1,16,3],grubość_lewej=1,grubość_prawej=1,grubość_górnej=1,grubość_dolnej=0.5,tekstura="bricks,0")
struktury.element_podłużny(x=61,y=80,warstwa=9,grubość=0.75,tekstura="TEXHydro_pipe_rusted",trasa=[6,0,0,3,0,5,1,6,2,7,4,7,15])
struktury.element_podłużny(x=61,y=82,warstwa=6,grubość=0.75,tekstura="TEXHydro_pipe_rusted",trasa=[2,0,0,35,0],kolizja=True)
struktury.element_podłużny(x=69.5,y=76,warstwa=5,grubość=0.6,tekstura="TEXVent_pipe",trasa=[8,0,0,0,16,1,16.5,2,17,5,17,6,17.5,7,18,7,19])
struktury.element_podłużny(x=68.5,y=76,warstwa=4,grubość=0.1,tekstura="TEXElectricity_cable_red",szerokość_tekstury=32,trasa=[6,0,0,0,4,-2,4,-2,6,0,6,0,19])
struktury.element_podłużny(x=68.7,y=76,warstwa=5,grubość=0.1,tekstura="TEXElectricity_cable_yellow",trasa=[6,0,0,0,4.4,-2.4,4.4,-2.4,5.6,0,5.6,0,19])
struktury.element_podłużny(x=65,y=95,warstwa=8,grubość=0.5,tekstura="TEXSteel_stick_rusted",trasa=[8,0,0,0,-2,0.5,-2.5,1,-3,3,-3,3.5,-2.5,4,-2,4,0],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=96,y=76,warstwa=8,grubość=0.5,tekstura="TEXSteel_stick_rusted",trasa=[8,0,0,0,17,-0.5,17,-1,17.5,-3,17.5,-3.5,18,-4,18.5,-4,19])
struktury.element_podłużny(x=99,y=91,warstwa=6,grubość=0.5,tekstura="TEXVent_pipe",trasa=[6,0,0,-1.5,0,-1.5,-1,-1,-1,-0.75,-1.25,-0.75,-15],kolizja=True)
struktury.element_podłużny(x=98,y=76,warstwa=8,grubość=0.5,tekstura="TEXSteel_stick_rusted",trasa=[2,0,0,0,19])
struktury.element_podłużny(x=97.25,y=76,warstwa=9,grubość=0.1,tekstura="TEXElectricity_cable_green",szerokość_tekstury=32,trasa=[5,0,0,0,14.5,-1,14.5,-1,15,1.75,15])
struktury.element_podłużny(x=97.4,y=76,warstwa=7,grubość=0.1,tekstura="TEXElectricity_cable_yellow",szerokość_tekstury=32,trasa=[7,0,0,0,3,-0.4,3,-0.4,7,1,7,1,14.7,1.6,14.7])
struktury.schody(x=98,y=95,obrót=-1,tekstura="concrete,0",blokada=False)
struktury.schody(x=10,y=95,obrót=-1,wysokość=8,długość=8,stopnie=10,tekstura="paving,0",blokada=False)
struktury.schody(x=20,y=87,obrót=-1,wysokość=3,długość=3,stopnie=8,tekstura="concrete,0")
struktury.schody(x=20,y=87,wysokość=3,długość=3,stopnie=8,tekstura="concrete,0")
struktury.schody(x=68,y=95,tekstura="concrete,0",blokada=False)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=12,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=17,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=22,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=27,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=32,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=37,y=99)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=42,y=99)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=47,y=99)
elektrownia_węglowa.generator(x=62,y=95)
podstawy.platforma(x=40,y=75,wierzchołki=["v",4,0,0,0,1,1,1,1,0,],tekstura="concrete,0")
podstawy.platforma(x=40,y=75,wierzchołki=obiekty.wielokąt_foremny(n=7),tekstura="asphalt,0")

lvl.save('level.xml')