// Correct driving force for a constant mass flow rate
{
    // Extract the mean velocity in the flow direction
    dimensionedScalar magUbarStar = (flowDirection & U)().weightedAverage(mesh.V());

    // Calculate the pressure gradient increment needed to
    // adjust the average flow-rate to the correct value
    // damping pDelta by relaxation (tends to dampen ocilation in gradP)
    dimensionedScalar pDelta = relaxation * (magUbar - magUbarStar)/rAU.weightedAverage(mesh.V());

    // update velocity field for incremental pressure-gradient
    // rAU & rAUf declared at ppEqn and updated in the PRIME loop
    U += rAU * (flowDirection * pDelta);
    U.correctBoundaryConditions();

    //update phi as well
    // phi += (fvc::interpolate((flowDirection*rAU*pDelta)) & mesh.Sf());
    phi += rAUf * ((flowDirection * pDelta) & mesh.Sf());

    // update driving pressure gradient to maintain mass & momentum flux
    gradP -= pDelta;

    Info<< "Uncorrected Ubar = " << magUbarStar.value() << tab
        << "pressure gradient = " << gradP.value() << tab
        << "incremental pressure gradient= " << pDelta.value() << tab
        << "damping coeffs: "<< relaxation
        << endl;
}
