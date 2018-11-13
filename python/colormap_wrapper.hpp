/*************************************************************************************/
/*                                                                                   */
/* Copyright (c) 2018, De Graef Group, Carnegie Mellon University                    */
/* Author: William Lenthe                                                            */
/* All rights reserved.                                                              */
/*                                                                                   */
/* Redistribution and use in source and binary forms, with or without                */
/* modification, are permitted provided that the following conditions are met:       */
/*                                                                                   */
/*     - Redistributions of source code must retain the above copyright notice, this */
/*       list of conditions and the following disclaimer.                            */
/*     - Redistributions in binary form must reproduce the above copyright notice,   */
/*       this list of conditions and the following disclaimer in the documentation   */
/*       and/or other materials provided with the distribution.                      */
/*     - Neither the copyright holder nor the names of its                           */
/*       contributors may be used to endorse or promote products derived from        */
/*       this software without specific prior written permission.                    */
/*                                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"       */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE         */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE    */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE      */
/* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL        */
/* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER        */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,     */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE         */
/* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                   */
/*************************************************************************************/

#ifndef _colormap_wrapper_h_
#define _colormap_wrapper_h_

#include "Python.h"
#define NPY_NO_DEPRECATED_API NPY_API_VERSION
#include "numpy/arrayobject.h"

#include "colormap.hpp"

#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

//@brief: python bindings for perceptually uniform color map functions defined in colormap.hpp

//names and descriptions of available ramp color maps, these need to match the options defined in 'getRamp'
std::string rampDescriptions(std::string prefix) {
	return prefix + "-'gray' : black -> white\n"
	     + prefix + "-'fire' : black -> puruple -> magenta -> yellow -> white\n"
	     + prefix + "-'ocean': black -> blue -> green -> yellow -> white\n"
	     + prefix + "-'ice'  : black -> purple -> blue -> cyan -> white\n"
	     + prefix + "-'div'  : blue -> white -> red\n";
}

//names and descriptions of available cyclic color maps, these need to match the options defined in 'getCyclic'
std::string cyclicDescriptions(std::string prefix) {
	return prefix + "-'gray': black -> white -> black\n"
	     + prefix + "-'four': red -> blue -> green -> yellow -> red\n"
	     + prefix + "-'six' : red -> magenta -> blue -> teal -> green -> yellow -> red\n"
	     + prefix + "-'div' : blue -> gray -> red -> blue\n";
}

//names and descriptions of available disk color maps, these need to match the options defined in 'getDisk'
std::string diskDescriptions(std::string prefix) {
	return prefix + "-'four': red -> blue -> green -> yellow -> red (perimeter)\n"
	     + prefix + "-'six' : red -> magenta -> blue -> teal -> green -> yellow -> red (perimeter)\n";
}
std::string sphereDescriptions(std::string prefix) {return diskDescriptions(prefix);}
std::string ballDescriptions(std::string prefix) {return diskDescriptions(prefix);}

//name of module
const std::string module_name = "colormap";//make sure this matches the signature of the PyMODINIT_FUNC

//name of module functions
const std::string ramp_name     = "ramp"   ;//linear ramp color map function
const std::string cyclic_name   = "cyclic" ;//cyclic color map function
const std::string disk_name     = "disk"   ;//disk color map function
const std::string sphere_name   = "sphere" ;//sphere color map function
const std::string ball_name     = "ball"   ;//ball color map function
const std::string legend_suffix = "_legend";//legend suffix
const std::string ramp_legend_name   = ramp_name   + legend_suffix;//linear ramp legend function
const std::string cyclic_legend_name = cyclic_name + legend_suffix;//cyclic legend function
const std::string disk_legend_name   = disk_name   + legend_suffix;//disk legend function
const std::string sphere_legend_name = sphere_name + legend_suffix;//sphere legend function
const std::string ball_legend_name   = ball_name   + legend_suffix;//ball legend function

const std::string module_help = "\
perceptually uniform color maps based on:\n\
-Peter Kovesi. Good Colour Maps: How to Design Them.\n\
 arXiv:1509.03700 [cs.GR] 2015\n\
-Lenthe (submitted)\n\n\
The following color maps are available:\n\
  *linear (via " + module_name + "." + ramp_name + "):\n" + rampDescriptions("    ") + "\
  *cyclic (via " + module_name + "." + cyclic_name + "):\n" + cyclicDescriptions("    ") + "\
  *disk   (via " + module_name + "." + disk_name + "):\n" + diskDescriptions("    ") + "\
  *sphere (via " + module_name + ".sphere):\n\
  *ball   (via " + module_name + ".ball):\n\
Legend generation functions are also available via " + module_name + ".type" + legend_suffix + "() functions";
////////////////////////////////////////////////////////////////
//            Python Wrapper for Linear Colormaps             //
////////////////////////////////////////////////////////////////

//@brief wrapper function for linear color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword scalars: array of values to compute color map for
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
static PyObject* ramp_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for ramp_wrapper
const std::string ramp_help = "\
@brief        : map an array of scalars to an array of rgb values with a linear colormap\n\
@param scalars: scalar values to compute map of\n\
@param map    : name of color map to use\n" + rampDescriptions("                ") + "\
@param fill   : fill value for scalars falling outside of [0,1] and NANs (all 3/4 channels are filled with the same value)\n\
@param scale  : True/False to rescale input to [0,1] before coloring\n\
@param alpha  : True/False to include alpha channel (rgba/rgb)\n\
@param float  : True/False to return colors as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@return       : array of rgb(a) values\n"
 + module_name + '.' + ramp_name + "(scalars, map = 'fire', fill = 0, scale = False, alpha = False, float = False)";

////////////////////////////////////////////////////////////////
//            Python Wrapper for Cyclic Colormaps             //
////////////////////////////////////////////////////////////////

//@brief wrapper function for cyclic color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword scalars: array of values to compute color map for
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
static PyObject* cyclic_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for cyclic_wrapper
const std::string cyclic_help = "\
@brief        : map an array of scalars to an array of rgb values with a periodic colormap\n\
@param scalars: scalar values to compute map of\n\
@param map    : name of color map to use\n" + cyclicDescriptions("                ") + "\
@param fill   : fill value for scalars falling outside of [0,1] and NANs (all 3/4 channels are filled with the same value)\n\
@param scale  : True/False to rescale input to [0,1] before coloring\n\
@param alpha  : True/False to include alpha channel (rgba/rgb)\n\
@param float  : True/False to return colors as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@return       : array of rgb(a) values\n"
 + module_name + '.' + cyclic_name + "(scalars, map = 'four', fill = 0, scale = False, alpha = False, float = False)";

////////////////////////////////////////////////////////////////
//             Python Wrapper for Disk Colormaps              //
////////////////////////////////////////////////////////////////

