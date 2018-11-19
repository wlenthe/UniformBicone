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

//orientation coloring routines based on
// -Nolze, Gert and Hielscher Ralf. "Orientations Perfectly Colors." J. Appl. Crystallogr. 49.5 (2016): 1786-1802.

#ifndef _coloring_h_
#define _coloring_h_

#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>

#include "colormap.hpp"

namespace ipf {
	//ipf colors are only implemented for the Laue groups for conciseness, but other symmetries are possible
	//all functions have the same signature

	//@brief: compute IPF color for the Laue group of the named crystal system
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	//@param jumpFree: the \bar{3} group cannot be uniquely colored without jumps, use true/false for ambiguous jump free coloring/unambigous coloring with jumps (see Nolze figure 8a)
	template <typename Real> void triclinic     (Real const * const n, Real * const rgb                      );// \bar{1}
	template <typename Real> void monoclinic    (Real const * const n, Real * const rgb                      );// 2/m
	template <typename Real> void orthorhombic  (Real const * const n, Real * const rgb                      );// mmm
	template <typename Real> void tetragonalLow (Real const * const n, Real * const rgb                      );// 4/m
	template <typename Real> void tetragonalHigh(Real const * const n, Real * const rgb                      );// 4/mmm
	template <typename Real> void trigonalLow   (Real const * const n, Real * const rgb, bool jumpFree = true);// \bar{3}
	template <typename Real> void trigonalHigh  (Real const * const n, Real * const rgb                      );// \bar{3}m
	template <typename Real> void hexagonalLow  (Real const * const n, Real * const rgb                      );// 6/m
	template <typename Real> void hexagonalHigh (Real const * const n, Real * const rgb                      );// 6/mmm
	template <typename Real> void cubicLow      (Real const * const n, Real * const rgb                      );// m\bar{3}
	template <typename Real> void cubicHigh     (Real const * const n, Real * const rgb                      );// m\bar{3}m

	//@brief: helper function to access cyclic/dihedral groups with order template
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <size_t N, typename Real> void cyclic  (Real const * const n, Real * const rgb);// 2/m  , 4/m  , \bar{3} , or 6\m
	template <size_t N, typename Real> void dihedral(Real const * const n, Real * const rgb);// 2/mmm, 4/mmm, \bar{3}m, or 6\mmm
}

namespace ipf {
	namespace detail {
		//@brief: move a direction to the northern hemispehre if needed by applying \bar{1} symmetry
		//@param n: direction to modify
		template <typename Real> inline void bar1(Real * const n) {if(std::signbit(n[2])) std::transform(n, n+3, n, std::negate<Real>());}

		//helpers for rotation about Z axis
		//these could all be done with a simple atan2 and fmod, but atan2 is really expensive

		//@brief: move a direction to the +y hemispehre if needed by applying 2 fold symmetry about the z axis
		//@param n: direction to modify
		template <typename Real> inline void z2(Real * const n) {if(std::signbit(n[1])) std::transform(n, n+2, n, std::negate<Real>());}

		//@brief: move a direction to the first 3 fold sector
		//@param n: direction to modify
		template <typename Real> inline void z3(Real * const n) {
			static const Real k3 = std::sqrt(Real(3));
			static const Real c = Real(-1) / 2;
			static const Real s = k3 / 2;
			if(std::signbit(n[0])) {//(90, 270)
				const Real t = n[1] / n[0];
				if(t > -k3) {//(120,270),
					if(t > k3) {//(240,270), rotate by 120 @ 001
						const Real x = c * n[0] - s * n[1];
						n[1] = c * n[1] + s * n[0];
						n[0] = x;
					} else {//(120,240], rotate by -120 @ 001
						const Real x = c * n[0] + s * n[1];
						n[1] = c * n[1] - s * n[0];
						n[0] = x;
					}
				// } else {//(90,270]
					//already in fundamental sector
				}
			} else {//[-90, 90]
				if(std::signbit(n[1])) {//[270, 360), rotate by 120 @ 001
					//need to rotate by 120 @ 001
					const Real x = c * n[0] - s * n[1];
					n[1] = c * n[1] + s * n[0];
					n[0] = x;
				//} else {//[0, 90]
					//already in fundamental sector
				}
			}
		}

		//@brief: move a direction to the +x hemispehre if needed by applying 4 fold symmetry about the z axis
		//@param n: direction to modify
		template <typename Real> inline void z4(Real * const n) {
			z2(n);//first move to +y hemisphere
			if(std::signbit(n[0])) {
				std::swap(n[0], n[1]);//(y, x, z)
				n[1] = -n[1];//(y, -x, z)
			}
		}

