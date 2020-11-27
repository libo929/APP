{
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../test.root");
   if (!f || !f->IsOpen()) {
      f = new TFile("../test.root");
   }

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

   //for(unsigned int i = 0; i < 5; ++i)
   for(unsigned int i = 0; i < tree->GetEntries(); ++i)
   {
	   tree->GetEntry(i);

	   cout << "Evt: " << i << ", nClusters: " << nClusters << ", pos: " << clustersCOGX->size() << endl;
	   if(clustersCOGX->size() <= 1) continue;

	   std::vector<TVector3> clusterCentroids;
	   std::vector<float> clusterEngs;

	   for(int iClu = 0; iClu < clustersCOGX->size(); ++iClu)
	   {
		   std::cout << "  --- " << clustersCOGX->at(iClu) << ", " << clustersCOGY->at(iClu) 
			    << ", " << clustersCOGZ->at(iClu) << ", E: " << clusterEnergies->at(iClu) << std::endl;

		   TVector3 clusterCentroid(clustersCOGX->at(iClu), clustersCOGY->at(iClu), clustersCOGZ->at(iClu));

		   clusterCentroids.push_back(clusterCentroid);
		   clusterEngs.push_back(clusterEnergies->at(iClu));
	   }

	   // loop for combination
	   for(int iClu = 0; iClu < clusterCentroids.size(); ++iClu)
	   {
		   TVector3 iClusterCentroid = clusterCentroids.at(iClu);
		   float iClusterEnergy = clusterEngs.at(iClu);

		   if(iClusterEnergy<0.05) continue;

		   for(int jClu = iClu+1; jClu < clusterCentroids.size(); ++jClu)
		   {
			   TVector3 jClusterCentroid = clusterCentroids.at(jClu);
			   float jClusterEnergy = clusterEngs.at(jClu);
		   
			   if(jClusterEnergy<0.05) continue;

			   float theta = iClusterCentroid.Angle(jClusterCentroid);
			   float mass2 = 2. * iClusterEnergy * jClusterEnergy * ( 1. - TMath::Cos(theta) );
			   float mass = sqrt(mass2);

			   if(mass > 0.02 && mass < 0.3)
			   {
				   std::cout << "cluE1: " << iClusterEnergy << ", cluE2: " << jClusterEnergy << ", mass: " << mass << std::endl;
				   hist->Fill(mass);
			   }
		   }
	   }
   }

   hist->Draw();
}
