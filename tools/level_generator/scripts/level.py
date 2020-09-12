from level_base import level

lvl = level('Poziom testowy python')

import pułapki
pułapki.lvl = lvl
import struktury
struktury.lvl = lvl
import mechanizmy
mechanizmy.lvl = lvl

struktury.komnata()
pułapki.spadający_obiekt()
lvl.save('level.xml')