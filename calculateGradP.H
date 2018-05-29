// Correct driving force for a constant mass flow rate
{
    // Extract the mean velocity in the flow direction
    dimensionedScalar magUbarStar =
        (flowDirection & U)().weightedAverage(mesh.V());

    // Calculate the pressure gradient increment needed to
    // adjust the average flow-rate to the correct value
    dimensionedScalar pDelta =
        (magUbar - magUbarStar)/rAU.weightedAverage(mesh.V());

    U += flowDirection*rAU*pDelta;
    U.correctBoundaryConditions();

    gradP += alpha*pDelta; // damping pDelta by alpha

    Info<< "Uncorrected Ubar = " << magUbarStar.value() << tab
        << "pressure gradient = " << gradP.value() << endl;
}
