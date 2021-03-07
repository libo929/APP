{
	const double pi0Mass = 0.135; // GeV
	const double totalEnergy = 10.135; // GeV
	const double gammaFactor = totalEnergy / pi0Mass;
	const double betaFactor  = sqrt(1. - 1./gammaFactor/gammaFactor);

	const double ecal_a0 = 0.17;
	const double ecal_a1 = 0.01;

	ROOT::Math::XYZTVector pi0Vec;
	ROOT::Math::XYZTVector gmVec1;
	ROOT::Math::XYZTVector gmVec2;

	//std::cout << setprecision(10) << "beta: " << betaFactor << std::endl;
	ROOT::Math::BoostZ boostz(betaFactor);

	TH1F* mass = new TH1F("hist", "#pi^{0} invariant mass", 100, 0.02, 0.23);

	for(unsigned int i = 0; i < 50000; ++i)
	{
		//std::cout << "Event: " << i << std::endl;

		// photon direction 
		double costh = gRandom->Uniform(-1, 1);
	    double phi   = gRandom->Uniform(0., 2. * TMath::Pi());
	    double sinth = sqrt(1-costh*costh);

		// photon energy is half that of pi0
		double E  = pi0Mass/2.;

	    double pX = E * sinth * cos(phi);
	    double pY = E * sinth * sin(phi);
	    double pZ = E * costh;

		pi0Vec.SetPxPyPzE(0, 0, 0, pi0Mass);

		gmVec1.SetPxPyPzE(pX, pY, pZ, E);
		gmVec2.SetPxPyPzE(-pX, -pY, -pZ, E);

		pi0Vec = boostz(pi0Vec);
		gmVec1 = boostz(gmVec1);
		gmVec2 = boostz(gmVec2);

		ROOT::Math::XYZTVector vecSum = gmVec1 + gmVec2;

		// ECAL
		TVector3 gmVec1_ecal_dir;
		TVector3 gmVec2_ecal_dir;

		gmVec1_ecal_dir.SetXYZ( gmVec1.X(), gmVec1.Y(), gmVec1.Z() );
		gmVec2_ecal_dir.SetXYZ( gmVec2.X(), gmVec2.Y(), gmVec2.Z() );

		gmVec1_ecal_dir = gmVec1_ecal_dir.Unit();
		gmVec2_ecal_dir = gmVec2_ecal_dir.Unit();
		
	    ROOT::Math::XYZTVector gmVec1_ecal;
	    ROOT::Math::XYZTVector gmVec2_ecal;

		double gm1E = gmVec1.E();
		double gm2E = gmVec2.E();

		// shower energy smearing
		gm1E = gRandom->Gaus( gm1E, ecal_a0 * sqrt(gm1E) + ecal_a1 * gm1E );
		gm2E = gRandom->Gaus( gm2E, ecal_a0 * sqrt(gm2E) + ecal_a1 * gm2E );

		// set new shower energy 
		gmVec1_ecal.SetPxPyPzE(gm1E * gmVec1_ecal_dir.X(), gm1E * gmVec1_ecal_dir.Y(), gm1E * gmVec1_ecal_dir.Z(), gm1E);
		gmVec2_ecal.SetPxPyPzE(gm2E * gmVec2_ecal_dir.X(), gm2E * gmVec2_ecal_dir.Y(), gm2E * gmVec2_ecal_dir.Z(), gm2E);

		ROOT::Math::XYZTVector vecSum_ecal = gmVec1_ecal + gmVec2_ecal;

		//std::cout << "invariant mass (ECAL): " << vecSum_ecal.M() << std::endl << std::endl;

		mass->Fill( vecSum_ecal.M() );
	}

	mass->Draw();
}
