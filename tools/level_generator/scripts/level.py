from level_base import level

lvl = level('Elektrownia_Węglowa_X3Y2')

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
struktury.komnata(x=6.5,y=0,warstwa=8,szerokość=43.5,wysokość=48,grubość_prawej=1,grubość_lewej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="bricks,0",tekstura_tła="asphalt,0",otwory_lewe=[1,43,3])
#budynek generatora - parter
elektrownia_węglowa.generator(x=9.5,y=47.1,warstwa=5,turbiny=[3,"TURBINA_04","TURBINA_05","TURBINA_06"])
struktury.komnata(x=41.5,y=39.5,warstwa=8,szerokość=7.5,wysokość=7.5,grubość_prawej=1,grubość_lewej=1,grubość_górnej=1,grubość_dolnej=1,tekstura="bricks,0",tło=False,otwory_lewe=[1,3.5,2])
struktury.schody(x=41.5,y=47,obrót=-1,wysokość=1,długość=1.5,stopnie=3,przesunięcie=0.5)
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
struktury.element_podłużny(x=42.5,y=30.5,warstwa=7,tekstura="TEXVent_pipe",grubość=0.8,trasa=[10,0,0,0,8.5,0.5,9,3,9,3.5,9.5,3.5,10,3.5,10.5,4,11.5,5,12,5.5,12])
podstawy.platforma(x=42.5,y=43,warstwa=8,wierzchołki=["v",4,0,0,0,1,1.5,1,1.5,0],tekstura="bricks,0")
#budynek generatora - pierwszy poziom techniczny
podstawy.platforma(x=7.5,y=39.5,wierzchołki=["v",4,0,0,0,1,1,1,1,0],tekstura="panels,2",warstwa=8)
podstawy.platforma(x=11.5,y=39.5,wierzchołki=["v",4,0,0,0,1,30,1,30,0],tekstura="panels,1",warstwa=8)
podstawy.platforma(x=8,y=39.7,wierzchołki=["v",4,0,0,0,0.6,4,0.6,4,0],tekstura="panels,0",warstwa=7)
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
struktury.element_podłużny(x=9.5,y=32.2,warstwa=8,tekstura="TEXVent_pipe",grubość=0.8,trasa=[7,2,0,1,0,0,1,0,2.5,1,3.5,32,3.5,32.5,3],kolizja=True)
struktury.element_podłużny(x=7.5,y=38.25,warstwa=4,tekstura="TEXHydro_pipe",grubość=1.5,trasa=[2,0,0,41.5,0])
struktury.element_podłużny(x=7.5,y=37.25,warstwa=5,tekstura="TEXSteel_stick",grubość=0.8,trasa=[2,0,0,41.5,0])
struktury.element_podłużny(x=7.5,y=39.25,warstwa=5,tekstura="TEXSteel_stick",grubość=0.8,trasa=[2,0,0,41.5,0])
struktury.element_podłużny(x=7.5,y=37.55,warstwa=5,tekstura="TEXElectricity_cable_green",grubość=0.08,trasa=[2,0,0,41.5,0],szerokość_tekstury=32)
struktury.element_podłużny(x=7.5,y=37.65,warstwa=5,tekstura="TEXElectricity_cable_red",grubość=0.06,trasa=[2,0,0,41.5,0],szerokość_tekstury=32)
struktury.element_podłużny(x=43,y=30,warstwa=4,tekstura="TEXHydro_pipe",grubość=0.8,trasa=[2,0,0,0,6],kolizja=True)
struktury.element_podłużny(x=46.5,y=32,warstwa=9,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,2.5,0],kolizja=True,jednostronność=True)
struktury.element_podłużny(x=46.5,y=29.5,warstwa=9,tekstura="TEXSteel_stick_rusted",grubość=0.35,trasa=[2,0,0,0,10])
struktury.schody(x=49,y=39.5,warstwa=5,wysokość=3.5,długość=3.5,stopnie=8,tekstura="bricks,0",obrót=-1)
grafiki.świetlówka(x=15,y=37.5,warstwa=4,długość=3)
grafiki.świetlówka(x=24,y=37.5,warstwa=4,długość=3)
grafiki.świetlówka(x=33,y=37.5,warstwa=4,długość=3)
podstawy.dźwięk(x=10,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=22,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=34,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)
podstawy.dźwięk(x=46,y=38.25,dźwięk="RURY",głośność=60,wyciszanie=0.5,zasięg=10)

lvl.save('level.xml')