		//@brief: move a direction to the first 6 fold sector
		//@param n: direction to modify
		template <typename Real> inline void z6(Real * const n) {
			static const Real k3 = std::sqrt(Real(3));
			static const Real c = Real(-1) / 2;
			static const Real s = k3 / 2;
			z2(n);//first move to +y hemisphere
			const Real t = n[1] / n[0];
			if(std::fabs(t) > k3) {//(60, 120), rotate by -60 @ 001
				const Real x = -c * n[0] + s * n[1];
				n[1] = -c * n[1] - s * n[0];
				n[0] = x;
			} else if(std::signbit(t)) {//(120,180], rotate by -120 @ 001
				const Real x = c * n[0] + s * n[1];
				n[1] = c * n[1] - s * n[0];
				n[0] = x;
			// } else {//[0,120]
				//already in fundamental sector
			}
		}

		//helpers to move to the high symmetry fundamental sector

		//@brief: move a direction to the orthorhombic fundamenal sector
		//@param n: direction to move to fundamental sector
		//@return: true / false if a mirror plane was / wasn't required to get to the fundamental zone
		template <typename Real> inline bool orthorhombic(Real * const n) {
			bar1(n);//start by moving to northern hemisphere with inversion symmetry
			z2(n);//move to +y hemisphere with 2 fold axis
			if(std::signbit(n[0])) {//check if mirror across x plane is needed
				n[0] = -n[0];
				return true;
			}
			return false;
		}

		//@brief: move a direction to the orthorhombic fundamenal sector
		//@param n: direction to move to fundamental sector
		//@return: true / false if a mirror plane was / wasn't required to get to the fundamental zone
		template <typename Real> inline bool tetragonal(Real * const n) {
			bar1(n);//start by moving to northern hemisphere with inversion symmetry
			z4(n);//move to +x/+y quadrant hemisphere with 4 fold axis
			if(n[1] > n[0]) {//check if mirror across 45 degree plane is needed
				std::swap(n[0], n[1]);
				return true;
			}
			return false;

		}

		//@brief: move a direction to the orthorhombic fundamenal sector
		//@param n: direction to move to fundamental sector
		//@return: true / false if a mirror plane was / wasn't required to get to the fundamental zone
		template <typename Real> inline bool trigonal(Real * const n) {
			bar1(n);//start by moving to northern hemisphere with inversion symmetry
			z3(n);//move to first 3 fold sector

			//flip across additional mirror plane if needed
			static const Real k3 = std::sqrt(Real(3));
			static const Real c = Real(1) / 2;
			static const Real s = k3 / 2;
			if(n[1] / n[0] > k3 || std::signbit(n[0])) {
				const Real x = s * n[1] - c * n[0];
				n[1] = s * n[0] + c * n[1];
				n[0] = x;
				return true;
			}
			return false;
		}

		//@brief: move a direction to the orthorhombic fundamenal sector
		//@param n: direction to move to fundamental sector
		//@return: true / false if a mirror plane was / wasn't required to get to the fundamental zone
		template <typename Real> inline bool hexagonal(Real * const n) {
			bar1(n);//start by moving to northern hemisphere with inversion symmetry
			z6(n);//move to first 3 fold sector

			//flip across additional mirror plane if needed
			static const Real k3 = std::sqrt(Real(3));
			static const Real k13 = Real(1) / k3;
			static const Real c = Real(1) / 2;
			static const Real s = k3 / 2;
			if(n[1] / n[0] > k13) {
				const Real x = s * n[1] + c * n[0];
				n[1] = s * n[0] - c * n[1];
				n[0] = x;
				return true;
			}
			return false;
		}

		//@brief: move a direction to the orthorhombic fundamenal sector
		//@param n: direction to move to fundamental sector
		//@return: true / false if a mirror plane was / wasn't required to get to the fundamental zone
		template <typename Real> inline bool cubic(Real * const n) {
			bar1(n);//start by moving to northern hemisphere with inversion symmetry
			z2(n);//move to +y hemisphere with 2 fold z axis
			n[0] = std::fabs(n[0]);//move to +x/+y quadrant with mirror across x plane

			//move to highest z third of first quadrant
			if(n[0] >= n[1]) {
				if(n[0] > n[2]) std::rotate(n, n+1, n+3);//rotate -120 @ 111
			} else {
				if(n[1] > n[2]) std::rotate(n, n+2, n+3);//rotate  120 @ 111
			}

			//flip across additional mirror plane if needed
			if(n[1] > n[0]) {
				std::swap(n[0], n[1]);
				return true;
			}
			return false;
		}

