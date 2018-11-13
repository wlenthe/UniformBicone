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

#include "colormap_wrapper.hpp"

//method table
static PyMethodDef methodDef[] = {//{function name in python module, function pointer, argument types, description}
	//color map functions
	{ramp_name         .c_str(), (PyCFunction) ramp_wrapper         , METH_VARARGS | METH_KEYWORDS, ramp_help         .c_str()},
	{cyclic_name       .c_str(), (PyCFunction) cyclic_wrapper       , METH_VARARGS | METH_KEYWORDS, cyclic_help       .c_str()},
	{disk_name         .c_str(), (PyCFunction) disk_wrapper         , METH_VARARGS | METH_KEYWORDS, disk_help         .c_str()},
	{sphere_name       .c_str(), (PyCFunction) sphere_wrapper       , METH_VARARGS | METH_KEYWORDS, sphere_help       .c_str()},
	{ball_name         .c_str(), (PyCFunction) ball_wrapper         , METH_VARARGS | METH_KEYWORDS, ball_help         .c_str()},

	//legend functions
	{cyclic_legend_name.c_str(), (PyCFunction) cyclic_legend_wrapper, METH_VARARGS | METH_KEYWORDS, cyclic_legend_help.c_str()},
	{ramp_legend_name  .c_str(), (PyCFunction) ramp_legend_wrapper  , METH_VARARGS | METH_KEYWORDS, ramp_legend_help  .c_str()},
	{disk_legend_name  .c_str(), (PyCFunction) disk_legend_wrapper  , METH_VARARGS | METH_KEYWORDS, disk_legend_help  .c_str()},
	{sphere_legend_name.c_str(), (PyCFunction) sphere_legend_wrapper, METH_VARARGS | METH_KEYWORDS, sphere_legend_help.c_str()},
	{ball_legend_name  .c_str(), (PyCFunction) ball_legend_wrapper  , METH_VARARGS | METH_KEYWORDS, ball_legend_help  .c_str()},
	{NULL, NULL, 0, NULL}//sentinel
};

//module definition
static struct PyModuleDef moduleDef = {
	PyModuleDef_HEAD_INIT,
	module_name.c_str(), //module name
	module_help.c_str(),//module documentation, may be NULL
	-1,//size of per-interpreter state of the module or -1 if the module keeps state in global variables.
	methodDef//method table
};

//module initialization function
PyMODINIT_FUNC PyInit_colormap(void) {
	import_array();//import numpy
	PyObject* m = PyModule_Create(&moduleDef);//try to create module object
	return m;//return module object (NULL on failure to create)
}