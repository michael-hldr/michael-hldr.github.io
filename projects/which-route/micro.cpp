#include <bits/stdc++.h>
using namespace std;
//This code assumes all routes are straight.
const int Q = 6, Z = 300;
int R, P, N, freq[Q], routeP, thisPoint, lastPoint, D, links, cnt, bus[Q][Z], proportion[Z], sumPrp, counter;
int r, r2, psa[Z], ll, rr, ctr, ridershipCount[Z], bus2;
//travelTime is in minutes. Speed in km/h.
double travelTime1, travelTime2, speed1, speed2, travelTime[Q][Z], DDD, nTT, TTT = 0;
//adjacency list. <<Time, point>, route>
vector<pair<pair<double, int>, int>> adjList[Z];
priority_queue<pair<int, int>> DD;

int main(){
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    //R = number of routes
    //P = number of points
    //number of people in this sample
    cin>>R>>P>>N;
    sumPrp = 0;
    for(int i = 1; i <= P; i++){
        cin>>proportion[i];
        sumPrp += proportion[i];
        psa[i] = sumPrp;
    }
    for(int i = 1; i <= R; i++){
        cin>>freq[i]>>routeP>>lastPoint;
        for(int j = 1; j <= routeP; j++){
            //speeds in km/h, D for distance (metres)
            cin>>speed1>>speed2>>D>>thisPoint;
            //convert to m/s and solve for time in both directions
            travelTime1 = 3.6*D/(60*speed1);
            travelTime2 = 3.6*D/(60*speed2);
            //cout<<travelTime1<<endl;
            //add info to adjacency list.
            adjList[lastPoint].push_back({{travelTime1, thisPoint}, i});
            adjList[thisPoint].push_back({{travelTime2, lastPoint}, i});
            lastPoint = thisPoint;
        }
    }
    cin>>links;
    while(links--){
        //Route 0 = walking
        cin>>D>>lastPoint>>thisPoint;
        //0.022 perceived minutes per metre.
        travelTime1 = D*0.022;
        //add to adjacency list
        adjList[lastPoint].push_back({{travelTime1, thisPoint}, 0});
        adjList[thisPoint].push_back({{travelTime1, lastPoint}, 0});
    }
    for(int j = 0; j <= R; j++) for(int i = 1; i <= P; i++) travelTime[j][i] = 1e9;
    //Now that the adjacency lists have been filled, start with walking time to all points.
    //The "routes" can be done in number order. One route in existence at a time,
    //but the first instance has no routes running.
    //After the first one runs, the walking time will still be there. 
    //This will prevent any "time with bus A" from overriding the walking time unless taking that bus is faster.
    //The links are still weighted
    
    //counter = 0;
    for(int i = 0; i <= R; i++){//For the existence of each "route", including the null case 0.
        //Make sure you allow links with numbers 0 and i.
        //Start is always point 0.
        DD.push({0, 0});
        while(DD.size()){
            //take info from top point (prioritized)
            DDD = DD.top().first;
            thisPoint = DD.top().second;
            DD.pop();
            //cout<<DDD<<'^'<<thisPoint<<endl;
            for(pair<pair<double, int>, int> j:adjList[thisPoint]){
                //cout<<i<<'&'<<j.first.first<<'!'<<j.first.second<<'!'<<j.second<<' '<<travelTime[i][j.first.second]<<endl;
                //ensure the link is on the chosen route or is a walking link.
                if(!j.second || j.second == i){
                    //if(j.second) cout<<DDD<<'*'<<j.first.first<<' '<<thisPoint<<'*'<<j.first.second<<endl;
                    if(DDD + j.first.first < travelTime[i][j.first.second]){
                        nTT = DDD + j.first.first;
                        travelTime[i][j.first.second] = nTT;
                        bus[i][j.first.second] = max(j.second, bus[i][thisPoint]);
                        //counter++;
                        //if(counter < 100) 
                            DD.push({nTT, j.first.second});
                    }
                }
            }
        }
    }
    //The code below should only be activated in the event you are testing to see the distances for all points
    //This may be useful for debugging.
    /*
    for(int j = 0; j <= R; j++) for(int i = 0; i <= P; i++){
        cout<<i<<' '<<travelTime[j][i]<<' '<<bus[j][i]<<' '<<j<<endl;
    }
    */
    //END of debug code

    //the counterr is a debug feature to determine proportions.
    //Good with a large sample to determine if rand is working properly.
    //SIMULATE !!!
    int counterr[Z];
    for(int i = 0; i <= P; i++) counterr[i] = 0;
    for(int i = 0; i < N; i++){
        //Generate point
        r = rand()%sumPrp+1;
        //Binary search to do the weighted random
        ll = 1;
        rr = P;
        while(rr-ll > 1){
            ctr = (ll+rr)/2;
            //cout<<ctr<<'!'<<psa[ctr]<<' ';
            if(psa[ctr] >= r) rr = ctr;
            else ll = ctr+1;
        }
        if(rr != ll){
            //cout<<rr<<'^'<<ll<<" ";
            if(psa[ll] >= r) ctr = ll;
            else ctr = rr;
        }
        else ctr = ll;
        counterr[ctr]++;
        //default to walking distance (should hopefully be the longest)
        nTT = travelTime[0][ctr];
        bus2 = 0;
        //check each bus route
        for(int j = 1; j <= R; j++){
            r2 = rand()%freq[j];
            if(nTT > 1.8*r2 + travelTime[j][ctr]){
                nTT = min(nTT, r2 + travelTime[j][ctr]);
                bus2 = j;
            }
        }
        ridershipCount[bus2]++;
        TTT += nTT;
        //cout<<nTT<<endl;
    }
    //for(int i = 1; i <= P; i++) cout<<counterr[i]<<' '; cout<<endl;
    cout<<"WALKED: "<<ridershipCount[0]<<'\n';
    for(int i = 1; i <= R; i++){
        cout<<"ROUTE "<<i<<": "<<ridershipCount[i]<<'\n';
    }
    cout<<"TOTAL TRAVEL TIME: "<<TTT<<'\n';
    //
    //
    //Notes from while I was figuring this out:
    //
    //This appears to be Dijkstra's from what my CS colleagues have told me, will need to learn it.
    //It might actually be a different algorithm, whatever outputs the minimum walking distance to VMC, all routes considered.
    //Dijkstra for walking distance. STOP if more than 60 perceived min. (2,727 m)
    //check travel times with each route bus, assuming zero wait. Do Dijkstra like before for each bus stop.
    //STOP if another node's travel time is not decreased, or if more than 45 perceived min. (2,045 m)
    //Idea here is to find the shortest perceived time taken (excluding waiting, which will be added later).

    //After all perceived times are generated, do an RNG sample to determine proportions taking each route
    //The sample will be weighted based on each workplace's area.
    
    //This model assumes everyone will go to VMC. In reality, some will follow the Jane bus further to Hwy. 407.
    //That transfer penalty could be added as an average algebraically, since users would need to 'turn up and wait'
    //and do not have a choice on what route to approach unlike when just leaving their place of work.

    //REVISED JUNE 9, 2026
}