		//@brief: helper class for mapping a polygon on the sphere to the unit hemisphere
		template <typename Real>
		class SphericalTriangle {

			//vector helpers
			static Real Dot(Real const * const v1, Real const * const v2) {return std::inner_product(v1, v1+3, v2, Real(0));}

			static Real Mag(Real const * const v) {return std::sqrt(Dot(v, v));}

			static void Normalize(Real * const v) {
				const Real mag = Mag(v);
				std::transform(v, v+3, v, [mag](const Real& i){return i/mag;});
			}

			static void Cross(Real const * const v1, Real const * const v2, Real * const x) {
				const Real cross[3] = {
					x[0] = v1[1] * v2[2] - v1[2] * v2[1],
					x[1] = v1[2] * v2[0] - v1[0] * v2[2],
					x[2] = v1[0] * v2[1] - v1[1] * v2[0],
				};
				std::copy(cross, cross+3, x);
			}

			Real rx          [3];//direction of red from center
			Real ry          [3];//direction perpendicular to rx and center
			Real center      [3];//centroid of spherical triangle
			Real normals  [3][3];//normals of spherical triangle edges
			Real cutoffs  [ 3*3];//cutoff angles (start/end of fillets and angles of vertices)
			Real coeffs   [3][4];//fillet polynomial coefficients
			Real cumAngles[ 3+1];//cumulative angles of triangle vertices

			public:
				//@brief: construct a spherical triangle patch for a cyclic symmetry
				//@template N: order of z axis rotational symmetry
				//@return: filled triangular patch
				template <size_t N> static SphericalTriangle Cyclic() {
					const Real angle = Real(2) * std::acos(Real(0)) / Real(N);//pi/N
					const Real s = std::sin(angle);
					const Real c = std::cos(angle);
					const Real nRed  [3] = {Real(0), Real(0), Real(1)};
					const Real nGreen[3] = {Real(1), Real(0), Real(0)};
					const Real nBlue [3] = {     c ,      s , Real(0)};
					return SphericalTriangle(nRed, nGreen, nBlue);
				}

				//@brief: construct a spherical triangle patch for cubic symmetry
				//@return: filled triangular patch
				static SphericalTriangle Cubic() {
					const Real r2 = Real(1) / std::sqrt(Real(2));
					const Real r3 = Real(1) / std::sqrt(Real(3));
					const Real nRed  [3] = {Real(0), Real(0), Real(1)};
					const Real nGreen[3] = {r2     , Real(0),    r2  };
					const Real nBlue [3] = {r3     ,    r3  ,    r3  };
					return SphericalTriangle(nRed, nGreen, nBlue);
				}

