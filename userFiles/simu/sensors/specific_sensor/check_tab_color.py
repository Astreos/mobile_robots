# script to check that tabular encoding is correct
import numpy as np
from matplotlib import pyplot as plt

# colors
WW = 2.5
GG = 1.7
KK = 0.0
BB = 1.0
YY = 3.2
RR = 5.0

# tabular
map_color = np.array([
	[ YY ,YY ,YY ,YY ,YY ,GG ,GG ,WW ,GG ,KK ,KK ,KK ,GG ,GG ,GG ,BB ,BB ,BB ,BB ,BB ] , \
	[ YY ,YY ,YY ,YY ,YY ,GG ,GG ,GG ,KK ,WW ,KK ,GG ,KK ,KK ,WW ,BB ,BB ,BB ,BB ,BB ] , \
	[ YY ,YY ,YY ,YY ,YY ,GG ,KK ,GG ,KK ,KK ,WW ,GG ,GG ,GG ,KK ,BB ,BB ,BB ,BB ,BB ] , \
	[ YY ,YY ,YY ,YY ,YY ,KK ,WW ,GG ,WW ,GG ,KK ,KK ,KK ,WW ,WW ,BB ,BB ,BB ,BB ,BB ] , \
	[ YY ,YY ,YY ,YY ,YY ,WW ,GG ,GG ,GG ,GG ,GG ,WW ,GG ,GG ,GG ,BB ,BB ,BB ,BB ,BB ] , \
	[ YY ,YY ,YY ,YY ,YY ,KK ,GG ,WW ,GG ,WW ,KK ,WW ,WW ,WW ,GG ,WW ,GG ,KK ,KK ,KK ] , \
	[ WW ,KK ,WW ,WW ,WW ,GG ,KK ,GG ,GG ,WW ,WW ,GG ,KK ,KK ,WW ,WW ,WW ,GG ,KK ,WW ] , \
	[ WW ,KK ,KK ,WW ,GG ,KK ,KK ,GG ,KK ,WW ,WW ,GG ,WW ,GG ,WW ,GG ,WW ,GG ,KK ,KK ] , \
	[ KK ,WW ,KK ,WW ,KK ,WW ,GG ,GG ,GG ,GG ,KK ,WW ,KK ,WW ,KK ,KK ,GG ,WW ,WW ,KK ] , \
	[ WW ,WW ,GG ,GG ,WW ,WW ,GG ,WW ,GG ,GG ,GG ,KK ,KK ,KK ,KK ,KK ,KK ,WW ,WW ,WW ] , \
	[ KK ,KK ,WW ,GG ,WW ,KK ,GG ,KK ,WW ,KK ,KK ,GG ,KK ,WW ,GG ,KK ,KK ,KK ,KK ,KK ] , \
	[ WW ,WW ,GG ,KK ,GG ,WW ,WW ,GG ,RR ,RR ,RR ,RR ,WW ,KK ,GG ,GG ,GG ,GG ,GG ,KK ] , \
	[ KK ,GG ,KK ,GG ,KK ,WW ,WW ,KK ,RR ,WW ,GG ,WW ,KK ,KK ,KK ,KK ,KK ,WW ,KK ,KK ] , \
	[ GG ,GG ,GG ,KK ,KK ,GG ,WW ,KK ,RR ,GG ,KK ,KK ,KK ,KK ,KK ,KK ,WW ,WW ,KK ,KK ] , \
	[ WW ,WW ,GG ,WW ,WW ,RR ,RR ,RR ,RR ,KK ,WW ,GG ,GG ,WW ,KK ,KK ,WW ,WW ,KK ,WW ] , \
	[ WW ,KK ,GG ,KK ,WW ,RR ,RR ,RR ,RR ,WW ,GG ,KK ,KK ,KK ,GG ,KK ,KK ,WW ,KK ,KK ] , \
	[ KK ,KK ,GG ,KK ,KK ,WW ,WW ,WW ,RR ,WW ,KK ,GG ,KK ,WW ,WW ,KK ,KK ,KK ,GG ,KK ] , \
	[ WW ,KK ,WW ,WW ,WW ,KK ,GG ,WW ,RR ,KK ,GG ,WW ,WW ,WW ,GG ,GG ,KK ,WW ,KK ,GG ] , \
	[ GG ,KK ,KK ,GG ,WW ,WW ,KK ,WW ,RR ,RR ,RR ,RR ,KK ,WW ,KK ,KK ,GG ,WW ,KK ,WW ] , \
	[ WW ,GG ,KK ,WW ,KK ,GG ,GG ,WW ,GG ,GG ,GG ,GG ,KK ,WW ,GG ,GG ,GG ,WW ,KK ,KK ] , \
	[ GG ,WW ,KK ,GG ,WW ,KK ,KK ,GG ,KK ,WW ,GG ,WW ,GG ,WW ,WW ,GG ,KK ,GG ,KK ,WW ] , \
	[ KK ,GG ,WW ,WW ,KK ,GG ,KK ,WW ,GG ,WW ,GG ,GG ,KK ,WW ,KK ,KK ,WW ,WW ,KK ,KK ] , \
	[ GG ,WW ,KK ,KK ,GG ,GG ,KK ,GG ,KK ,GG ,WW ,WW ,GG ,KK ,GG ,GG ,KK ,WW ,WW ,GG ] , \
	[ KK ,GG ,KK ,KK ,WW ,KK ,KK ,WW ,GG ,KK ,WW ,GG ,GG ,GG ,GG ,WW ,WW ,WW ,KK ,WW ] , \
	[ BB ,BB ,BB ,BB ,BB ,WW ,KK ,KK ,GG ,WW ,KK ,KK ,GG ,GG ,GG ,GG ,GG ,KK ,GG ,GG ] , \
	[ BB ,BB ,BB ,BB ,BB ,WW ,GG ,WW ,KK ,KK ,WW ,KK ,WW ,GG ,GG ,YY ,YY ,YY ,YY ,YY ] , \
	[ BB ,BB ,BB ,BB ,BB ,KK ,WW ,GG ,GG ,WW ,GG ,WW ,WW ,KK ,GG ,YY ,YY ,YY ,YY ,YY ] , \
	[ BB ,BB ,BB ,BB ,BB ,KK ,GG ,GG ,WW ,KK ,WW ,WW ,WW ,GG ,KK ,YY ,YY ,YY ,YY ,YY ] , \
	[ BB ,BB ,BB ,BB ,BB ,GG ,WW ,KK ,WW ,GG ,GG ,GG ,KK ,GG ,KK ,YY ,YY ,YY ,YY ,YY ] , \
	[ BB ,BB ,BB ,BB ,BB ,GG ,GG ,KK ,GG ,GG ,WW ,KK ,KK ,WW ,KK ,YY ,YY ,YY ,YY ,YY ] \
])

#plot
im = plt.matshow(map_color)
plt.colorbar(im)
plt.show()
