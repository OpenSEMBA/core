// OpenSEMBA
// Copyright (C) 2015 Salvador Gonzalez Garcia        (salva@ugr.es)
//                    Luis Manuel Diaz Angulo         (lmdiazangulo@semba.guru)
//                    Miguel David Ruiz-Cabello Nuñez (miguel@semba.guru)
//                    Daniel Mateos Romero            (damarro@semba.guru)
//
// This file is part of OpenSEMBA.
//
// OpenSEMBA is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// OpenSEMBA is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenSEMBA. If not, see <http://www.gnu.org/licenses/>.
#include "SolverDGTD.h"

SolverDGTD::SolverDGTD(SmbData* smb) {

    // Smb data adaptation and validation.
    MeshVolume mesh(*smb->mesh->castTo<MeshUnstructured>());
    comm_ = initMPI();

    // Time integrator initialization.
    options_ = smb->solverOptions->castTo<OptionsSolverDGTD>();
    integrator_ = initIntegrator(&mesh, smb->pMGroup, options_);
    integrator_->partitionate(&mesh, comm_);

    // Spatial discretization.
    dg_ = new DGExplicit(mesh, *smb->pMGroup, *smb->emSources, *options_, comm_);
    integrator_->setSolver(dg_);

    // Exporter initialization.
    cout << " - Initializing exporter... " << flush;
    out_ = new ExporterGiD(smb, smb->getFolder() + smb->getProjectName());
    cout << "[OK]" << endl;
}

SolverDGTD::~SolverDGTD() {
    delete out_;
    delete dg_;
    delete integrator_;
    delete comm_;
}

bool SolverDGTD::run() {
    Real tSum = 0.0;
    Real tRunning = 0.0;
    Real time = 0.0;
    const Real dt = integrator_->getMaxDT();
    assert(dt != 0.0);
    while (time < options_->getFinalTime()) {
        out_->process(time, *dg_->getElectric(), *dg_->getMagnetic());
        Real initCPUTime = storeCPUTime();
        integrator_->timeIntegrate(time);
        tSum += storeCPUTime() - initCPUTime;
        time += dt;
        printTimeProfilingInfo(tSum, tRunning, time / dt,
                options_->getNumberOfTimeSteps());
    }
    return true;
}

Integrator* SolverDGTD::initIntegrator(
        const MeshVolume* mesh,
        const PMGroup* pMGroup,
        const OptionsSolverDGTD* arg) {
    Integrator* res;
    switch (arg->getTimeIntegrator()) {
    case OptionsSolverDGTD::lserk4:
        cout<< "- Initializing LSERK Integrator." << endl;
        res = new IntegratorLSERK(*mesh, *pMGroup, arg);
        break;
    case OptionsSolverDGTD::lf2:
        cout<< "- Initializing LF2 Integrator." << endl;
        res = new IntegratorLF2(*mesh, *pMGroup, arg);
        break;
    case OptionsSolverDGTD::lf2full:
        cout<< "- Initializing LF2Full Integrator." << endl;
        res = new IntegratorLF2Full(*mesh, *pMGroup, arg);
        break;
    case OptionsSolverDGTD::verlet:
        cout<< "- Initializing Verlet Integrator." << endl;
        res = new IntegratorVerlet(*mesh, *pMGroup, arg);
        break;
    default:
        throw Error("Undefined time integrator.");
    }
    return res;
}

Comm* SolverDGTD::initMPI() {
#ifdef USE_MPI
    return new CommMPI();
#else
    return new CommNone();
#endif
}

bool SolverDGTD::canRun() const {
    throw ErrorNotImplemented("Can Run is not implemented.");
}
