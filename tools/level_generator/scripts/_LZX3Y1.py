from level_base import level

lvl = level('LZX3Y1')

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
podstawy.zdarzenie("Room2Entered")
podstawy.zdarzenie("Room2Left")
podstawy.zdarzenie("Room2FloorDoorOpen")
podstawy.zdarzenie("Room2FloorDoorClose")
podstawy.zdarzenie("Room3Entered")
podstawy.zdarzenie("Room3Left")
podstawy.zdarzenie("Room4Entered")
podstawy.zdarzenie("Room4Left")
podstawy.zdarzenie("Corridor1Entered")
podstawy.zdarzenie("Corridor1Left")
podstawy.zdarzenie("Corridor1Room4Entered")
podstawy.zdarzenie("Corridor1Room4Left")

#stałe
tekstura_ścian_tylnych = "wall0"
tekstura_ścian_przednich = "bricks,3"
tekstura_murów = "bricks,3"
powierzchnia_murów = "posadzka_hala"
#lochy - schody na pierwszy poziom (pomieszczenie 1)
struktury.schody(x=27,y=4,wysokość=5,długość=5,obrót=-1,tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,blokada=True,stopnie=10,warstwa=8)
podstawy.platforma(x=27,y=0,wierzchołki=["v",4,0,0,1,0,1,5,0,5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=18.5,y=4,wierzchołki=["v",4,0,0,8.5,0,8.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=18,y=0,wierzchołki=["v",4,0,0,1,0,1,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.ściana(x=18.5,y=0,wierzchołki=["v",4,0,0,9.5,0,9.5,5,0,5],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=18.5,y=0,wierzchołki=["v",4,0,0,9.5,0,9.5,5,0,5],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[2,"default","Room1Entered"],[1,"Room1Left"])
podstawy.strefa_zdarzeń(x=19,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",2,"Room1Entered","Room2Left"])
#lochy - właz na drugi poziom (pomieszczenie 2)
#PRZECIWNIK
struktury.schody(x=18.5,y=6,wysokość=2,długość=2,obrót=-1,tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,blokada=True,stopnie=4,warstwa=8)
podstawy.platforma(x=15.5,y=6,wierzchołki=["v",4,0,0,3,0,3,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
struktury.schody(x=9,y=6,wysokość=2,długość=2,tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,blokada=True,stopnie=4,warstwa=8)
podstawy.platforma(x=9,y=6,wierzchołki=["v",4,0,0,3,0,3,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=8.5,y=0,wierzchołki=["v",4,0,0,1,0,1,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
grodzie = podstawy.platforma(x=12.25,y=6.25,wierzchołki=["v",4,-0.25,-0.25,3.25,-0.25,3.25,0.25,-0.25,0.25],tekstura="rivets,0",powierzchnia="metal_rura",zwróć_tekst=True)
mechanizmy.obracalny_element(grodzie,["Room2FloorDoorOpen","Room2FloorDoorClose"],["Room2FloorDoorOpening","Room2FloorDoorClosing"],120,obrót=-70)
podstawy.ściana(x=9,y=0,wierzchołki=["v",4,0,0,9.5,0,9.5,7,0,7],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=9,y=0,wierzchołki=["v",4,0,0,9.5,0,9.5,6,0,6],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[1,"Room2Entered"],[2,"default","Room2Left"])
podstawy.strefa_zdarzeń(x=17.5,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",2,"Room2Entered","Room1Left"])
podstawy.strefa_zdarzeń(x=9.5,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",2,"Room2Entered","Room3Left"])
#lochy - sterownia włazu na drugi poziom (pomieszczenie 3)
podstawy.platforma(x=0,y=0,wierzchołki=["v",4,0,0,0.5,0,0.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=0,y=4,wierzchołki=["v",4,0,0,0.5,0,0.5,5.5,0,5.5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=9,y=7,wierzchołki=["v",4,0,0,1,0,1,2.5,0,2.5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=0,y=9.5,wierzchołki=["v",4,0,0,10,0,10,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
struktury.element_podłużny(x=9,y=4.1,grubość=0.2,tekstura="TEXSteel_stick_rusted",trasa=[4,0.5,0,-2.5,0,-3,0.5,-3,5.4],kolizja=True,jednostronność=True,warstwa=7,powierzchnia="metal_rura",kolor=[128,128,255])
struktury.element_podłużny(x=0.5,y=4.1,grubość=0.2,tekstura="TEXSteel_stick_rusted",trasa=[4,0,0,2.5,0,3,0.5,3,5.4],kolizja=True,jednostronność=True,warstwa=7,powierzchnia="metal_rura",kolor=[128,128,255])
struktury.element_podłużny(x=0.5,y=4.1,grubość=0.2,tekstura="TEXSteel_stick_rusted",trasa=[2,0,0,8.5,0],kolizja=True,jednostronność=True,warstwa=5,powierzchnia="metal_rura",kolor=[100,100,220])
struktury.drabina(x=4.75,y=4,szerokość=1.2,długość=5.5,tekstura_poprzeczek="TEXSteel_stick_rusted",tekstura_szczebli="TEXSteel_stick_rusted",warstwa=4,szczeble=8,szczeble_nad_poprzeczkami=False,powierzchnia="metal_rura",kolor_szczebli=[100,100,220],kolor_poprzeczek=[128,128,255])
podstawy.ściana(x=0,y=0,wierzchołki=["v",4,0,0,9,0,9,10.5,0,10.5],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=0,y=0,wierzchołki=["v",4,0,0,9,0,9,10.5,0,10.5],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[1,"Room3Entered"],[2,"default","Room3Left"])
podstawy.strefa_zdarzeń(x=8,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",2,"Room3Entered","Room2Left"])
podstawy.strefa_zdarzeń(x=0.5,y=1,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tylko_gracz=True,zdarzenia=["e",2,"Room3Entered","Room2Left"])
#lochy - klatka schodowa na trzeci poziom (pomieszczenie 4)
podstawy.platforma(x=0,y=16,wierzchołki=["v",4,0,0,5.5,0,5.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=0,y=10.5,wierzchołki=["v",4,0,0,0.5,0,0.5,3,0,3],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=9.5,y=10.5,wierzchołki=["v",4,0,0,1,0,1,6.5,0,6.5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=10,y=7,wierzchołki=["v",4,0,0,0.5,0,0.5,3.5,0,3.5],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.ściana(x=0,y=10.5,wierzchołki=["v",4,0,0,10,0,10,6,0,6],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=0,y=10.5,wierzchołki=["v",4,0,0,10,0,10,6,0,6],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[1,"Room4Entered"],[2,"default","Room4Left"])
podstawy.strefa_zdarzeń(x=0.5,y=13.5,wierzchołki=["v",4,0,0,0.5,0,0.5,2.5,0,2.5],tylko_gracz=True,zdarzenia=["e",2,"Room4Entered","Corridor1Room4Entered"])
podstawy.strefa_zdarzeń(x=5.5,y=15,wierzchołki=["v",4,0,0,3,0,3,0.5,0,0.5],tylko_gracz=True,zdarzenia=["e",2,"Corridor1Left","Room4Entered"])
#lochy - korytarz na trzecim poziomie (korytarz 1)
#KILKU PRZECIWNIKÓW
struktury.schody(x=9.5,y=20.5,wysokość=4.5,długość=5,obrót=-1,przesunięcie=0.5,tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,blokada=False,stopnie=9,warstwa=5)
podstawy.platforma(x=0,y=20.5,wierzchołki=["v",4,0,0,23,0,23,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.platforma(x=10.5,y=16,wierzchołki=["v",4,0,0,12.5,0,12.5,1,0,1],tekstura=tekstura_murów,powierzchnia=powierzchnia_murów,warstwa=8)
podstawy.ściana(x=0,y=16.5,wierzchołki=["v",4,0,0,23,0,23,5,0,5],tekstura=tekstura_ścian_tylnych)
ściana = podstawy.ściana(x=0,y=16.5,wierzchołki=["v",4,0,0,10,0,10,5,0,5],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[1,"Corridor1Room4Entered"],[2,"default","Corridor1Room4Left"])
ściana = podstawy.ściana(x=10,y=16.5,wierzchołki=["v",4,0,0,13,0,13,5,0,5],tekstura=tekstura_ścian_przednich,warstwa=9,zwróć_tekst=True)
operacje.znikanie_zdarzeniowe(ściana,[1,"Corridor1Room4Entered"],[2,"default","Corridor1Room4Left"])
podstawy.strefa_zdarzeń(x=5.5,y=18,wierzchołki=["v",4,0,0,3,0,3,0.5,0,0.5],tylko_gracz=True,zdarzenia=["e",2,"Corridor1Entered","Room4Left"])
podstawy.strefa_zdarzeń(x=22,y=17,wierzchołki=["v",4,0,0,0.5,0,0.5,3.5,0,3.5],tylko_gracz=True,zdarzenia=["e",2,"Corridor1Entered","Corridor1Room4Entered"])

lvl.save('LZX3Y1.xml')