void FormatGraph(TGraphErrors * gr, TString title, unsigned int color);
TString countryName;

void plot(TString country="",Int_t startDay=0) {
  countryName=country;
  TTree * tr = new TTree("tr","tr");
  tr->ReadFile("table.dat","date/C:day/I:nC/F:nD/F:nR/F");
  Int_t day,endDay;
  Float_t nC,nD,nR,nA;
  tr->SetBranchAddress("day",&day);
  tr->SetBranchAddress("nC",&nC);
  tr->SetBranchAddress("nD",&nD);
  tr->SetBranchAddress("nR",&nR);
  
  TGraphErrors * grNumC = new TGraphErrors();
  TGraphErrors * grNumD = new TGraphErrors();
  TGraphErrors * grNumR = new TGraphErrors();
  TGraphErrors * grNumA = new TGraphErrors();
  FormatGraph(grNumC,"num confirmed",kRed);
  FormatGraph(grNumD,"num dead",kOrange+1);
  FormatGraph(grNumR,"num recovered",kAzure);
  FormatGraph(grNumA,"num active",kGreen+1);

  TGraphErrors * grGrowthC = new TGraphErrors();
  TGraphErrors * grGrowthD = new TGraphErrors();
  TGraphErrors * grGrowthR = new TGraphErrors();
  TGraphErrors * grGrowthA = new TGraphErrors();
  FormatGraph(grGrowthC,"growth factor confirmed",kRed);
  FormatGraph(grGrowthD,"growth factor dead",kOrange+1);
  FormatGraph(grGrowthR,"growth factor recovered",kAzure);
  FormatGraph(grGrowthA,"growth factor active",kGreen+1);

  TGraphErrors * grMortality = new TGraphErrors();
  TGraphErrors * grRecovery = new TGraphErrors();
  FormatGraph(grMortality,"mortality",kOrange+1);
  FormatGraph(grRecovery,"recovery",kAzure);

  Float_t nCprev,nDprev,nRprev,nAprev;
  Float_t newC,newD,newR,newA;
  Float_t newCprev,newDprev,newRprev,newAprev;
  nCprev=nDprev=nRprev=nAprev=0;
  newCprev=newDprev=newRprev=newAprev=0;
  Int_t iC,iD,iR,iA;
  iC=iD=iR=iA=0;
  for(int i=0; i<tr->GetEntries(); i++) {
    tr->GetEntry(i);
    nA = nC - nD - nR;
    newC = nC - nCprev;
    newD = nD - nDprev;
    newR = nR - nRprev;
    newA = nA - nAprev;
    grNumC->SetPoint(i,day,nC);
    grNumD->SetPoint(i,day,nD);
    grNumR->SetPoint(i,day,nR);
    grNumA->SetPoint(i,day,nA);
    grMortality->SetPoint(i,day,nC>0?nD/nC:0);
    grRecovery->SetPoint(i,day,nC>0?nR/nC:0);
    if(fabs(newCprev)>0) grGrowthC->SetPoint(iC++,day,newC/newCprev);
    if(fabs(newDprev)>0) grGrowthD->SetPoint(iD++,day,newD/newDprev);
    if(fabs(newRprev)>0) grGrowthR->SetPoint(iR++,day,newR/newRprev);
    if(fabs(newAprev)>0) {
      grGrowthA->SetPoint(iA,day,nA/nAprev);
      //grGrowthA->SetPointError(iA,0,0.1); // fake error!
      iA++;
    };
    nCprev = nC;
    nDprev = nD;
    nRprev = nR;
    nAprev = nA;
    newCprev = newC;
    newDprev = newD;
    newRprev = newR;
    newAprev = newA;
  };
  endDay = tr->GetMaximum("day");

  gStyle->SetOptFit(0);
  Int_t extrapolateDay = 100;
  Int_t inflectionDay = 0;
  Float_t nCinflection = 0;
  Double_t dd,nn;
  TString poly = "pol3";
  if(startDay>0) {
    grGrowthC->Fit(poly,"Q","",startDay,endDay+extrapolateDay);
    grGrowthD->Fit(poly,"Q","",startDay,endDay+extrapolateDay);
    grGrowthR->Fit(poly,"Q","",startDay,endDay+extrapolateDay);
    grGrowthA->Fit(poly,"Q","",startDay,endDay+extrapolateDay);

    printf("searching for inflection point...\n");
    for(int d=startDay; d<=endDay+100; d++) {
      if(grGrowthA->GetFunction(poly)->Eval(d) <= 1) {
        inflectionDay = d;
        break;
      };
    };
    if(inflectionDay>0) {
      printf("inflection day = %d\n",inflectionDay);
      if(inflectionDay > endDay) {
        printf("predicted inflection day is in the future; extrapolating...\n");
        grNumC->Fit("expo","Q","",startDay,endDay+extrapolateDay);
        nCinflection = grNumC->GetFunction("expo")->Eval(inflectionDay);
      } else {
        printf("inflection day found in the past\n");
        for(int i=0; i<grNumC->GetN(); i++) {
          grNumC->GetPoint(i,dd,nn);
          if(fabs(dd-inflectionDay)<0.1) nCinflection = nn;
        };
      };

      printf("# predicted confirmed cases:\n best case\t%.0f\n worst case:\t%.0f\n",
        1.1 * nCinflection,
        2 * nCinflection);
        
    }
    else {
      printf("could not find inflection point\n");
      printf("failed to predict the total number of cases\n");
    };
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
  grGrowthC->GetXaxis()->SetRangeUser(startDay>0?startDay-5:0,endDay+extrapolateDay);
  grGrowthD->GetXaxis()->SetRangeUser(startDay>0?startDay-5:0,endDay+extrapolateDay);
  grGrowthR->GetXaxis()->SetRangeUser(startDay>0?startDay-5:0,endDay+extrapolateDay);
  grGrowthA->GetXaxis()->SetRangeUser(startDay>0?startDay-5:0,endDay+extrapolateDay);
};

void FormatGraph(TGraphErrors * gr, TString title, unsigned int color) {
  gr->SetTitle(TString(countryName+": "+title+";day;"+title));
  gr->SetMarkerStyle(kFullCircle);
  gr->SetMarkerColor(color);
};