//@brief wrapper function for disk color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword radii  : array of radii to compute color map for
//             @keyword angles : array of angles to compute color map for (must be the same shape as radii)
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen  : true/false white/black center
//             @keyword sym    : type of inversion symmetry to apply
static PyObject* disk_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for disk_wrapper
const std::string disk_help = "\
@brief       : map pairs of (radius, angle) to an array of rgb values with a disk colormap\n\
@param radii : radii to use for color map calculation\n\
@param angles: angles to use for color map calculation (must be the same shape as radii)\n\
@param map   : name of color map to use\n" + diskDescriptions("               ") + "\
@param fill  : fill value for scalars falling outside of [0,1] and NANs (all 3/4 channels are filled with the same value)\n\
@param scale : True/False to rescale input to [0,1] before coloring\n\
@param alpha : True/False to include alpha channel (rgba/rgb)\n\
@param float : True/False to return colors as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen : True/False for " + disk_name + "(r==0) --> white/black\n\
@param sym   : type of inversion symmetry to apply\n\
               -None: no inversion symmetry\n\
               -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
               -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@return      : array of rgb(a) values\n"
 + module_name + '.' + disk_name + "(radii, angles, map = 'four', fill = 0, scale = False, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//            Python Wrapper for Sphere Colormaps             //
////////////////////////////////////////////////////////////////

//@brief wrapper function for sphere color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword azimuths: array of radii to compute color map for
//             @keyword polars  : array of angles to compute color map for (must be the same shape as radii)
//             @keyword map     : [optional] name of color map to use
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale   : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : true/false white/black center
//             @keyword sym     : type of inversion symmetry to apply
static PyObject* sphere_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for sphere_wrapper
const std::string sphere_help = "\
@brief         : map pairs of (azimuthal angle, polar angle) to an array of rgb values with a sphere colormap\n\
@param azimuths: azimuthal angles to use for color map calculation\n\
@param polars  : polar angles to use for color map calculation (must be the same shape as azimuths)\n\
@param map     : name of color map to use\n" + sphereDescriptions("               ") + "\
@param fill    : fill value for scalars falling outside of [0,1] and NANs (all 3/4 channels are filled with the same value)\n\
@param scale   : True/False to rescale input to [0,1] before coloring\n\
@param alpha   : True/False to include alpha channel (rgba/rgb)\n\
@param float   : True/False to return colors as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen   : True/False for " + sphere_name + "(r==0) --> white/black\n\
@param sym     : type of inversion symmetry to apply\n\
                 -None: no inversion symmetry\n\
                 -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
                 -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@return        : array of rgb(a) values\n"
 + module_name + '.' + sphere_name + "(azimuths, polars, map = 'four', fill = 0, scale = False, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//             Python Wrapper for Ball Colormaps              //
////////////////////////////////////////////////////////////////

//@brief wrapper function for ball color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword radii   : array of radii to compute color map for
//             @keyword azimuths: array of azimuthal angles to compute color map for (must be the same shape as radii)
//             @keyword polars  : array of polar angles to compute color map for (must be the same shape as radii)
//             @keyword map     : [optional] name of color map to use
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale   : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : true/false white/black center
//             @keyword sym     : type of inversion symmetry to apply
static PyObject* ball_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for ball_wrapper
//python help string for ball_wrapper
const std::string ball_help = "\
@brief         : map pairs of (azimuthal angle, polar angle) to an array of rgb values with a ball colormap\n\
@param radii   : radii to use for color map calculation\n\
@param azimuths: azimuthal angles to use for color map calculation\n\
@param polars  : polar angles to use for color map calculation (must be the same shape as azimuths)\n\
@param map     : name of color map to use\n" + ballDescriptions("               ") + "\
@param fill    : fill value for scalars falling outside of [0,1] and NANs (all 3/4 channels are filled with the same value)\n\
@param scale   : True/False to rescale input to [0,1] before coloring\n\
@param alpha   : True/False to include alpha channel (rgba/rgb)\n\
@param float   : True/False to return colors as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen   : True/False for " + ball_name + "(r==0) --> white/black\n\
@param sym     : type of inversion symmetry to apply\n\
                 -None: no inversion symmetry\n\
                 -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
                 -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@return        : array of rgb(a) values\n"
 + module_name + '.' + ball_name + "(radii, azimuths, polars, map = 'four', fill = 0, scale = False, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//              Python Wrapper for Ramp Legends               //
////////////////////////////////////////////////////////////////

//@brief wrapper function for ramp legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map   : name of color map to create legend for
//             @keyword width : [optional] width of legend in pixels
//             @keyword height: [optional] height of legend in pixels
//             @keyword ripple: [optional] true/false for rippled / flat legend
//             @keyword alpha : [optional] true / false to include an alpha channel
//             @keyword float : [optional] true / false to return array of doubles / uint8_t
static PyObject* ramp_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for ramp_legend_wrapper
const std::string ramp_legend_help = "\
@brief       : create a legend for a linear colormap\n\
@param map   : name of color map to use\n" + rampDescriptions("               ") + "\
@param width : width of legend in pixels\n\
@param height: height of legend in pixels\n\
@param ripple: True/False to create rippled or flat legend\n\
@param alpha : True/False to include alpha channel (rgba/rgb)\n\
@param float : True/False to return rgb values as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@return      : 3D rgb(a) array of legend image\n"
 + module_name + '.' + ramp_legend_name + "(map, width = 512, height = 128, ripple = True, alpha = False, float = False)";

////////////////////////////////////////////////////////////////
//             Python Wrapper for Cyclic Legends              //
////////////////////////////////////////////////////////////////

//@brief wrapper function for cyclic legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map   : name of color map to create legend for
//             @keyword width : [optional] width & height of legend in pixels
//             @keyword fill  : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple: [optional] true/false for rippled / flat legend
//             @keyword alpha : [optional] true / false to include an alpha channel
//             @keyword float : [optional] true / false to return array of doubles / uint8_t
static PyObject* cyclic_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for cyclic_legend_wrapper
const std::string cyclic_legend_help = "\
@brief       : create a legend for a cyclic colormap\n\
@param map   : name of color map to use\n" + cyclicDescriptions("               ") + "\
@param width : width & height of legend in pixels\n\
@param fill  : fill value for scalars falling outside of [0,1] (background pixels, all 3/4 channels are filled with the same value)\n\
@param ripple: True/False to create rippled or flat legend\n\
@param alpha : True/False to include alpha channel (rgba/rgb)\n\
@param float : True/False to return rgb values as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@return      : 3D rgb(a) array of legend image\n"
 + module_name + '.' + cyclic_legend_name + "(map, width = 512, fill = 0, ripple = True, alpha = False, float = False)";

////////////////////////////////////////////////////////////////
//              Python Wrapper for Disk Legends               //
////////////////////////////////////////////////////////////////

