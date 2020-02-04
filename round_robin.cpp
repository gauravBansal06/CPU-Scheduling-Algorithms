            //Round Robin Scheduling
//Input File name :- "input.dat" (see reference input file)
//Output File name :- "round_robin_out.dat"
//Compile:- g++ round_robin.cpp -o round_robin
//Run:- ./round_robin
//Assuming processes in file are in sorted arrival time manner
#include<bits/stdc++.h>
using namespace std;

int main()
{
    string line;
    int n,pid,arrival=0,burst=0,quantum,prev=-1,start=0,count=0,tat_sum=0,rt_sum=0,wt_sum=0;

    ifstream infile("input.dat");                  
    ofstream outfile("round_robin_out.dat");

    if(!infile)
    {
      cerr << "Datafile does not exist" << endl;
      exit(-1);
    }

    getline(infile,line);
    stringstream str(line);
    str >> n;                         //no. of processes
    
    vector<tuple<int,int,int>> process(n);     //tuple = <burst_time,arrival_time,pid>
    map<int,pair<int,int>> proc_count;                   //(arrival,(index,count))

    for(int i=0; i < n; i++)
    {
       getline(infile,line);
       stringstream str(line);
       str >> arrival;
       str >> burst;
       
       if(arrival == prev)
         count++;
       else
        {  
           if(i!=0)
           {
              proc_count.insert({prev,{start,count}});
           }
           count = 1; prev = arrival;
           start = i;
        }
        
        process[i] = make_tuple(burst,arrival,i+1);
        
    }

    proc_count.insert({prev,{start,count}});
    
    getline(infile,line);
    stringstream str1(line);       //getting quantum value
    str1 >> quantum;

    infile.close();            //read complete
    
   //Round Robin scheduling calculations
  
    tuple<int,int,int> curr_process;
    pair<int,int> st_ct;
    vector<tuple<int,int,int>> proc_queue;
    vector<int> turn_around_time(n),waiting_time(n),response_time(n),tsched(n,-1),completion(n,0);
    int time = -1,flag = 0,lastpid = 0;

    count = 0;  //no. of completed processes
    prev = 0;

    while(count != n)
    {
       if(!proc_queue.empty())
       {
          curr_process = proc_queue[0];
          burst = get<0>(curr_process);
          arrival = get<1>(curr_process);
          pid = get<2>(curr_process);

          proc_queue.erase(proc_queue.begin());  //dequeing
          completion[pid-1] += quantum;         //time spent by process in CPU
          if(tsched[pid-1] == -1)
          {
             tsched[pid-1] = time;
             response_time[pid-1] = time - arrival;
             waiting_time[pid-1] = response_time[pid-1];
          }    
          else
          {
             waiting_time[pid-1] += time - tsched[pid-1] - quantum;
             tsched[pid-1] = time;
          }           
  
          if(completion[pid-1] < burst)
          {
             flag = 1;
             time += quantum; 
          }
          else if(completion[pid-1] == burst)
          {
              count++;
              flag = 0;
              time += quantum;
              turn_around_time[pid-1] = time - arrival;
          }
          else
          {
              count++;
              flag = 0;
              time += completion[pid-1] - burst;
              turn_around_time[pid-1] = time - arrival;
          }
          
          if(pid != lastpid)
          {
             lastpid = pid;
             cout << "Process " << pid <<" at time " << tsched[pid-1] << endl;
          }
          prev = tsched[pid-1];
       }
      
       else
       {
          prev = time;
          time++;
       }

       for(int i=prev+1; i <= time; i++)
       {
          auto itr = proc_count.find(i);
          if(itr != proc_count.end())
          {
            st_ct = itr->second;
            start = st_ct.first;
            for(int j=start; j < start+st_ct.second; j++)
                proc_queue.push_back(process[j]);
          }
       } 
       
       if(flag)
          proc_queue.push_back(curr_process);
 
    }  //while loop ends

    cout << endl;
    outfile << "PID\tTurnaround\tResponse\tWaiting" << endl;
    cout << "PID\tTurnaround\tResponse\tWaiting" << endl;
    for(int i=0; i < n; i++)
    {
        outfile << i+1 << "\t\t" << turn_around_time[i] << "\t\t  " << response_time[i] << "\t\t\t" << waiting_time[i] << endl;
        cout << i+1 << "\t" << turn_around_time[i] << "\t\t" << response_time[i] << "\t\t" << waiting_time[i] << endl;    
        tat_sum += turn_around_time[i];
        rt_sum += response_time[i];
        wt_sum += waiting_time[i];
    }
    outfile << endl;
    cout << endl;
    outfile << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    outfile << "Average Response Time = " << (float)rt_sum/n << endl;
    outfile << "Average Waiting Time = " << (float)wt_sum/n << endl;
    cout << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    cout << "Average Response Time = " << (float)rt_sum/n << endl;
    cout << "Average Waiting Time = " << (float)wt_sum/n << endl;
    
    outfile.close();
 
      
    return 0; 
}
