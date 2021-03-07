{
   f = new TFile("../reco_pi0_10GeV.root");

   TTree* tree;
   f->GetObject("testTree",tree);

   cout << tree << endl;

   cout << "Evts: " << tree->GetEntries() << endl;

   Int_t           nClusters;
   Int_t           nMainClusters;
   Float_t         mainClustersEnergy;
   Float_t         eventEnergy;

   vector<float>   *clusterEnergies;

   vector<float>   *clustersCOGX;
   vector<float>   *clustersCOGY;
   vector<float>   *clustersCOGZ;

   tree->SetBranchAddress("nClusters", &nClusters);
   tree->SetBranchAddress("nMainClusters", &nMainClusters);
   tree->SetBranchAddress("mainClustersEnergy", &mainClustersEnergy);
   tree->SetBranchAddress("eventEnergy", &eventEnergy);

   tree->SetBranchAddress("clusterEnergies", &clusterEnergies);
   tree->SetBranchAddress("clustersCOGX", &clustersCOGX);
   tree->SetBranchAddress("clustersCOGY", &clustersCOGY);
   tree->SetBranchAddress("clustersCOGZ", &clustersCOGZ);

   TH1F* hist = new TH1F("h", "hist", 100, 0., 0.3);

   for(unsigned int i = 0; i < tree->GetEntries(); ++i)
   {
	   tree->GetEntry(i);

	   //cout << "Evt: " << i << ", nClusters: " << nClusters << ", pos: " << clustersCOGX->size() << endl;
	   if(clustersCOGX->size() <= 1) continue;

	   std::map<float, TVector3> energyCOG;

	   for(int iClu = 0; iClu < clustersCOGX->size(); ++iClu)
	   {
		   //std::cout << "  --- " << clustersCOGX->at(iClu) << ", " << clustersCOGY->at(iClu) 
			//    << ", " << clustersCOGZ->at(iClu) << ", E: " << clusterEnergies->at(iClu) << std::endl;

		   TVector3 clusterCentroid(clustersCOGX->at(iClu), clustersCOGY->at(iClu), clustersCOGZ->at(iClu));

		   energyCOG[clusterEnergies->at(iClu)] = clusterCentroid;
	   }

	   if(energyCOG.size()<2) continue;

	   // the cluster with the largest energy
	   auto it1 = energyCOG.rbegin();

	   float iClusterEnergy = it1->first;
	   TVector3 iClusterCentroid = it1->second;

	   if(iClusterEnergy<1.) continue;

	   // the cluster with the second largest energy
	   ++it1;
	   float jClusterEnergy = it1->first;
	   TVector3 jClusterCentroid = it1->second;
	   
	   if(jClusterEnergy<1.) continue;

	   float theta = iClusterCentroid.Angle(jClusterCentroid);
	   float mass2 = 2. * iClusterEnergy * jClusterEnergy * ( 1. - TMath::Cos(theta) );
	   float mass = sqrt(mass2);

	   if(mass > 0.02 && mass < 0.3)
	   {
	       //std::cout << "cluE1: " << iClusterEnergy << ", cluE2: " << jClusterEnergy << ", mass: " << mass << std::endl;
	       hist->Fill(mass);
	   }
   }

   hist->Draw();
}
