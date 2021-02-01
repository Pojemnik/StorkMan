from level_base import level

lvl = level('EWX2Y2')

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
podstawy.zdarzenie("OpenCanalDoor")
podstawy.zdarzenie("CloseCanalDoor")
#ziemia
podstawy.platforma(x=0,y=49,wierzchołki=["v",4,0,0,0,1,13,1,13,0],tekstura="concrete,0",R=128,G=128,B=128,warstwa=7)
podstawy.platforma(x=13,y=47,wierzchołki=["v",4,0,0,0,3,15.5,3,15.5,0],tekstura="dirt,0",powierzchnia="piach")
podstawy.platforma(x=35.5,y=47,wierzchołki=["v",4,0,0,0,3,14.5,3,14.5,0],tekstura="dirt,0",powierzchnia="piach")
#chłodnia kominowa
struktury.element_podłużny(x=1.5,y=0,tekstura="concrete,0",grubość=3,warstwa=9,szerokość_tekstury=192,kolor=[128,128,128],trasa=[10,0.1,-1,0.125,0,0.5,4,1.125,8,2,12,3.125,16,4.5,20,6.125,24,8,28,10.125,32])
struktury.element_podłużny(x=1.5,y=0,tekstura="concrete,0",grubość=3,warstwa=7,szerokość_tekstury=192,kolor=[128,128,128],trasa=[7,0.1,-1,0.125,0,0.5,4,1.125,8,2,12,3.125,16,4.5,20],kolizja=True)
struktury.element_podłużny(x=1.5,y=0,tekstura="concrete,0",grubość=3,warstwa=7,szerokość_tekstury=192,kolor=[128,128,128],trasa=[3,6.125,24,8,28,10.125,32],kolizja=True)
podstawy.platforma(x=12.5,y=46,wierzchołki=["v",4,0,0,0,3,0.5,3,0.5,0],tekstura="concrete,0",warstwa=7,R=128,G=128,B=128)
podstawy.platforma(x=0,y=41,wierzchołki=["v",4,0,0,0,1,13,1,13,0],tekstura="concrete,0",warstwa=8,R=128,G=128,B=128)
podstawy.platforma(x=13,y=31,wierzchołki=["v",4,0,0,0,6,-1,6,-3.5,0],tekstura="concrete,0",warstwa=7,R=128,G=128,B=128)
podstawy.ściana(x=0,y=0,wierzchołki=["v",9,0,0,2,0,2.5,5.5,4,12.5,5.5,17.5,8,24,12.5,32.5,12.5,49,0,49],tekstura="concrete,0")
podstawy.platforma(x=14,y=41,tekstura="rivets,0",wierzchołki=["v",4,-2,0,0.75,0,0.75,0.25,-2,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=14.5,y=41.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=15,y=42,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=15.5,y=42.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=16,y=43,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=16.5,y=43.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=17,y=44,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=17.5,y=44.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=18,y=45,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=18.5,y=45.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=19,y=46,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=19.5,y=46.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,0.75,0,0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=14,y=41,tekstura="panels,1",wierzchołki=["v",6,0,0,6,6,5.85,6,0,0.15,-1,0.2,-1,0],jednostronna=True,R=128,G=128,B=128,warstwa=7)
podstawy.ściana(x=13,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,0,0,256,0,1,0,256,256,1,1,0,256,0,1,0,0])
podstawy.ściana(x=13,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,1,0,256,256,7,6,256,1792,7,7,0,1792,1,1,0,256])
podstawy.ściana(x=0,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,0,0,256,0,13,0,256,3328,13,1,0,3328,0,1,0,0])
#chłodnia kominowa - dolny poziom
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=5,y=49,wysokość=7,rozstaw=2.5,odbicie=-1)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=10,y=49,wysokość=7,rozstaw=2.5,odbicie=-1)
#kanał spalin odsiarczonych
struktury.element_podłużny(x=0,y=22,tekstura="TEXVent_pipe",grubość=3,warstwa=8,kolor=[192,192,192],trasa=[4,0,0,28.5,0,32,3.5,32,28])
struktury.element_podłużny(x=0,y=20.75,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,kolor=[128,128,128],trasa=[4,0,0,29,0,33.25,4.25,33.25,19.25],kolizja=True)
struktury.element_podłużny(x=0,y=23.25,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,kolor=[128,128,128],trasa=[4,0,0,28,0,30.75,2.75,30.75,16.75],kolizja=True)
podstawy.platforma(x=4.5,y=19.5,wierzchołki=["v",3,0.1,1,2.9,0,3.5,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=7)
podstawy.platforma(x=6,y=23.5,wierzchołki=["v",3,-0.2,0,3,0,0.45,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=7)
podstawy.platforma(x=29,y=40,wierzchołki=["v",5,1.5,0,2,0,2,2,0,2,0,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=35,y=40,wierzchołki=["v",5,0,2,-2,2,-2,0,-1.5,0,0,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=28.5,y=47,wierzchołki=["v",4,0,0,0.5,0,0.5,2.5,0,2.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=35,y=47,wierzchołki=["v",4,0,0,0.5,0,0.5,2.5,0,2.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=28.5,y=49.5,wierzchołki=["v",4,0,0,2,0,2,0.5,0,0.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=33.5,y=49.5,wierzchołki=["v",4,0,0,2,0,2,0.5,0,0.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=28.5,y=42,wierzchołki=["v",4,0,0,1,0,1,1.5,0,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
podstawy.platforma(x=34.5,y=42,wierzchołki=["v",4,0,0,1,0,1,1.5,0,1.5],tekstura="concrete,0",R=128,G=128,B=128,warstwa=9)
grodzie = podstawy.platforma(x=29,y=46.5,wierzchołki=["v",4,-0.5,0.5,-0.5,-3,0.5,-3,0.5,0.5],tekstura="panels,1",R=192,G=192,B=192,zwróć_tekst=True,warstwa=9)
mechanizmy.obracalny_element(grodzie,["OpenCanalDoor","CloseCanalDoor"],["CanalDoorOpening","CanalDoorClosing"],czas_obrotu=20,obrót=90)
grodzie = podstawy.platforma(x=35,y=46.5,wierzchołki=["v",4,-0.5,0.5,-0.5,-3,0.5,-3,0.5,0.5],tekstura="panels,1",R=192,G=192,B=192,zwróć_tekst=True,warstwa=9)
mechanizmy.obracalny_element(grodzie,["OpenCanalDoor","CloseCanalDoor"],["CanalDoorOpening","CanalDoorClosing"],czas_obrotu=20,obrót=-90)

lvl.save('EWX2Y2.xml')