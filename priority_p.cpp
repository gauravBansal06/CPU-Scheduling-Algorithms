            //Priority Preemptive Scheduling
//Input File name :- "input.dat" (see reference input file)
//Output File name :- "priority_preemptive_out.dat"
//Compile:- g++ priority_p.cpp -o priority_p
//Run:- ./priority_p
//Assuming processes in file are in sorted arrival time manner
#include<bits/stdc++.h>
using namespace std;

int main()
{
    string line;
    int n,pid,arrival=0,burst=0,priority,prev=-1,start=0,count=0,tat_sum=0,rt_sum=0,wt_sum=0;

    ifstream infile("input.dat");                  
    ofstream outfile("priority_preemptive_out.dat");

    if(!infile)
    {
      cerr << "Datafile does not exist" << endl;
      exit(-1);
    }

    getline(infile,line);
    stringstream str(line);
    str >> n;                         //no. of processes
    
    vector<tuple<int,int,int,int>> process(n);     //tuple = <priority,pid,arrival_time,burst_time>
    map<int,pair<int,int>> proc_count;                   //(arrival,(index,count))

    for(int i=0; i < n; i++)
    {
       getline(infile,line);
       stringstream str(line);
       str >> arrival;
       str >> burst;
       str >> priority;
       
       if(arrival == prev)
         count++;
       else
        {  
           if(i!=0)
           {
              sort(process.begin()+start,process.begin()+start+count);      //sorting based on priority for same arrival time 
              proc_count.insert({prev,{start,count}});
           }
           count = 1; prev = arrival;
           start = i;
        }
        
        process[i] = make_tuple(priority,i+1,arrival,burst);
        
    }

    sort(process.begin()+start,process.begin()+start+count);
    proc_count.insert({prev,{start,count}});

    infile.close();            //read complete
    
   //Priority Preemptive scheduling calculations
  
    tuple<int,int,int,int> curr_process;
    pair<int,int> st_ct;
    vector<tuple<int,int,int,int>> proc_queue;
    vector<int> turn_around_time(n),waiting_time(n),response_time(n),tsched(n,-1),completion(n,0);
    int time = -1,flag = 0,lastpid = 0;

    count = 0;  //no. of completed processes
    prev = 0;

    while(count != n)
    {
       if(!proc_queue.empty())
       {
          curr_process = proc_queue[0];
          burst = get<3>(curr_process);
          arrival = get<2>(curr_process);
          pid = get<1>(curr_process);

          proc_queue.erase(proc_queue.begin());  //dequeing
          completion[pid-1]++;         //time spent by process in CPU
          if(tsched[pid-1] == -1)
          {
             tsched[pid-1] = time;
             response_time[pid-1] = time - arrival;
             waiting_time[pid-1] = response_time[pid-1];
          }    
          else
          {
             waiting_time[pid-1] += time - tsched[pid-1] - 1;
             tsched[pid-1] = time;
          }           
  
          if(completion[pid-1] < burst)
          {
             flag = 1;
             time++; 
          }
          else
          {
              count++;
              flag = 0;
              time++;
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
       
       sort(proc_queue.begin(),proc_queue.end());  
 
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
