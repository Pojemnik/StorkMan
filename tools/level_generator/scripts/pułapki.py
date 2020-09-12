global lvl
def spadający_obiekt(x=0, y=0, prędkość=8, odstęp=180, spad=10, wysokość=2, rozmiar=1, strefa="<v>0,0</v><v>1,0</v><v>1,-2</v><v>0,-2</v>", czas_obrażeń=60, szczyt_obrażeń=15, obrażenia=100, rozdrobnienie=10, obiekt="<v>0,0</v><v>1,0</v><v>1,1</v><v>0,1</v>", tekstura="chest0", animacja="bricks,0"):
    czas1 = round(((spad-wysokość)/prędkość)*60)
    czas2 = round((wysokość/prędkość)*60)
    czas3 = round(czas_obrażeń-czas2)
    czas4 = round(odstęp-czas3)
    czas_rozdrobnienia = (czas_obrażeń/rozdrobnienie)
    zona = """<damage_zone position="{0},{1}">""".format(x, y)+strefa+"""<d>{0},{1}</d>""".format(0, czas1)
    for i in range(rozdrobnienie):
        if czas_rozdrobnienia*(i+0.5) < szczyt_obrażeń:
            zona = zona+"""<d>"""+str(round(((czas_rozdrobnienia*(i+0.5))/szczyt_obrażeń)*obrażenia))+""","""+str(round((i+1)*czas_rozdrobnienia)-round(i*czas_rozdrobnienia))+"""</d>"""
        else:
            zona = zona+"""<d>"""+str(round((czas_obrażeń-(czas_rozdrobnienia*(i+0.5)))/(czas_obrażeń-szczyt_obrażeń)*obrażenia))+""","""+str(round((i+1)*czas_rozdrobnienia)-round(i*czas_rozdrobnienia))+"""</d>"""
    zona = zona+"""<d>{0},{1}</d>
        </damage_zone>""".format(0, czas4)
    lvl.add(zona)
    lvl.add("""<linear_platform position="{0},{1}" visible="0">
        <p>{2},{3},{4}</p>
        <p>{5},{6},{7}</p>
        <p>{8},{9},{10}</p>""".format(x, y-rozmiar, 0, -spad, czas1, 0, -wysokość, 0, 0, -spad, czas2+czas3+czas4)+obiekt+"""</linear_platform>""")
    lvl.add("""<moving_object position="{0},{1}" texture="{2}" height="{3}">
        <p>{4},{5},{6}</p>
        <p>{7},{8},{9}</p>
        <p>{10},{11},{12}</p>
        </moving_object>""".format(x, y-rozmiar, tekstura, rozmiar, 0, -spad, czas1+czas2, 0, 0, 0, 0, -spad, czas3+czas4))