//@brief wrapper function for disk legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width & height of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_r: [optional] magnitude of ripple in radial direction
//             @keyword ripple_a: [optional] magnitude of ripple in azimuthal direction
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
static PyObject* disk_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for disk_legend_wrapper
const std::string disk_legend_help = "\
@brief: create a legend for a cyclic colormap\n\
@param map     : name of color map to use\n" + diskDescriptions("                 ") + "\
@param width   : width & height of legend in pixels\n\
@param fill    : fill value for scalars falling outside of [0,1] (background pixels, all 3/4 channels are filled with the same value)\n\
@param ripple_r: magnitude of ripple in radial direction (0 for flat)\n\
@param ripple_a: magnitude of ripple in angular direction (0 for flat)\n\
@param alpha   : True/False to include alpha channel (rgba/rgb)\n\
@param float   : True/False to return rgb values as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen   : True/False for " + disk_name + "(r==0) --> white/black\n\
@param sym     : type of inversion symmetry to apply\n\
                 -None: no inversion symmetry\n\
                 -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
                 -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@return: 3D rgb (solid background) or rgba (transparent background) array of legend image\n"
 + module_name + '.' + disk_legend_name + "(map, width = 512, fill = 0, ripple_r = 0, ripple_a = 0, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//             Python Wrapper for Sphere Legends              //
////////////////////////////////////////////////////////////////

//@brief wrapper function for spherical legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width & height of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_p: [optional] magnitude of ripple in polar direction
//             @keyword ripple_a: [optional] magnitude of ripple in azimuthal direction
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
//             @keyword proj    : [optional] sphere -> disk projection type
static PyObject* sphere_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for sphere_legend_wrapper
const std::string sphere_legend_help = "\
@brief         : create a legend for a spherical colormap\n\
@param map     : name of color map to use\n" + sphereDescriptions("                 ") + "\
@param width   : width & height of legend in pixels\n\
@param fill    : fill value for scalars falling outside of [0,1] (background pixels, all 3/4 channels are filled with the same value)\n\
@param ripple_p: magnitude of ripple in polar direction (0 for flat)\n\
@param ripple_a: magnitude of ripple in azimuthal direction (0 for flat)\n\
@param alpha   : True/False to include alpha channel (rgba/rgb)\n\
@param float   : True/False to return rgb values as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen   : True/False for " + sphere_name + "(polar angle==0) --> white/black\n\
@param sym     : type of inversion symmetry to apply\n\
                 -None: no inversion symmetry\n\
                 -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
                 -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@param proj    : type of hemisphere |-> sphere mapping\n\
                 -'o': orthographic (parallel)\n\
                 -'s': stereographic (equal angle)\n\
                 -'l': lambert (equal area)\n\
                 -'d': equal distance (from pole)\n\
@return        : 3D rgb (solid background) or rgba (transparent background) array of legend image\n"
 + module_name + '.' + sphere_legend_name + "(map, width = 512, fill = 0, ripple_p = 0, ripple_a = 0, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//              Python Wrapper for Ball Legends               //
////////////////////////////////////////////////////////////////

//@brief wrapper function for spherical legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width, height, and depth of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_r: [optional] magnitude of ripple in radial direction
//             @keyword ripple_p: [optional] magnitude of ripple in polar direction
//             @keyword ripple_a: [optional] magnitude of ripple in azimuthal direction
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
static PyObject* ball_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds);

//python help string for ball_legend_wrapper
const std::string ball_legend_help = "\
@brief         : create a legend for a spherical colormap\n\
@param map     : name of color map to use\n" + ballDescriptions("                 ") + "\
@param width   : width & height of legend in pixels\n\
@param fill    : fill value for scalars falling outside of [0,1] (background pixels, all 3/4 channels are filled with the same value)\n\
@param ripple_r: magnitude of ripple in radial direction (0 for flat)\n\
@param ripple_p: magnitude of ripple in polar direction (0 for flat)\n\
@param ripple_a: magnitude of ripple in azimuthal direction (0 for flat)\n\
@param alpha   : True/False to include alpha channel (rgba/rgb)\n\
@param float   : True/False to return rgb values as 64 bit floats [0,1] or 8 bit uints [0,255]\n\
@param w_cen   : True/False for " + ball_name + "(polar angle==0) --> white/black\n\
@param sym     : type of inversion symmetry to apply\n\
                 -None: no inversion symmetry\n\
                 -'a' : double azimuthal angle (fewer degenerate colors but perceptual flat spot at equator)\n\
                 -'p' : double polar angle (equator is degenerate but no perceptual flat spot)\n\
@return        : 3D rgb (solid background) or rgba (transparent background) array of legend image\n"
 + module_name + '.' + ball_legend_name + "(map, width = 512, fill = 0, ripple_p = 0, ripple_a = 0, alpha = False, float = False, w_cen = False, sym = None)";

////////////////////////////////////////////////////////////////
//                      Helper Functions                      //
////////////////////////////////////////////////////////////////

//@brief: remove white space and convert string to lower case
//@param cStr: c string to clean
//@return: cleaned string
std::string cleanString(const char* cStr) {
	std::string str(cStr);//convert to std::string
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){return std::tolower(c);});//convert to lower case
	str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char c){return std::isspace(c);}), str.end());//remove whitespace
	return str;
}

//@brief: convert from name to ramp function
//@param cName: name of ramp function to get function for
//@return: ramp function matching name (or NULL if a match wasn't found)
colormap::ramp::func<double> getRamp(const char* cName) {
	const std::string name = cleanString(cName);
	if     (0 == name.compare("gray" )) return colormap::ramp::gray <double>;
	else if(0 == name.compare("fire" )) return colormap::ramp::fire <double>;
	else if(0 == name.compare("ocean")) return colormap::ramp::ocean<double>;
	else if(0 == name.compare("ice"  )) return colormap::ramp::ice  <double>;
	else if(0 == name.compare("div"  )) return colormap::ramp::div  <double>;
	return NULL;
}

//@brief: convert from name to cyclic function
//@param cName: name of ramp function to get function for
//@return: ramp function matching name (or NULL if a match wasn't found)
colormap::cyclic::func<double> getCyclic(const char* cName) {
	const std::string name = cleanString(cName);
	if     (0 == name.compare("gray" )) return colormap::cyclic::gray<double>;
	else if(0 == name.compare("four" )) return colormap::cyclic::four<double>;
	else if(0 == name.compare("six"  )) return colormap::cyclic::six <double>;
	else if(0 == name.compare("div"  )) return colormap::cyclic::div <double>;
	return NULL;
}

//@brief: convert from name to disk function
//@param cName: name of ramp function to get function for
//@return: ramp function matching name (or NULL if a match wasn't found)
colormap::disk::func<double> getDisk(const char* cName) {
	const std::string name = cleanString(cName);
	if     (0 == name.compare("four")) return colormap::disk::four<double>;
	else if(0 == name.compare("six" )) return colormap::disk::six <double>;
	return NULL;
}

