/*
    This file is part of Graph Cut (Gc) combinatorial optimization library.
    Copyright (C) 2008-2010 Centre for Biomedical Image Analysis (CBIA)
    Copyright (C) 2008-2010 Ondrej Danek

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published 
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Graph Cut library. If not, see <http://www.gnu.org/licenses/>.
*/

/**
    @file
    Pairwise clique potentials approximating a general Riemannian metric
    (O. Danek approximation).

    @author Ondrej Danek <ondrej.danek@gmail.com>
    @date 2010
*/

#ifndef GC_ENERGY_POTENTIAL_METRIC_RIEMANNIANDANEK_H
#define GC_ENERGY_POTENTIAL_METRIC_RIEMANNIANDANEK_H

#include "../../../Core.h"
#include "../../../Type.h"
#include "../../../Math/Algebra/SquareMatrix.h"
#include "../../Neighbourhood.h"

namespace Gc
{    
    namespace Energy
    {
        /** Clique potential definitions. */
        namespace Potential
        {
            /** Clique potentials that approximate a metric. */
            namespace Metric
            {
                /** 2-clique potentials approximating a general Riemannian metric. 

                    This class implements calculation of edge weights approximating
                    a general Riemannian metric. 
                    
                    It is based on the following papers:
                    - O. Danek and P. Matula. <em>An Improved Riemannian %Metric Approximation 
                     for Graph Cuts</em>, In 16th IAPR International Conference on Discrete 
                     Geometry for Computer Imagery, 2011.
                    - O. Danek and P. Matula. <em>Graph Cuts and Approximation of the
                     Euclidean %Metric on Anisotropic Grids</em>, In VISAPP '10:
                     Proceedings of the International Conference on Computer Vision
                     Theory and Applications. 2010.
                    - Y. Boykov and V. Kolmogorov. <em>Computing geodesics and minimal 
                     surfaces via graph cuts</em>. In ICCV '03: Proceedings of the Ninth 
                     IEEE International Conference on Computer Vision, page 26, 2003.

                    Assume we have a regular orthogonal grid of nodes and that all these
                    nodes have equivalent neighbourhood \f$ \mathcal{N} = (n_1, n_2, \dots, n_N) \f$.
                    Further, assume we are given a metric tensor (symmetric positive definite 
                    matrix) \c D describing the Riemannian space in one given node and 
                    that \f$ T = \sqrt{D} \f$ is something what we call the <em>transformation 
                    matrix</em>. The weights approximating the Riemannian space in given
                    node are calculated as follows:

                    \f[
                        w_i = \frac{\Delta \rho_i \Delta \phi^v_i}{k}
                    \f]

                    where k is \f$ 2 \f$ or \f$ \pi \f$ for \c N=2 and \c N=3, respectively,
                    \f$ \Delta \phi^v_i \f$ is partitioning of the space of all angular orientations
                    among vectors in \f$ \mathcal{N} \f$ and \f$ \Delta \rho_i \f$ is the density 
                    of lines generated by vector \f$ n_i \f$ in the Riemannian space.
                    
                    To calculate \f$ \Delta \rho_i \f$ one can use the following formula: 

                    \f[
                        \Delta \rho_i = \frac{\mathrm{det} \, T}{|T \cdot n_i|}
                    \f]

                    To calculate \f$ \Delta \phi^v_i \f$ we use the following approach.
                    Let \f$ \mathcal{S} = (\frac{T \cdot n_1}{|T \cdot n_1|}, \dots, 
                    \frac{T \cdot n_N}{|T \cdot n_N|}) \f$ be
                    a set of points of a hypersphere representing the space of all
                    possible angular orientations. We calculate the Voronoi diagram of 
                    these points on the \f$ N-1 \f$ hypersphere surface manifold and
                    set \f$ \Delta \phi^v_i \f$ to be the area/length of the Voronoi cell
                    corresponding to point \f$ \frac{T \cdot n_i}{|T \cdot n_i|} \f$.

                    The difference between this approach and the one used in RiemannianBoykov
                    is that this approach gives better approximation with smaller error
                    but is more complicated to calculate (slower). Comparison can
                    be found in the referenced papers.

                    For info how to simulate the Euclidean metric or grid with specific
                    isotropic/anisotropic node spacing resolution see SetTransformationMatrix().

                    @see RiemannianBoykov, SetTransformationMatrix().

                    @tparam N Number of dimensions.
                    @tparam T Precision.
                */
                template <Size N, class T>
                class GC_DLL_EXPORT RiemannianDanek
                {
                protected:
                    /** Neighbourhood. */
                    Neighbourhood<N,T> m_nb;
                    /** Edge weights. */
                    System::Collection::Array<1,T> m_rw;

                public:
                    /** Constructor.

                        @param[in] n Neighbourhood used.
                        
                        @remarks Don't forget to call SetTransformation() before accessing 
                            the edge weights.
                    */
                    RiemannianDanek(const Neighbourhood<N,T> &n)
                        : m_nb(n)
                    {
                        m_rw.Resize(m_nb.Elements(), 0);
                    }

                    /** Specify the transformation of the Riemannian space.

                        @param[in] mt Positive definite symmetric matrix specifying the 
                        transformation of the space. The space is stretched in the directions 
                        given by the eigenvectors of this matrix by the amount equal to the
                        corresponding eigenvalues.
                        
                        To approximate Euclidean metric use identity matrix as the transformation
                        matrix. To simulate a possibly anisotropic grid with resolution 
                        \f$ \delta = (\delta_1, \delta_2, \dots) \f$ use \f$ T = \mathrm{diag}(\delta)\f$ 
                        transformation matrix .
                         
                        @warning Don't confuse the transformation matrix with the metric tensor. 
                        The metric tensor is a square of the transformation matrix or conversely 
                        the transformation matrix is the square root of the metric tensor, i.e., it 
                        has the same eigenvectors but eigenvalues are square roots of the metric 
                        tensor ones.                        
                    */
                    RiemannianDanek<N,T>& SetTransformationMatrix
                        (const Math::Algebra::SquareMatrix<N,T> &mt);

                    /** Get edge weights approximating the Euclidean metric. */
                    const System::Collection::Array<1,T>& EdgeWeights() const
                    {
                        return m_rw;
                    }

                    /** Get edge weight for i-th neighbour. */
                    T operator[] (Size i) const
                    {
                        return m_rw[i];
                    }
                };
            }
        }
    }
}

#endif
