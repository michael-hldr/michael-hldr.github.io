#include <bits/stdc++.h>
using namespace std;
//personal project thing
const int Q = 5;
int n, m, w, minf[Q], prop[Q], rtt[Q], tt, f1, f2, f3, k, b1, b2, b3, aq[Q];
double dist[Q], sum = 0, aa, ls = 1e9, wd[Q];

int main(){
    ios_base::sync_with_stdio(0); 
    cin.tie(0);

    cin>>n>>m; //n = 3 (number of routes), m = number of buses given.
    for(int i = 1; i <= n; i++) cin>>minf[i]; //minimum frequency (headway) for each route.
    for(int i = 1; i < n; i++) {
        cin>>w; //distance in metres between each route.
        dist[i] = w*0.022; //perceived walk time (in minutes) is 0.022 * distance.
    }
    for(int i = 1; i <= n; i++) cin>>prop[i]; //proportion of area nearest to each route.
    for(int i = 1; i <= n; i++){
        cin>>w; //avg walking distance to route from parts of its local area, in meteres
        wd[i] = w*0.022; //perceived walk time (in minutes) is 0.022 * distance.
        //cout<<wd[i]<<' ';
    }
    cout<<'\n';
    for(int i = 1; i <= n; i++) cin>>aq[i]; //extra time to get to destination on each route.
    for(int i = 1; i <= n; i++) cin>>rtt[i]; //minimum round-trip time of each route (including lay-overs).
    //"The brute force method". Looks at all possible combinations of bus distribution.
    for(int i = 0; i <= m; i++){
        if(minf[1] == 0 || ceil(rtt[1]*1.0/i) <= minf[1]){ 
            //if minimum frequency requirement for route 1 is met, distribute remaining buses between routes 2 and 3.
        for(int j = 1; j <= m-i; j++){//always at least 1 bus allotted to route 2.
            //cout<<j<<' '<<rtt[2]*1.0/j<<' '<<ceil(rtt[2]*1.0/j)<<' '<<minf[2]<<endl;
            if((minf[2] == 0 || ceil(rtt[2]*1.0/j) <= minf[2]) && (minf[3] == 0 || ceil(rtt[3]*1.0/(m-i-j)) <= minf[3])){
                //above: ensure minimum frequency requirement is met on routes 2 and 3.
                sum = 0;
                if(i){ //if buses are allotted to this route
                    f1 = ceil(rtt[1]*1.0/i); //f1 is frequency of route 1
                    k = (dist[1]+f2*1.3/2)/1.3; //
                    dist[1] += f2*1.3/2; //
                    if(k > f1 + wd[1]){ //
                        sum += prop[1]*((f1+wd[1])*1.3/2+aq[1]);
                    }
                    else{
                        aa = k*1.0/(f1+wd[1]);
                        sum += aa*prop[1]*(k*1.3/2+aq[1]);
                        sum += (1-aa)*prop[1]*(dist[1]+f2*1.3/2+aq[2]);
                    }
                }
                else{ //if no buses are allotted to this route
                    f1 = 0;
                    sum += prop[1]*dist[1];
                }
                //cout<<sum<<' ';
                if(j){//if buses are allotted to this route
                    //FIND:
                    f2 = ceil(rtt[2]*1.0/j); //f2 is frequency of route 2
                    sum += prop[2]*((f2+wd[2])*1.3/2+aq[2]);
                }
                else{//if no buses are allotted to this route. NOTE: This part currently does not work well and should not be used.
                    f2 = 0;
                    sum += prop[2]*(dist[2]+dist[1]+aq[1]+aq[3]+f1*1.3/2+f3*1.3/2)/2;
                }
                //cout<<sum<<' ';
                if(m-i-j){//if buses are allotted to this route
                    f3 = ceil(rtt[3]*1.0/(m-i-j));
                    k = (dist[2]+f2*1.3/2)/1.3;
                    if(k > f3 + wd[3]){
                        sum += prop[3]*((f3+wd[3])*1.3/2+aq[3]);
                    }
                    else{
                        k = (dist[2]+f2*1.3/2)/1.3;
                        aa = k*1.0/(f3+wd[3]);
                        //cout<<"\n!!!!!!!"<<aa<<"!!!!!!!"<<endl;
                        sum += aa*prop[3]*(k*1.3/2+aq[3]);
                        sum += (1-aa)*prop[3]*(dist[2]+f2*1.3/2+aq[2]);
                    }
                }
                else{//if no buses are allotted to this route
                    f3 = 0;
                    sum += prop[3]*dist[2];
                }
                //cout<<f1<<'!'<<f2<<'!'<<f3<<' '<<sum<<endl;
                if(ls > sum){
                    //cout<<ls<<' '<<sum<<'\n';
                    b1 = i;
                    b2 = j;
                    b3 = m-i-j;
                    ls = sum;
                }
            }
        }
        }
    }
    if(!b1 && !b2 && !b3) cout<<"ERROR: TOO FEW BUSES GIVEN"; //prints if m is too low for the required minf values.
    else{
        //cout<<ls<<endl; //total travel time

        if(b1) cout<<"ROUTE 1: "<<b1<<" buses, "<<ceil(rtt[1]*1.0/b1)<<"-min headway\n";
        else cout<<"No service on route 1\n"; //printed if route 1 has no service

        cout<<"ROUTE 2: "<<b2<<" buses, "<<ceil(rtt[2]*1.0/b2)<<"-min headway\n";

        if(b3) cout<<"ROUTE 3: "<<b3<<" buses, "<<ceil(rtt[3]*1.0/b3)<<"-min headway\n";
        else cout<<"No service on route 3\n";//printed if route 3 has no service
    }
}