//@brief: convert from name to sphere function
//@param cName: name of ramp function to get function for
//@return: ramp function matching name (or NULL if a match wasn't found)
colormap::sphere::func<double> getSphere(const char* cName) {
	const std::string name = cleanString(cName);
	if     (0 == name.compare("four")) return colormap::sphere::four<double>;
	else if(0 == name.compare("six" )) return colormap::sphere::six <double>;
	return NULL;
}

//@brief: convert from name to ball function
//@param cName: name of ramp function to get function for
//@return: ramp function matching name (or NULL if a match wasn't found)
colormap::ball::func<double> getBall(const char* cName) {
	const std::string name = cleanString(cName);
	if     (0 == name.compare("four")) return colormap::ball::four<double>;
	else if(0 == name.compare("six" )) return colormap::ball::six <double>;
	return NULL;
}

//@brief: convert a numpy array from 32 bit -> 8 bit
//@param input: 32 bit numpy array with values [0,1]
//@return: 8 bit numpy array with values [0,255]
PyObject* to8Bit(PyArrayObject* input) {
	//get dimensions of input array and pointer to data
	int ndims = PyArray_NDIM(input);
	npy_intp* dims = PyArray_DIMS(input);
	size_t totalPoints = 1;
	for(npy_intp i = 0; i < ndims; i++) totalPoints *= (size_t)dims[i];
	double const * const pIn  = (double const*const)PyArray_DATA(input );

	//create output array and transform into
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY(ndims, dims, NPY_UINT8, 0);
	std::transform(pIn, pIn + totalPoints, (uint8_t* const)PyArray_DATA(output), [](const double& v){return (uint8_t)std::round(v * 255);});
	return (PyObject*)output;
}

//@brief: helper function to find min/max element in range ignoring NANS (minmax_element doesn't handle nan) and compute scaling to [0,1]
//@param data: pointer to start of range
//@param count: number of elements
//@param delta: location to write b in (x + b) * m -> [0,1]
//@param delta: location to write m in (x + b) * m -> [0,1]
void minMaxScale(double const * const data, size_t count, double& delta, double& scale) {
	//make sure we know how comparisons invovling NAN behave and check for an empty array
	static_assert(std::numeric_limits<double>::is_iec559, "double must conform to iec559");//make sure we know how comparisons invovling NAN behave
	if(0 == count || data == NULL) {
		delta = scale = NAN;
		return;
	}

	//find the first non-NAN value in the array
	bool foundGood = false;
	size_t start;
	for(start = 0; start < count; start++) {
		if(!std::isnan(data[start])) {
			foundGood = true;//we found a value that isn't NaN
			break;
		}
	}

	//handle the case where the entire array is NAN
	if(!foundGood) {
		delta = scale = NAN;
	} else {
		//if we made it this far there is at least one good value in the array
		double vMin = *data, vMax = *data;
		for(size_t i = start+1; i < count; i++) {
			const double& v = data[i];
			if(v > vMax) vMax = v;
			else if(v < vMin) vMin = v;
		}
		delta = -vMin;
		const double range = vMax - vMin;
		scale = 0.0 != range ? 1.0 / range : 1.0;//avoid divde by 0 when all numbers are the same
	}
}

//@brief: apply a linear transformation to a value then clamp to [0,1]
//@param v: value to transform
//@param delta: added to v before scaling
//@param scale: scale factor
//@return: (v+delta) * scale -> [0,1]
std::function<double(const double&, const double, const double)> scaleClamp = [](const double& v, const double delta, const double scale) {
	const double vs = (v + delta) * scale;
	return vs < 0 ? 0 : (vs > 1 ? 1 : vs);
};

//default fill value
static const double defaultFill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use

//@brief: check if a fill value was passed and set to default if needed
//@param fill: fill value to check
//@param fillPassed: set to true/false if a value was / wasn't provided
//@return: true on success, false on failure (PyErr will be set)
bool getFill(double& fill, bool& fillPassed) {
	fillPassed = !(std::isnan(fill) && std::signbit(fill));//check if the default value has been changed
	if(fillPassed) {//fill value was passed
		if(fill < 0.0 || fill > 1.0) {//make passed fill value is valid
			PyErr_SetString(PyExc_ValueError, "fill value must fall in [0,1]");
			return false;//bad value passed
		}
	} else {//no fill passed
		fill = 0;//fall back to 0
	}
	return true;//no problems
}

//@brief: check if a valid color map was found and fall back to default if needed
//@param colorFunc: location to store color function
//@param funcName: name of function to use (or NULL for default function)
//@param defaultFunc: function to use if colorFunc is NULL (or NULL if a name must be provided)
//@param fetchFunc: function to parse color function from string
//@return: true on success, false on failure (PyErr will be set)
template <typename FuncType>
bool getMap(FuncType& colorFunc, char* funcName, FuncType defaultFunc, FuncType(*fetchFunc)(const char*)) {
	if(NULL != funcName) {
		colorFunc = fetchFunc(funcName);//get function from name if a name was provided
		if(NULL == colorFunc) {//handle an unparseable name
			colorFunc = defaultFunc;
			std::stringstream ss;
			ss << "couldn't find a color map matching '" << funcName << "'";
			if(NULL == defaultFunc) {//there is nothing to fall back to
				PyErr_SetString(PyExc_ValueError, ss.str().c_str());
				return false;
			} else {//there is something to fall back to
				ss << ", falling back to default";
				PyErr_WarnEx(NULL, ss.str().c_str(), 1);
			}
		}
	} else if(NULL != defaultFunc) {
		colorFunc = defaultFunc;//use default function if no name was provided
	} else {
		throw std::logic_error("cannot pass NULL string and default map");
	}
	return true;
}

//@brief: convert from a PyObject containing a string descroption of a symmetry type to a colormap::Sym
//@param symName: PyObject to parse (or NULL)
//@param sym: location to write parsed symmetry
//@return: true on success, false on failure (PyErr will be set)
bool parseSym(PyObject* symName, colormap::Sym& sym) {
	sym = colormap::Sym::None;//default to no symmetry
	if(Py_None != symName && NULL != symName) {//only try to parse if there is actually an object there
		const char * cName = PyUnicode_AsUTF8(symName);//try to convert object to string
		if(NULL == cName) {
			PyErr_SetString(PyExc_ValueError, "couldn't convert 'sym' to string");
			return false;
		}
		std::string name = cleanString(cName);
		if(name.empty()) {}
		else if(0 == name.compare("a")) sym = colormap::Sym::Azimuth;
		else if(0 == name.compare("p")) sym = colormap::Sym::Polar  ;
		else {
			std::stringstream ss;
			ss << "couldn't find a symmetry type matching '" << name << "'";
			PyErr_SetString(PyExc_ValueError, "'sym' must be one of {None, 'a', 'p'}");
			return false;
		}
	}
	return true;
}

