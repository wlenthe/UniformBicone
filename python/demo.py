import numpy as np
import png

#@brief save an array as a png
#@param array: 1, 2, or 3D array of 8 or 16 bit ints to save
#@param filename: name of file to write image to
def savePng(array, filename):
	# check array / data type
	if not isinstance(array, (np.ndarray, np.generic) ): # check if already a np array
		array = np.array(array) # convert to np array if needed
	if not issubclass(array.dtype.type, np.integer): # check if array holds integer types
		raise ValueError("only integer data types are supported")
	
	# check shape and use dimensionallity to infer pixel samples
	if len(array.shape) < 3: # treat 1d and 2d arrays as grayscale
		mode = 'L'
	elif len(array.shape) == 3: # handle 3d arrays depending on samples per pixel
		samples = array.shape[-1]
		if 1 == samples: # 1 sample -> grayscale
			mode = 'L'
		elif 2 == samples: # 2 samples -> grayscale + alpha
			mode = 'LA'
		elif 3 == samples: # 3 samples -> rgb
			mode = 'RGB'
		elif 4 == samples: # 4 samples -> rgba
			mode = 'RGBA'
		else:
			raise ValueError("only 1, 2, 3, or 4 samples per pixel are supported")
	else:
		raise ValueError("only 1D, 2D, and 3D numpy arrays are supported")
	
	# check bit depth
	if 1 == array.dtype.itemsize: # nothing special is required for 8 bit images
		pass
	elif 2 == array.dtype.itemsize: # handle 16 bit images
		mode += ';16' # can techincally handle any bitdepth from 1->16 but I'll just handle 8 and 16
	else:
		raise ValueError("only 8 and 16 bit numpy arrays are supported")

	# cartesian -> image coordinate system
	if 2 == len(array.shape) or 3 == len(array.shape):
		array = np.flip(array, axis = 0)
	png.from_array(array, mode).save(filename)

import colormap as cm

# generate some scalar test data with values outside of [0,1]
x = np.linspace(0, 1, 256)
x, y = np.meshgrid(np.sin(x * 8 * np.pi), x)
im = x * y # [-1, 1]

# generate some disk test data
x = np.linspace(-1, 1, 256)
x, y = np.meshgrid(x, x)
r = np.sqrt(x*x + y*y)
a = np.arctan2(y, x) / (np.pi * 2)

# flags for which maps to apply
use_ramps  = False
use_cycles = False
use_disks  = False

# flags for which maps to generate legends for
make_ramps  = False
make_cycles = True
make_disks  = False

if not(use_ramps and use_cycles and use_disks and make_ramps and make_cycles and make_disks):
	help(cm) # if nothing is selected just pring module help

if use_ramps:
	help(cm.ramp)
	savePng(cm.ramp(im              ), "ramp_raw.png"   ) # bad values will be filled with default fill (0)
	savePng(cm.ramp(im, alpha = True), "ramp_alpha.png" ) # alpha channel -> transparent bad values
	savePng(cm.ramp(im, fill = 1    ), "ramp_white.png" ) # specifying a fill value silences out of range warning
	savePng(cm.ramp(im, scale = True), "ramp_scaled.png") # rescaling prevents out of range values (except NANs)
	floatMap = cm.ramp(im, scale = True, float = True) # float returns 64 bit fp values instead of 8 bit uints
	savePng(cm.ramp(im, "divBR", scale = True), "ramp_div.png"   ) # use divegent legend to show deviation from midpoint

if use_cycles:
	help(cm.cyclic)
	savePng(cm.cyclic(im, scale = True), "cyclic.png"   ) # most positive and most negative values will be the same colors with cyclic legends

if use_disks:
	help(cm.disk)
	savePng(cm.ramp(r, 'gray', scale = True), "r.png") # disk maps take fractional radius as first argument and fractional angle as second
	savePng(cm.ramp(a, 'gray', scale = True), "a.png") # disk maps take fractional radius as first argument and fractional angle as second
	savePng(cm.disk(r, a, scale = True), "disk.png") # disk maps take fractional radius as first argument and fractional angle as second

if make_ramps:
	help(cm.ramp_legend)
	for m in ['gray', 'fire', 'ocean', 'ice', 'div']:
		savePng(cm.ramp_legend(m),'r_' + m + '.png')

if make_cycles:
	help(cm.cyclic_legend)
	for m in ['gray', 'four', 'six', 'div']:
		savePng(cm.cyclic_legend(m, alpha = True), 'c_' + m + '.png')

if make_disks:
	help(cm.disk_legend)
	for m in ['four', 'six']:
		savePng(cm.disk_legend(m, alpha = True), 'd_' + m + '.png')
	savePng(cm.disk_legend('four', alpha = True, w_cen = True), 'd_four_w.png') # white center instead of black
	savePng(cm.disk_legend('four', alpha = True, sym = 'a'), 'd_four_a.png') # make inversion symmetric by doubling azimuthal angle
	savePng(cm.disk_legend('four', alpha = True, sym = 'p'), 'd_four_p.png') # make inversion symmetric by doubling polar angle
