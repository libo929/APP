struct CaloHit
{
	int   cell_x;
	int   cell_y;
	int   cell_z;

	float pos_x;
	float pos_y;
	float pos_z;

	float energy;
	float time;
};

struct CaloEvent
{
	int evtIndex;
	std::vector<CaloHit> eventHits;
};

///////////////////////////////////////////////////////////

//	caloHit.pos_x = (caloHit.cell_x - 384. - 0.5) * 10.1;
//	caloHit.pos_y = (caloHit.cell_y - 312. - 0.5) * 10.1;
//	caloHit.pos_z = 12520. + caloHit.cell_z * 5.5 - 4.;


void read()
{
	TFile* f = new TFile("calo.root");
	TTree* inTree = (TTree*)gDirectory->Get("ce");

	CaloEvent *event = new CaloEvent();
	inTree->SetBranchAddress("event", &event);

	for(unsigned int i= 0; i < inTree->GetEntries(); ++i)
	{
		inTree->GetEntry(i);
		CaloEvent& evt = *event;
		std::cout << "event: " << evt.evtIndex << ", hit: " << evt.eventHits.size() << std::endl;

		for(unsigned int ihit = 0; ihit < evt.eventHits.size(); ++ihit)
		{
			CaloHit& ch = evt.eventHits.at(ihit);
		}
	}

	delete event;
}
