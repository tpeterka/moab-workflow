# Copyright 2013-2020 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

from spack import *


class MoabWorkflow(CMakePackage):
    """Example of Testing Moab with Wilkins."""

    homepage = "https://github.com/tpeterka/moab-workflow.git"
    url      = "https://github.com/tpeterka/moab-workflow.git"
    git      = "https://github.com/tpeterka/moab-workflow.git"

    version('main', branch='main')

    depends_on('mpich')
    depends_on('hdf5+mpi+hl', type='link')
    depends_on('lowfive', type='link')
    depends_on('wilkins', type='link')

    def cmake_args(self):
        args = ['-DCMAKE_C_COMPILER=%s' % self.spec['mpich'].mpicc,
                '-DCMAKE_CXX_COMPILER=%s' % self.spec['mpich'].mpicxx,
                '-DBUILD_SHARED_LIBS=false',
                '-DLOWFIVE_PATH=%s' % self.spec['lowfive'].prefix]
        return args
