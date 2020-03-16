void FormatGraph(TGraph * gr, TString title, unsigned int color);
TString countryName;

void plot(TString country="") {
  countryName=country;
  TTree * tr = new TTree("tr","tr");
  tr->ReadFile("table.dat","date/C:day/I:country/C:nC/F:nD/F:nR/F");
  Int_t day;
  Float_t nC,nD,nR,nA;
  tr->SetBranchAddress("day",&day);
  tr->SetBranchAddress("nC",&nC);
  tr->SetBranchAddress("nD",&nD);
  tr->SetBranchAddress("nR",&nR);
  
  TGraph * grNumC = new TGraph();
  TGraph * grNumD = new TGraph();
  TGraph * grNumR = new TGraph();
  TGraph * grNumA = new TGraph();
  FormatGraph(grNumC,"num confirmed",kRed);
  FormatGraph(grNumD,"num dead",kOrange+1);
  FormatGraph(grNumR,"num recovered",kAzure);
  FormatGraph(grNumA,"num active",kGreen+1);

  TGraph * grGrowthC = new TGraph();
  TGraph * grGrowthD = new TGraph();
  TGraph * grGrowthR = new TGraph();
  TGraph * grGrowthA = new TGraph();
  FormatGraph(grGrowthC,"growth factor confirmed",kRed);
  FormatGraph(grGrowthD,"growth factor dead",kOrange+1);
  FormatGraph(grGrowthR,"growth factor recovered",kAzure);
  FormatGraph(grGrowthA,"growth factor active",kGreen+1);

  TGraph * grMortality = new TGraph();
  TGraph * grRecovery = new TGraph();
  FormatGraph(grMortality,"mortality",kOrange+1);
  FormatGraph(grRecovery,"recovery",kAzure);

  Float_t nCtmp,nDtmp,nRtmp,nAtmp;
  nCtmp=nDtmp=nRtmp=nAtmp=0;
  Int_t nCcnt,nDcnt,nRcnt,nAcnt;
  nCcnt=nDcnt=nRcnt=nAcnt=0;
  for(int i=0; i<tr->GetEntries(); i++) {
    tr->GetEntry(i);
    nA = nC - nD - nR;
    grNumC->SetPoint(i,day,nC);
    grNumD->SetPoint(i,day,nD);
    grNumR->SetPoint(i,day,nR);
    grNumA->SetPoint(i,day,nA);
    grMortality->SetPoint(i,day,nC>0?nD/nC:0);
    grRecovery->SetPoint(i,day,nC>0?nR/nC:0);
    if(nCtmp>0) grGrowthC->SetPoint(nCcnt++,day,nC/nCtmp);
    if(nDtmp>0) grGrowthD->SetPoint(nDcnt++,day,nD/nDtmp);
    if(nRtmp>0) grGrowthR->SetPoint(nRcnt++,day,nR/nRtmp);
    if(nAtmp>0) grGrowthA->SetPoint(nAcnt++,day,nA/nAtmp);
    nCtmp = nC;
    nDtmp = nD;
    nRtmp = nR;
    nAtmp = nA;
  };

  TCanvas * canvLin = new TCanvas("canvLin","canvLin",1000,1000);
  TCanvas * canvLog = new TCanvas("canvLog","canvLog",1000,1000);
  TCanvas * canvFracs = new TCanvas("canvFracs","canvFracs",1000,1000);
  TCanvas * canvGrowth = new TCanvas("canvGrowth","canvGrowth",1000,1000);
  canvLin->Divide(2,2);
  canvLog->Divide(2,2);
  canvFracs->Divide(2,1);
  canvGrowth->Divide(2,2);
  for(int p=1; p<=4; p++) {
    canvLin->GetPad(p)->SetGrid(1,1);
    canvLog->GetPad(p)->SetGrid(1,1);
    canvGrowth->GetPad(p)->SetGrid(1,1);
    if(p<3) canvFracs->GetPad(p)->SetGrid(1,1);
    canvLog->GetPad(p)->SetLogy(1);
  };
  canvLin->cd(1); grNumC->Draw("AP");
  canvLin->cd(2); grNumD->Draw("AP");
  canvLin->cd(3); grNumR->Draw("AP");
  canvLin->cd(4); grNumA->Draw("AP");
  canvLog->cd(1); grNumC->Draw("AP");
  canvLog->cd(2); grNumD->Draw("AP");
  canvLog->cd(3); grNumR->Draw("AP");
  canvLog->cd(4); grNumA->Draw("AP");
  canvFracs->cd(1); grMortality->Draw("AP");
  canvFracs->cd(2); grRecovery->Draw("AP");
  canvGrowth->cd(1); grGrowthC->Draw("AP");
  canvGrowth->cd(2); grGrowthD->Draw("AP");
  canvGrowth->cd(3); grGrowthR->Draw("AP");
  canvGrowth->cd(4); grGrowthA->Draw("AP");
};

void FormatGraph(TGraph * gr, TString title, unsigned int color) {
  gr->SetTitle(TString(countryName+": "+title+";day;"+title));
  gr->SetMarkerStyle(kFullCircle);
  gr->SetMarkerColor(color);
};