				//@brief: construct a spherical triangle patch for IPF coloring
				//@param nRed: unit direction to color red
				//@param nGreen: unit direction to color green
				//@param nBlue: unit direction to color blue
				//@note: defines triangle patch in CCW order
				SphericalTriangle(Real const*const nRed, Real const*const nGreen, Real const*const nBlue) {
					//copy normals to array for convenience
					Real verts[3][3];
					std::copy(nRed  , nRed  +3, verts[0]);
					std::copy(nGreen, nGreen+3, verts[1]);
					std::copy(nBlue , nBlue +3, verts[2]);

					//first check if the three points are outside the hemisphere
					const Real det = verts[0][0] * verts[1][1] * verts[2][2]
					            + verts[0][1] * verts[1][2] * verts[2][0]
					            + verts[0][2] * verts[1][0] * verts[2][1]
					            - verts[0][0] * verts[1][2] * verts[2][1]
					            - verts[0][1] * verts[1][0] * verts[2][2]
					            - verts[0][2] * verts[1][1] * verts[2][0];
					if(det < std::numeric_limits<Real>::epsilon()) throw std::runtime_error("spherical triangle must be within single hemisphere");

					//compute center of spherical triangle
					for(size_t i = 0; i < 3; i++) center[i] = verts[0][i] + verts[1][i] + verts[2][i];
					Normalize(center);

					//build orthogonal coordinate system for center -> each vertex
					Real vx[3][3], vy[3][3];
					for(size_t i =0; i < 3; i++) {
						Cross(center, verts[i], vy[i]);
						Cross(vy[i] , center  , vx[i]);
						Normalize(vx[i]);
						Normalize(vy[i]);
					}
					std::copy(vx[0], vx[0]+3, rx);//red is the global x direction
					std::copy(vy[0], vy[0]+3, ry);//global y is perpindicular to x and patch center (z)

					//compute angles between successive verts
					Real angles[3];
					for(size_t i = 0; i < 3; i++) angles[i] = std::acos(Dot(vx[i], vx[(i+1)%3]));
					std::partial_sum(angles, angles+3, cumAngles+1);

					//compute normals of circles defining edges of domain
					for(size_t i = 0; i < 3; i++) {
						Cross(verts[i], verts[(i+1)%3], normals[i]);
						Normalize(normals[i]);
					}

					//compute cutoff angles for filleting
					static const Real filletFrac(0.05);//must be <= 0.5, currently 
					const Real deltas[3] = {M_PI * 2 / 3, M_PI * 2 / 3, M_PI * 2 / 3};
					for(size_t i = 0; i < 3; i++) {//loop over edges
						for(size_t j = 0; j < 3; j++) {//loop over verts
							const Real delta = 2 == j ? 0 : std::copysign(filletFrac * deltas[i], 0 == j ? 1 : -1);
							cutoffs[i*3+j] = cumAngles[ i +(j==0 ? 0 : 1)] + delta;
						}
					}

					//numerically compute r and dr/dtheta at transition points between linear and filleted regions
					Real radii[6], dRadii[6];
					for(size_t i = 0; i < 3; i++) {//loop over edges
						//compute angles where radius needs to be calculated
						const Real dT (0.1);//angular offset from vertex -> transition points (for numerical derivative calculation), ~1/2 degree
						static const Real hf(0.01);//fractional distance numerical derivative calculation (should probably be <= 1)
						const Real thetas[6] = {
							cutoffs[3*i+0] - hf * dT,//symmetric points for derivative calculation
							cutoffs[3*i+0]          ,//first transition
							cutoffs[3*i+0] + hf * dT,//symmetric points for derivative calculation
							cutoffs[3*i+1] - hf * dT,//symmetric points for derivative calculation
							cutoffs[3*i+1]          ,//second transition
							cutoffs[3*i+1] + hf * dT //symmetric points for derivative calculation
						};

						//apply rotations and compute radius/angle at each point
						Real r[6], t[6];
						for(size_t j = 0; j < 6; j++) {
							//compute normal of circle at desired angle (ry rotated about center)
							const Real c = std::cos(thetas[j] / 2);
							const Real s = std::sin(thetas[j] / 2);

							//q * n (w == 0 since rotation axis is perpendicular to vector)
							const Real x = c * ry[0] + s * (center[1] * ry[2] - center[2] * ry[1]);
							const Real y = c * ry[1] + s * (center[2] * ry[0] - center[0] * ry[2]);
							const Real z = c * ry[2] + s * (center[0] * ry[1] - center[1] * ry[0]);

							const Real m[3] = {//q * n * q.conj() [normal of circle at desired angle]
								x * c + s * (z * center[1] - y * center[2]),
								y * c + s * (x * center[2] - z * center[0]),
								z * c + s * (y * center[0] - x * center[1]),
							};

							//now compute intersection of two unit circles at origin w/ normals v and normals[edge]
							const Real& nx = normals[i][0];
							const Real& ny = normals[i][1];
							const Real& nz = normals[i][2];
							const Real& mx = m[0];
							const Real& my = m[1];
							const Real& mz = m[2];
							const Real den = std::sqrt(  nx * nx * ( my * my + mz * mz ) + ny * ny * ( mz * mz + mx * mx ) + nz * nz * ( mx * mx + my * my ) - Real(2) * ( nz * nx * mz * mx + nx * ny * mx * my + ny * nz * my * mz ) );

							Real v[3] = {//intersection of two circles (point along edge i at angle thetas[j])
								(ny * mz - nz * my) / den,
								(nz * mx - nx * mz) / den,
								(nx * my - ny * mx) / den,
							};
							if(std::signbit(Dot(v, center))) std::transform(v, v+3, v, std::negate<Real>());//select intersection point closest to center
							r[j] = std::acos(Dot(v, center));//compute angle from center -> edge at this theta
						}

						//save radii and compute derivative
						radii [i*2+0] = r[1];
						radii [i*2+1] = r[4];
						dRadii[i*2+0] = (r[2] - r[0]) / (hf * dT * 2);
						dRadii[i*2+1] = (r[5] - r[3]) / (hf * dT * 2);
					}

					//compute polynomial coefficients to remove discontinuity in r
					for(size_t i = 0; i < 3; i++) {//loop over edge
						const size_t j = (i+1)%3;//get index of next edge
						const Real v1 = radii [i*2+1];//value of radius at transition point in edge i (near edge j)
						const Real v2 = radii [j*2+0];//value of radius at transition point in edge j (near edge i)
						const Real m1 = dRadii[i*2+1] * filletFrac * angles[i];//value of d(radius)/d(theta) at transition point (multiply by range of -1->0 to correct derivative scaling)
						const Real m2 = dRadii[j*2+0] * filletFrac * angles[j];//value of d(radius)/d(theta) at transition point (multiply by range of  0->1 to correct derivative scaling)
						coeffs[i][0] = ( m1 + m2 + v1     - v2    ) / 4;
						coeffs[i][1] = (-m1 + m2                  ) / 4;
						coeffs[i][2] = (-m1 - m2 - v1 * 3 + v2 * 3) / 4;
						coeffs[i][3] = ( m1 - m2 + v1 * 2 + v2 * 2) / 4;
					}
				}

