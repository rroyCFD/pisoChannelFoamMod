/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
Application
    pisoChannelFoamMod

Description
    Transient solver for incompressible flow in a periodic channel.
    Turbulence modelling is generic, i.e. laminar, RAS or LES may be selected.

    Algorithom is implemented followed by the outline in book:
        The Finite Volume Method in Computational Fluid Dynamics
        Section 15.7 (SIMPLE family of algorithoms)

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulenceModel.H"
#include "orthogonalSnGrad.H"

#include "IFstream.H"
#include "OFstream.H"
#include "IOmanip.H" // for input/ouput format control
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H" // step 1 (set inital value)
    #include "readTransportProperties.H"
    #include "createFields.H"
    #include "initContinuityErrs.H"
    #include "readTimeControls.H" // for fixed courant number
    #include "createGradP.H"

    // define owner to neighbour unit-vector "ed" and gradpDiff_f fields
    #include "RhieChow.H"
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    // run-time coverge and advance loop
    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << endl;

        #include "readPISOControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H" // for fixed courant number, variable timeStep

        // // store old values for temporal discretization,
        // // & temporal correction to phi in Rhie-Chow flux calculation.
        // U.storeOldTime();  // known lead inaccurate solver performance, reason unknown
        // phi.storeOldTime();// known lead inaccurate solver performance, reason unknown

        // --- PISO loop
        for (int corr=1; corr<=nCorr; corr++)
        {
            #include "UEqn.H"
            #include "ppEqn.H"

            // PRIME loop
            for (int nPrime=1; nPrime <= nPrimeIterations; nPrime++)
            {
                #include "PRIME.H"
            }
            // end of PRIME loop


            if(corr == nCorr)
            {
                // estimate continuty error
                #include "continuityErrs.H"

                // calculate gradP and update velocity field (at the end of convergence)
                #include "calculateGradP.H"
            }

        }// end of corrector loop

        // update turbulence and nuEff
        turbulence->correct();
        nuEff = turbulence->nuEff();

        runTime.write();
        #include "writeGradP.H"

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    } // end of runTime loop

    Info<< "End\n" << endl;

    return 0;
}

// ************************************************************************* //