//@brief: get input array as double and count number of dimensions
//@param array: PyObject to get as array of doubles
//@param input: location to store pointer to array as doubles
//@param totalPoints: number of points in array
//@param pDims: location to write array dimensions (or NULL)
//@return: true on success, false on failure (PyErr will be set)
bool getArray(PyObject* array, PyArrayObject*& input, size_t& totalPoints, std::vector<npy_intp>* pDims = NULL) {
	//get input as doubles
	input = (PyArrayObject*)PyArray_FROM_OTF(array, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
	if(input == NULL) {
		PyErr_SetString(PyExc_ValueError, "couldn't convert input to numpy array of doubles");
		Py_XDECREF(input);
		return false;
	}

	//get dimensions and count points
	int ndims = PyArray_NDIM(input);
	npy_intp* dims = PyArray_DIMS(input);
	totalPoints = 1;
	for(npy_intp i = 0; i < ndims; i++) totalPoints *= (size_t)dims[i];

	//wrap dimensions as vector if needed and return
	if(pDims != NULL) pDims->assign(dims, dims + ndims);
	return true;
}

//@brief wrapper function for 1 parameter color maps (ramp + cyclic)
//@template cyclic: true/false for cyclic/ramp color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword scalars: array of values to compute color map for
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
template<bool cyclic>
static PyObject* linear_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	static const char* defaultName = cyclic ? "four" : "fire";
	static const colormap::ramp::func<double> defaultFunc = cyclic ? getCyclic(defaultName) : getRamp(defaultName);
	static_assert(std::is_same<colormap::ramp::func<double>, colormap::cyclic::func<double> >::value, "ramp and cyclic color maps must have the same signature to share wrapper function as written");

	//parse arguments
	PyObject* array = NULL;
	char* map = NULL;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	int iScale = 0, iAlpha = 0, iFloat = 0;//python predicate takes a pointer to an int
	static char const* kwlist[] = {"scalars", "map", /*begin keyword only*/ "fill", "scale", "alpha", "float", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O|s$dppp", const_cast<char**>(kwlist), &array, &map, &fill, &iScale, &iAlpha, &iFloat)) return NULL;
	const bool scale = iScale != 0, alpha = iAlpha != 0, fp = iFloat != 0;//convert from int -> boolean

	//parse color map function and fill value
	colormap::ramp::func<double> colorFunc;
	bool fillPassed;
	getMap(colorFunc, map, defaultFunc, cyclic ? getCyclic : getRamp);
	if(!getFill(fill, fillPassed)) return NULL;

	//get array object as doubles and its dimensions
	PyArrayObject* input;
	size_t totalPoints;
	std::vector<npy_intp> newDims;
	if(!getArray(array, input, totalPoints, &newDims)) return NULL;

	//create new array with an extra dimension tacked onto the end
	const size_t stride = alpha ? 4 : 3;
	newDims.push_back(stride);//add rgb dimension
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY((int)newDims.size(), newDims.data(), NPY_DOUBLE, 0);

	//get data pointers
	double const * const values = (double const*const)PyArray_DATA(input );
	double       * const rgb    = (double      *const)PyArray_DATA(output);
	if(alpha) for(size_t i = 0; i < totalPoints; i++) rgb[4*i+3] = 1.0;//fill in alpha channel with 1 if needed

	//loop over scalars computing color
	bool hasNans = false;
	bool outOfRange = false;
	if(scale) {
		//build function to scale input to [0,1]		
		double delta, scale;
		minMaxScale(values, totalPoints, delta, scale);
		const std::function<double(const double&)> vFunc = std::bind(scaleClamp, std::placeholders::_1, delta, scale);

		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double t = vFunc(values[i]);//get rescaled value
			if(std::isnan(t)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(t, rgb + stride * i);//compute color for non nan numbers
			}
		}
	} else {
		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double t = values[i];//get raw value
			if(std::isnan(t)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for NANs
			} else if(t < 0.0 || t > 1.0) {//handle values outside of [0,1]
				outOfRange = true;
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(t, rgb + stride * i);//compute color for in valid values
			}
		}
	}

	//warn if the fill value was used without being explicitly passed and return
	if(hasNans &&    !fillPassed) PyErr_WarnEx(NULL, "NAN values were colored with the default fill value", 1);
	if(outOfRange && !fillPassed) PyErr_WarnEx(NULL, "values outside of [0,1] colored with the default fill value", 1);
	Py_XDECREF(input);
	return fp ? (PyObject*)output : to8Bit(output);
}

