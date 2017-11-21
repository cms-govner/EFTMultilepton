void PlotHelper::bookHistos()
{
    // this function is ONLY for booking the histo/tgraph arrays!
    
    cout << "Booking histograms..." << endl;
               
    TH1D *th1ds[arrsize]; 
    TH2D *th2ds[arrsize];
    //TGraph *tgraphs[arrsize];
       
    for (int i=0; i<(int)arrsize; i++)
    {
        th1ds[i] = new TH1D("blah"+int2ss(i),"blah"+int2ss(i),10,0,1);
        th2ds[i] = new TH2D("blah2D"+int2ss(i),"blah2D"+int2ss(i),10,0,1,10,0,1);
    }
    /////////////////////////////////////////////////////////////////////////
    // Actually book them here. The histos each need to be uniquely named
    // because they are inserted into a dictionary and looked up later.
            
    unsigned int i=0;
    
    // 1,2,3 are different pt ranges
    th1ds[i]->SetName("eleMVA sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                          
    th1ds[i]->SetName("eleMVA sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("lepMVA sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("eleMVA bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("lepMVA bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("lepMVA bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;                            

    th1ds[i]->SetName("eleMVA GP WP90 sig1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            

    th1ds[i]->SetName("eleMVA GP WP90 bkd1 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd2 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd3 barrel");    th1ds[i]->SetBins(500,-1,1);    i++;            

    th1ds[i]->SetName("eleMVA sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                          
    th1ds[i]->SetName("eleMVA sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("lepMVA sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("eleMVA bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("eleMVA bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             

    th1ds[i]->SetName("lepMVA bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                             
    th1ds[i]->SetName("lepMVA bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            
    th1ds[i]->SetName("lepMVA bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;                            

    th1ds[i]->SetName("eleMVA GP WP90 sig1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 sig3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            

    th1ds[i]->SetName("eleMVA GP WP90 bkd1 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd2 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;            
    th1ds[i]->SetName("eleMVA GP WP90 bkd3 endcap");    th1ds[i]->SetBins(500,-1,1);    i++;  


    // inclusive in pt
    th1ds[i]->SetName("eleMVA sig barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("eleMVA bkd barrel");             th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("lepMVA sig barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("lepMVA bkd barrel");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("eleMVA GP WP90 sig barrel");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("eleMVA GP WP90 bkd barrel");     th1ds[i]->SetBins(500,-1,1);    i++;

    th1ds[i]->SetName("eleMVA sig endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("eleMVA bkd endcap");             th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("lepMVA sig endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
    th1ds[i]->SetName("lepMVA bkd endcap");             th1ds[i]->SetBins(500,-1,1);    i++;        
            
    th1ds[i]->SetName("eleMVA GP WP90 sig endcap");     th1ds[i]->SetBins(500,-1,1);    i++;
    th1ds[i]->SetName("eleMVA GP WP90 bkd endcap");     th1ds[i]->SetBins(500,-1,1);    i++;











//    th1ds[12]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var1 sig","",100,0,1);  //0.04
//    th1ds[13]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var2 sig","",100,0,1);  //0.04
//    th1ds[14]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var3 sig","",100,0,1);  //0.04
//    th1ds[15]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var1 bkd","",100,0,1);  //0.04                            
//    th1ds[16]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var2 bkd","",100,0,1);  //0.04
//    th1ds[17]->SetNameTitle("","");                 = new TH1F("sigmaIeatIeta var3 bkd","",100,0,1);  //0.04
    
    
    
    
//    th1ds[12]->SetNameTitle("test","test");                         th1ds[12]->SetBins(100,0,400);    
   
    


    /////////////////////////////////////////////////////////////////////////    
    // Done booking. Populate the dictionaries:
    
    for (int i=0; i<(int)arrsize; i++)
    {
        th1d[th1ds[i]->GetName()] = th1ds[i];
        th2d[th2ds[i]->GetName()] = th2ds[i];
    }
    
    
}