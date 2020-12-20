from level_base import level

lvl = level('Elektrownia_Weglowa_X3Y2')

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

#ziemia
podstawy.platforma(x=0,y=47,wierzchołki=["v",4,0,0,0,3,50,3,50,0],tekstura="dirt,0")
#budynek generatora
struktury.komnata(x=6.5,y=0,warstwa=8,kolor=[128,128,128],szerokość=43.5,wysokość=48,grubość_prawej=1,grubość_lewej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="bricks,0",tekstura_tła="asphalt,0",otwory_lewe=[2,27.5,3,43,3])
#budynek generatora - parter
elektrownia_węglowa.generator(x=9.5,y=47.1,warstwa=5,turbiny=[3,"TURBINA_04","TURBINA_05","TURBINA_06"])
struktury.komnata(x=41.5,y=39.5,warstwa=8,kolor=[128,128,128],szerokość=7.5,wysokość=7.5,grubość_prawej=1,grubość_lewej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="bricks,0",tło=False,otwory_lewe=[1,3.5,2])
struktury.schody(x=41.5,y=47,obrót=-1,wysokość=1,długość=1.5,stopnie=3,przesunięcie=0.5,kolor=[128,128,128])
struktury.element_podłużny(x=14.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=18.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=23.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=27.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=32.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=36.5,y=40.5,warstwa=4,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=8,y=40.5,warstwa=7,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[3,0,0,0,1.5,-1,2.5])
struktury.element_podłużny(x=12,y=40.5,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=21,y=40.5,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=30,y=40.5,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=39,y=40.5,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=12.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=12.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=16.7,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=15.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=18.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=21.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=24.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=20.9,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=25.1,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=28.1,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=27.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=30.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=33.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=32.3,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=36.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[9,-0.5,-0.5,0,0,0.7,0.4,1.4,0.6,2.1,0.7,2.8,0.6,3.5,0.4,4.2,0,4.7,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=36.5,y=40.5,warstwa=7,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[9,-0.5,-0.5,0,0,0.5,0.20,1,0.33,1.5,0.4,2,0.33,2.5,0.20,3,0,3.5,-0.5],szerokość_tekstury=32)
podstawy.obiekt(x=12.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
podstawy.obiekt(x=19.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
podstawy.obiekt(x=21.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
podstawy.obiekt(x=28.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
podstawy.obiekt(x=30.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
podstawy.obiekt(x=37.25,y=41,wysokość=1.5,tekstura="krata0",warstwa=4)
grafiki.świetlówka(x=15,y=40.5,warstwa=4,długość=3)
grafiki.świetlówka(x=19.5,y=40.5,warstwa=4,długość=3)
grafiki.świetlówka(x=24,y=40.5,warstwa=4,długość=3)
grafiki.świetlówka(x=28.5,y=40.5,warstwa=4,długość=3)
grafiki.świetlówka(x=33,y=40.5,warstwa=4,długość=3)
#budynek generatora - parter - stróżówka
struktury.element_podłużny(x=42.5,y=30.5,warstwa=7,tekstura="TEXVent_pipe",grubość=0.8,trasa=[10,0,-2,0,8.5,0.5,9,3,9,3.5,9.5,3.5,10,3.5,10.5,4,11.5,5,12,5.5,12])
podstawy.platforma(x=42.5,y=43,warstwa=8,wierzchołki=["v",4,0,0,0,1,1.5,1,1.5,0],tekstura="bricks,0",R=128,G=128,B=128)
#budynek generatora - pierwszy poziom techniczny
podstawy.platforma(x=7.5,y=39.5,wierzchołki=["v",4,0,0,0,1,1,1,1,0],tekstura="panels,2",powierzchnia="metal",warstwa=8,R=128,G=128,B=128)
podstawy.platforma(x=11.5,y=39.5,wierzchołki=["v",4,0,0,0,1,30,1,30,0],tekstura="panels,1",powierzchnia="metal",warstwa=8,R=128,G=128,B=128)
podstawy.platforma(x=8,y=39.7,wierzchołki=["v",4,0,0,0,0.6,4,0.6,4,0],tekstura="panels,0",powierzchnia="metal",warstwa=7)
struktury.element_podłużny(x=8,y=36,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=12,y=36,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=21,y=36,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=30,y=36,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=39,y=36,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=14.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=18.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=23.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=27.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=32.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=36.5,y=36,warstwa=5,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,3.5])
struktury.element_podłużny(x=12.5,y=36,warstwa=5,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[2,0,0,0,3.5],szerokość_tekstury=32)
struktury.element_podłużny(x=39.5,y=36,warstwa=6,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[2,0,0,0,3.5],szerokość_tekstury=32)
struktury.element_podłużny(x=40.7,y=36,warstwa=5,tekstura="TEXElectricity_cable_yellow",grubość=0.1,trasa=[2,0,0,0,3.5],szerokość_tekstury=32)
struktury.element_podłużny(x=7.5,y=36.5,warstwa=9,tekstura="TEXHydro_pipe_rusted",grubość=1.1,trasa=[2,0,0,41.5,0],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=42,y=29,warstwa=9,tekstura="TEXHydro_pipe_rusted",grubość=1.1,trasa=[7,7,0,3,0,1.5,0.5,1,1,0.5,1.5,0,3,0,7],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=9.5,y=32.2,warstwa=8,tekstura="TEXVent_pipe",grubość=0.8,trasa=[7,2,-4.5,2,-2,0,1,0,2.5,1,3.5,32,3.5,32.5,3],kolizja=True)
struktury.element_podłużny(x=7.5,y=38.25,warstwa=4,tekstura="TEXHydro_pipe",grubość=1.5,trasa=[2,0,0,41.5,0],kolor=[192,192,192])
struktury.element_podłużny(x=7.5,y=37.25,warstwa=5,tekstura="TEXSteel_stick",grubość=0.8,trasa=[2,0,0,41.5,0])
struktury.element_podłużny(x=7.5,y=39.25,warstwa=5,tekstura="TEXSteel_stick",grubość=0.8,trasa=[2,0,0,41.5,0])
struktury.element_podłużny(x=7.5,y=37.55,warstwa=5,tekstura="TEXElectricity_cable_green",grubość=0.08,trasa=[2,0,0,41.5,0],szerokość_tekstury=32)
struktury.element_podłużny(x=7.5,y=37.65,warstwa=5,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[2,0,0,41.5,0],szerokość_tekstury=32)
struktury.element_podłużny(x=43,y=28.5,warstwa=4,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[2,0,0,0,7.5],kolizja=True)
struktury.element_podłużny(x=46.5,y=32,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,2.5,0],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=46.5,y=28.5,warstwa=8,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,11])
struktury.schody(x=49,y=39.5,warstwa=5,wysokość=3.5,długość=3.5,stopnie=8,tekstura="bricks,0",obrót=-1)
grafiki.świetlówka(x=15,y=37.5,warstwa=4,długość=3)
grafiki.świetlówka(x=24,y=37.5,warstwa=4,długość=3)
grafiki.świetlówka(x=33,y=37.5,warstwa=4,długość=3)
podstawy.dźwięk(x=10,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=22,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=34,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=46,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
początek_emisji = podstawy.animowany_obiekt(x=-0.5,y=-0.4,wysokość=1,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,zwróć_tekst=True)
koniec_emisji = podstawy.animowany_obiekt(x=-1.25,y=-1.5,wysokość=2.5,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,zwróć_tekst=True)
emitery = ""
emitery += pułapki.emiter(x=13,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=20,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=22,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=29,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=31,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=38,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
emitery += pułapki.emiter(x=40,y=37,warstwa=7,długość=2.5,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
operacje.element_potomny(emitery,"ruch",["linear",3,0,0,120,0,0,60,-1,-1,-1])
koniec_emisji = ""
koniec_emisji += podstawy.animowany_obiekt(x=-1,y=-1.1,wysokość=2.5,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,czas_klatki=1,zwróć_tekst=True)
koniec_emisji += podstawy.animowany_obiekt(x=-0.875,y=-0.975,wysokość=2.25,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,czas_klatki=0.7,zwróć_tekst=True)
koniec_emisji += podstawy.animowany_obiekt(x=-0.75,y=-0.85,wysokość=2,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,czas_klatki=0.4,zwróć_tekst=True)
koniec_emisji += podstawy.strefa_obrażeń(x=0.25,y=0.15,wierzchołki=obiekty.wielokąt_foremny(n=6,bok=1,początek=0),obrażenia=["d",2,15,6,0,0],zwróć_tekst=True)
koniec_emisji = operacje.element_potomny(koniec_emisji,"ruch",ruchy.wstrząsy(0.1,7,0,10),True)
pułapki.emiter(x=42.5,y=31.5,warstwa=7,długość=2.5,zakończenie=1,kąt_emisji=25,obrót=30,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji)
podstawy.obiekt(x=43.75,y=30.25,wysokość=2,tekstura="krata0",warstwa=4)
#budynek generatora - drugi poziom techniczny
podstawy.platforma(x=41.5,y=28,tekstura="panels,0",wierzchołki=["v",4,0,0,0,0.5,5.5,0.5,5.5,0],R=192,B=192,G=192,warstwa=7)
struktury.element_podłużny(x=42,y=28.5,warstwa=6,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,7],kolor=[192,192,192],kolizja=True)
struktury.element_podłużny(x=41,y=21.5,warstwa=9,tekstura="TEXHydro_pipe_rusted",grubość=1.1,trasa=[5,0,0.5,0,2,0.5,3,1.5,3.5,8,3.5],kolizja=True)
struktury.element_podłużny(x=43,y=24.5,warstwa=8,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[2,0,0,0,4])
struktury.element_podłużny(x=46.5,y=25.5,warstwa=7,tekstura="TEXElectricity_cable_yellow",grubość=0.07,trasa=[9,-0.5,-0.5,0,0,0.4,0.20,0.8,0.33,1.2,0.4,1.6,0.33,2,0.20,2.4,0,3.5,-0.5],szerokość_tekstury=32)
struktury.element_podłużny(x=43.5,y=25.5,warstwa=8,tekstura="TEXElectricity_cable_blue",grubość=0.1,trasa=[3,0,0,-0.6,0.5,-0.6,3],szerokość_tekstury=64)
struktury.element_podłużny(x=43.5,y=25.5,warstwa=6,tekstura="TEXElectricity_cable_blue",grubość=0.1,trasa=[2,-0.6,3,-0.6,4],szerokość_tekstury=64)
struktury.element_podłużny(x=42.5,y=24.5,warstwa=8,tekstura="TEXVent_pipe",grubość=0.8,trasa=[2,0,0,0,4])
grafiki.świetlówka(x=43.5,y=25.5,warstwa=4,długość=3)
początek_emisji = podstawy.animowany_obiekt(x=-0.5,y=-0.4,wysokość=1,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,zwróć_tekst=True)
koniec_emisji = podstawy.animowany_obiekt(x=-1.25,y=-1.5,wysokość=2.5,tekstura="AOBSteam_Cloud_Concentrated",warstwa=7,zwróć_tekst=True)
emitery = ""
emitery += pułapki.emiter(x=48,y=25.5,warstwa=7,długość=5,zakończenie=0.5,kąt_emisji=15,obrót=90,początek_emisji=początek_emisji,koniec_emisji=koniec_emisji,zwróć_tekst=True)
operacje.element_potomny(emitery,"ruch",["linear",3,0,0,100,0,0,100,-1,-1,-1])
struktury.element_podłużny(x=11.875,y=28,warstwa=7,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,7.5],kolor=[192,192,192])
struktury.schody(x=7.5,y=28,wysokość=1,długość=6,stopnie=2,przesunięcie=5,tekstura="bricks,0",kolor=[128,128,128],warstwa=8)
podstawy.platforma(x=7.5,y=27,tekstura="bricks,0",wierzchołki=["v",4,0,0,0,1,5,1,5,0],R=128,B=128,G=128,warstwa=7)
#budynek generatora - drugi poziom techniczny - wieże_instalacyjne
podstawy.platforma(x=29.5,y=22,tekstura="concrete,0",wierzchołki=["v",4,0,0,6,0,5,1,1,1],R=196,B=196,G=196,warstwa=7)
podstawy.platforma(x=30.5,y=27.5,tekstura="panels,0",wierzchołki=["v",4,0,0,0,1.5,4,1.5,4,0],R=196,B=196,G=196,warstwa=7)
podstawy.platforma(x=30.5,y=23,tekstura="panels,2",wierzchołki=["v",4,0,0,0,0.5,4,0.5,4,0],R=196,B=196,G=196,warstwa=7)
podstawy.ściana(x=31,y=23.5,tekstura="panels,1",wierzchołki=["v",4,0,0,0,4,3,4,3,0],R=128,B=128,G=128,warstwa=5)
struktury.element_podłużny(x=31.5,y=29,warstwa=5,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=33.5,y=29,warstwa=5,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=32.5,y=29,warstwa=5,tekstura="TEXHydro_pipe_rusted",grubość=1.1,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=31.7,y=29,warstwa=5,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[4,0,-5.5,0,3.5,-1,4.5,-1,6.5],kolor=[192,192,192])
struktury.element_podłużny(x=33.3,y=29,warstwa=5,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[4,0,-5.5,0,3.5,1,4.5,1,6.5],kolor=[192,192,192])
podstawy.obiekt(x=31,y=27.75,wysokość=1,tekstura="krata0",warstwa=9)
podstawy.obiekt(x=33,y=27.75,wysokość=1,tekstura="krata0",warstwa=9)
grafiki.świetlówka(x=31,y=23.5,warstwa=7,długość=3)
podstawy.platforma(x=18.5,y=22,tekstura="concrete,0",wierzchołki=["v",4,0,0,6,0,5,1,1,1],R=196,B=196,G=196,warstwa=7)
podstawy.platforma(x=19.5,y=27.5,tekstura="panels,0",wierzchołki=["v",4,0,0,0,1.5,4,1.5,4,0],R=196,B=196,G=196,warstwa=7)
podstawy.platforma(x=19.5,y=23,tekstura="panels,2",wierzchołki=["v",4,0,0,0,0.5,4,0.5,4,0],R=196,B=196,G=196,warstwa=7)
podstawy.ściana(x=20,y=23.5,tekstura="panels,1",wierzchołki=["v",4,0,0,0,4,3,4,3,0],R=128,B=128,G=128,warstwa=5)
struktury.element_podłużny(x=20.5,y=29,warstwa=5,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=22.5,y=29,warstwa=5,tekstura="TEXSteel_stick",grubość=0.75,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=21.5,y=29,warstwa=5,tekstura="TEXHydro_pipe_rusted",grubość=1.1,trasa=[2,0,0,0,6.5])
struktury.element_podłużny(x=20.7,y=29,warstwa=5,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[4,0,-5.5,0,3.5,-1,4.5,-1,6.5],kolor=[192,192,192])
struktury.element_podłużny(x=22.3,y=29,warstwa=5,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[4,0,-5.5,0,3.5,1,4.5,1,6.5],kolor=[192,192,192])
podstawy.obiekt(x=20,y=27.75,wysokość=1,tekstura="krata0",warstwa=9)
podstawy.obiekt(x=22,y=27.75,wysokość=1,tekstura="krata0",warstwa=9)
grafiki.świetlówka(x=20,y=23.5,warstwa=7,długość=3)
#budynek generatora - przejście rurami zewnętrznymi
podstawy.ściana(x=6.5,y=28.5,tekstura="bricks,0",wierzchołki=["v",4,0,0,0,3,1,3,1,0],R=128,B=128,G=128,warstwa=8)
struktury.element_podłużny(x=4.5,y=13,warstwa=7,tekstura="TEXSteel_stick",grubość=3,trasa=[6,4,0,1.5,0,0,1.5,0,15.5,1.5,17,4,17])

lvl.save('level.xml')