//@brief wrapper function for disk color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword azimuths/radii : array of azimuthal angles or radii to compute color map for (isSphere true/false)
//             @keyword polars  /angles: array of polar angles or angles to compute color map for (must be the same shape as azimuths/radii) (isSphere true/false)
//             @keyword map            : [optional] name of color map to use
//             @keyword fill           : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale          : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha          : [optional] true / false to include an alpha channel
//             @keyword float          : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen          : true/false white/black center
//             @keyword sym            : type of inversion symmetry to apply
template <bool isSphere>
static PyObject* circ_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	static const char* defaultName = "four";
	static const colormap::disk::func<double> defaultFunc = isSphere ? getSphere(defaultName) : getDisk(defaultName);
	static_assert(std::is_same<colormap::disk::func<double>, colormap::sphere::func<double> >::value, "ramp and cyclic color maps must have the same signature to share wrapper function as written");
	const std::string arg1 = isSphere ? "azimuths" : "radii" ;
	const std::string arg2 = isSphere ? "polars"   : "angles";

	//parse arguments
	PyObject *array1 = NULL, *array2 = NULL, *symName = NULL;
	char* map = NULL;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	int iScale = 0, iAlpha = 0, iFloat = 0, iW0 = 0;//python predicate takes a pointer to an int
	static char const* kwlist[] = {arg1.c_str(), arg2.c_str(), "map", /*begin keyword only*/ "fill", "scale", "alpha", "float", "w_cen", "sym", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO|s$dppppO", const_cast<char**>(kwlist), &array1, &array2, &map, &fill, &iScale, &iAlpha, &iFloat, &iW0, &symName)) return NULL;
	const bool scale = iScale != 0, alpha = iAlpha != 0, fp = iFloat != 0, w0 = iW0 != 0;//convert from int -> boolean

	//parse color map function, fill value, and symmetry
	colormap::disk::func<double> colorFunc;
	bool fillPassed;
	colormap::Sym sym;
	getMap(colorFunc, map, defaultFunc, isSphere ? getSphere : getDisk);
	if(!getFill(fill, fillPassed)) return NULL;
	if(!parseSym(symName, sym)) return NULL;

	//get array objects as doubles and their dimensions
	PyArrayObject *input1, *input2;
	size_t totalPoints;
	std::vector<npy_intp> newDims;
	if(!getArray(array1, input1, totalPoints, &newDims)) return NULL;
	if(!getArray(array2, input2, totalPoints          )) {
		Py_XDECREF(input1);
		return NULL;
	}

	//make sure the arrays at the same shape
	if(!PyArray_SAMESHAPE(input1, input2)) {
		PyErr_SetString(PyExc_ValueError, "both input arrays must have the same shape");
		Py_XDECREF(input1);
		Py_XDECREF(input2);
		return NULL;
	}

	//create new array with an extra dimension tacked onto the end
	const size_t stride = alpha ? 4 : 3;
	newDims.push_back(stride);//add rgb dimension
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY((int)newDims.size(), newDims.data(), NPY_DOUBLE, 0);

	//get array pointers
	double const * const v1  = (double const*const)PyArray_DATA(input1);
	double const * const v2  = (double const*const)PyArray_DATA(input2);
	double       * const rgb = (double      *const)PyArray_DATA(output);
	
	//loop over scalars computing color
	bool hasNans = false;
	bool outOfRange = false;
	if(scale) {//rescale data before mapping to colors
		//build function to scale input to [0,1]		
		double delta1, scale1, delta2, scale2;
		minMaxScale(v1, totalPoints, delta1, scale1);
		minMaxScale(v2, totalPoints, delta2, scale2);
		const std::function<double(const double&)> func1 = std::bind(scaleClamp, std::placeholders::_1, delta1, scale1);
		const std::function<double(const double&)> func2 = std::bind(scaleClamp, std::placeholders::_1, delta2, scale2);

		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double& x1 = func1(v1[i]);//get rescaled value
			const double& x2 = func2(v2[i]);//get rescaled value
			if(std::isnan(x1) || std::isnan(x2)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(x1, x2, rgb + stride * i, w0, sym);//compute color for in valid values
			}
		}
	} else {//use data as is
		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double& x1 = v1[i];//get raw value
			const double& x2 = v2[i];//get raw value
			if(std::isnan(x1) || std::isnan(x2)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for NANs
			} else if(x1 < 0.0 || x1 > 1.0 || x2 < 0.0 || x2 > 1.0) {//handle values outside of [0,1]
				outOfRange = true;
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(x1, x2, rgb + stride * i, w0, sym);//compute color for in valid values
			}
		}
	}

	//warn if the fill value was used without being explicitly passed and return
	if(hasNans    && !fillPassed) PyErr_WarnEx(NULL, "NAN values were colored with the default fill value"        , 1);
	if(outOfRange && !fillPassed) PyErr_WarnEx(NULL, "values outside of [0,1] colored with the default fill value", 1);
	Py_XDECREF(input1);
	Py_XDECREF(input2);
	return fp ? (PyObject*)output : to8Bit(output);
}

//@brief wrapper function for disk legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map       : name of color map to create legend for
//             @keyword width     : [optional] width & height of legend in pixels
//             @keyword fill      : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_r/p: [optional] width & height of legend in pixels
//             @keyword ripple_a  : [optional] width & height of legend in pixels
//             @keyword alpha     : [optional] true / false to include an alpha channel
//             @keyword float     : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen     : [optional] width & height of legend in pixels
//             @keyword sym       : [optional] width & height of legend in pixels
template <bool isSphere>
static PyObject* circ_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	static_assert(std::is_same<colormap::disk::func<double>, colormap::sphere::func<double> >::value, "disk and sphere color maps must have the same signature to share wrapper function as written");

	//parse arguments
	char *map = NULL, *projName = NULL;
	unsigned int width = 512;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	double prRipple = 0.0, atRipple = 0.0;
	int iAlpha = 0, iFloat = 0, iW0 = isSphere ? 1 : 0;//python predicate takes a pointer to an int
	PyObject* symName = NULL;
	static char const* kwlist[] = {"map", "width", /*begin keyword only*/ "fill", isSphere ? "ripple_p" : "ripple_r", "ripple_a", "alpha", "float", "w_cen", "sym", "proj", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "s|I$dddpppOs", const_cast<char**>(kwlist), &map, &width, &fill, &prRipple, &atRipple, &iAlpha, &iFloat, &iW0, &symName, &projName)) return NULL;
	const bool alpha = iAlpha != 0, fp = iFloat != 0, w0 = iW0 != 0;//convert from int -> boolean

	//parse projection type
	colormap::sphere::Projection proj = colormap::sphere::Projection::Stereo;//default to stereographic projection
	if(NULL != projName) {//a projection name was specified
		if(isSphere) {
			std::string name = cleanString(projName);
			if     (0 == name.compare("o")) proj = colormap::sphere::Projection::Ortho  ;
			else if(0 == name.compare("s")) proj = colormap::sphere::Projection::Stereo ;
			else if(0 == name.compare("l")) proj = colormap::sphere::Projection::Lambert;
			else if(0 == name.compare("d")) proj = colormap::sphere::Projection::Dist   ;
			else {
				std::stringstream ss;
				ss << "couldn't find a projection type matching '" << name << "'";
				PyErr_SetString(PyExc_ValueError, "'proj' must be one of {'o', 's', 'l', 'd'}");
				return NULL;
			}
		} else {//projection name is disallowed for disk legend
			PyErr_SetString(PyExc_ValueError, "unknown argument 'proj'");
			return NULL;
		}
	}

	//parse color map function, fill value, and symmetry
	colormap::disk::func<double> colorFunc;
	bool fillPassed;
	colormap::Sym sym;
	if(!getMap(colorFunc, map, (colormap::disk::func<double>) NULL, isSphere ? getSphere : getDisk)) return NULL;
	if(!getFill(fill, fillPassed)) return NULL;
	if(!parseSym(symName, sym)) return NULL;

	//build color map and return
	npy_intp mapDims[3] = {(npy_intp)width * (isSphere ? 2 : 1), (npy_intp)width, alpha ? 4 : 3};//output array dimensions
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY(3, mapDims, NPY_DOUBLE, 0);
	double * const pData = (double *const)PyArray_DATA(output);
	if(isSphere) {
		const size_t offset = width * width * (alpha ? 4 : 3);//get offset between hemispheres
		colormap::sphere::legend(colorFunc, pData         , false, proj, w0, sym, prRipple, atRipple, alpha, width, fill);//southern hemisphere
		colormap::sphere::legend(colorFunc, pData + offset, true , proj, w0, sym, prRipple, atRipple, alpha, width, fill);//northern hemisphere
	} else {
		colormap::disk  ::legend(colorFunc, pData         ,              w0, sym, prRipple, atRipple, alpha, width, fill);
	}
	return fp ? (PyObject*)output : to8Bit(output);
}

////////////////////////////////////////////////////////////////
//              Colormap Wrapper Implementations              //
////////////////////////////////////////////////////////////////

//@brief wrapper function for linear color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword scalars: array of values to compute color map for
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
static PyObject* ramp_wrapper  (PyObject* self, PyObject* args, PyObject* kwds) {return linear_wrapper<false>(self, args, kwds);}

