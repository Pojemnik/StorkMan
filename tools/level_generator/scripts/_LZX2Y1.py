from level_base import level

lvl = level('LZX2Y1')

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
podstawy.zdarzenie("Room1Entered")
podstawy.zdarzenie("Room1Left")

#stałe
tekstura_ścian_tylnych = "wall0"
tekstura_ścian_przednich = "bricks,3"
tekstura_murów = "bricks,3"
powierzchnia_murów = "posadzka_hala"
#lochy - drabina na trzeci poziom (pomieszczenie 1)
podstawy.platforma(x=49.5,y=0,wierzchołki=["v",4,0,0,0.5,0,0.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=49.5,y=4,wierzchołki=["v",4,0,0,0.5,0,0.5,6.5,0,6.5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=46.5,y=0,wierzchołki=["v",4,0,0,1,0,1,8,0,8],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=46.5,y=10.5,wierzchołki=["v",4,0,0,1,0,1,3,0,3],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=46.5,y=16,wierzchołki=["v",4,0,0,3.5,0,3.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=49.5,y=10.5,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
struktury.drabina(x=48.5,y=0,szerokość=1.5,długość=16,grubość_poprzeczki=0.4,tekstura_poprzeczek="TEXSteel_stick_rusted",tekstura_szczebli="TEXSteel_stick_rusted",warstwa=4,szczeble=32,szczeble_nad_poprzeczkami=False,powierzchnia="metal_rura",kolor_szczebli=[100,100,220],kolor_poprzeczek=[128,128,255])
podstawy.ściana(x=47,y=0,wierzchołki=["v",4,0,0,3,0,3,16,0,16],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=47,y=0,wierzchołki=["v",4,0,0,3,0,3,16,0,16],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[2,"default","Room1Entered"],[1,"Room1Left"])
podstawy.strefa_zdarzeń(x=49,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",1,"Room1Entered"])
podstawy.strefa_zdarzeń(x=49,y=13.5,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",1,"Room1Entered"])
podstawy.strefa_zdarzeń(x=47,y=13.5,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",1,"Room1Entered"])
podstawy.strefa_zdarzeń(x=47,y=8,wierzchołki=["v",4,0,0,0.5,0,0.5,2.5,0,2.5],tylko_gracz=True,zdarzenia=["e",1,"Room1Entered"])

lvl.save('LZX2Y1.xml')