				//@brief: convert a unit direction in a fundamental sector to fractional (0-1) polar coordinates on the northern hemisphere
				//@param n: unit direction to color
				//@param theta: fractional azimuthal angle
				//@param phi: fractional polar angle
				void fundToHemi(Real const * const n, Real& theta, Real& phi) const {
					//compute angle with red direction
					Real v[3];
					std::transform(n, n + 3, center, v, std::minus<Real>());
					Real angle = std::atan2(Dot(ry, v), Dot(rx, v));
					if(std::signbit(angle)) angle+= M_PI * 2;
					theta = angle / (Real(M_PI) * 2);//convert angle w.r.t. red --> fractional
					
					//compute polar angle
					const size_t idx = std::distance(cutoffs, std::lower_bound(cutoffs, cutoffs + 9, angle));//determine which region this angle falls in
					const size_t i = idx / 3;//index of edge
					phi = std::acos(Dot(n, center));//angle between center and point
					if(phi < std::numeric_limits<Real>::epsilon()) return;//avoid divide by zero issues
					switch(idx - i * 3) {
						case 1: {//in linear region, normalize angle by max possible angle
							Real nxc[3];
							Cross(n         , center, nxc);//normal of arc through n/center
							Cross(normals[i], nxc   , v  );//intersection of two circles (edge of patch in direction theta)
							Normalize(v);
							phi /= std::acos(Dot(v, center)) * Real(2);//compute fractional progress ot edge
						} break;

						case 0: {//in first fillet
							const size_t j = (i+3-1)%3;
							Real x =  (angle - cumAngles[i  ]) / (cutoffs[idx  ] - cumAngles[i  ]);
							const Real den = coeffs[j][0] * x * x * x + coeffs[j][1] * x * x + coeffs[j][2] * x + coeffs[j][3];
							phi /= std::max(phi, den) * Real(2);//normalize, clipping at 1/2
						} break;

						case 2: {//in second fillet
							Real x = -(angle - cumAngles[i+1]) / (cutoffs[idx-1] - cumAngles[i+1]);
							const Real den = coeffs[i][0] * x * x * x + coeffs[i][1] * x * x + coeffs[i][2] * x + coeffs[i][3];
							phi /= std::max(phi, den) * Real(2);//normalize, clipping at 1/2
						} break;//num/2; break;
					}
				}

				//@brief: compute ipf coloring for a unit direction in the fundamental sector
				//@param n: unit direction in fundamental sector (undefined behavior for directions outside of sector)
				//@param rgb: location to write ipf color (0->1)
				//@param whiteCenter: was the direction reflected such that a white vs black center is required
				//@param hasMirror: does this symmetry group have a mirror at the fundamental sector boundary (if there are points with a black center false, otherwise true)
				void fundToIpf(Real const * const n, Real * const rgb, const bool whiteCenter = true, const bool hasMirror = true) {
					Real theta, phi;
					fundToHemi(n, theta, phi);
					colormap::detail::Maps<Real>::SixBi(theta, whiteCenter ? Real(1) - phi : phi, rgb, hasMirror);
				}
		};
	}