//@brief wrapper function for cyclic color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword scalars: array of values to compute color map for
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
static PyObject* cyclic_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {return linear_wrapper<true >(self, args, kwds);}

//@brief wrapper function for disk color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword radii  : array of radii to compute color map for
//             @keyword angles : array of angles to compute color map for (must be the same shape as radii)
//             @keyword map    : [optional] name of color map to use
//             @keyword fill   : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale  : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha  : [optional] true / false to include an alpha channel
//             @keyword float  : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen  : true/false white/black center
//             @keyword sym    : type of inversion symmetry to apply
static PyObject* disk_wrapper  (PyObject* self, PyObject* args, PyObject* kwds) {return circ_wrapper<false>(self, args, kwds);}

//@brief wrapper function for sphere color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword azimuths: array of radii to compute color map for
//             @keyword polars  : array of angles to compute color map for (must be the same shape as radii)
//             @keyword map     : [optional] name of color map to use
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale   : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : true/false white/black center
//             @keyword sym     : type of inversion symmetry to apply
static PyObject* sphere_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {return circ_wrapper<true >(self, args, kwds);}

//@brief wrapper function for ball color maps
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword radii   : array of radii to compute color map for
//             @keyword azimuths: array of azimuthal angles to compute color map for (must be the same shape as radii)
//             @keyword polars  : array of polar angles to compute color map for (must be the same shape as radii)
//             @keyword map     : [optional] name of color map to use
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword scale   : [optional] flag to rescale values to [0,1] before coloring
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : true/false white/black center
//             @keyword sym     : type of inversion symmetry to apply
static PyObject* ball_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	static const char* defaultName = "four";
	static const colormap::ball::func<double> defaultFunc = getBall(defaultName);

	//parse arguments
	PyObject *array1 = NULL, *array2 = NULL, *array3 = NULL, *symName = NULL;
	char* map = NULL;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	int iScale = 0, iAlpha = 0, iFloat = 0, iW0 = 0;//python predicate takes a pointer to an int
	static char const* kwlist[] = {"radii", "azimuths", "polars", "map", /*begin keyword only*/ "fill", "scale", "alpha", "float", "w_cen", "sym", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OOO|s$dppppO", const_cast<char**>(kwlist), &array1, &array2, &array3, &map, &fill, &iScale, &iAlpha, &iFloat, &iW0, &symName)) return NULL;
	const bool scale = iScale != 0, alpha = iAlpha != 0, fp = iFloat != 0, w0 = iW0 != 0;//convert from int -> boolean

	//parse color map function, fill value, and symmetry
	colormap::ball::func<double> colorFunc;
	bool fillPassed;
	colormap::Sym sym;
	getMap(colorFunc, map, defaultFunc, getBall);
	if(!getFill(fill, fillPassed)) return NULL;
	if(!parseSym(symName, sym)) return NULL;

	//get array objects as doubles and their dimensions
	PyArrayObject *input1, *input2, *input3;
	size_t totalPoints;
	std::vector<npy_intp> newDims;
	if(!getArray(array1, input1, totalPoints, &newDims)) return NULL;
	if(!getArray(array2, input2, totalPoints          )) {
		Py_XDECREF(input1);
		return NULL;
	}
	if(!getArray(array3, input3, totalPoints          )) {
		Py_XDECREF(input1);
		Py_XDECREF(input2);
		return NULL;
	}

	//make sure the arrays at the same shape
	if(!PyArray_SAMESHAPE(input1, input2) || !PyArray_SAMESHAPE(input1, input3)) {
		PyErr_SetString(PyExc_ValueError, "all three input arrays must have the same shape");
		Py_XDECREF(input1);
		Py_XDECREF(input2);
		Py_XDECREF(input3);
		return NULL;
	}

	//create new array with an extra dimension tacked onto the end
	const size_t stride = alpha ? 4 : 3;
	newDims.push_back(stride);//add rgb dimension
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY((int)newDims.size(), newDims.data(), NPY_DOUBLE, 0);

	//get array pointers
	double const * const v1  = (double const*const)PyArray_DATA(input1);
	double const * const v2  = (double const*const)PyArray_DATA(input2);
	double const * const v3  = (double const*const)PyArray_DATA(input3);
	double       * const rgb = (double      *const)PyArray_DATA(output);
	
	//loop over scalars computing color
	bool hasNans = false;
	bool outOfRange = false;
	if(scale) {//rescale data before mapping to colors
		//build function to scale input to [0,1]		
		double delta1, scale1, delta2, scale2, delta3, scale3;
		minMaxScale(v1, totalPoints, delta1, scale1);
		minMaxScale(v2, totalPoints, delta2, scale2);
		minMaxScale(v3, totalPoints, delta3, scale3);
		const std::function<double(const double&)> func1 = std::bind(scaleClamp, std::placeholders::_1, delta1, scale1);
		const std::function<double(const double&)> func2 = std::bind(scaleClamp, std::placeholders::_1, delta2, scale2);
		const std::function<double(const double&)> func3 = std::bind(scaleClamp, std::placeholders::_1, delta3, scale3);

		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double& x1 = func1(v1[i]);//get rescaled value
			const double& x2 = func2(v2[i]);//get rescaled value
			const double& x3 = func2(v3[i]);//get rescaled value
			if(std::isnan(x1) || std::isnan(x2) || std::isnan(x3)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(x1, x2, x3, rgb + stride * i, w0, sym);//compute color for in valid values
			}
		}
	} else {//use data as is
		//loop over values computing colors
		for(size_t i = 0; i < totalPoints; i++) {
			const double& x1 = v1[i];//get raw value
			const double& x2 = v2[i];//get raw value
			const double& x3 = v3[i];//get raw value
			if(std::isnan(x1) || std::isnan(x2) || std::isnan(x3)) {//handle NANs
				hasNans = true;//at least once value was outside of [0,1]
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for NANs
			} else if(x1 < 0.0 || x1 > 1.0 || x2 < 0.0 || x2 > 1.0 || x3 < 0.0 || x3 > 1.0) {//handle values outside of [0,1]
				outOfRange = true;
				std::fill(rgb + stride * i, rgb + stride * i + stride, fill);//use fill color for out of range values
			} else {
				colorFunc(x1, x2, x3, rgb + stride * i, w0, sym);//compute color for in valid values
			}
		}
	}

	//warn if the fill value was used without being explicitly passed and return
	if(hasNans    && !fillPassed) PyErr_WarnEx(NULL, "NAN values were colored with the default fill value"        , 1);
	if(outOfRange && !fillPassed) PyErr_WarnEx(NULL, "values outside of [0,1] colored with the default fill value", 1);
	Py_XDECREF(input1);
	Py_XDECREF(input2);
	Py_XDECREF(input3);
	return fp ? (PyObject*)output : to8Bit(output);
}

