Info<< "Reading transportProperties\n" << endl;
IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED, //MUST_READ,
        IOobject::NO_WRITE,
        false
    )
);

dimensionedScalar nu
(
    transportProperties.lookup("nu")
);
Info << "Viscosity: " << nu.value() << endl;

//  Read centerline velocity for channel simulations
dimensionedVector Ubar
(
    transportProperties.lookup("Ubar")
);
Info << "Mean Velocity: " << Ubar << endl;

dimensionedScalar magUbar = mag(Ubar);
vector flowDirection = (Ubar/magUbar).value();

scalar alpha(transportProperties.lookupOrDefault("alpha", 1.0));
Info << "Incremental pressure gradient damping coefficinet (alpha): "
    << alpha << endl;
// ************************************************************************* //
