from level_base import level

lvl = level('Elektrownia_Weglowa_X1Y2')

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
podstawy.platforma(x=0,y=47,wierzchołki=["v",4,0,0,0,3,13.5,3,13.5,0],tekstura="dirt,0")
podstawy.platforma(x=13.5,y=49,wierzchołki=["v",4,0,0,0,1,23.5,1,23.5,0],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=39.5,y=49,wierzchołki=["v",4,0,0,0,1,10.5,1,10.5,0],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
#chłodnia kominowa
struktury.element_podłużny(x=25,y=0,tekstura="concrete,0",grubość=3,warstwa=7,szerokość_tekstury=192,kolor=[128,128,128],trasa=[10,-0.1,-1,-0.125,0,-0.5,4,-1.125,8,-2,12,-3.125,16,-4.5,20,-6.125,24,-8,28,-10.125,32],kolizja=True)
podstawy.platforma(x=13.5,y=46,wierzchołki=["v",4,0,0,0,3,0.5,3,0.5,0],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=13.5,y=41,wierzchołki=["v",4,0,0,0,1,36.5,1,36.5,0],warstwa=8,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=13.5,y=31,wierzchołki=["v",4,0,0,3.5,0,1,6,0,6],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.ściana(x=14,y=0,wierzchołki=["v",9,36,0,10.5,0,10,5.5,8.5,12.5,7,17.5,4.5,24,0,32.5,0,49,36,49],tekstura="concrete,0")
podstawy.platforma(x=12.5,y=41,tekstura="rivets,0",wierzchołki=["v",4,2,0,-0.75,0,-0.75,0.25,2,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=12,y=41.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=11.5,y=42,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=11,y=42.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=10.5,y=43,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=10,y=43.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=9.5,y=44,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=9,y=44.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=8.5,y=45,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=8,y=45.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=7.5,y=46,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=7,y=46.5,tekstura="rivets,0",wierzchołki=["v",4,0,0,-0.75,0,-0.75,0.25,0,0.25],jednostronna=True,R=192,G=192,B=192,warstwa=7)
podstawy.platforma(x=12.5,y=41,tekstura="panels,1",wierzchołki=["v",6,0,0,-6,6,-5.85,6,0,0.15,1,0.2,1,0],jednostronna=True,R=128,G=128,B=128,warstwa=7)
podstawy.ściana(x=13.5,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,0,0,256,0,-1,0,256,256,-1,1,0,256,0,1,0,0])
podstawy.ściana(x=13.5,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,-1,0,256,256,-7,6,256,1792,-7,7,0,1792,-1,1,0,256])
podstawy.ściana(x=13.5,y=40,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,0,0,256,0,36.5,0,256,9344,36.5,1,0,9344,0,1,0,0])
#chłodnia kominowa - dolny poziom
chłodnia_kominowa.słup_chłodniczy_potrójny(x=16.5,y=49,wysokość=7,rozstaw=2.5)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=21.5,y=49,wysokość=7,rozstaw=2.5)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=29,y=49,wysokość=7,rozstaw=2.5,odbicie=-1)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=34,y=49,wysokość=7,rozstaw=2.5,odbicie=-1)
chłodnia_kominowa.słup_chłodniczy_potrójny(x=42.5,y=49,wysokość=7,rozstaw=2.5)
chłodnia_kominowa.słup_chłodniczy_poczwórny(x=47.5,y=49,wysokość=7,rozstaw=2.5)
#chłodnia kominowa - stelaż kanału spalin
struktury.element_podłużny(x=38.25,y=22.25,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,trasa=[2,0,0,-3,3],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=40,y=23.5,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,trasa=[2,0,0,0,1.5],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=49,y=22,tekstura="TEXSteel_stick",grubość=3,warstwa=8,trasa=[2,0,0,1,0])
struktury.element_podłużny(x=38.25,y=18.5,tekstura="TEXVent_pipe",grubość=3,warstwa=8,kolor=[192,192,192],trasa=[4,0,0.5,0,2,1.5,3.5,10.75,3.5])
struktury.element_podłużny(x=37,y=18.5,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,kolor=[128,128,128],trasa=[4,0,0.5,0,2.5,2.25,4.75,13,4.75],kolizja=True)
struktury.element_podłużny(x=39.5,y=18.5,tekstura="TEXSteel_stick",grubość=0.5,warstwa=7,kolor=[128,128,128],trasa=[4,0,0.5,0,1.5,0.75,2.25,10.5,2.25],kolizja=True)
podstawy.platforma(x=35,y=25,wierzchołki=["v",4,0,0,6.5,0,6.5,1,0,1],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=35,y=29,wierzchołki=["v",4,0,0,6.5,0,6.5,1,0,1],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=35,y=33,wierzchołki=["v",4,0,0,6.5,0,6.5,1,0,1],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=35,y=37,wierzchołki=["v",4,0,0,6.5,0,6.5,1,0,1],warstwa=7,tekstura="concrete,0",R=128,G=128,B=128)
podstawy.platforma(x=35,y=25,wierzchołki=["v",4,0,0,1,0,1,24,0,24],warstwa=7,tekstura="concrete,0",R=192,G=192,B=192,jednostronna=True)
podstawy.platforma(x=40.5,y=25,wierzchołki=["v",4,0,0,1,0,1,24,0,24],warstwa=7,tekstura="concrete,0",R=192,G=192,B=192,jednostronna=True)
podstawy.ściana(x=35,y=24,warstwa=8,tekstura="TEXSteel_guardrail",wierzchołki=["vt",4,0,0,256,0,6.5,0,256,1664,6.5,1,0,1664,0,1,0,0])

lvl.save('Elektrownia_Weglowa_X1Y2.xml')