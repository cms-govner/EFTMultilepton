// You should never need to touch this.
void HistMaker::run(TTreeReader & newreader, int firstevent, int lastevent)
{
    if (firstevent>-1 || lastevent>-1) newreader.SetEntriesRange(firstevent,lastevent);
    cout << "Starting event loop..." << endl;
    int count = 0;
    event_counter = 0;
    while (newreader.Next()) {
        doOneEvent();
        count++;
        // if (event_counter > 100) break;
        if (count%5000==0) cout << "Event: " << count << endl;
    }
}
