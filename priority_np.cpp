            //Priority Non-Preemptive Scheduling
//Input File name :- "input.dat" (see reference input file)
//Output File name :- "priority_non_preemptive_out.dat"
//Compile:- g++ priority_np.cpp -o priority_np
//Run:- ./priority_np
//Assuming processes in file are in sorted arrival time manner
#include<bits/stdc++.h>
using namespace std;

int main()
{
    string line;
    int n,pid,arrival=0,burst=0,priority,prev=-1,start=0,count=0,tsched=0, turn_around_time, response_time, waiting_time,tat_sum=0,rt_sum=0;

    ifstream infile("input.dat");                  
    ofstream outfile("priority_non_preemptive_out.dat");

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
    
   //Priority-Non Preemptive scheduling calculations
  
    tuple<int,int,int,int> curr_process;
    pair<int,int> st_ct;
    vector<tuple<int,int,int,int>> proc_queue;
    int time = -1;

    outfile << "PID\tTurnaround\tResponse\tWaiting" << endl;
    cout << "PID\tTurnaround\tResponse\tWaiting" << endl;

    count = 0;  //no. of completed processes

    while(count != n)
    {
       if(!proc_queue.empty())
       {
          curr_process = proc_queue[0];
          burst = get<3>(curr_process);
          arrival = get<2>(curr_process);
          pid = get<1>(curr_process);
          proc_queue.erase(proc_queue.begin());
          count++;
          tsched = time;
          time += burst;

          turn_around_time = tsched + burst - arrival;            //calculations
          tat_sum += turn_around_time;
          response_time = tsched - arrival;
          rt_sum += response_time;
          waiting_time = response_time;
          
          outfile << pid << "\t\t" << turn_around_time << "\t\t  " << response_time << "\t\t\t" << waiting_time << endl;
          cout << pid << "\t" << turn_around_time << "\t\t" << response_time << "\t\t" << waiting_time << endl;
       }
      
       else
       {
          tsched = time;
          time++;
       }

       for(int i=tsched+1; i <= time; i++)
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
       
       if(!proc_queue.empty())
         sort(proc_queue.begin(),proc_queue.end());
       
    }
    
    outfile << endl;
    cout << endl;
    outfile << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    outfile << "Average Response Time = " << (float)rt_sum/n << endl;
    outfile << "Average Waiting Time = " << (float)rt_sum/n << endl;
    cout << "Average Turnaround Time = " << (float)tat_sum/n << endl;
    cout << "Average Response Time = " << (float)rt_sum/n << endl;
    cout << "Average Waiting Time = " << (float)rt_sum/n << endl;
    
    outfile.close();
 
      
    return 0; 
}