////////////////////////////////////////////////////////////////
//               Legend Wrapper Implementations               //
////////////////////////////////////////////////////////////////

//@brief wrapper function for ramp legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map   : name of color map to create legend for
//             @keyword width : [optional] width of legend in pixels
//             @keyword height: [optional] height of legend in pixels
//             @keyword ripple: [optional] true/false for rippled / flat legend
//             @keyword alpha : [optional] true / false to include an alpha channel
//             @keyword float : [optional] true / false to return array of doubles / uint8_t
static PyObject* ramp_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	//parse arguments
	char* map = NULL;
	unsigned int width = 512;
	unsigned int height = 128;
	int iRipple = 1, iAlpha = 0, iFloat = 0;//python predicate takes a pointer to an int
	static char const* kwlist[] = {"map", "width", "height", /*begin keyword only*/ "ripple", "alpha", "float", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "s|II$ppp", const_cast<char**>(kwlist), &map, &width, &height, &iRipple, &iAlpha, &iFloat)) return NULL;
	const bool ripple = iRipple != 0, alpha = iAlpha != 0, fp = iFloat != 0;//convert from int -> boolean

	//check if a color map name was provided and select the corresponding function
	colormap::ramp::func<double> colorFunc;
	if(!getMap(colorFunc, map, (colormap::ramp::func<double>)NULL, getRamp)) return NULL;

	//build color map and return
	npy_intp mapDims[3] = {(npy_intp)height, (npy_intp)width, alpha ? 4 : 3};//output array dimensions
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY(3, mapDims, NPY_DOUBLE, 0);
	colormap::ramp::legend(colorFunc, (double *const)PyArray_DATA(output), ripple, alpha, width, height);
	return fp ? (PyObject*)output : to8Bit(output);
}

//@brief wrapper function for cyclic legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map   : name of color map to create legend for
//             @keyword width : [optional] width & height of legend in pixels
//             @keyword fill  : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple: [optional] true/false for rippled / flat legend
//             @keyword alpha : [optional] true / false to include an alpha channel
//             @keyword float : [optional] true / false to return array of doubles / uint8_t
static PyObject* cyclic_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	//parse arguments
	char* map = NULL;
	unsigned int width = 512;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	int iRipple = 1, iAlpha = 0, iFloat = 0;//python predicate takes a pointer to an int
	static char const* kwlist[] = {"map", "width", /*begin keyword only*/ "fill", "ripple", "alpha", "float", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "s|I$dppp", const_cast<char**>(kwlist), &map, &width, &fill, &iRipple, &iAlpha, &iFloat)) return NULL;
	const bool ripple = iRipple != 0, alpha = iAlpha != 0, fp = iFloat != 0;//convert from int -> boolean

	//parse color map function and fill value
	colormap::ramp::func<double> colorFunc;
	bool fillPassed;
	if(!getMap(colorFunc, map, (colormap::ramp::func<double>)NULL, getCyclic)) return NULL;
	if(!getFill(fill, fillPassed)) return NULL;

	//build color map and return
	npy_intp mapDims[3] = {(npy_intp)width, (npy_intp)width, alpha ? 4 : 3};//output array dimensions
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY(3, mapDims, NPY_DOUBLE, 0);
	colormap::cyclic::legend(colorFunc, (double *const)PyArray_DATA(output), ripple, alpha, width, fill);
	return fp ? (PyObject*)output : to8Bit(output);
}

//@brief wrapper function for disk legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width & height of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_r: [optional] width & height of legend in pixels
//             @keyword ripple_a: [optional] width & height of legend in pixels
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
static PyObject* disk_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {return circ_legend_wrapper<false>(self, args, kwds);}

//@brief wrapper function for spherical legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width & height of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_p: [optional] width & height of legend in pixels
//             @keyword ripple_a: [optional] width & height of legend in pixels
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
//             @keyword proj    : [optional] sphere -> disk projection type
static PyObject* sphere_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {return circ_legend_wrapper<true >(self, args, kwds);}

//@brief wrapper function for spherical legend generation
//@param self: NULL or object pointed to at module creation
//@param args: arguments
//@param kwds: keywords
//             @keyword map     : name of color map to create legend for
//             @keyword width   : [optional] width, height, and depth of legend in pixels
//             @keyword fill    : [optional] color for bad pixels (NAN or outside of [0,1])
//             @keyword ripple_r: [optional] magnitude of ripple in radial direction
//             @keyword ripple_p: [optional] magnitude of ripple in polar direction
//             @keyword ripple_a: [optional] magnitude of ripple in azimuthal direction
//             @keyword alpha   : [optional] true / false to include an alpha channel
//             @keyword float   : [optional] true / false to return array of doubles / uint8_t
//             @keyword w_cen   : [optional] width & height of legend in pixels
//             @keyword sym     : [optional] width & height of legend in pixels
static PyObject* ball_legend_wrapper(PyObject* self, PyObject* args, PyObject* kwds) {
	//parse arguments
	char* map = NULL;
	unsigned int width = 128;
	double fill = -NAN;//technically this can be passed by float('-nan'), but shouldn't happen in normal use
	double rRipple = 0.0, pRipple = 0.0, aRipple = 0.0;
	int iAlpha = 0, iFloat = 0, iW0 = 1;//python predicate takes a pointer to an int
	PyObject* symName = NULL;
	static char const* kwlist[] = {"map", "width", /*begin keyword only*/ "fill", "ripple_r", "ripple_p", "ripple_a", "alpha", "float", "w_cen", "sym", NULL};
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "s|I$ddddpppO", const_cast<char**>(kwlist), &map, &width, &fill, &rRipple, &pRipple, &aRipple, &iAlpha, &iFloat, &iW0, &symName)) return NULL;
	const bool alpha = iAlpha != 0, fp = iFloat != 0, w0 = iW0 != 0;//convert from int -> boolean

	//parse color map function, fill value, and symmetry
	colormap::ball::func<double> colorFunc;
	bool fillPassed;
	colormap::Sym sym;
	if(!getMap(colorFunc, map, (colormap::ball::func<double>) NULL, getBall)) return NULL;
	if(!getFill(fill, fillPassed)) return NULL;
	if(!parseSym(symName, sym)) return NULL;

	//build color map and return
	npy_intp mapDims[4] = {(npy_intp)width, (npy_intp)width, (npy_intp)width, alpha ? 4 : 3};//output array dimensions
	PyArrayObject* output = (PyArrayObject*)PyArray_EMPTY(4, mapDims, NPY_DOUBLE, 0);
	colormap::ball::legend(colorFunc, (double *const)PyArray_DATA(output), w0, sym, rRipple, pRipple, aRipple, alpha, width, fill, 32);
	return fp ? (PyObject*)output : to8Bit(output);
}

#endif//_colormap_wrapper_h_