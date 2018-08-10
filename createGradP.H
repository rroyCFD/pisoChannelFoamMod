//  Read centerline velocity for channel simulations
dimensionedVector Ubar
(
    transportProperties.lookup("Ubar")
);
Info << "Mean Velocity: " << Ubar << endl;

dimensionedScalar magUbar = mag(Ubar);
vector flowDirection = (Ubar/magUbar).value();

scalar relaxation(transportProperties.lookupOrDefault("relaxation", 1.0));
Info << "Incremental pressure gradient damping coefficient (relaxation): " << relaxation << endl;

//-----------------------------------------------------------------------------------------//
dimensionedScalar gradP
(
    "gradP",
    dimensionSet(0, 1, -2, 0, 0),
    0.0
);


IFstream gradPFile
(
    runTime.path()/runTime.timeName()/"uniform"/"gradP.raw"
);

if (gradPFile.good())
{
    gradPFile >> gradP;
    Info<< "Average pressure gradient at time " << runTime.timeName()<< ": " <<gradP << endl;
}
else
{
    Info<< "Initializing with 0 pressure gradient" <<endl << endl;
};