	//@brief: compute IPF color for the \bar{1} Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline triclinic     (Real const * const n, Real * const rgb) {
		//get fundamenetal sector equivalent of n
		Real nFS[3] = {n[0], n[1], n[2]};
		detail::bar1(nFS);//move to northern hemisphere with inversion symmetry
		Real theta = std::atan2(nFS[1], nFS[0]) / (Real(M_PI) * 2);
		if(std::signbit(theta)) theta += Real(1);
		const Real phi = std::acos(nFS[2]) / Real(M_PI);
		colormap::detail::Maps<Real>::SixBi.sphere(theta, phi, rgb, true, colormap::Sym::Polar);
	}

	//@brief: compute IPF color for the 2/m Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline monoclinic    (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<2>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		const bool mirrored = detail::orthorhombic(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb, !mirrored, false);
	}

	//@brief: compute IPF color for the mmm Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline orthorhombic  (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<2>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		detail::orthorhombic(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb);
	}

	//@brief: compute IPF color for the 4/m Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline tetragonalLow (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<4>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		const bool mirrored = detail::tetragonal(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb, !mirrored, false);
	}

	//@brief: compute IPF color for the 4/mmm Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline tetragonalHigh(Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<4>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		detail::tetragonal(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb);
	}

	//@brief: compute IPF color for the \bar{3} Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	//@param jumpFree: the \bar{3} group cannot be uniquely colored without jumps, use true/false for ambiguous jump free coloring/unambigous coloring with jumps (see Nolze figure 8a)
	template <typename Real> void inline trigonalLow   (Real const * const n, Real * const rgb, const bool jumpFree) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<3>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		const bool mirrored = detail::trigonal(nFS);//move direction to fundamental sector
		if(jumpFree) {
			Real theta, phi;
			tri.fundToHemi(nFS, theta, phi);
			colormap::detail::Maps<Real>::SixBi(theta, Real(1) - phi * 2, rgb, false);
		} else {
			tri.fundToIpf(nFS, rgb, !mirrored, false);
		}
	}

	//@brief: compute IPF color for the \bar{3}m Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline trigonalHigh  (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<6>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		detail::trigonal(nFS);//move direction to fundamental sector
		const bool mirrored = detail::hexagonal(nFS);
		tri.fundToIpf(nFS, rgb, !mirrored);
	}

	//@brief: compute IPF color for the 6/m Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline hexagonalLow  (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<6>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		const bool mirrored = detail::hexagonal(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb, !mirrored, false);
	}

	//@brief: compute IPF color for the 6/mmm Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline hexagonalHigh (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::template Cyclic<6>();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		detail::hexagonal(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb);
	}

	//@brief: compute IPF color for the m\bar{3} Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline cubicLow      (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::Cubic();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		const bool mirrored = detail::cubic(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb);
	}

	//@brief: compute IPF color for the m\bar{3}m Laue group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <typename Real> void inline cubicHigh     (Real const * const n, Real * const rgb) {
		static detail::SphericalTriangle<Real> tri = detail::SphericalTriangle<Real>::Cubic();
		Real nFS[3] = {n[0], n[1], n[2]};//copy direction
		detail::cubic(nFS);//move direction to fundamental sector
		tri.fundToIpf(nFS, rgb);
	}

	//@brief: compute IPF color for the N fold cyclic group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <size_t N, typename Real> void cyclic  (Real const * const n, Real * const rgb) {
		static_assert(N == 2 || N == 3 || N == 4 || N == 6, "only 2, 3, 4, and 6 fold cyclic groups are allowed");
		switch(N) {
			case 2: return monoclinic   (n, rgb);
			case 3: return trigonalLow  (n, rgb);
			case 4: return tetragonalLow(n, rgb);
			case 6: return hexagonalLow (n, rgb);
		}
	}

	//@brief: compute IPF color for the N fold cyclic group
	//@param n: unit crystallographic direction to compute color for
	//@param rgb: location to write ip color (0->1)
	template <size_t N, typename Real> void dihedral(Real const * const n, Real * const rgb) {
		static_assert(N == 2 || N == 3 || N == 4 || N == 6, "only 2, 3, 4, and 6 fold dihedral groups are allowed");
		switch(N) {
			case 2: return orthorhombic  (n, rgb);
			case 3: return trigonalHigh  (n, rgb);
			case 4: return tetragonalHigh(n, rgb);
			case 6: return hexagonalHigh (n, rgb);
		}
	}
}

#endif//_coloring_h_