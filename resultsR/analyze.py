# import libraries
import numpy as np
from matplotlib import pyplot as plt
plt.switch_backend('Qt4Agg')

# check if the curve index is in the arguments
# return 1 if it is the case (or if no argument), 0 otherwise
# index: index of the current curve
# args: arguments with indexes to plot
def check_args(index, args):

	# no argument
	if len(args) == 0:
		return 1

	# loop on all the arguments
	for i in args:
		if i == index:
			return 1

	return 0

# plot result function
# res_name: result file name (without extension)
# args: arguments with indexes to plot
def plot_res(res_name, *args):

	#get file name
	filename = '{}.res'.format(res_name)
	
	# get data
	data = np.loadtxt(filename)

	# get size of the data matrix
	[a, b] = np.shape(data)

	# at least two columns are requested
	if b < 2:
		print('error: only {} colums !'.format(b))
		return

	# time
	t = data[:,0]

	# new figure
	plt.figure()

	# plot curves
	nb_curves = 0
	for i in range(1, b):
		if check_args(i, args):
			plt.plot(t, data[:,i], label='{}'.format(i))
			nb_curves += 1

	if nb_curves == 0:
		return
	
	# plot properties
	plt.title(res_name)
	plt.xlabel('time [s]')
	plt.xlim([min(t),max(t)])
	
	# legends
	if b > 2:
		plt.legend(loc=4, numpoints=1)


# Use the 'plot_res' function to plot the requested .res file.
# The first argument is the name of the file, without its extension.
# You can add extra arguments (after the file name) to specify the indexes of the curves
# to plot (starting at 1). Each index must be separated by a coma. If no indexes are indicated,
# all the curves are plotted.
# You can also use the line 'plt.savefig('./figure.pdf')' to save a pdf of the plotted figure.
# Use this line after the corresponding 'plot_res()' (before 'plt.show()'). This works also
# with other formats like pgf.
#
# examples:
#     plot_res('dirdyn_q')
#     plt.savefig('./dirdyn_q.pdf')
#     plot_res('dirdyn_q', 1, 3, 9)
#     plot_res('my_var')
#     plt.savefig('./my_var.pdf')
#

plot_res('dirdyn_q')

# show plots
plt.show()
